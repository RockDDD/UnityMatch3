// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PaperSprite.h"
#include "Tile.h"
#include "Grid.generated.h"

USTRUCT(BlueprintType)
struct FTileType
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Probability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInstanceConstant* TileMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ATile> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor EffectColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTileAbilities Abilities;

	FTileType()
		:Probability(1.0f)
	{

	}
};

UCLASS()
class MYUNREALMATCH_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<ATile*> GameTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTileType> TileLibrary;

	UPROPERTY(EditAnywhere, Category = Tile)
		FVector2D TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		int32 MinimumRunLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		int32 GridWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		int32 GridHeight;

	
	ATile* CreateTile(TSubclassOf<class ATile> TileToSpawn, class UMaterialInstanceConstant* TileMaterial, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID);
	
	int32 SelectTileFromLibrary();

	ATile* GetTileFromGridAddress(int32 GridAddress) const;

	UFUNCTION(BlueprintCallable, Category = Initialization)
		void InitGrid();

	UFUNCTION(BlueprintImplementableEvent, meta = (ExpandEnumExecs = "MoveType"), Category = Tile)
		void OnMoveMade(EMatch3MoveType::Type MoveType);

	UFUNCTION(BlueprintCallable, Category = Audio)
		void ReturnMatchSounds(TArray<USoundWave*>& MatchSound);

	UFUNCTION(BlueprintCallable, Category = Tile)
		FVector GetLocationFromGridAddress(int32 GridAddress) const;

	FVector GetLocationFromGridAddressWithOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles) const;

	UFUNCTION(BlueprintCallable, Category = Tile)
		bool GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32& ReturnGridAddress) const;

	bool AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB) const;

	void OnTileFinishedFalling(ATile* Tile, int32 LandingGridAddress);

	void OnTileFinishedMatching(ATile* InTile);

	void OnSwapDisplayFinished(ATile* InTile);

	void RespawnTiles();

	void SwapTiles(ATile* A, ATile* B, bool bRepositionTileActor = false);

	bool IsMoveLegal(ATile* A, ATile* B);

	TArray<ATile*> GetExplostionList(ATile* A) const;

	TArray<ATile* >FindNeighbors(ATile* StartingTile, bool bMustMatchID = true,int32 RunLength=-1) const;

	TArray<ATile*> FindTileOfType(int32 TileTypeID) const;

	void ExecuteMatch(const TArray<ATile*>& MatchingTiles);

	void OnTileWasSelected(ATile* NewSelectedTile);

	bool IsUnwinnable();

	void SetLastMove(EMatch3MoveType::Type MoveType);

	EMatch3MoveType::Type GetLastMove();

	UFUNCTION(BlueprintNativeEvent, Category = Game)
		int32 GetScoreMultiplierForMove(EMatch3MoveType::Type LastMoveType);
	virtual int32 GetScoreMultiplierForMove_Implementation(EMatch3MoveType::Type LastMoveType);

	ATile* GetCurrentlySelectedTile() const { return CurrentlySelectedTile; }


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile)
		ATile* CurrentlySelectedTile;

private:
	TArray<ATile*> LastLegalMatch;
	TArray<ATile*> FallingTiles;
	TArray<ATile*> SwappingTiles;
	TArray<ATile*> TilesToCheck;
	TArray<ATile*> TilesBeingDestroyed;
	TMap<APlayerController*, EMatch3MoveType::Type> LastMoves;
	uint32 bPendingSwapMove : 1;
	uint32 bPendingSwapMoveSuccess : 1;
};
