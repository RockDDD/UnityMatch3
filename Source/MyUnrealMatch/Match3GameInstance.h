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

	// Internal function to bind to viewport resizing delegate
	void OnViewportResize_Internal(FViewport* Viewport, uint32 ID);

	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void OnViewportResize();

private:
	FDelegateHandle ViewportHandle;
};
