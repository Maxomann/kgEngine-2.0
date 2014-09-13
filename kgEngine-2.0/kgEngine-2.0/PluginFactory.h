#pragma once
#include "stdafx.h"

namespace kg
{
	template<class T>
	class PluginFactoryInterface
	{
		const int m_pluginId = -1;

	public:
		PluginFactoryInterface( const int& pluginId )
			:m_pluginId( pluginId )
		{ };

		int getId()const
		{
			return m_pluginId;
		};

		virtual std::shared_ptr<T> create()const = 0;
	};

	template<class FakeType, class RealType>
	class PluginFactory : PluginFactoryInterface<FakeType>
	{
		const int m_pluginId = -1;

	public:

		std::shared_ptr<FakeType> create()
		{
			return std::static_pointer_cast<FakeType>( std::make_shared<RealType>() );
		};
	};
}
