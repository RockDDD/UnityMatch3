// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Match3BlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MYUNREALMATCH_API UMatch3BlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Match3 Gameplay, Meta = (WorldContext = "WorldContextObject"))
		static APlayerController* GetLocalPlayerController(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = Match3 Gameplay)
		static FString GetOnlineAccountID(APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, Category = Match3 Gameplay£¬Meta = (WorldContext = "WorldContextObject"))
		static bool IsGameActive(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = Match3 Gameplay£¬Meta = (WorldContext = "WorldContextObject"))
		static void PauseGameTimer(UObject* WorldContextObject, bool bPuse);
	
	
};
