#include "ComponentManager.h"

namespace kg
{

	void ComponentManager::updateAllComponentsByImportance( Engine& engine, World& world )
	{
		for( auto& el : m_componentsByType )
			el.second->update( engine, world, *this );
	}

}
