#include "ComponentManager.h"

using namespace std;

namespace kg
{
	void ComponentManager::updateAllComponentsByImportance( Engine& engine, World& world, const sf::Time& frameTime )
	{
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

	std::vector<std::pair<size_t, Component*>>::const_iterator ComponentManager::m_findComponentByType( const size_t& hash_code )const
	{
		for( auto it = m_componentsByType.begin(); it != m_componentsByType.end(); ++it )
			if( it->first == hash_code )
				return it;
		return m_componentsByType.end();
	}

	void ComponentManager::initComponentsByImportance( Engine& engine, const World& world )
	{
		m_checkComponentsSortedByUpdateImportance();

		for( auto& el : m_componentsByUpdateImportance )
			el->init( engine, world, *this );
	}

	bool ComponentManager::hasComponent( const std::vector<size_t>& componentTypes )const
	{
		auto entIt = end( m_componentsByType );

		for( const auto& type : componentTypes )
			if( m_findComponentByType( type ) == entIt )
				return false;
		return true;
	}

	bool ComponentManager::hasComponent( const Plugin::Id& componentId ) const
	{
		for( const auto& el : m_componentsByUpdateImportance )
			if( el->getPluginId() == componentId )
				return true;
		return false;
	}

	const std::vector<Component*>& ComponentManager::getAllComponentsByUpdateImportance() const
	{
		return m_componentsByUpdateImportance;
	}

	void ComponentManager::addComponent( std::unique_ptr<Component>& component )
	{
		auto typeHashCode = component->getRTTI_hash();

#ifdef DEBUG
		auto it = m_findComponentByType( typeHashCode );
		if( it != m_componentsByType.end() )
			throw std::exception();
#endif // DEBUG

		m_componentsByType.emplace_back( typeHashCode, component.get() );
		m_componentsByUpdateImportance.emplace_back( component.get() );
		m_components.emplace_back( std::move( component ) );
	}
}