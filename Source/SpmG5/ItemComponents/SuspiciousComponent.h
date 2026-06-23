// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "ItemComponents/BaseItemComponent.h"
#include "SuspiciousComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPMG5_API USuspiciousComponent : public UBaseItemComponent
{
	GENERATED_BODY()
public:
	virtual int GetPoints() override;
	bool GetIsBad(){return IsBad;}
	void SetIsBad(bool SetTo){IsBad = SetTo;}
	//icon??
	
private:
	bool IsBad = false;
	int Points = -15;
};
