// Copyright Epic Games, Inc. All Rights Reserved.

#include "PTToolModule.h"

#include "EditorModeRegistry.h"
#include "PTToolEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "PTToolModule"

void FPTToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FPTToolEditorModeCommands::Register();
	   // 注册编辑器模式（使用模板中定义的静态 ID）
	FEditorModeRegistry::Get().RegisterMode<FPTToolLegacyEdMode>(
		FPTToolLegacyEdMode::EM_PTToolLegacyEdModeId,
		LOCTEXT("PTToolModeName", "PT Tool Legacy Mode"),
		FSlateIcon(),
		true // Visible
	);
}

void FPTToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FPTToolEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPTToolModule, PTTool)