// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Match3PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYUNREALMATCH_API AMatch3PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMatch3PlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = Game)
		void AddScore(int32 Points, bool bForceImmediateUpdate = false);

	UFUNCTION(BlueprintCallable, Category = Game)
		int32 GetScore();
	UFUNCTION(BlueprintCallable, Category = Game)
		int32 GetDisplayScore();

	UFUNCTION(BlueprintNativeEvent, Category = Game)
		int32 CalculateBombPower();
	virtual int32 CalculateBombPower_Implementation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
		int32 ComboPower;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
		int32 MaxComboPower;

protected:
	UPROPERTY()
		int32 Score;

	UPROPERTY()
		float DisplayedScore;
	UPROPERTY(EditAnywhere)
		float ScoreChangeRate;

	void TickScoreDisplay();
	FTimerHandle TickScoreDisplayHandle;
};
