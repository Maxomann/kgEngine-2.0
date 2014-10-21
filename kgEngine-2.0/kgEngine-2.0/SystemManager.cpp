#include "SystemManager.h"

namespace kg
{
	void SystemManager::updateAllSystemsByImportance( Engine& engine, World& world )
	{
		// the lower the importance, the earlier the system gets updated
		// this is due to sorting from low to high key values in std::Map
		for( auto& el : m_systemsByUpdateImportance )
			el.second->update( engine, world );
	}

	void SystemManager::initSystemsByImportance( Engine& engine, World& world )
	{
		for( auto& system : m_systemsByUpdateImportance )
			system.second->init( engine, world );
	}

	void SystemManager::forwardSfmlEventByImportance( const sf::Event& sfEvent )
	{
		for( const auto& el : m_systemsByUpdateImportance )
			el.second->sfmlEvent( sfEvent );
	}
}
