#pragma once
#include "stdafx.h"
#pragma warning(push)
#pragma warning(disable: 4275) // possible loss of data

namespace kg
{
	class DLL_EXPORT ComponentMissingException : public std::exception
	{
		const std::string m_msg;

	public:
		ComponentMissingException( int requieringComponentId,
								   int missingComponentId )
			: m_msg(
				std::string( "ComponentMissingException\n" ) +
				std::to_string( requieringComponentId ) + std::string( "\n" ) +
				std::to_string( missingComponentId ) + std::string( "\n" ) )
		{ }

		virtual const char* what()const
		{
			return m_msg.c_str();
		};
	};
}

#pragma warning(pop)