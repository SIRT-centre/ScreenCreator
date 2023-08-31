// Copyright 2023 Sheridan College

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "ScreenCreatorComponent.h"
#include "IScreenCreatorEditorPlugin.h"
#include "ScreenCreatorComponentDetails.h"




class FScreenCreatorEditorPlugin : public IScreenCreatorEditorPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FScreenCreatorEditorPlugin, ProcWarpMeshEditor )



void FScreenCreatorEditorPlugin::StartupModule()
{
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomClassLayout(UScreenCreatorComponent::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FScreenCreatorComponentDetails::MakeInstance));
	}
}


void FScreenCreatorEditorPlugin::ShutdownModule()
{
	
}



