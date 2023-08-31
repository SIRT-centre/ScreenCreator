// Copyright 2023 Sheridan College

#include "ScreenCreatorComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Algo/Reverse.h"

#if WITH_EDITOR
	#include "UnrealEdGlobals.h"
	#include "Editor/UnrealEdEngine.h"
#endif


UScreenCreatorComponent::UScreenCreatorComponent(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	bAutoActivate = true;
	//bAutoRegister = true;
	//PrimaryComponentTick.bStartWithTickEnabled = true;
	//PrimaryComponentTick.TickGroup = TG_PrePhysics;
	//SetMobility(EComponentMobility::Movable);
	//SetupActorComponentTickFunction(&PrimaryComponentTick);
}

void UScreenCreatorComponent::BeginPlay() {
	GenerateWarpMesh();
}

void UScreenCreatorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UScreenCreatorComponent::GenerateWarpMesh() {

	ClearAllMeshSections();

	nextColumnPosition = FVector();
	float x;
	float z;

	TArray<FVector> vertices;
	TArray<FVector2D> uvs;
	TArray<int> triangles;
	TArray<int> backwardsTriangles;
	TArray<FVector> normals;
	TArray<FVector> backwardsNormals;
	TArray<FProcMeshTangent> tangents;
	TArray<FProcMeshTangent> backwardsTangents;


	for (int i = 0; i <= columns; i++) {

		for (int g = 0; g <= rows; g++)
		{
			z = g * tileHeight;

			FVector vertex = FVector(nextColumnPosition.X, nextColumnPosition.Y, z);
			vertices.Add(vertex);

			float u = (float)i / (float)columns;
			float v = 1 - ((float)g / (float)rows);

			FVector2D uv = FVector2D(u, v);
			uvs.Add(uv);
		}

		x = nextColumnPosition.X + tileWidth;

		float rotX = (UKismetMathLibrary::DegCos(i * rotationAngle) * tileWidth) + nextColumnPosition.X;
		float rotY = (UKismetMathLibrary::DegSin(i * rotationAngle) * tileWidth) + nextColumnPosition.Y;

		nextColumnPosition.X = rotX;
		nextColumnPosition.Y = rotY;
	}

	TArray<FColor> emptyArray;

	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(columns + 1, rows + 1, true, triangles);
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, triangles, uvs, normals, tangents);
	CreateMeshSection(0, vertices, triangles, normals, uvs, emptyArray, tangents, true);
	SetMaterial(0, screenMaterial);

	if (doubleSided)
	{
		UKismetProceduralMeshLibrary::CreateGridMeshTriangles(columns + 1, rows + 1, false, backwardsTriangles);
		UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, backwardsTriangles, uvs, backwardsNormals, backwardsTangents);
		CreateMeshSection(1, vertices, backwardsTriangles, backwardsNormals, uvs, emptyArray, backwardsTangents, true);
		SetMaterial(1, screenMaterial);
	}

	nextPosition = CalculateEndPosition();
	nextRotation = CalculateEndRotation(); 

	TArray<USceneComponent*> foundChildren;
	GetChildrenComponents(false, foundChildren);
	
	for (size_t i = 0; i < foundChildren.Num(); i++)
	{
		UScreenCreatorComponent* validComponent = Cast<UScreenCreatorComponent>(foundChildren[i]);
		if (IsValid(validComponent))
		{
			validComponent->SetRelativeLocation(CalculateEndPosition());
			validComponent->SetRelativeRotation(CalculateEndRotation());
			validComponent->GenerateWarpMesh();
		}
	}
	
}

void UScreenCreatorComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	//Super::PostEditChangeProperty(PropertyChangedEvent);
	GenerateWarpMesh();
	generateWall = true;
}

void UScreenCreatorComponent::AddNewWarpMesh()
{

	UScreenCreatorComponent* warpMesh = NewObject<UScreenCreatorComponent>(this);

	warpMesh->CreationMethod = EComponentCreationMethod::Instance;
	warpMesh->bEditableWhenInherited = true;
	warpMesh->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

	// Setting up parent attachment
	warpMesh->SetupAttachment(this);
	this->GetOwner()->AddInstanceComponent(warpMesh);
	this->GetOwner()->AddOwnedComponent(warpMesh);
	warpMesh->RegisterComponent();

	// Copying attributes from current proc warp mesh to new one
	warpMesh->tileHeight = tileHeight;
	warpMesh->tileWidth = tileWidth;
	warpMesh->columns = columns;
	warpMesh->rows = rows;
	warpMesh->rotationAngle = rotationAngle;

	// Set position to end of the wall
	warpMesh->SetRelativeLocation(CalculateEndPosition());
	warpMesh->SetRelativeRotation(FRotator(0, 0, 0));
	warpMesh->AddRelativeRotation(FRotator(0, columns * rotationAngle, 0));

	warpMesh->GenerateWarpMesh();

#if WITH_EDITOR
	GUnrealEd->UpdateFloatingPropertyWindows();
	GUnrealEd->EditorUpdateComponents();
#endif
}

FVector UScreenCreatorComponent::CalculateEndPosition()
{
	FVector endPosition;

	for (int i = 0; i < columns; i++) {

		float rotX = (UKismetMathLibrary::DegCos(i * rotationAngle) * tileWidth) + endPosition.X;
		float rotY = (UKismetMathLibrary::DegSin(i * rotationAngle) * tileWidth) + endPosition.Y;

		endPosition.X = rotX;
		endPosition.Y = rotY;
		endPosition.Z = 0;
	}

	return endPosition;
}

FRotator UScreenCreatorComponent::CalculateEndRotation() 
{
	return FRotator(0, columns * rotationAngle, 0);
}