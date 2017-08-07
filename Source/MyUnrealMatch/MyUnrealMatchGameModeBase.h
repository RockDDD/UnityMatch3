// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "Match3SaveGame.h"
#include "Blueprint/UserWidget.h"
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

	UFUNCTION(BlueprintCallable, Category = Game)
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		float TileMoveSpeed;

	UFUNCTION(BlueprintCallable, Category = Game)
		void GameRestart();

	UFUNCTION(BlueprintCallable, Category = Game)
		void GameOver();

	UFUNCTION(BlueprintCallable, Category = Game)
		bool IsGameActive() const;

	UFUNCTION(BlueprintCallable, Category = Game)
		void PauseGameTimer(bool bPause);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Game)
		TArray<FMatch3Reward> Rewards;

	UFUNCTION(BlueprintCallable, Category = Game)
		FString GetRemainingTimeAsString();

	UFUNCTION(BlueprintCallable, Category = Game)
		bool GetTimerPaused();

	UFUNCTION(BlueprintImplementableEvent, Category = Game)
		void AwardPlace(int32 NewPlace, int32 PointsGiven);

	UFUNCTION(BlueprintImplementableEvent, Category = Game)
		void AwardBonus();

	void AddScore(int32 Points);

	void SetComboPower(int32 NewComboPower);

	UFUNCTION(BlueprintPure, Category = Game)
		int32 GetComboPower();

	UFUNCTION(BlueprintPure, Category = Game)
		int32 GetMaxComboPower();

	UFUNCTION(BlueprintNativeEvent, Category = Game)
		int32 CalculateBombPower();
	virtual int32 CalculateBombPower_Implementation();

	UFUNCTION(BlueprintImplementableEvent, Category = Game)
		int32 GameWasWon(bool bGameWasWon);

	UFUNCTION(BlueprintCallable, Category = "Save Game")
		void UpdateScoresFromLeaderBoard(int32 GoldScore, int32 SilverScore, int32 BronzeScore);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FMatch3LevelSaveData SaveGameData;
	UPROPERTY(BlueprintReadOnly, Category = Score)
		int32 FinalPlace;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
		TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
		TSubclassOf<UUserWidget> DefeatWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
		TSubclassOf<UUserWidget> VictoryWidgetClass;

	UPROPERTY()
		UUserWidget* CurrentWidget;
	UPROPERTY(EditAnywhere)
		float TimeRemaining;

	FTimerHandle GameOverTimer;

	bool bGameWillBeWon;
	
};
