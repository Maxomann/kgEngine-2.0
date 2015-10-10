#pragma once
#include "GameState.h"

namespace kg
{
	class DLL_EXPORT GameStateManager
	{
		Engine& engine;
		World& world;
		SaveManager& saveManager;

		std::vector<std::shared_ptr<GameState>> m_gameStateStack;

	public:
		GameStateManager( Engine& engine, World& world, SaveManager& saveManager );

		void init();

		void forwardFrameTime( const sf::Time& frameTime );

		void push( const std::shared_ptr<GameState>& gameState );

		void pop();

		const std::shared_ptr<GameState>& top()const;

		void onUpdate();
	};
}
