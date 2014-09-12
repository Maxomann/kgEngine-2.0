#include "SystemManager.h"

namespace kg
{

	template<class T>
	bool SystemManager::addSystem( std::shared_ptr<System>& system )
	{
		size_t typeId = typeid(T).hash_code();
		double updateImportance = system->getUpdateImportance();

		auto it = m_systemsByType.find( typeId );

		m_systemsByType[typeId] = system;
		m_systemsByUpdateImportance[updateImportance] = system;

		//it != m_systemsByType.end(); means that a system has been overwritten
		return it != m_systemsByType.end();
	}


	template<class T>
	std::shared_ptr<System>& SystemManager::getSystem()
	{
		auto it = m_systemsByType.find( typeid(T).hash_code() );
		if( it == m_systemsByType.end() )
			return nullptr;
		else
			return it->second;
	}


	template<class T, class CastTo>
	std::shared_ptr<CastTo>& SystemManager::getSystemWithCast()
	{
		return std::static_pointer_cast< CastTo >(getSystem<T>());
	}



	void SystemManager::updateAllSystemsByImportance( Engine& engine, World& world )
	{
		for( auto& el : m_systemsByType )
			el.second->update( *this, engine, world );
	}

}
