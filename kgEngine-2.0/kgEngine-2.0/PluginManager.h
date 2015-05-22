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

	class DLL_EXPORT PluginManager
	{
		// The component plugin factories sorted by ID.
		std::map<Plugin::Id, std::shared_ptr<PluginFactoryInterface<Component>>> m_componentPluginFactorysById;
		// The system plugin factories sorted by ID.
		std::map<Plugin::Id, std::shared_ptr<PluginFactoryInterface<System>>> m_systemPluginFactorysById;


		// The component plugin factories sorted by name.
		std::map<Plugin::Name, std::shared_ptr<PluginFactoryInterface<Component>>> m_componentPluginFactorysByName;
		// The system plugin factories sorted by name.
		std::map<Plugin::Name, std::shared_ptr<PluginFactoryInterface<System>>> m_systemPluginFactorysByName;


		std::map < size_t, std::pair <
			std::map<Plugin::Id, std::shared_ptr<PluginFactoryGenericInterface>>,
			std::map<Plugin::Name, std::shared_ptr<PluginFactoryGenericInterface> >> >m_pluginsByGenericTypeHash;

	public:
		template<class BasePluginType>
		void addPluginFactory( std::shared_ptr<PluginFactoryInterface<BasePluginType>>& pluginFactory )
		{
			size_t genericPluginType = typeid(BasePluginType).hash_code();
			auto& pair = m_pluginsByGenericTypeHash[genericPluginType];

			auto& first = pair.first[pluginFactory->getId()];
			auto& second = pair.second[pluginFactory->getName()];

			if( first != nullptr || second != nullptr )
				throw PluginRegistrationException( pluginFactory->getId() );

			auto casted = std::static_pointer_cast< PluginFactoryGenericInterface >(pluginFactory);
			first = casted;
			second = casted;

		};
		template<>
		void addPluginFactory<Component>( std::shared_ptr<PluginFactoryInterface<Component>>& pluginFactory )
		{
			auto& byId = m_componentPluginFactorysById[pluginFactory->getId()];
			auto& byName = m_componentPluginFactorysByName[pluginFactory->getName()];

			if( byId != nullptr || byName != nullptr )
				throw PluginRegistrationException( pluginFactory->getId() );

			byId = pluginFactory;
			byName = pluginFactory;
		}
		template<>
		void addPluginFactory<System>( std::shared_ptr<PluginFactoryInterface<System>>& pluginFactory )
		{
			auto& byId = m_systemPluginFactorysById[pluginFactory->getId()];
			auto& byName = m_systemPluginFactorysByName[pluginFactory->getName()];

			if( byId != nullptr || byName != nullptr )
				throw PluginRegistrationException( pluginFactory->getId() );

			byId = pluginFactory;
			byName = pluginFactory;
		}
		template<class T, class T2>
		void addPluginFactory( std::shared_ptr<PluginFactory<T, T2>>& pluginFactory )
		{
			addPluginFactory( std::dynamic_pointer_cast< PluginFactoryInterface<T> >(pluginFactory) );
		}


		template<class BasePluginType>
		std::shared_ptr<BasePluginType> createPlugin( const Plugin::Id& id )const
		{
			size_t genericPluginType = typeid(genericPluginType).hash_code();
			try
			{
				auto& plugin = m_pluginsByGenericTypeHash.at( genericPluginType ).first.at( id );
				if( plugin == nullptr )
					throw PluginRequestException( id );

				return std::static_pointer_cast< BasePluginType >(plugin->create());
			}
			catch( const std::exception& )
			{
				throw PluginRequestException( id );
			}
		}
		template<>
		std::shared_ptr<Component> createPlugin<Component>( const Plugin::Id& id )const
		{
			size_t genericPluginType = typeid(genericPluginType).hash_code();
			try
			{
				auto& plugin = m_componentPluginFactorysById.at( id );
				if( plugin == nullptr )
					throw PluginRequestException( id );

				return plugin->create();
			}
			catch( const std::exception& )
			{
				throw PluginRequestException( id );
			}
		}
		template<>
		std::shared_ptr<System> createPlugin<System>( const Plugin::Id& id )const
		{
			size_t genericPluginType = typeid(genericPluginType).hash_code();
			try
			{
				auto& plugin = m_systemPluginFactorysById.at( id );
				if( plugin == nullptr )
					throw PluginRequestException( id );

				return plugin->create();
			}
			catch( const std::exception& )
			{
				throw PluginRequestException( id );
			}
		}

		template<class BasePluginType>
		std::shared_ptr<BasePluginType> createPlugin( const Plugin::Name& name )const
		{
			size_t genericPluginType = typeid(genericPluginType).hash_code();
			try
			{
				auto& plugin = m_pluginsByGenericTypeHash.at( genericPluginType ).second.at( name );
				if( plugin == nullptr )
					throw PluginRequestException( name );

				return plugin->create();
			}
			catch( const std::exception& )
			{
				throw PluginRequestException( name );
			}
		}
		template<>
		std::shared_ptr<Component> createPlugin<Component>( const Plugin::Name& name )const
		{
			size_t genericPluginType = typeid(genericPluginType).hash_code();
			try
			{
				auto& plugin = m_componentPluginFactorysByName.at( name );
				if( plugin == nullptr )
					throw PluginRequestException( name );

				return plugin->create();
			}
			catch( const std::exception& )
			{
				throw PluginRequestException( name );
			}
		}
		template<>
		std::shared_ptr<System> createPlugin<System>( const Plugin::Name& name )const
		{
			size_t genericPluginType = typeid(genericPluginType).hash_code();
			try
			{
				auto& plugin = m_systemPluginFactorysByName.at( name );
				if( plugin == nullptr )
					throw PluginRequestException( name );

				return plugin->create();
			}
			catch( const std::exception& )
			{
				throw PluginRequestException( name );
			}
		}

		std::vector<std::shared_ptr<System>> createEverySystemAvailable()const;

		template<class BasePluginType>
		const std::vector<std::shared_ptr<PluginFactoryInterface<BasePluginType>>> getEveryUserDefinedPlugin()
		{
			std::vector<std::shared_ptr<PluginFactoryInterface<BasePluginType>>> retVal;

			auto type_hash = typeid(BasePluginType).hash_code();

			for( const auto& el : m_pluginsByGenericTypeHash[type_hash].first )
				retVal.push_back( static_pointer_cast<PluginFactoryInterface<BasePluginType>>(el.second) );

			return retVal;
		};

	};

}
