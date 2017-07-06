// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "Match3GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYUNREALMATCH_API UMatch3GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMatch3GameInstance();
	void Init() override;
	void Shutdown() override;
	void InitSaveGameSlot();
	bool FindSaveDataFroLevel(UObject* WorldContextObject, struct FMatch3LevelSaveData& OutSaveData);
	UFUNCTION(BlueprintCallable, Category = "Saved Game")
		void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Saved Game")
		bool LoadCustomInt(FString FieldName, int32& Value);

	UFUNCTION(BlueprintCallable, Category = "Saved Game")
		void SaveCustomInt(FString FieldName, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Saved Game")
		void ClearCustomInt(FString FieldName);

	void UpdateSave(UObject* WorldContextObject, struct FMatch3LevelSaveData& NewData);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateUIAfterSave();

	UPROPERTY()
	class UMatch3SaveGame* InstanceGameData;

	UFUNCTION(BlueprintCallable, Category = "Online")
		void RegisterOnlineID(FString NewOnlineID);

	UFUNCTION(BlueprintImplementableEvent, Category = "Online")
		void OnLoginChanged(bool bLoggingIn, int32 UserID, int32 UserIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "Online")
		void OnEnteringForeground();

	UFUNCTION(BlueprintImplementableEvent, Category = "Online")
		void OnEnteringBackground();

	// Internal function to bind to viewport resizing delegate
	void OnViewportResize_Internal(FViewport* Viewport, uint32 ID);

	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void OnViewportResize();

protected:
	FString GetSaveSlotName() const;
	FString SaveGamePrefix;
	FString DefaultSaveGameSlot;
private:
	FDelegateHandle LoginChangedHandle;
	FDelegateHandle EnteringForegroundHandle;
	FDelegateHandle EnteringBackgroundHandle;
	FDelegateHandle ViewportHandle;
};
