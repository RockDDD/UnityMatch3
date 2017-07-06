// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUnrealMatch.h"
#include "MyUnrealMatchGameModeBase.h"
#include "GameFramework/Pawn.h"
#include "Match3PlayerController.h"

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
		//if(AMatch3PlayerController* PC = Cast<AMatch3PlayerController>())
	}
}
