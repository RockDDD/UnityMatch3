// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUnrealMatch.h"
#include "Match3BlueprintFunctionLibrary.h"
#include "MyUnrealMatchGameModeBase.h"

APlayerController* UMatch3BlueprintFunctionLibrary::GetLocalPlayerController(UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			if (PlayerController->IsLocalController())
			{
				return PlayerController;
			}
		}
	}
	return nullptr;
}

FString UMatch3BlueprintFunctionLibrary::GetOnlineAccountID(APlayerController* PlayerController)
{
	if (PlayerController && PlayerController->PlayerState&& PlayerController->PlayerState->UniqueId->IsValid())
	{
		return PlayerController->PlayerState->UniqueId->GetHexEncodedString();
	}
	return FString();
}

bool UMatch3BlueprintFunctionLibrary::IsGameActive(UObject* WorldContextObject)
{
	if (AMyUnrealMatchGameModeBase* GameMode = Cast<AMyUnrealMatchGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		if (GameMode->IsGameActive())
		{
			return true;
		}
	}
	return false;
}

void UMatch3BlueprintFunctionLibrary::PauseGameTimer(UObject* WorldContextObject, bool bPuse)
{
	if (AMyUnrealMatchGameModeBase* GameMode = Cast<AMyUnrealMatchGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		GameMode->PauseGameTimer(bPuse);
	}
}