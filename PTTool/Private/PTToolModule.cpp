// Copyright Epic Games, Inc. All Rights Reserved.

#include "PTToolModule.h"

#include "EditorModeRegistry.h"
#include "PTToolLegacyEdMode.h"
#include "Editor/PTToolEditorModeCommands.h"
#include "Editor/PTToolEditorModeToolkit.h"
#include "LevelEditor.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "PTToolModule"

void FPTToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Register editor mode commands
	FPTToolEditorModeCommands::Register();

	// Register the Legacy EditorMode
	FEditorModeRegistry::Get().RegisterMode<FPTToolLegacyEdMode>(
		FPTToolLegacyEdMode::EM_PTToolLegacyEdModeId,
		LOCTEXT("PTToolLegacyModeName", "PT Tool Legacy Mode"),
		FSlateIcon(),
		true // Visible
	);

	// Register the PTTool tab with LevelEditorTabManager
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FTabManager> LevelEditorTabManager = LevelEditorModule.GetLevelEditorTabManager();
	
	if (LevelEditorTabManager.IsValid() && !LevelEditorTabManager->HasTabSpawner(FPTToolLegacyEdMode::PTToolTabId))
	{
		LevelEditorTabManager->RegisterTabSpawner(
			FPTToolLegacyEdMode::PTToolTabId,
			FOnSpawnTab::CreateRaw(this, &FPTToolModule::OnSpawnPTToolTab))
		.SetDisplayName(LOCTEXT("PTToolTabTitle", "PT Tool"))
		.SetMenuType(ETabSpawnerMenuType::Enabled)
		.SetGroup(WorkspaceMenu::GetMenuStructure().GetDeveloperToolsMiscCategory());
	}
}

void FPTToolModule::ShutdownModule()
{
	// Unregister the tab spawner
	if (FModuleManager::Get().IsModuleLoaded("LevelEditor"))
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
		TSharedPtr<FTabManager> LevelEditorTabManager = LevelEditorModule.GetLevelEditorTabManager();
		
		if (LevelEditorTabManager.IsValid() && LevelEditorTabManager->HasTabSpawner(FPTToolLegacyEdMode::PTToolTabId))
		{
			LevelEditorTabManager->UnregisterTabSpawner(FPTToolLegacyEdMode::PTToolTabId);
		}
	}

	// Clean up toolkit
	Toolkit.Reset();

	// Unregister the EditorMode
	FEditorModeRegistry::Get().UnregisterMode(FPTToolLegacyEdMode::EM_PTToolLegacyEdModeId);

	// Unregister commands
	FPTToolEditorModeCommands::Unregister();
}

TSharedRef<SDockTab> FPTToolModule::OnSpawnPTToolTab(const FSpawnTabArgs& Args)
{
	UE_LOG(LogTemp, Log, TEXT("PTTool: OnSpawnPTToolTab called"));

	// Create the toolkit if it doesn't exist
	if (!Toolkit.IsValid())
	{
		Toolkit = MakeShareable(new FPTToolEditorModeToolkit);
	}

	// Build the standalone widget
	TSharedRef<SWidget> ToolkitWidget = Toolkit->BuildStandaloneWidget();

	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			ToolkitWidget
		];
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FPTToolModule, PTTool)
