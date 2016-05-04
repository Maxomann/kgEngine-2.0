#pragma once
#include "stdafx.h"
#pragma warning(push)
#pragma warning(disable: 4275) // possible loss of data

namespace kg
{
	class DLL_EXPORT PluginRequestException : public std::exception
	{
		const std::string m_msg;

	public:
		PluginRequestException( const int& pluginId )
			:m_msg( "plugin with id: " + std::to_string( pluginId ) + " does not exist" )
		{ };
		PluginRequestException( const std::string& pluginName )
			:m_msg( "plugin with name: " + pluginName + " does not exist" )
		{ }

		virtual const char* what()const override
		{
			return m_msg.c_str();
		};
	};
}

#pragma warning(pop)