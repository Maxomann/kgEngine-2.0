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

	void SystemManager::initSystems( Engine& engine )
	{
		for( auto& system : m_systemsByType )
			system.second->init( engine,*this );
	}

}