#pragma once
#include "stdafx.h"
#include "Plugin.h"

namespace kg
{
	class DLL_EXPORT PluginFactoryGenericInterface
	{
	public:
		PluginFactoryGenericInterface()
		{ };
		virtual ~PluginFactoryGenericInterface()
		{ };

		virtual int getId()const = 0;
		virtual const std::string& getName()const = 0;
	};

	template<class PluginType>
	class DLL_EXPORT PluginFactoryInterface : public PluginFactoryGenericInterface
	{
		const int m_pluginId = -1;
		const std::string m_name = "-1";

	public:
		PluginFactoryInterface( const int& pluginId, const std::string& pluginName )
			:m_pluginId( pluginId ),
			m_name( pluginName )
		{ }

		int getId()const
		{
			return m_pluginId;
		}

		const std::string& getName()const
		{
			return m_name;
		}

		virtual std::shared_ptr<PluginType> create()const = 0;
	};

	template<class PluginType, class ClassType>
	class DLL_EXPORT PluginFactory : public PluginFactoryInterface < PluginType >
	{
	public:
		PluginFactory( const int& pluginId, const std::string& pluginName )
			: PluginFactoryInterface( pluginId, pluginName )
		{ }

		virtual std::shared_ptr<PluginType> create()const
		{
			return std::static_pointer_cast< PluginType >(std::make_shared<ClassType>());
		};
	};
}
