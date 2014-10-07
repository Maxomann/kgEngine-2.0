#pragma once
#include "stdafx.h"

namespace kg
{
	template<class T>
	class PluginFactoryInterface
	{
		const int m_pluginId = -1;
		const std::string m_name = "-1";

	public:
		PluginFactoryInterface( const int& pluginId, const std::string& pluginName )
			:m_pluginId( pluginId ),
			m_name( pluginName )
		{ }

		int getId()const
		{
			return m_pluginId;
		}

		std::string getName()const
		{
			return m_name;
		}

		virtual std::shared_ptr<T> create()const = 0;

		virtual size_t getRealTypeHashCode()const = 0;
	};

	template<class FakeType, class RealType>
	class PluginFactory : public PluginFactoryInterface<FakeType>
	{
	public:
		PluginFactory( const int& pluginId, const std::string& pluginName )
			: PluginFactoryInterface( pluginId, pluginName )
		{ }

		virtual std::shared_ptr<FakeType> create()const
		{
			return std::static_pointer_cast<FakeType>( std::make_shared<RealType>() );
		};

		virtual size_t getRealTypeHashCode() const
		{
			auto retVal = typeid(RealType).hash_code();
			return retVal;
		}

	};
}
