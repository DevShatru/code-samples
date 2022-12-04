// Copyright (C) Mortal Enemies 2022, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveListItem.generated.h"

class ACarriableStatic;
class UObjectiveDataObject;
class URichTextBlock;
class UCheckBox;
/**
 * 
 */
UCLASS()
class TENMINUTESTOBREATHE_API UObjectiveListItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	void UpdateText(FText NewText) const;
	FText GetText() const;
	FString GetObjectiveText() const;

	// Target text, and timer duration - controlled by owner
	FString TargetObjectiveText;
	float TypewriterDelay;

	// Timer handles for update and completion
	FTimerHandle TypewriterUpdateHandle;
	FTimerHandle TypewriterCompleteHandle;

	// Timer delegates for update and completion
	UFUNCTION()
	void TypewriteUpdateText();
	
	UFUNCTION()
	void TypewriteCompleteText();
	
	// IUserObjectListEntry overrides
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnItemExpansionChanged(bool bIsExpanded) override;
	virtual void NativeOnEntryReleased() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void NativeConstruct() override;

	// Bound rich text block
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	URichTextBlock* ObjectiveText;

	static const FString StrikeOpenTag;
	static const FString StrikeCloseTag;

protected:
	// Contains objective data - Owner, text, completion status
	UObjectiveDataObject* DataObject;
};
