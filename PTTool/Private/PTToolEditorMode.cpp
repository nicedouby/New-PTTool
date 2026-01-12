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
const FName FPTToolEditorMode::EM_PTToolLegacyEdModeId = TEXT("EM_PTToolLegacyEdMode");

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