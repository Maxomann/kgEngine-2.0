#include "SystemManager.h"

namespace kg
{

	void SystemManager::updateAllSystemsByImportance( Engine& engine, World& world )
	{
		for( auto& el : m_systemsByType )
			el.second->update( engine, world );
	}

	void SystemManager::initSystems( Engine& engine )
	{
		for( auto& system : m_systemsByType )
			system.second->init( engine,*this );
	}

}
