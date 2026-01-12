#include "PTToolEditorMode.h"

#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogPTTool, Log, All);

// 静态模式 ID（与注册处一致）
const FName FPTToolEditorMode::EM_PTToolLegacyEdModeId = TEXT("EM_PTToolEditorMode");

// Tab ID（打开/注册时使用）
static const FName PTToolTabId("PTToolWindow");

FPTToolEditorMode::FPTToolEditorMode()
    : FEdMode()
{
    UE_LOG(LogPTTool, Log, TEXT("PTTool: Constructor"));
}

FPTToolEditorMode::~FPTToolEditorMode()
{
    UE_LOG(LogPTTool, Log, TEXT("PTTool: Destructor"));
}

void FPTToolEditorMode::Enter()
{
    FEdMode::Enter();
    UE_LOG(LogPTTool, Log, TEXT("PTTool: Enter"));

    // 使用全局��理器注册 Nomad tab（若你更想绑定到 ToolkitHost，请在 Toolkit::Init 中注册）
    if (FGlobalTabmanager::Get()->HasTabSpawner(PTToolTabId) == false)
    {
        FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
            PTToolTabId,
            FOnSpawnTab::CreateRaw(this, &FPTToolEditorMode::SpawnPTToolTab))
        .SetDisplayName(NSLOCTEXT("PTTool", "PTToolTabTitle", "PT Tool"))
        .SetMenuType(ETabSpawnerMenuType::Enabled) // Enabled -> 出现在 Window 菜单
        .SetGroup(WorkspaceMenu::GetMenuStructure().GetDeveloperToolsMiscCategory());
    }

    // 显示 Tab（尝试打开）
    TSharedPtr<SDockTab> Created = FGlobalTabmanager::Get()->TryInvokeTab(PTToolTabId);
    if (!Created.IsValid())
    {
        UE_LOG(LogPTTool, Warning, TEXT("PTTool: TryInvokeTab failed or returned invalid"));
    }

#ifdef PTTOOL_FORCE_DOCK_LEFT
    // ============================================================================
    // OPTIONAL FEATURE: Force PTTool to dock on the left side (like Landscape Editor)
    // ============================================================================
    // 
    // When PTTOOL_FORCE_DOCK_LEFT is defined, this section attempts to dock the 
    // PTTool tab to the left side of the editor viewport, similar to how the 
    // Landscape Editor is positioned.
    //
    // CURRENT IMPLEMENTATION:
    // This is a placeholder that logs the intent. The actual docking behavior 
    // is not modified yet to maintain compatibility and safety without local 
    // UE environment validation.
    //
    // TODO - FUTURE IMPLEMENTATION:
    // To implement actual left-side docking, the following steps are recommended:
    //
    // 1. Obtain the Level Editor's TabManager:
    //    TSharedPtr<FTabManager> LevelEditorTabManager = 
    //        FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor")
    //        .GetLevelEditorTabManager();
    //
    // 2. Use InsertNewDocumentTab or similar API to force docking position:
    //    if (LevelEditorTabManager.IsValid() && Created.IsValid())
    //    {
    //        // Example approach (needs verification with target UE version):
    //        TSharedPtr<SDockingArea> DockArea = LevelEditorTabManager->GetLeftDockArea();
    //        if (DockArea.IsValid())
    //        {
    //            // Insert the tab into the left dock area
    //            // Note: Exact API may vary by UE version (4.27, 5.0, 5.1, etc.)
    //            LevelEditorTabManager->InsertNewDocumentTab(
    //                PTToolTabId,
    //                FTabManager::ESearchPreference::RequireClosedTab,
    //                Created.ToSharedRef()
    //            );
    //        }
    //    }
    //
    // 3. Alternative approach using DrawAttention or SetTabLocation:
    //    Created->DrawAttention();
    //    // Or use platform-specific dock positioning APIs
    //
    // IMPORTANT NOTES:
    // - The exact API and approach depend on the target Unreal Engine version
    // - This must be tested in a local UE environment before deployment
    // - Consider checking UE source code for FLandscapeModeToolkit or similar
    //   editor modes to see how they achieve left-side docking
    // - May need to handle tab lifecycle events (OnTabRelocated, etc.)
    //
    // COMPATIBILITY:
    // - This macro is disabled by default to ensure existing behavior is preserved
    // - Enable by adding PTTOOL_FORCE_DOCK_LEFT to preprocessor definitions
    // - Recommended to test thoroughly in target UE version before enabling
    //
    // ============================================================================
    
    UE_LOG(LogPTTool, Log, TEXT("PTTool: PTTOOL_FORCE_DOCK_LEFT is enabled - placeholder for left-side docking"));
    UE_LOG(LogPTTool, Log, TEXT("PTTool: Actual docking implementation pending - see TODO comments in PTToolEditorMode.cpp"));
    
#endif // PTTOOL_FORCE_DOCK_LEFT
}

TSharedRef<SDockTab> FPTToolEditorMode::SpawnPTToolTab(const FSpawnTabArgs& Args)
{
    UE_LOG(LogPTTool, Log, TEXT("PTTool: SpawnPTToolTab called"));
    return SNew(SDockTab)
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(STextBlock).Text(NSLOCTEXT("PTTool", "Placeholder", "PT Tool Content"))
            ]
        ];
}

void FPTToolEditorMode::Exit()
{
    UE_LOG(LogPTTool, Log, TEXT("PTTool: Exit"));

    if (FGlobalTabmanager::Get()->HasTabSpawner(PTToolTabId))
    {
        FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(PTToolTabId);
    }

    FEdMode::Exit();
}

bool FPTToolEditorMode::UsesTransformWidget() const
{
    return true;
}

bool FPTToolEditorMode::UsesTransformWidget(UE::Widget::EWidgetMode InWidgetMode) const
{
    return true;
}