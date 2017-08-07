// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUnrealMatch.h"
#include "Tile.h"
#include "Grid.h"
#include "Kismet/GameplayStatics.h"
#include "MyUnrealMatchGameModeBase.h"

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;


	if (RootComponent)
	{
		RootComponent->SetMobility(EComponentMobility::Movable);
	}
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	Grid = Cast<AGrid>(GetOwner());
	OnInputTouchBegin.AddUniqueDynamic(this, &ATile::TilePress);
	OnInputTouchEnter.AddUniqueDynamic(this, &ATile::TileEnter);
}

void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATile::SetTileMaterial_Implementation(class UMaterialInstanceConstant* TileMaterial)
{
	GetRenderComponent()->SetMaterial(0, TileMaterial);
}

void ATile::TilePress(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	if (!UGameplayStatics::IsGamePaused(this) && Grid)
	{
		Grid->OnTileWasSelected(this);
	}
}

void ATile::TileEnter(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	if (!UGameplayStatics::IsGamePaused(this) && Grid)
	{
		ATile* CurrentSelectedTile = Grid->GetCurrentlySelectedTile();
		if (CurrentSelectedTile && (CurrentSelectedTile != this))
		{
			TilePress(FingerIndex, TouchedActor);
		}
	}
}

void ATile::TilePress_Mouse(AActor* ClickedActor, FKey ButtonClicked)
{
	TilePress(ETouchIndex::Touch1, ClickedActor);
}

void ATile::TileEnter_Mouse(AActor* MouseOverActor)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (PC->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			TileEnter(ETouchIndex::Touch1, MouseOverActor);
		}
	}
}

void ATile::OnMatched_Implementation(EMatch3MoveType::Type MoveType)
{
	Grid->OnTileFinishedMatching(this);
}

void ATile::OnSwapMove_Implementation(ATile* OtherTile, bool bMoveWillSucceed)
{
	Grid->OnSwapDisplayFinished(this);
}

void ATile::StartFalling(bool bUseCurrentWorldLocation /* = false */)
{
	float FallingDistance;
	FallingStartTime = GetWorld()->GetTimeSeconds();
	FallingStartLocation = GetActorLocation();
	GetWorldTimerManager().SetTimer(TickFallingHandle, this, &ATile::TickFalling, 0.001f, true);
	check(Grid);

	if (!bUseCurrentWorldLocation)
	{
		int32 YOffset = 0;
		int32 HeightAboveBottom = 1;
		while (true)
		{
			++YOffset;
			if (Grid->GetGridAddressWithOffset(GetGridAddress(), 0, -YOffset, LandingGridAddress))
			{
				if (ATile* TileBelow = Grid->GetTileFromGridAddress(LandingGridAddress))
				{
					if (TileBelow->TileState == ETileState::ETS_Falling)
					{
						++HeightAboveBottom;
						continue;
					}
					else if (TileBelow->TileState == ETileState::ETS_PendingDelete)
					{
						continue;
					}
				}
				else
				{
					continue;
				}
			}

			YOffset -= HeightAboveBottom;
			Grid->GetGridAddressWithOffset(GetGridAddress(), 0, -YOffset, LandingGridAddress);
			break;
		}
		FallingDistance = Grid->TileSize.Y * YOffset;
		FallingEndLocation = FallingStartLocation;
		FallingEndLocation.Z -= FallingDistance;
	}
	else
	{
		LandingGridAddress = GetGridAddress();
		FallingEndLocation = Grid->GetLocationFromGridAddress(LandingGridAddress);
		FallingDistance = FallingStartLocation.Z - FallingEndLocation.Z;
	}
	
	AMyUnrealMatchGameModeBase* CurrentGameMode = Cast<AMyUnrealMatchGameModeBase>(UGameplayStatics::GetGameMode(this));
	TotalFallingTime = 0.0f;
	if (CurrentGameMode && (CurrentGameMode->TileMoveSpeed > 0.0f))
	{
		TotalFallingTime = FallingDistance / CurrentGameMode->TileMoveSpeed;
	}
	if (TotalFallingTime <= 0.0f)
	{
		TotalFallingTime = 0.75f;
	}
	StartFallingEffect();
}

void ATile::TickFalling()
{
	AMyUnrealMatchGameModeBase* CurrentGameMode = Cast<AMyUnrealMatchGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (CurrentGameMode)
	{
		check(Grid);
		check(TotalFallingTime > 0.0f);
		float FallingCompleteFraction = (GetWorld()->GetTimeSeconds() - FallingStartTime) / TotalFallingTime;
		if (FallingCompleteFraction >= 1.0f)
		{
			FinishedFalling();
		}
		else
		{
			FVector NewLocation = FMath::Lerp(FallingStartLocation, FallingEndLocation, FallingCompleteFraction);
			SetActorLocation(NewLocation);
		}
	}
	else
	{
		FinishedFalling();
	}
}

void ATile::FinishedFalling()
{
	GetWorldTimerManager().ClearTimer(TickFallingHandle);
	SetActorLocation(FallingEndLocation);
	Grid->OnTileFinishedFalling(this, LandingGridAddress);
	StopFallingEffect();
}

void ATile::SetGridAddress(int32 NewLocation)
{
	GridAddress = NewLocation;
}

int32 ATile::GetGridAddress() const
{
	return GridAddress;
}

USoundWave* ATile::GetMatchSound()
{
	return MatchSound;
}