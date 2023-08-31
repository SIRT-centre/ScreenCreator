// Copyright 2023 Sheridan College

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ScreenCreatorComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class SCREENCREATOR_API UScreenCreatorComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
public:
	UScreenCreatorComponent(const FObjectInitializer& PCIP);
	virtual void BeginPlay();

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

	void GenerateWarpMesh();

	UFUNCTION(CallInEditor, Category = "Screen Creator")
	void AddNewWarpMesh();

	UPROPERTY(EditAnywhere, Category = "Screen Creator")
	float tileWidth;
	UPROPERTY(EditAnywhere, Category = "Screen Creator")
	float tileHeight;
	UPROPERTY(EditAnywhere, Category = "Screen Creator")
	int columns;
	UPROPERTY(EditAnywhere, Category = "Screen Creator")
	int rows;
	UPROPERTY(EditAnywhere, Category = "Screen Creator")
	float rotationAngle;
	UPROPERTY(EditAnywhere, Category = "Screen Creator")
	bool doubleSided;

	UPROPERTY(EditAnywhere, Category = "Screen Creator")
	UMaterialInterface* screenMaterial;

	FVector nextColumnPosition;

	UPROPERTY(BlueprintReadOnly, Category = "Screen Creator")
	FVector nextPosition;
	UPROPERTY(BlueprintReadOnly, Category = "Screen Creator")
	FRotator nextRotation;

	FVector storedLastColumn;
	
	FVector CalculateEndPosition();
	FRotator CalculateEndRotation();

private:
	bool generateWall;

};
