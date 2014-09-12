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
		// This function overwrites the old system with the parameter of this function
		template<class T>
		bool addSystem( std::shared_ptr<System>& system );

		template<class T>
		std::shared_ptr<System>& getSystem();

		template<class T, class CastTo>
		std::shared_ptr<CastTo>& getSystemWithCast();

		void updateAllSystemsByImportance( Engine& engine, World& world );
	};
}
