# PTTool Migration and Editor Mode Registration Fix

## Overview

This PR ports the PTTool plugin from the `nicedouby/PTTool` repository into `nicedouby/New-PTTool` and applies critical fixes to ensure the PT Tool panel operates as a docked Level Editor panel (PanelTab) instead of a floating Nomad window.

## Key Changes

### 1. **File Migration**
All files from the PTTool repository have been imported and organized:
- **Core/** (18 files): Runtime classes including PTSplinePathActor, PTGameMode, PerformanceGraph, etc.
- **Editor/** (10 files): Editor-specific classes including PTToolEditorModeToolkit, Commands, Settings
- **Tools/** (4 files): Interactive tools for the editor mode
- **Private/** & **Public/**: Module and Legacy EdMode implementation

### 2. **Tab Registration Architecture** 
The most important change is moving tab registration from the EdMode to the module startup:

**Before (PTTool repo):**
- EdMode `Enter()` called `RegisterNomadTabSpawner` and created a floating window
- Each activation created duplicate windows
- Tab was not docked to Level Editor

**After (New-PTTool):**
```cpp
// In FPTToolModule::StartupModule():
FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
TSharedPtr<FTabManager> LevelEditorTabManager = LevelEditorModule.GetLevelEditorTabManager();

LevelEditorTabManager->RegisterTabSpawner(
    FPTToolLegacyEdMode::PTToolTabId,
    FOnSpawnTab::CreateRaw(this, &FPTToolModule::OnSpawnPTToolTab))
.SetDisplayName(LOCTEXT("PTToolTabTitle", "PT Tool"))
.SetMenuType(ETabSpawnerMenuType::Enabled)
.SetGroup(WorkspaceMenu::GetMenuStructure().GetDeveloperToolsMiscCategory());
```

### 3. **EdMode Enter() Behavior**
The Legacy EdMode no longer registers tabs. Instead, it invokes the existing tab:

```cpp
// In FPTToolLegacyEdMode::Enter():
FLevelEditorModule* LevelEditorModule = FModuleManager::GetModulePtr<FLevelEditorModule>("LevelEditor");
if (LevelEditorModule)
{
    TSharedPtr<FTabManager> LevelEditorTabManager = LevelEditorModule->GetLevelEditorTabManager();
    if (LevelEditorTabManager.IsValid())
    {
        Created = LevelEditorTabManager->TryInvokeTab(PTToolTabId);
    }
}
```

### 4. **PanelTab Role**
The tab is explicitly created as a `PanelTab` to ensure proper docking behavior:

```cpp
return SNew(SDockTab)
    .TabRole(ETabRole::PanelTab)  // <- Critical for docking
    [
        ToolkitWidget
    ];
```

### 5. **Build Dependencies**
Updated `PTTool.Build.cs` to include all required dependencies:
- Core modules: Core, CoreUObject, Engine
- Editor modules: Slate, SlateCore, UnrealEd, LevelEditor
- Tools: InteractiveToolsFramework, EditorInteractiveToolsFramework
- Additional: PropertyEditor, RHI, RenderCore, ApplicationCore, ImageWrapper, UMG

## File Structure

```
PTTool/
├── Core/                          # Runtime classes (18 files)
│   ├── PTSplinePathActor.cpp/h
│   ├── PTGameMode.cpp/h
│   ├── PerformanceGraph.cpp/h
│   └── ... (other core files)
├── Editor/                        # Editor classes (10 files)
│   ├── PTToolEditorModeToolkit.cpp/h
│   ├── PTToolEditorModeCommands.cpp/h
│   ├── PTToolSettingsObject.cpp/h
│   └── ... (other editor files)
├── Tools/                         # Interactive tools (4 files)
│   ├── PTToolInteractiveTool.cpp/h
│   └── PTToolSimpleTool.cpp/h
├── Private/
│   ├── PTToolModule.cpp          # Module with tab registration
│   └── PTToolLegacyEdMode.cpp    # EdMode that invokes tab
├── Public/
│   ├── PTToolModule.h
│   └── PTToolLegacyEdMode.h
└── PTTool.Build.cs               # Build configuration
```

## Testing Instructions

### Building the Plugin

1. **Copy the PTTool folder** to your Unreal Engine project's `Plugins/` directory
2. **Regenerate project files**: Right-click your .uproject → Generate Visual Studio project files
3. **Build in Visual Studio** or let the Editor compile the plugin
4. **Enable the plugin**: Editor → Edit → Plugins → Search "PTTool" → Enable

### Activating the PT Tool

1. **Open the Modes panel**: Window → Modes (or press Shift+1)
2. **Find "PT Tool Legacy Mode"** in the modes list
3. **Click to activate** the mode

### Expected Behavior

✅ **PT Tool panel appears as a docked tab** in the Level Editor (not floating)
✅ **Tab can be docked** to the left, right, top, or bottom of the editor
✅ **Multiple activations** do not create duplicate windows
✅ **Layout is persistent** across editor sessions
✅ **Transform widget (gizmo)** is enabled when mode is active

### Verification

Check the Output Log for expected messages:
```
LogPTTool: FPTToolLegacyEdMode::Enter() called
LogPTTool: Attempted to invoke tab from LevelEditor TabManager
LogPTTool: Tab successfully invoked
LogTemp: PTTool: OnSpawnPTToolTab called
```

### Common Issues

**Issue**: Tab appears as floating window
- **Solution**: Ensure you're using Unreal Engine 5.x. The PanelTab role requires modern UE versions.

**Issue**: Mode not visible in Modes panel
- **Solution**: Check that the plugin is enabled and the module loaded successfully. Look for "PTToolModule: Legacy FEdMode registered" in the Output Log.

**Issue**: Build errors about missing dependencies
- **Solution**: Verify PTTool.Build.cs includes all dependencies listed above.

## Implementation Details

### Module Lifecycle

1. **StartupModule()**: 
   - Registers FPTToolEditorModeCommands
   - Registers FPTToolLegacyEdMode with FEditorModeRegistry
   - Registers PTToolTabId with LevelEditor's TabManager

2. **ShutdownModule()**:
   - Unregisters tab spawner
   - Cleans up toolkit
   - Unregisters EdMode
   - Unregisters commands

### EdMode Lifecycle

1. **Enter()**: 
   - Calls TryInvokeTab() on LevelEditor TabManager
   - Falls back to Global TabManager if needed
   - Logs success/failure

2. **Exit()**: 
   - Calls base FEdMode::Exit()
   - Does NOT unregister tab (managed by module)

### Tab Spawning

- **OnSpawnPTToolTab()**: Creates or reuses FPTToolEditorModeToolkit
- **BuildStandaloneWidget()**: Builds the UI with Generate/Manage/Test tabs
- **Tab contains**: Spline generation tools, actor management, performance testing

## Screenshots

![PT Tool Panel - Current Implementation](image-url-here)
*The PT Tool window as it should appear - docked on the left side of the Level Editor*

![New Plugin Template Reference](image-url-here)
*Reference screenshot from the Unreal Engine New Plugin dialog*

## Technical Notes

- **EditorModeID**: `EM_PTToolLegacyEdModeId` = `"EM_PTToolLegacyEdMode"`
- **TabID**: `PTToolTabId` = `"PTToolWindow"`
- **Tab Menu Location**: Developer Tools → Misc category
- **Transform Widget**: Enabled by default via `UsesTransformWidget() { return true; }`

## Future Enhancements

- Consider migrating from FEdMode (legacy) to UEdMode (modern) in a future update
- Add toolbar buttons for quick mode activation
- Implement additional interactive tools
- Add keyboard shortcuts for common operations

## References

- Original PTTool repository: https://github.com/nicedouby/PTTool
- Unreal Engine Editor Mode documentation
- Slate UI framework documentation
