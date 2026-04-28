// Marcus hopefully approves of this.


#include "GameManager.h"

#include "ToolMenusEditor.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UGameManager::StartLevel(TSoftObjectPtr<UWorld> level)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), level);
	
	if (HUDClass && !HUDWidget)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDClass);
	}
	
	if (HUDWidget && !HUDWidget->IsInViewport())
	{
		HUDWidget->AddToViewport();
		
		if (MenuClass && MenuWidget->IsInViewport())
		{
			MenuWidget->RemoveFromParent();
		}
	}
}
