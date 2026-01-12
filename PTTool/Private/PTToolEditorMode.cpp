#include "PTToolEditorMode.h"

#include "LevelEditor.h"
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

    // Try to invoke the tab from LevelEditorTabManager (or fallback to global tab manager)
    TSharedPtr<SDockTab> Created;
    
    FLevelEditorModule* LevelEditorModule = FModuleManager::GetModulePtr<FLevelEditorModule>("LevelEditor");
    if (LevelEditorModule)
    {
        TSharedPtr<FTabManager> LevelEditorTabManager = LevelEditorModule->GetLevelEditorTabManager();
        if (LevelEditorTabManager.IsValid())
        {
            Created = LevelEditorTabManager->TryInvokeTab(PTToolTabId);
        }
    }
    
    // Fallback to global tab manager if LevelEditor tab manager is not available
    if (!Created.IsValid())
    {
        Created = FGlobalTabmanager::Get()->TryInvokeTab(PTToolTabId);
    }
    
    if (!Created.IsValid())
    {
        UE_LOG(LogPTTool, Warning, TEXT("PTTool: TryInvokeTab failed or returned invalid"));
    }
}

TSharedRef<SDockTab> FPTToolEditorMode::SpawnPTToolTab(const FSpawnTabArgs& Args)
{
    UE_LOG(LogPTTool, Log, TEXT("PTTool: SpawnPTToolTab called"));
    return SNew(SDockTab)
        .TabRole(ETabRole::PanelTab)
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
    // No need to unregister tab spawner here - it's managed by the module
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