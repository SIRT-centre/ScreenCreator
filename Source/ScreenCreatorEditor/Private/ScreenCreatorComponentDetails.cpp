// Copyright 2023 Sheridan College

#include "ScreenCreatorComponentDetails.h"
#include "ProceduralMeshConversion.h"
#include "Modules/ModuleManager.h"
#include "Misc/PackageName.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Application/SlateWindowHelper.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Engine/StaticMesh.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "DetailCategoryBuilder.h"
#include "IDetailsView.h"
#include "ScreenCreatorComponent.h"
#include "ProceduralMeshComponent.h"
#include "StaticMeshAttributes.h"
#include "PhysicsEngine/BodySetup.h"
#include "Dialogs/DlgPickAssetPath.h"
#include "AssetRegistryModule.h"
#include "UnrealEdGlobals.h"
#include "Widgets/Input/SVectorInputBox.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Editor/UnrealEdEngine.h"

#define LOCTEXT_NAMESPACE "ProcWarpMeshComponentDetails"

TSharedRef<IDetailCustomization> FScreenCreatorComponentDetails::MakeInstance()
{
	return MakeShareable(new FScreenCreatorComponentDetails);
}

void FScreenCreatorComponentDetails::CustomizeDetails( IDetailLayoutBuilder& DetailBuilder )
{
	IDetailCategoryBuilder& ProcMeshCategory = DetailBuilder.EditCategory("Screen Creator");

	// Cache set of selected things
	SelectedObjectsList = DetailBuilder.GetSelectedObjects();

	UScreenCreatorComponent* component = nullptr;
	component = GetFirstSelectedScreenCreatorComponent();
	if (component != nullptr)
	{
		FVector position = component->CalculateEndPosition();
		FString positionString = position.ToString();
		FText positionText = FText::FromString(positionString);

		FString positionNameString = "Next Position";
		FText positionNameText = FText::FromString(positionNameString);

		FRotator rotation = component->nextRotation;
		FString rotationString = rotation.ToString();
		FText rotationText = FText::FromString(rotationString);

		FString rotationNameString = "Next Rotation";
		FText rotationNameText = FText::FromString(rotationNameString);

		// NOTE: Prior implementation to display the vector values of the Next Position and Next Rotation for a given Screen
		//ProcMeshCategory.AddCustomRow(LOCTEXT("NextPosition", "Next Position"), false)
		//	.NameContent()
		//	[
		//		SNew(STextBlock)
		//		.Text(positionNameText)
		//		
		//	]
		//.ValueContent()
		//	.VAlign(VAlign_Center)
		//	.MaxDesiredWidth(250)
		//	[
		//		SNew(SVectorInputBox)
		//		.X(position.X)
		//		.Y(position.Y)
		//		.Z(position.Z)
		//		.bColorAxisLabels(true)
		//	];

		//ProcMeshCategory.AddCustomRow(LOCTEXT("NextRotation", "Next Rotation"), false)
		//	.NameContent()
		//	[
		//		SNew(STextBlock)
		//		.Text(rotationNameText)
		//	]
		//.ValueContent()
		//	.VAlign(VAlign_Center)
		//	.MaxDesiredWidth(250)
		//	[
		//		SNew(SVectorInputBox)
		//		.X(rotation.X)
		//		.Y(rotation.Y)
		//		.Z(rotation.Z)
		//		.bColorAxisLabels(true)
		//	];

		ProcMeshCategory.AddCustomRow(LOCTEXT("CopyButtons", ""), false)
			.NameContent()
			[
				SNew(SButton)
				.VAlign(VAlign_Center)
			.ToolTipText(LOCTEXT("CopyPosition", "Copy the next position values."))
			.OnClicked(this, &FScreenCreatorComponentDetails::OnCopyPositionDataClicked)
			.IsEnabled(this, &FScreenCreatorComponentDetails::CreateNextWarpMeshEnabled)
			.Content()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("CopyPosition", "Copy Next Position"))
			]
			]
		.ValueContent()
			.VAlign(VAlign_Center)
			.MaxDesiredWidth(250)
			[
				SNew(SButton)
				.VAlign(VAlign_Center)
			.ToolTipText(LOCTEXT("CopyPosition", "Copy the next Rotation values."))
			.OnClicked(this, &FScreenCreatorComponentDetails::OnCopyRotationDataClicked)
			.IsEnabled(this, &FScreenCreatorComponentDetails::CreateNextWarpMeshEnabled)
			.Content()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("CopyPosition", "Copy Next Rotation"))
			]
			];
	}

	//DetailBuilder.ForceRefreshDetails();

}

UScreenCreatorComponent* FScreenCreatorComponentDetails::GetFirstSelectedScreenCreatorComponent() const
{
	// Find first selected valid ScreenCreatorComponent
	UScreenCreatorComponent* screenCreatorComp = nullptr;
	for (const TWeakObjectPtr<UObject>& Object : SelectedObjectsList)
	{
		UScreenCreatorComponent* TestScreenComp = Cast<UScreenCreatorComponent>(Object.Get());
		// See if this one is good
		if (TestScreenComp != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Valid Actor"));
			screenCreatorComp = TestScreenComp;
			break;
		}
		else 
		{
			//UE_LOG(LogTemp, Warning, TEXT("Invalid Actor"));
		}
	}

	return screenCreatorComp;
}

bool FScreenCreatorComponentDetails::CreateNextWarpMeshEnabled() const
{
	return GetFirstSelectedScreenCreatorComponent() != nullptr;
}

FReply FScreenCreatorComponentDetails::OnCreateNextScreenClicked()
{
	// Find first selected ScreenCreatorComponent
	UScreenCreatorComponent* screenCreatorComp = GetFirstSelectedScreenCreatorComponent();
	if (screenCreatorComp != nullptr)
	{
		// NOTE: Currently unusable due to asset editor properties not properly obtaining parent objects for attaching

		//UScreenCreatorComponent* meshToBuildFrom = screenCreatorComp;
		//for (size_t i = 0; i < screenCreatorComp->additionalWallCount; i++)
		//{
		//	// Create component
		//	UScreenCreatorComponent* warpMesh = NewObject<UScreenCreatorComponent>(meshToBuildFrom);
		//	screenCreatorComp->nextComponents.Add(warpMesh);

		//	warpMesh->CreationMethod = EComponentCreationMethod::Instance;
		//	warpMesh->bEditableWhenInherited = true;
		//	warpMesh->RegisterComponent();
		//	warpMesh->AttachToComponent(meshToBuildFrom, FAttachmentTransformRules::KeepWorldTransform);

		//	// Setting up parent attachment
		//	//warpMesh->SetupAttachment(screenCreatorComp);
		//	//screenCreatorComp->GetOwner()->AddInstanceComponent(warpMesh);

		//	//warpMesh->RegisterComponent();

		//	// Copying attributes from current proc warp mesh to new one
		//	warpMesh->tileHeight = screenCreatorComp->tileHeight;
		//	warpMesh->tileWidth = screenCreatorComp->tileWidth;
		//	warpMesh->columns = screenCreatorComp->columns;
		//	warpMesh->rows = screenCreatorComp->rows;
		//	warpMesh->rotationAngle = screenCreatorComp->rotationAngle;

		//	// Set position to end of the wall
		//	warpMesh->SetWorldLocation(meshToBuildFrom->CalculateEndPosition());
		//	warpMesh->SetRelativeRotation(FRotator(0, 0, 0));
		//	warpMesh->AddRelativeRotation(meshToBuildFrom->CalculateEndRotation());

		//	meshToBuildFrom = warpMesh;

		//	warpMesh->GenerateWarpMesh(); 
		//}
	}

	return FReply::Handled();
}

FReply FScreenCreatorComponentDetails::OnCopyPositionDataClicked() 
{
	UScreenCreatorComponent* ProcMeshComp = GetFirstSelectedScreenCreatorComponent();
	if (ProcMeshComp != nullptr)
	{
		FVector position = ProcMeshComp->CalculateEndPosition();
		FString positionString = position.ToString();

		FPlatformApplicationMisc::ClipboardCopy(*positionString);
	}

	return FReply::Handled();
}

FReply FScreenCreatorComponentDetails::OnCopyRotationDataClicked() 
{
	UScreenCreatorComponent* ProcMeshComp = GetFirstSelectedScreenCreatorComponent();
	if (ProcMeshComp != nullptr)
	{
		FRotator rotation = ProcMeshComp->CalculateEndRotation();
		FString rotationString = rotation.ToString();

		FPlatformApplicationMisc::ClipboardCopy(*rotationString);
	}

	return FReply::Handled();
}


#undef LOCTEXT_NAMESPACE
