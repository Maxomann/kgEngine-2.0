#pragma once
#include "stdafx.h"
#pragma warning(push)
#pragma warning(disable: 4275) // possible loss of data

namespace kg
{
	//throw in preInit if a requiered blueprint value is not found
	class DLL_EXPORT BlueprintValueReadErrorException : public std::exception
	{
		const std::string m_msg;

	public:
		BlueprintValueReadErrorException( const std::string& pluginName, const std::string& identifier )
			:m_msg( "component: " + pluginName + " is missing value: " + identifier )
		{ }

		virtual const char* what()const
		{
			return m_msg.c_str();
		};
	};
}

#pragma warning(pop)