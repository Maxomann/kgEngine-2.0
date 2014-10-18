#pragma once
#include "stdafx.h"

namespace kg
{
	class ComponentMissingException : public std::exception
	{
		std::string m_msg;

	public:
		ComponentMissingException( const std::string& requieringComponentName,
								   int requieringComponentId,
								   const std::string& missingComponentName,
								   int missingComponentId )
		{
			m_msg += "ComponentMissingException\n";
			m_msg += "requieringComponentName=" + requieringComponentName + ":" + std::to_string( requieringComponentId ) + "\n";
			m_msg += "missingComponentName=" + missingComponentName + ":" + std::to_string( missingComponentId ) + "\n";
		}

		virtual const char* what()const
		{
			return m_msg.c_str();
		};
	};
}
