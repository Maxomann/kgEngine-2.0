#include "ComponentManager.h"

namespace kg
{

	void ComponentManager::updateAllComponentsByImportance( Engine& engine, World& world )
	{
		for( auto& el : m_componentsByType )
			el.second->update( engine, world, *this );
	}

	void ComponentManager::initComponents( Engine& engine )
	{
		for( auto& component : m_componentsByType )
			component.second->init( engine, *this );
	}

}
