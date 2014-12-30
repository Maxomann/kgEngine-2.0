#pragma once
#include "stdafx.h"
#include "ComponentManager.h"
#include "ComponentMissingException.h"

namespace kg
{
	class DLL_EXPORT Entity : public ComponentManager
	{
	public:
		typedef unsigned long long Id;
	};
}
