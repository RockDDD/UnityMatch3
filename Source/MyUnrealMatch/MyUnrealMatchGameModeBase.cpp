// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUnrealMatch.h"
#include "MyUnrealMatchGameModeBase.h"
#include "GameFramework/Pawn.h"
#include "Match3PlayerController.h"
#include "Match3BlueprintFunctionLibrary.h"
#include "Match3GameInstance.h"

AMyUnrealMatchGameModeBase::AMyUnrealMatchGameModeBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = APawn::StaticClass();
	PlayerControllerClass = AMatch3PlayerController::StaticClass();
	TileMoveSpeed = 50.0f;
	TimeRemaining = 5.0f;
	FinalPlace = 0;
}

void AMyUnrealMatchGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	bGameWillBeWon = false;
	ChangeMenuWidget(StartingWidgetClass);
	GetWorldTimerManager().SetTimer(GameOverTimer, this, &AMyUnrealMatchGameModeBase::GameOver, TimeRemaining, false);
	UMatch3GameInstance* GameInstance = Cast<UMatch3GameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance)
	{
		if (!GameInstance->FindSaveDataFroLevel(this, SaveGameData))
		{
			GameInstance->UpdateSave(this, SaveGameData);
		}
	}
}

void AMyUnrealMatchGameModeBase::GameRestart()
{
	ChangeMenuWidget(nullptr);
	FName LevelName(*UGameplayStatics::GetCurrentLevelName(this, true));
	UGameplayStatics::OpenLevel(this, LevelName);
}

void AMyUnrealMatchGameModeBase::GameOver()
{
	GetWorldTimerManager().ClearTimer(GameOverTimer);
	if (bGameWillBeWon)
	{
		UMatch3GameInstance* GameInstance = Cast<UMatch3GameInstance>(UGameplayStatics::GetGameInstance(this));
		if (AMatch3PlayerController* PC = Cast<AMatch3PlayerController>(UMatch3BlueprintFunctionLibrary::GetLocalPlayerController(this)))
		{
			SaveGameData.TopScore = FMath::Max(PC->GetScore(), SaveGameData.TopScore);
		}
		GameInstance->UpdateSave(this, SaveGameData);
		GameInstance->SaveGame();
	}
	ChangeMenuWidget(bGameWillBeWon ? VictoryWidgetClass : DefeatWidgetClass);
	GameWasWon(bGameWillBeWon);
}

bool AMyUnrealMatchGameModeBase::IsGameActive() const
{
	FTimerManager& WorldTimerManager = GetWorldTimerManager();
	return (WorldTimerManager.IsTimerActive(GameOverTimer) || WorldTimerManager.IsTimerPaused(GameOverTimer));
}

void AMyUnrealMatchGameModeBase::PauseGameTimer(bool bPause)
{
	if (bPause)
	{
		GetWorldTimerManager().PauseTimer(GameOverTimer);
	}
	else
	{
		GetWorldTimerManager().UnPauseTimer(GameOverTimer);
	}
}

FString AMyUnrealMatchGameModeBase::GetRemainingTimeAsString()
{
	int32 OutInt = FMath::CeilToInt(GetWorldTimerManager().GetTimerRemaining(GameOverTimer));
	return FString::Printf(TEXT("%03i"), FMath::Max(0, OutInt));
}

bool AMyUnrealMatchGameModeBase::GetTimerPaused()
{
	return GetWorldTimerManager().IsTimerPaused(GameOverTimer);
}

void AMyUnrealMatchGameModeBase::AddScore(int32 Points)
{
	if (AMatch3PlayerController* PC = Cast<AMatch3PlayerController>(UMatch3BlueprintFunctionLibrary::GetLocalPlayerController(this)))
	{
		int32 OldScore = PC->GetScore();
		PC->AddScore(Points);
		int32 NewScore = PC->GetScore();
		if (NewScore >= SaveGameData.BronzeScore)
		{
			bGameWillBeWon = true;
		}

		if (NewScore > SaveGameData.GoldScore)
		{
			FinalPlace = 1;
			AwardPlace(1, Points);
		}
		else if (NewScore > SaveGameData.SilverScore)
		{
			FinalPlace = 2;
			AwardPlace(2, Points);
		}
		else if (NewScore > SaveGameData.BronzeScore)
		{
			FinalPlace = 3;
			AwardPlace(3, Points);
		}
		else
		{
			FinalPlace = 0;
			AwardPlace(0, Points);
		}

		for (const FMatch3Reward& Reward : Rewards)
		{
			check(Reward.ScoreInterval > 0);
			int32 ScoreAwardCount = (NewScore / Reward.ScoreInterval) - (OldScore / Reward.ScoreInterval);
			if (ScoreAwardCount > 0)
			{
				float StartingTimeValue = GetWorldTimerManager().GetTimerRemaining(GameOverTimer);
				if (StartingTimeValue >= 0.0f)
				{
					GetWorldTimerManager().SetTimer(GameOverTimer, this, &AMyUnrealMatchGameModeBase::GameOver, StartingTimeValue + (ScoreAwardCount * Reward.TimeAwareded),false);
					AwardBonus();
				}
			}
		}

	}
}

void AMyUnrealMatchGameModeBase::UpdateScoresFromLeaderBoard(int32 GoldScore, int32 SilverScore, int32 BronzeScore)
{
	UMatch3GameInstance* GameInstance = Cast<UMatch3GameInstance>(UGameplayStatics::GetGameInstance(this));
	SaveGameData.BronzeScore = BronzeScore;
	SaveGameData.SilverScore = SilverScore;
	SaveGameData.GoldScore = GoldScore;
	GameInstance->SaveGame();
}

void AMyUnrealMatchGameModeBase::SetComboPower(int32 NewComboPower)
{
	if (AMatch3PlayerController* PC = Cast<AMatch3PlayerController>(UMatch3BlueprintFunctionLibrary::GetLocalPlayerController(this)))
	{
		PC->ComboPower = NewComboPower;
	}
}

int32 AMyUnrealMatchGameModeBase::GetComboPower()
{
	if (AMatch3PlayerController* PC = Cast<AMatch3PlayerController>(UMatch3BlueprintFunctionLibrary::GetLocalPlayerController(this)))
	{
		return PC->ComboPower;
	}
	return 0;
}

int32 AMyUnrealMatchGameModeBase::GetMaxComboPower()
{
	if (AMatch3PlayerController* PC = Cast<AMatch3PlayerController>(UMatch3BlueprintFunctionLibrary::GetLocalPlayerController(this)))
	{
		return PC->MaxComboPower;
	}
	return 0;
}

int32 AMyUnrealMatchGameModeBase::CalculateBombPower_Implementation()
{
	if (AMatch3PlayerController* PC = Cast<AMatch3PlayerController>(UMatch3BlueprintFunctionLibrary::GetLocalPlayerController(this)))
	{
		return PC->CalculateBombPower();
	}
	return 0;
}

void AMyUnrealMatchGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass)
	{
		if (AMatch3PlayerController* PC = Cast<AMatch3PlayerController>(UMatch3BlueprintFunctionLibrary::GetLocalPlayerController(this)))
		{
			CurrentWidget = CreateWidget<UUserWidget>(PC, NewWidgetClass);
			if (CurrentWidget)
			{
				CurrentWidget->AddToViewport();
			}
		}
	}
}
