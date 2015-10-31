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

	void SystemManager::addSystem( std::unique_ptr<System>&& system )
	{
		m_systemsByType[system->getRTTI_hash()] = system.get();
		m_systemsByUpdateImportance[system->getUpdateImportance()].push_back( std::move( system ) );
	}

	void SystemManager::saveSystemConfigFilesByImportance( Engine& engine )
	{
		for( auto& vec : m_systemsByUpdateImportance )
			for( auto& el : vec.second )
			{
				auto configFile = engine.resourceManager.getConfigFile<ConfigFile>( el->getPluginName() );
				el->saveChangesToConfigFile( configFile );
				configFile->saveToFile();
			}
	}
}