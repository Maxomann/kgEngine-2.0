#pragma once
#include "stdafx.h"
#include "PluginFactory.h"
#include "PluginRegistrationException.h"
#include "PluginRequestException.h"

namespace kg
{
	class Component;
	class System;

	class PluginManager
	{
		std::map<int, std::shared_ptr<PluginFactoryInterface<Component>>> m_componentPluginFactorys;
		std::map<int, std::shared_ptr<PluginFactoryInterface<System>>> m_systemPluginFactorys;

	public:
		void addComponentPlugin( const std::shared_ptr<PluginFactoryInterface<Component>>& componentPluginFactory );
		void addSystemPlugin( const std::shared_ptr<PluginFactoryInterface<System>>& systemPluginFactory );

		std::shared_ptr<Component> createComponentPlugin( const int& pluginId );
		std::shared_ptr<System> createSystemPlugin( const int& pluginId );

		//systemId; realTypeHashCode; System;
		std::vector<std::tuple<int, size_t, std::shared_ptr<System>>> createEverySystemAvailable()const;
	};
}
