// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "MyUnrealMatchGameModeBase.generated.h"


USTRUCT()
struct FMatch3Reward 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ScoreInterval;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TimeAwareded;
};

/**
 * 
 */
UCLASS()
class MYUNREALMATCH_API AMyUnrealMatchGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMyUnrealMatchGameModeBase(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	
};
