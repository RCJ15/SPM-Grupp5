// Marcus hopefully approves of this.


#include "ItemComponents/SuspiciousComponent.h"

int USuspiciousComponent::GetPoints()
{
	if (IsBad)
	{
		return Points;
	}
	return 0;
}	
