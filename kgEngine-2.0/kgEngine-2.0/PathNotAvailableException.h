#pragma once
#include "stdafx.h"

namespace kg
{
	class DLL_EXPORT PathNotAvailableException : public std::exception
	{
		const std::string m_msg;

	public:
		PathNotAvailableException( const std::string& path )
			:m_msg( "Path not found: " + path )
		{
		}

		virtual const char* what()const
		{
			return m_msg.c_str();
		};
	};
}
