// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUnrealMatch.h"
#include "Match3SaveGame.h"


bool UMatch3SaveGame::LoadCustomInt(FString FieldName, int32& Value)
{
	const int32* ValuePointer = Match3CustomData.Find(FieldName);
	if (ValuePointer != nullptr)
	{
		Value = *ValuePointer;
		return true;
	}
	return false;
}

void UMatch3SaveGame::SaveCustomInt(FString FieldName, int32 Value)
{
	Match3CustomData.FindOrAdd(FieldName) = Value;
}

void UMatch3SaveGame::ClearCustomInt(FString FieldName)
{
	Match3CustomData.Remove(FieldName);
}



