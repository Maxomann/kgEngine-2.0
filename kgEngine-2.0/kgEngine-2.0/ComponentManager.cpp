#include "ComponentManager.h"

using namespace std;

namespace kg
{
	void ComponentManager::updateAllComponentsByImportance( Engine& engine, World& world, const sf::Time& frameTime )
	{
		// the lower the importance, the earlier the component gets updated
		// this is due to sorting from low to high key values in std::map
		for( auto& vec : m_componentsByUpdateImportance )
			for( auto& el : vec.second )
				el->update( engine, world, *this, frameTime );
	}

	void ComponentManager::initComponentsByImportance( Engine& engine, World& world )
	{
		for( auto& vec : m_componentsByUpdateImportance )
			for( auto& el : vec.second )
				el->init( engine, world, *this );
	}

	bool ComponentManager::hasComponent( const std::vector<size_t>& componentTypes )const
	{
		auto entIt = end( m_componentsByType );

		for( const auto& type : componentTypes )
			if( m_componentsByType.find( type ) == entIt )
				return false;
		return true;
	}

	const ComponentManager::ComponentsByTypeContainer& ComponentManager::getAllComponentsByTypeHash() const
	{
		return m_componentsByType;
	}

	/*const std::unordered_map<Plugin::Id, std::shared_ptr<Component>>& ComponentManager::getAllComponentsByPluginId() const
	{
		return m_componentsByPluginId;
	}*/
}
