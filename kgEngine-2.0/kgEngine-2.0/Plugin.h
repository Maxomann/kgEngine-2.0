#pragma once
#include "stdafx.h"

namespace kg
{
	class DLL_EXPORT Plugin
	{
	public:
		typedef int Id;

		virtual const std::string& getPluginName()const = 0;
		virtual int getPluginId()const = 0;
	};
}
