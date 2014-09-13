#pragma once
#include "stdafx.h"
#include "System.h"

namespace kg
{
	class SystemManager
	{
		std::map<size_t, std::shared_ptr<System>> m_systemsByType;
		std::map<double, std::shared_ptr<System>> m_systemsByUpdateImportance;
	public:
		// If this function returns true a system of type T has already been registered.
		// This function overwrites the old system with the parameter of this function.
		template<class T>
		bool addSystem( std::shared_ptr<System>& system )
		{
			size_t typeId = typeid(T).hash_code();
			double updateImportance = system->getUpdateImportance();

			auto it = m_systemsByType.find( typeId );

			m_systemsByType[typeId] = system;
			m_systemsByUpdateImportance[updateImportance] = system;

			//it != m_systemsByType.end(); means that a system has been overwritten
			return it != m_systemsByType.end();
		};

		void initSystems( Engine& engine );

		template<class T>
		std::shared_ptr<System>& getSystem()
		{
			auto it = m_systemsByType.find( typeid(T).hash_code() );
			if( it == m_systemsByType.end() )
				return nullptr;
			else
				return it->second;
		};

		template<class T, class CastTo>
		std::shared_ptr<CastTo>& getSystemWithCast()
		{
			return std::static_pointer_cast< CastTo >(getSystem<T>());
		};

		void updateAllSystemsByImportance( Engine& engine, World& world );
	};
}
