// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

/**
 * Legacy FEdMode implementation for PTTool.
 * This mode invokes the PTTool tab which contains the toolkit UI.
 */
class FPTToolLegacyEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_PTToolLegacyEdModeId;
	const static FName PTToolTabId;

	FPTToolLegacyEdMode();
	virtual ~FPTToolLegacyEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	virtual bool UsesTransformWidget() const override;
	virtual bool UsesTransformWidget(UE::Widget::EWidgetMode CheckMode) const override;
};
