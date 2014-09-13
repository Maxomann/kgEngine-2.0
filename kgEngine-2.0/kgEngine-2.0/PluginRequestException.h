#pragma once
#include "stdafx.h"

namespace kg
{
	class PluginRequestException : public std::exception
	{
		int m_id;

	public:
		PluginRequestException( const int& pluginId )
			:m_id( pluginId )
		{ };

		virtual const char* what()const
		{
			return ("plugin with id" + std::to_string( m_id ) + "does not exist").c_str();
		};
	};
}
