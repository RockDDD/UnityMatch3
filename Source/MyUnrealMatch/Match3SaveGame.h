// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "Match3SaveGame.generated.h"

USTRUCT(BlueprintType)
struct FMatch3LevelSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 GoldScore;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 SilverScore;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 BronzeScore;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 TopScore;
};

/**
 * 
 */
UCLASS()
class MYUNREALMATCH_API UMatch3SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FString, FMatch3LevelSaveData> Match3SaveData;

	bool LoadCustomInt(FString FieldName, int32& Value);
	void SaveCustomInt(FString FieldName, int32 Value);
	void ClearCustomInt(FString FieldName);
	
protected:
	UPROPERTY()
	TMap<FString, int32> Match3CustomData;
	
};
