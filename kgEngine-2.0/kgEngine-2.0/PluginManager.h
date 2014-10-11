#pragma once
#include "stdafx.h"
#include "PluginFactory.h"
#include "PluginRegistrationException.h"
#include "PluginRequestException.h"

// USAGE EXAMPLE:
//
// m_engine.pluginManager.addComponentPlugin(
//	make_shared<PluginFactory<Component, PositionComponent>>( ComponentId::PositionComponent, "Position" ) );
// m_engine.pluginManager.addComponentPlugin(
//	make_shared<PluginFactory<Component, SpriteComponent>>( ComponentId::SpriteComponent, "Sprite" ) );
//
// m_engine.pluginManager.addSystemPlugin(
//	make_shared<PluginFactory<System, CameraSystem>>( SystemId::CameraSystem, "CameraSystem" ) );

namespace kg
{
	class Component;
	class System;

	class PluginManager
	{
		std::map<int, std::shared_ptr<PluginFactoryInterface<Component>>> m_componentPluginFactorys;
		std::map<int, std::shared_ptr<PluginFactoryInterface<System>>> m_systemPluginFactorys;

		std::map<std::string, std::shared_ptr<PluginFactoryInterface<Component>>> m_componentPluginFactorysByName;
		std::map<std::string, std::shared_ptr<PluginFactoryInterface<System>>> m_systemPluginFactorysByName;

	public:
		void addComponentPlugin( const std::shared_ptr<PluginFactoryInterface<Component>>& componentPluginFactory );
		void addSystemPlugin( const std::shared_ptr<PluginFactoryInterface<System>>& systemPluginFactory );

		//systemId; realTypeHashCode; System;
		std::tuple<int, size_t, std::shared_ptr<Component>> createComponentPlugin( const int& pluginId );
		//systemId; realTypeHashCode; System;
		std::tuple<int, size_t, std::shared_ptr<System>> createSystemPlugin( const int& pluginId );

		//systemId; realTypeHashCode; System;
		std::tuple<int, size_t, std::shared_ptr<Component>> createComponentPlugin( const std::string& pluginName );
		//systemId; realTypeHashCode; System;
		std::tuple<int, size_t, std::shared_ptr<System>> createSystemPlugin( const std::string& pluginName );

		//systemId; realTypeHashCode; System;
		std::vector<std::tuple<int, size_t, std::shared_ptr<System>>> createEverySystemAvailable()const;
	};
}
