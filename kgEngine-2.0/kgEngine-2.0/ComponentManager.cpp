#include "ComponentManager.h"

using namespace std;

namespace kg
{
	void ComponentManager::updateAllComponentsByImportance( Engine& engine, World& world, const sf::Time& frameTime )
	{
		m_checkComponentsSortedByUpdateImportance();

		// the lower the importance, the earlier the component gets updated
		// this is due to sorting from low to high key values in std::map
		for( auto& el : m_componentsByUpdateImportance )
				el->update( engine, world, *this, frameTime );
	}

	void ComponentManager::m_checkComponentsSortedByUpdateImportance()
	{
		if( m_componentsSorted )
			return;
		else
		{
			std::sort( m_componentsByUpdateImportance.begin(), m_componentsByUpdateImportance.end(), []( Component* lhs, Component* rhs )
			{
				const auto lhsImportance = lhs->getUpdateImportance();
				const auto rhsImportance = rhs->getUpdateImportance();

				return lhsImportance < rhsImportance;
			} );
			m_componentsSorted = true;
		}
	}

	void ComponentManager::initComponentsByImportance( Engine& engine, World& world )
	{
		m_checkComponentsSortedByUpdateImportance();

		for( auto& el : m_componentsByUpdateImportance )
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
