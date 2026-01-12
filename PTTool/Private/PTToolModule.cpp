// Copyright Epic Games, Inc. All Rights Reserved.

#include "PTToolModule.h"

#include "EditorModeRegistry.h"
#include "PTToolEditorMode.h"

#define LOCTEXT_NAMESPACE "PTToolModule"

void FPTToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FEditorModeRegistry::Get().RegisterMode<FPTToolEditorMode>(
		FPTToolEditorMode::EM_PTToolLegacyEdModeId,
		LOCTEXT("PTToolModeName", "PT Tool Legacy Mode"),
		FSlateIcon(),
		true // Visible
	);
}

void FPTToolModule::ShutdownModule()
{
	FEditorModeRegistry::Get().UnregisterMode(FPTToolEditorMode::EM_PTToolLegacyEdModeId);
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FPTToolModule, PTTool)