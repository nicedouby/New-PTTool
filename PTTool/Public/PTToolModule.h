// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Widgets/Docking/SDockTab.h"

class FPTToolEditorModeToolkit;

/**
 * This is the module definition for the editor mode. You can implement custom functionality
 * as your plugin module starts up and shuts down. See IModuleInterface for more extensibility options.
 */
class FPTToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** Get the toolkit widget for the PTTool tab */
	TSharedPtr<FPTToolEditorModeToolkit> GetToolkit() const { return Toolkit; }

private:
	/** Callback to spawn the PTTool tab */
	TSharedRef<SDockTab> OnSpawnPTToolTab(const class FSpawnTabArgs& Args);

	/** The toolkit instance used by the tab */
	TSharedPtr<FPTToolEditorModeToolkit> Toolkit;
};
