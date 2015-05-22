#pragma once
#include "stdafx.h"

namespace kg
{
	class DLL_EXPORT Plugin
	{
	public:
		virtual ~Plugin()
		{ };

		typedef int Id;
		typedef std::string Name;

		virtual const Plugin::Name& getPluginName()const = 0;
		virtual Plugin::Id getPluginId()const = 0;
	};
}
