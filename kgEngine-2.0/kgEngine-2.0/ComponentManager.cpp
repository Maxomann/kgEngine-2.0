#include "ComponentManager.h"

namespace kg
{

	void ComponentManager::updateAllComponentsByImportance( Engine& engine, World& world )
	{
		// the lower the importance, the earlier the component gets updated
		// this is due to sorting from low to high key values in std::Map
		for( auto& el : m_componentsByUpdateImportance )
			el.second->update( engine, world, *this );
	}

	void ComponentManager::initComponents( Engine& engine )
	{
		for( auto& component : m_componentsByType )
			component.second->init( engine, *this );
	}

}
