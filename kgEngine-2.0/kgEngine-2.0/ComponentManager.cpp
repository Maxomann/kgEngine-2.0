#include "ComponentManager.h"

using namespace std;

namespace kg
{
	void ComponentManager::updateAllComponentsByImportance( Engine& engine, World& world, const sf::Time& frameTime )
	{
		// the lower the importance, the earlier the component gets updated
		// this is due to sorting from low to high key values in std::Map
		for( auto& el : m_componentsByUpdateImportance )
			el.second->update( engine, world, *this, frameTime );
	}

	void ComponentManager::initComponentsByImportance( Engine& engine, World& world )
	{
		for( auto& component : m_componentsByUpdateImportance )
			component.second->init( engine, world, *this );
	}

	bool ComponentManager::hasComponent( const std::vector<size_t>& componentTypes )const
	{
		auto entIt = end( m_componentsByType );

		for( const auto& type : componentTypes )
			if( m_componentsByType.find( type ) == entIt )
				return false;
		return true;
	}

	const std::map<size_t, std::shared_ptr<Component>>& ComponentManager::getAllComponentsByTypeHash() const
	{
		return m_componentsByType;
	}

	const std::map<Plugin::Id, std::shared_ptr<Component>>& ComponentManager::getAllComponentsByPluginId() const
	{
		return m_componentsByPluginId;
	}
}