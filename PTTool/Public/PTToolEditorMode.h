// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorModes.h"
#include "EdMode.h"

/**
 * 最小的 Legacy EdMode 示例
 */
class FPTToolEditorMode : public FEdMode
{
public:
	// 在头文件声明静态 ID、构造/析构、以及必要的虚方法
	static const FName EM_PTToolLegacyEdModeId;

	FPTToolEditorMode();
	virtual ~FPTToolEditorMode();

	// FEdMode 接口
	virtual void Enter() override;
	virtual void Exit() override;

	// 是否使用变换 Widget
	virtual bool UsesTransformWidget() const override;
	virtual bool UsesTransformWidget(UE::Widget::EWidgetMode InWidgetMode) const override;

	// Tab spawn
	TSharedRef<class SDockTab> SpawnPTToolTab(const class FSpawnTabArgs& Args);
};