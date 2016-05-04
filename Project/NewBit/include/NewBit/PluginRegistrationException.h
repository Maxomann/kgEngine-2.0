#pragma once
#include "stdafx.h"
#pragma warning(push)
#pragma warning(disable: 4275) // possible loss of data

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

#pragma warning(pop)