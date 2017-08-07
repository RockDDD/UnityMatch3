// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperSpriteActor.h"
#include "PaperSpriteComponent.h"
#include "Tile.generated.h"


UENUM()
namespace ETileState
{
	enum Type
	{
		ETS_Normal,
		ETS_Falling,
		ETS_PendingDelete
	};
}


UENUM()
namespace EMatch3MoveType
{
	enum Type
	{
		MT_None,
		MT_Failure,
		MT_Standard,
		MT_MoreTiles,
		MT_Combo,
		MT_Bomb,
		MT_AllTheBombs,
		MT_MAX
	};
}

USTRUCT()
struct FTileAbilities
{
	GENERATED_USTRUCT_BODY();
	
	bool CanExplode() { return bExplodes; }
	bool CanSwap() { return (!bPreventSwapping && !bExplodes); }


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bExplodes : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bPreventSwapping : 1;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 BombPower;
};
/**
 * 
 */
UCLASS()
class MYUNREALMATCH_API ATile : public APaperSpriteActor
{
	GENERATED_BODY()
	
public:
	ATile();
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	/*When a Tile is Touched*/
	UFUNCTION()
	void TilePress(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	UFUNCTION()
	void TileEnter(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	UFUNCTION()
	void TilePress_Mouse(AActor* ClickedActor, FKey ButtonClicked);

	UFUNCTION()
	void TileEnter_Mouse(AActor* MouseOverActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "SpecialGameEvents")
	void PlayeSelectionEffect(bool bTurnEffectOn);

	UFUNCTION(BlueprintImplementableEvent, Category = "SpecialGameEvents")
	void StartFallingEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "SpecialGameEvents")
	void StopFallingEffect();

	UFUNCTION(BlueprintNativeEvent, Category = "SpecialGameEvents")
	void SetTileMaterial(class UMaterialInstanceConstant* TileMaterial);
	virtual void SetTileMaterial_Implementation(class UMaterialInstanceConstant* TileMaterial);

	UFUNCTION(BlueprintNativeEvent, Category = "SpecialGameEvents")
	void OnMatched(EMatch3MoveType::Type MoveType);
	virtual void OnMatched_Implementation(EMatch3MoveType::Type MoveType);

	UFUNCTION(BlueprintNativeEvent, Category = "SpecialGameEvents")
		void OnSwapMove(ATile* OtherTile, bool bMoveWillSucceed);
	virtual void OnSwapMove_Implementation(ATile* OtherTile, bool bMoveWillSucceed);

	void StartFalling(bool bUseCurrentWorldLocation = false);

	USoundWave* GetMatchSound();

	UFUNCTION()
	void TickFalling();

	void FinishedFalling();

	void SetGridAddress(int32 NewLocation);
	int32 GetGridAddress() const;

	UPROPERTY(BlueprintReadOnly)
		int32 TileTypeID;

	UPROPERTY()
		TEnumAsByte<ETileState::Type> TileState;

	UPROPERTY(BlueprintReadOnly)
		FTileAbilities Abilities;

protected:
	float TotalFallingTime;
	float FallingStartTime;
	FVector FallingStartLocation;
	FVector FallingEndLocation;
	FTimerHandle TickFallingHandle;

	UPROPERTY(BlueprintReadOnly, Category = Tile)
	int32 GridAddress;

	int32 LandingGridAddress;
	UPROPERTY(BlueprintReadOnly,Category = Tile)
	class AGrid* Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, Meta = (BlueprintProtected = True))
	USoundWave* MatchSound;
			
};
