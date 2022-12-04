// Copyright (C) Mortal Enemies 2022, All Rights Reserved


#include "UI/ObjectiveListItem.h"

#include "Components/CheckBox.h"
#include "Components/RichTextBlock.h"
#include "Gameplay/DeliveryVolume.h"
#include "Gameplay/TMtBDefaultGameMode.h"
#include "Props/CarriableStatic.h"
#include "UI/ObjectiveWidget.h"
#include "UI/TMtBDefaultHUD.h"
#include "UI/TMtBHUDWidget.h"
#include "UI/Data/ObjectiveDataObject.h"

// Rich text open/close tags for strike
const FString UObjectiveListItem::StrikeOpenTag = "<Strike>";
const FString UObjectiveListItem::StrikeCloseTag = "</>";

void UObjectiveListItem::UpdateText(const FText NewText) const
{
	ObjectiveText->SetText(NewText);
}

FText UObjectiveListItem::GetText() const
{
	return ObjectiveText->GetText();
}

FString UObjectiveListItem::GetObjectiveText() const
{
	return ObjectiveText->GetText().ToString();
}

void UObjectiveListItem::TypewriteUpdateText()
{
	const UWorld* World = GetWorld();

	// Check if our timer has started yet and start it if not
	if(!TypewriterUpdateHandle.IsValid())
	{
		World->GetTimerManager().SetTimer(TypewriterUpdateHandle, this, &UObjectiveListItem::TypewriteUpdateText, TypewriterDelay, true);
	}
	
	// Check if we need to write or delete
	FString CurrentObjectiveText = GetText().ToString();
	const int32 CurrentLength = CurrentObjectiveText.Len();

	// If the current text is empty or a substring of the target text, we're writing
	// If not, we're deleting
	const bool bWriting = CurrentLength == 0 || TargetObjectiveText.Contains(CurrentObjectiveText, ESearchCase::CaseSensitive);

	// If writing, append the next character from the target to the current text and update it
	if(bWriting)
	{
		CurrentObjectiveText += TargetObjectiveText.Mid(CurrentLength, 1);
		UpdateText(FText::FromString(CurrentObjectiveText));

		// If we've reached the target string, clear the timer
		if(CurrentObjectiveText.Len() == TargetObjectiveText.Len())
		{
			World->GetTimerManager().ClearTimer(TypewriterUpdateHandle);
			return;
		}
		return;
	}

	// If we get here, we're deleting
	// Remove the last character from the current and update the text
	CurrentObjectiveText.RemoveAt(CurrentLength - 1);
	UpdateText(FText::FromString(CurrentObjectiveText));
}

void UObjectiveListItem::TypewriteCompleteText()
{
	const UWorld* World = GetWorld();
	
	// Check if our timer has started yet and start it if not
	if(!TypewriterCompleteHandle.IsValid())
	{
		World->GetTimerManager().SetTimer(TypewriterCompleteHandle, this, &UObjectiveListItem::TypewriteCompleteText, TypewriterDelay, true);
	}

	// Check if the current text contains a strike tag
	FString Current = GetText().ToString();
	const bool bHasStrike = Current.Contains(StrikeOpenTag, ESearchCase::CaseSensitive);
	
	if(bHasStrike)
	{
		// If it does, find the position of the close tag
		const int32 Position = Current.Find(StrikeCloseTag, ESearchCase::CaseSensitive);
		int32 Len = Current.Len();

		// If the close tag is at the end of the string, we're done
		// Clear the timer
		if(Position == Len - StrikeCloseTag.Len())
		{
			World->GetTimerManager().ClearTimer(TypewriterCompleteHandle);
			return;
		}

		// If not, pop the close tag, and reinsert one character further down the string
		Current.RemoveAt(Position, StrikeCloseTag.Len());
		Current.InsertAt(Position + 1, StrikeCloseTag);
		UpdateText(FText::FromString(Current));
		return;
	}

	// If there are no strike tags, insert the open tag at the beginning of the string and the close tag after the first character
	FString Constructed = StrikeOpenTag + Current.Mid(0, 1);
	Constructed += StrikeCloseTag + Current.Mid(1);
	UpdateText(FText::FromString(Constructed));
}

void UObjectiveListItem::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}

void UObjectiveListItem::NativeOnItemExpansionChanged(bool bIsExpanded)
{
	IUserObjectListEntry::NativeOnItemExpansionChanged(bIsExpanded);
}

void UObjectiveListItem::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();
}

void UObjectiveListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	// If it has an owner, try to update the owner's objectives
	DataObject = Cast<UObjectiveDataObject>(ListItemObject);
	if(!DataObject || !DataObject->Data.Owner) return;
	
	DataObject->Data.Owner->TryUpdateObjectives();
}

void UObjectiveListItem::NativeConstruct()
{
	Super::NativeConstruct();
	bHasScriptImplementedTick = false;
	bHasScriptImplementedPaint = false;
}