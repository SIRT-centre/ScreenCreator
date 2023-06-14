// Adapted from Unreal's Procedural Mesh Component Details

#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "IDetailCustomization.h"
#include "Types/SlateEnums.h"
#include "ScreenCreatorComponent.h"

class IDetailLayoutBuilder;
class UScreenCreatorComponent;

class FScreenCreatorComponentDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails( IDetailLayoutBuilder& DetailBuilder ) override;

	/** Handle clicking the convert button */
	FReply OnCreateNextScreenClicked();

	FReply OnCopyPositionDataClicked();
	FReply OnCopyRotationDataClicked();

	/** Is the convert button enabled */
	bool CreateNextWarpMeshEnabled() const;

	/** Util to get the ProcMeshComp we want to convert */
	UScreenCreatorComponent* GetFirstSelectedScreenCreatorComponent() const;

	/** Cached array of selected objects */
	TArray< TWeakObjectPtr<UObject> > SelectedObjectsList;

	void OnSetDelta(float NewValue, ETextCommit::Type CommitInfo, int32 Axis)
	{
		
	}
};