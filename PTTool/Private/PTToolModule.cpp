// Copyright Epic Games, Inc. All Rights Reserved.

#include "PTToolModule.h"

#include "EditorModeRegistry.h"
#include "PTToolEditorMode.h"
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

	// Register the EditorMode
	FEditorModeRegistry::Get().RegisterMode<FPTToolEditorMode>(
		FPTToolEditorMode::EM_PTToolLegacyEdModeId,
		LOCTEXT("PTToolModeName", "PT Tool Legacy Mode"),
		FSlateIcon(),
		true // Visible
	);

	// Register the PTTool tab with LevelEditorTabManager
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FTabManager> LevelEditorTabManager = LevelEditorModule.GetLevelEditorTabManager();
	
	if (LevelEditorTabManager.IsValid() && !LevelEditorTabManager->HasTabSpawner(FPTToolEditorMode::PTToolTabId))
	{
		LevelEditorTabManager->RegisterTabSpawner(
			FPTToolEditorMode::PTToolTabId,
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
		
		if (LevelEditorTabManager.IsValid() && LevelEditorTabManager->HasTabSpawner(FPTToolEditorMode::PTToolTabId))
		{
			LevelEditorTabManager->UnregisterTabSpawner(FPTToolEditorMode::PTToolTabId);
		}
	}

	// Unregister the EditorMode
	FEditorModeRegistry::Get().UnregisterMode(FPTToolEditorMode::EM_PTToolLegacyEdModeId);
}

TSharedRef<SDockTab> FPTToolModule::OnSpawnPTToolTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock).Text(LOCTEXT("PTToolContent", "PT Tool Content"))
			]
		];
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FPTToolModule, PTTool)