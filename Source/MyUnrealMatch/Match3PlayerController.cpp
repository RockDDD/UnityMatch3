// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUnrealMatch.h"
#include "Match3PlayerController.h"

AMatch3PlayerController::AMatch3PlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableTouchEvents = bEnableClickEvents = true;
	bEnableTouchOverEvents = bEnableMouseOverEvents = true;
	ScoreChangeRate = 375.0f;
}

void AMatch3PlayerController::AddScore(int32 Points, bool bForceImmediateUpdate /* = false */)
{
	Score += Points;
	if (bForceImmediateUpdate)
	{
		DisplayedScore = (float)Score;
	}
	else
	{
		GetWorldTimerManager().SetTimer(TickScoreDisplayHandle, this, &AMatch3PlayerController::TickScoreDisplay, 0.001f, true);
	}
}

int32 AMatch3PlayerController::GetScore()
{
	return Score;
}

int32 AMatch3PlayerController::GetDisplayScore()
{
	return (int32)DisplayedScore;
}

int32 AMatch3PlayerController::CalculateBombPower_Implementation()
{
	return 0;
}


void AMatch3PlayerController::TickScoreDisplay()
{
	DisplayedScore += UGameplayStatics::GetWorldDeltaSeconds(this)*ScoreChangeRate;
	if (DisplayedScore >= (float)Score)
	{
		DisplayedScore = Score;
		GetWorldTimerManager().ClearTimer(TickScoreDisplayHandle);
	}
}

