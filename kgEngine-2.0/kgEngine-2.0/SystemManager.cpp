#include "SystemManager.h"

namespace kg
{
	void SystemManager::updateAllSystemsByImportance( Engine& engine, World& world, const sf::Time& frameTime )
	{
		// the lower the importance, the earlier the system gets updated
		// this is due to sorting from low to high key values in std::Map
		for( auto& el : m_systemsByUpdateImportance )
			el.second->update( engine, world, frameTime );
	}

	void SystemManager::initSystemsByImportance( Engine& engine, World& world )
	{
		for( auto& system : m_systemsByUpdateImportance )
			system.second->init( engine, world, engine.resourceManager.getConfigFile<ConfigFile>( system.second->getPluginName() ) );
	}

	void SystemManager::forwardSfmlEventByImportance( Engine& engine, const sf::Event& sfEvent )
	{
		for( const auto& el : m_systemsByUpdateImportance )
			el.second->sfmlEvent( engine, sfEvent );
	}

	std::shared_ptr<System> SystemManager::getSystemById( const Plugin::Id& id )const
	{
		return m_systemsByPluginId.at( id );
	}

	const std::map<Plugin::Id, std::shared_ptr<System>>& SystemManager::getAllSystemsByPluginId() const
	{
		return m_systemsByPluginId;
	}

}