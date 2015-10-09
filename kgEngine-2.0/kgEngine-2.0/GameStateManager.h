#pragma once
#include "GameState.h"

namespace kg
{
	class DLL_EXPORT GameStateManager
	{
		std::vector<std::shared_ptr<GameState>> m_gameStateStack;

	public:


	};
}