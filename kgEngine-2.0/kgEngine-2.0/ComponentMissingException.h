#pragma once
#include "stdafx.h"

namespace kg
{
	class DLL_EXPORT ComponentMissingException : public std::exception
	{
		const std::string m_msg;

	public:
		ComponentMissingException( const std::string& requieringComponentName,
								   int requieringComponentId,
								   const std::string& missingComponentName,
								   int missingComponentId )
								   : m_msg(
								   std::string( "ComponentMissingException\n") +
								   std::string( "requieringComponentName=" ) + requieringComponentName + std::string( ":" ) + std::to_string( requieringComponentId ) + std::string( "\n") +
								   std::string( "missingComponentName=" ) + missingComponentName + std::string( ":" ) + std::to_string( missingComponentId ) + std::string( "\n") )
		{

		}

		virtual const char* what()const
		{
			return m_msg.c_str();
		};
	};
}
