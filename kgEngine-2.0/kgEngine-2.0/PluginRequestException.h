#pragma once
#include "stdafx.h"

namespace kg
{
	class PluginRequestException : public std::exception
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
