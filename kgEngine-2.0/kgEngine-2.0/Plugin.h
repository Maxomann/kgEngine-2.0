#pragma once
#include "stdafx.h"

namespace kg
{
	class DLL_EXPORT Plugin
	{
	public:
		virtual const std::string& getPluginName()const = 0;
		virtual const int getPluginId()const = 0;
	};
}
