// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUnrealMatch.h"
#include "Match3GameInstance.h"




UMatch3GameInstance::UMatch3GameInstance()
{

}

void UMatch3GameInstance::Init()
{
	ViewportHandle = FViewport::ViewportResizedEvent.AddUObject(this, &UMatch3GameInstance::OnViewportResize_Internal);
	Super::Init();
}

void UMatch3GameInstance::Shutdown()
{
	FViewport::ViewportResizedEvent.Remove(ViewportHandle);
	Super::Shutdown();
}

void UMatch3GameInstance::OnViewportResize_Internal(FViewport* Viewport, uint32 ID)
{
	OnViewportResize();
}
