#pragma once
#include "stdafx.h"

namespace kg
{
	class DLL_EXPORT Plugin
	{
	public:
		typedef int Id;

		virtual const std::string& getPluginName()const = 0;
		virtual Plugin::Id getPluginId()const = 0;
	};
}
