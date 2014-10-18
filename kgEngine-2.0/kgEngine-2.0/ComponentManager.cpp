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

	bool ComponentManager::hasComponent( const std::vector<size_t>& componentTypes )
	{
		for( const auto& type : componentTypes )
			if( m_componentsByType.find( type ) == m_componentsByType.end() )
				return false;
		return true;
	}

	const std::map<size_t, std::shared_ptr<Component>>& ComponentManager::getAllComponentsByTypeHash() const
	{
		return m_componentsByType;
	}

}
