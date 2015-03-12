#include "SystemManager.h"

namespace kg
{
	void SystemManager::updateAllSystemsByImportance( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime )
	{
		// the lower the importance, the earlier the system gets updated
		// this is due to sorting from low to high key values in std::Map
		for( auto& vec : m_systemsByUpdateImportance )
			for( auto& el : vec.second )
			el->update( engine, world, saveManager, frameTime );
	}

	void SystemManager::initSystemsByImportance( Engine& engine, World& world, SaveManager& saveManager )
	{
		for( auto& vec : m_systemsByUpdateImportance )
			for( auto& el : vec.second )
				el->init( engine, world, saveManager, engine.resourceManager.getConfigFile<ConfigFile>( el->getPluginName() ) );
	}

	void SystemManager::forwardSfmlEventByImportance( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent )
	{
		for( auto& vec : m_systemsByUpdateImportance )
			for( auto& el : vec.second )
			el->sfmlEvent( engine, world, saveManager, sfEvent );
	}

	/*std::shared_ptr<System> SystemManager::getSystemById( const Plugin::Id& id )const
	{
	return m_systemsByPluginId.at( id );
	}*/

	/*const std::map<Plugin::Id, std::shared_ptr<System>>& SystemManager::getAllSystemsByPluginId() const
	{
	return m_systemsByPluginId;
	}*/
}
