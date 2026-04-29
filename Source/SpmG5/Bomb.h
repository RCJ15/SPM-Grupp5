// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "DangerousItem.h"
#include "Bomb.generated.h"

/**
 * 
 */
UCLASS()
class SPMG5_API ABomb : public ADangerousItem
{
	GENERATED_BODY()
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	bool bDoExplode = false; //används för att testa explosion
	void Explode();
};
