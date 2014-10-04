#include "PluginManager.h"

namespace kg
{


	void PluginManager::addComponentPlugin( const std::shared_ptr<PluginFactoryInterface<Component>>& componentPluginFactory )
	{
		int id = componentPluginFactory->getId();

		if( m_componentPluginFactorys.find( id ) != m_componentPluginFactorys.end() )
			throw PluginRegistrationException( id );

		m_componentPluginFactorys[id] = componentPluginFactory;
	}

	void PluginManager::addSystemPlugin( const std::shared_ptr<PluginFactoryInterface<System>>& systemPluginFactory )
	{
		int id = systemPluginFactory->getId();

		if( m_systemPluginFactorys.find( id ) != m_systemPluginFactorys.end() )
			throw PluginRegistrationException( id );

		m_systemPluginFactorys[id] = systemPluginFactory;
	}

	std::shared_ptr<Component> PluginManager::createComponentPlugin( const int& pluginId )
	{
		try
		{
			return m_componentPluginFactorys.at( pluginId )->create();
		}
		catch (std::out_of_range& e)
		{
			e;//to get rid of annoying warning C4101
			throw PluginRequestException( pluginId );
		}
	}

	std::shared_ptr<System> PluginManager::createSystemPlugin( const int& pluginId )
	{
		try
		{
			return m_systemPluginFactorys.at( pluginId )->create();
		}
		catch( std::out_of_range& e )
		{
			e;//to get rid of annoying warning C4101
			throw PluginRequestException( pluginId );
		}
	}

	std::vector<std::tuple<int, size_t, std::shared_ptr<System>>> PluginManager::createEverySystemAvailable()const
	{
		std::vector<std::tuple<int, size_t, std::shared_ptr<System>>> returnValue;

		for( const auto& el : m_systemPluginFactorys )
		{
			std::tuple<int, size_t, std::shared_ptr<System>> tempRetVal;

			std::get<0>( tempRetVal ) = el.second->getId();
			std::get<1>( tempRetVal ) = el.second->getRealTypeHashCode();
			std::get<2>( tempRetVal ) = el.second->create();

			returnValue.push_back( tempRetVal );
		}

		return returnValue;
	}

}
