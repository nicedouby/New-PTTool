// Copyright Epic Games, Inc. All Rights Reserved.

#include "PTToolLegacyEdMode.h"
#include "LevelEditor.h"
#include "Framework/Docking/TabManager.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogPTTool, Log, All);

#define LOCTEXT_NAMESPACE "PTToolLegacyEdMode"

const FEditorModeID FPTToolLegacyEdMode::EM_PTToolLegacyEdModeId = TEXT("EM_PTToolLegacyEdMode");
const FName FPTToolLegacyEdMode::PTToolTabId = TEXT("PTToolWindow");

FPTToolLegacyEdMode::FPTToolLegacyEdMode()
{
	UE_LOG(LogPTTool, Log, TEXT("FPTToolLegacyEdMode: Constructor called"));
}

FPTToolLegacyEdMode::~FPTToolLegacyEdMode()
{
	UE_LOG(LogPTTool, Log, TEXT("FPTToolLegacyEdMode: Destructor called"));
}

void FPTToolLegacyEdMode::Enter()
{
	UE_LOG(LogPTTool, Log, TEXT("FPTToolLegacyEdMode::Enter() called"));
	
	FEdMode::Enter();

	// Try to invoke the tab from LevelEditorTabManager (or fallback to global tab manager)
	TSharedPtr<SDockTab> Created;
	
	FLevelEditorModule* LevelEditorModule = FModuleManager::GetModulePtr<FLevelEditorModule>("LevelEditor");
	if (LevelEditorModule)
	{
		TSharedPtr<FTabManager> LevelEditorTabManager = LevelEditorModule->GetLevelEditorTabManager();
		if (LevelEditorTabManager.IsValid())
		{
			Created = LevelEditorTabManager->TryInvokeTab(PTToolTabId);
			UE_LOG(LogPTTool, Log, TEXT("PTTool: Attempted to invoke tab from LevelEditor TabManager"));
		}
	}
	
	// Fallback to global tab manager if LevelEditor tab manager is not available
	if (!Created.IsValid())
	{
		Created = FGlobalTabmanager::Get()->TryInvokeTab(PTToolTabId);
		UE_LOG(LogPTTool, Log, TEXT("PTTool: Attempted to invoke tab from Global TabManager"));
	}
	
	if (!Created.IsValid())
	{
		UE_LOG(LogPTTool, Warning, TEXT("PTTool: TryInvokeTab failed or returned invalid"));
	}
	else
	{
		UE_LOG(LogPTTool, Log, TEXT("PTTool: Tab successfully invoked"));
	}
}

void FPTToolLegacyEdMode::Exit()
{
	UE_LOG(LogPTTool, Log, TEXT("FPTToolLegacyEdMode::Exit() called"));
	// No need to unregister tab spawner here - it's managed by the module
	FEdMode::Exit();
}

bool FPTToolLegacyEdMode::UsesTransformWidget() const
{
	return true;
}

bool FPTToolLegacyEdMode::UsesTransformWidget(UE::Widget::EWidgetMode CheckMode) const
{
	return true;
}

#undef LOCTEXT_NAMESPACE
