#pragma once
#include "stdafx.h"

namespace kg
{
	class PluginRequestException : public std::exception
	{
		const int m_id=-1;
		const std::string m_name="-1";

	public:
		PluginRequestException( const int& pluginId )
			:m_id( pluginId )
		{ };
		PluginRequestException( const std::string& pluginName )
			:m_name(pluginName)
		{}

		virtual const char* what()const
		{
			if( m_id == -1 )
			{
				return ("plugin with name" + m_name + "does not exist").c_str();
			}
			else
			{
				return ("plugin with id" + std::to_string( m_id ) + "does not exist").c_str();
			}
		};
	};
}
