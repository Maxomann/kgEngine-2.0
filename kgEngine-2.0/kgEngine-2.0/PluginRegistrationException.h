#pragma once
#include "stdafx.h"

namespace kg
{
	class PluginRegistrationException : public std::exception
	{
		int m_id;

	public:
		PluginRegistrationException( const int& pluginId )
			:m_id( pluginId )
		{ };

		virtual const char* what()const
		{
			return ("plugin with id" + std::to_string(m_id) + "is already registered").c_str();
		};
	};
}