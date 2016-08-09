#pragma once

#include "Vec2.h"

namespace realtrick
{
	struct FIXED_UPDATE
	{
	
		int		validID;
		Vec2	position;
		Vec2	heading;
		// 상태
		// 뛰는 것 
	};

	struct ITEM_INFO
	{
		char name[20];
		Vec2 pos;
		int amount;
		int validID;
		
	};
}