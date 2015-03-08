#pragma once
#include "stdafx.h"
#include "SystemManager.h"
#include "EntityManager.h"

namespace kg
{
	class DLL_EXPORT World : public SystemManager, public EntityManager
	{
	};
}
