#include "ComponentManager.h"

namespace kg
{
	template<class T>
	bool ComponentManager::addComponent( std::shared_ptr<Component>& component )
	{
		size_t typeId = typeid(T).hash_code();
		double updateImportance = component->getUpdateImportance();

		auto it = m_componentsByType.find( typeId );

		m_componentsByType[typeId] = component;
		m_componentsByUpdateImportance[updateImportance] = component;

		//it != m_systemsByType.end(); means that a system has been overwritten
		return it != m_componentsByType.end();
	}


	template<class T>
	std::shared_ptr<Component>& ComponentManager::getComponent()
	{
		auto it = m_componentsByType.find( typeid(T).hash_code() );
		if( it == m_componentsByType.end() )
			return nullptr;
		else
			return it->second;
	}


	template<class T, class CastTo>
	std::shared_ptr<CastTo>& ComponentManager::getComponentWithCast()
	{
		return std::static_pointer_cast< CastTo >(getComponent<T>());
	}



	void ComponentManager::updateAllComponentsByImportance( Engine& engine, World& world )
	{
		for( auto& el : m_componentsByType )
			el.second->update( *this, engine, world );
	}
}
