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

	/// Manager for plugins.
	///
	/// @author	Kay
	/// @date	26.01.2015

	class DLL_EXPORT PluginManager
	{
		// The component plugin factories sorted by ID.
		std::map<int, std::shared_ptr<PluginFactoryInterface<Component>>> m_componentPluginFactorysById;
		// The system plugin factories sorted by ID.
		std::map<int, std::shared_ptr<PluginFactoryInterface<System>>> m_systemPluginFactorysById;

		// The component plugin factories sorted by name.
		std::map<std::string, std::shared_ptr<PluginFactoryInterface<Component>>> m_componentPluginFactorysByName;
		// The system plugin factories sorted by name.
		std::map<std::string, std::shared_ptr<PluginFactoryInterface<System>>> m_systemPluginFactorysByName;

		std::map<int, std::string> m_componentPluginNamesByIds;
		std::map<std::string, int> m_componentPluginIdsByNames;

		std::map<int, std::string> m_systemPluginNamesByIds;
		std::map<std::string, int> m_systemPluginIdsByNames;

		std::map<size_t, std::map<Plugin::Id, std::shared_ptr<PluginFactoryInterface<Plugin>>>> m_userDefinedPluginsByIdByGenericUserDefinedPluginTypeHash;

	public:

		template<class GenericUserDefinedPluginType, class T>
		void addUserDefinedPlugin( std::shared_ptr<PluginFactoryInterface<GenericUserDefinedPluginType>>& pluginFactory )
		{
			m_userDefinedPluginsByIdByGenericUserDefinedPluginTypeHash
				[typeid(GenericUserDefinedPluginType).hash_code()]
			[pluginFactory->getId()]
			= static_pointer_cast<std::shared_ptr<PluginFactoryInterface<Plugin>>>(pluginFactory);
		};

		template<class GenericUserDefinedPluginType>
		std::shared_ptr<GenericUserDefinedPluginType> createUserDefinedPlugin( Plugin::Id pluginId )
		{
			return std::static_pointer_cast< GenericUserDefinedPluginType >(
				m_userDefinedPluginsByIdByGenericUserDefinedPluginTypeHash
				[typeid(GenericUserDefinedPluginType).hash_code()]
			[pluginId]->create());
		};

		template<class GenericUserDefinedPluginType>
		std::vector<std::pair<Plugin::Id, std::shared_ptr<GenericUserDefinedPluginType>>> createEveryUserDefinedPlugin()
		{
			std::vector<std::pair<Plugin::Id, std::shared_ptr<GenericUserDefinedPluginType>>> retVal;

			for( const auto& el : m_userDefinedPluginsByIdByGenericUserDefinedPluginTypeHash[typeid(GenericUserDefinedPluginType).hash_code()] )
				retVal.emplace_back( el.first, std::static_pointer_cast< GenericUserDefinedPluginType >(el.second->create()) );

			return retVal;
		}

		template<class GenericUserDefinedPluginType>
		const std::map<Plugin::Id, std::shared_ptr<PluginFactoryInterface<Plugin>>>& getEveryUserDefinedPlugin()
		{
			return m_userDefinedPluginsByIdByGenericUserDefinedPluginTypeHash[typeid(GenericUserDefinedPluginType).hash_code()];
		}

		/// Adds a component plugin.
		///
		/// @author	Kay
		/// @date	26.01.2015
		///
		/// @param	componentPluginFactory	The component plugin factory.

		void addComponentPlugin( const std::shared_ptr<PluginFactoryInterface<Component>>& componentPluginFactory );

		/// Adds a system plugin.
		///
		/// @author	Kay
		/// @date	26.01.2015
		///
		/// @param	systemPluginFactory	The system plugin factory.

		void addSystemPlugin( const std::shared_ptr<PluginFactoryInterface<System>>& systemPluginFactory );

		/// Creates component plugin.
		///
		/// @author	Kay
		/// @date	26.01.2015
		///
		/// @param	pluginId	Identifier for the plugin.
		///
		/// @return	The new component plugin. <componentId; realTypeHashCode; Component;>

		std::tuple<int, size_t, std::shared_ptr<Component>> createComponentPlugin( const int& pluginId );

		/// Creates system plugin.
		///
		/// @author	Kay
		/// @date	26.01.2015
		///
		/// @param	pluginId	Identifier for the plugin.
		///
		/// @return	The new system plugin. <systemId; realTypeHashCode; System;>

		std::tuple<int, size_t, std::shared_ptr<System>> createSystemPlugin( const int& pluginId );

		/// Creates component plugin.
		///
		/// @author	Kay
		/// @date	26.01.2015
		///
		/// @param	pluginName	Name of the plugin.
		///
		/// @return	The new component plugin. <componentId; realTypeHashCode; Component;>

		std::tuple<int, size_t, std::shared_ptr<Component>> createComponentPlugin( const std::string& pluginName );

		/// Creates system plugin.
		///
		/// @author	Kay
		/// @date	26.01.2015
		///
		/// @param	pluginName	Name of the plugin.
		///
		/// @return	The new system plugin. <systemId; realTypeHashCode; System;>

		std::tuple<int, size_t, std::shared_ptr<System>> createSystemPlugin( const std::string& pluginName );

		/// Creates every system available.
		///
		/// @author	Kay
		/// @date	26.01.2015
		///
		/// @return	The new every system available. tuple:<systemId; realTypeHashCode; System;>

		std::vector<std::tuple<int, size_t, std::shared_ptr<System>>> createEverySystemAvailable()const;

		///
		/// @author	Kay
		/// @date	26.01.2015
		///
		/// @param	hashCode	The hash code.
		///
		/// @return	The plugin information by real type hash code. <first:pluginId second:pluginName>

		const std::pair<int, std::string> getComponentPluginInformationByRealTypeHashCode( size_t hashCode )const;

		const std::string& getComponentPluginNameForId( const int& id )const;
		const int& getComponentPluginIdForName( const std::string& name )const;

		const std::string& getSystemPluginNameForId( const int& id )const;
		const int& getSystemPluginIdForName( const std::string& name )const;
	};

	namespace WIP
	{
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


			std::map<size_t, std::pair<
				std::map<Plugin::Id, std::shared_ptr<PluginFactoryInterface<Plugin>>>,
				std::map<Plugin::Name, std::shared_ptr<PluginFactoryInterface<Plugin>> >> >m_pluginsByGenericTypeHash;

		public:
			template<class GenericPluginType>
			void addPluginFactory( const std::shared_ptr<PluginFactoryInterface<GenericPluginType>>& pluginFactory )
			{
				size_t genericPluginType = typeid(genericPluginType).hash_code();
				auto& pair = m_pluginsByGenericTypeHash[genericPluginType];

				if( pair.first != nullptr || pair.second != nullptr )
					throw PluginRegistrationException( pluginFactory->getId() );

				pair.first[pluginFactory->getId()] = pluginFactory;
				pair.second[pluginFactory->getName()] = pluginFactory;
			};
			template<>
			void addPluginFactory<Component>( const std::shared_ptr<PluginFactoryInterface<Component>>& pluginFactory )
			{
				m_componentPluginFactorysById[pluginFactory->getId()] = pluginFactory;
				m_componentPluginFactorysByName[pluginFactory->getName()] = pluginFactory;
			}
			template<>
			void addPluginFactory<System>( const std::shared_ptr<PluginFactoryInterface<System>>& pluginFactory )
			{
				m_systemPluginFactorysById[pluginFactory->getId()] = pluginFactory;
				m_systemPluginFactorysByName[pluginFactory->getName()] = pluginFactory;
			}


			template<class GenericPluginType>
			std::shared_ptr<GenericPluginType> createPlugin( const Plugin::Id& id )const
			{
				size_t genericPluginType = typeid(genericPluginType).hash_code();
				return m_pluginsByGenericTypeHash.at( genericPluginType ).first.at( id )->create();
			}
			template<>
			std::shared_ptr<Component> createPlugin<Component>( const Plugin::Id& id )const
			{

			}
			template<>
			std::shared_ptr<System> createPlugin<System>( const Plugin::Id& id )const
			{

			}

			template<class GenericPluginType>
			std::shared_ptr<GenericPluginType> createPlugin( const Plugin::Name& name )const
			{

			}
			template<>
			std::shared_ptr<Component> createPlugin<Component>( const Plugin::Name& name )const
			{

			}
			template<>
			std::shared_ptr<System> createPlugin<System>( const Plugin::Name& name )const
			{

			}

		};

	}
}
