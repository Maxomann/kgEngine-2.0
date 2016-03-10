#pragma once
#include "GameState.h"

namespace kg
{
	class DLL_EXPORT GameStateManager : boost::noncopyable
	{
		Engine& engine;
		World& world;
		SaveManager& saveManager;

		std::vector<std::unique_ptr<GameState>> m_gameStateStack;

	public:
		GameStateManager( Engine& engine, World& world, SaveManager& saveManager );

		void init();

		void forwardFrameTime( const sf::Time& frameTime );

		//passed reference will be nullptr after function call!
		void push( std::unique_ptr<GameState>&& gameState );

		void pop();

		const std::unique_ptr<GameState>& top()const;

		void onUpdate();

		void clear();

		bool hasAnyInstanceOf( const Plugin::Id& pluginId )const;
		GameState* getFirstInstanceOf( const Plugin::Id& pluginId )const;
		void removeAllInstancesOf( const Plugin::Id& pluginId );
	};
}
