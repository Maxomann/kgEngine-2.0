#pragma once
#include "stdafx.h"
#include "SystemManager.h"
#include "EntityContainer.h"

namespace kg
{
	class World : public SystemManager, public EntityContainer
	{

	};
}