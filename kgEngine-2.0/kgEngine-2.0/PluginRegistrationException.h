#pragma once
#include "stdafx.h"

namespace kg
{
	class DLL_EXPORT PluginRegistrationException : public std::exception
	{
		const std::string m_msg;

	public:
		PluginRegistrationException( const int& pluginId )
			:m_msg( std::to_string( pluginId ) )
		{ };

		virtual const char* what()const
		{
			return m_msg.c_str();
		};
	};
}