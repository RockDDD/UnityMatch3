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
}

void AMyUnrealMatchGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
