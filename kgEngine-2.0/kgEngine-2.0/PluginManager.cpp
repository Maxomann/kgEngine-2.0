#include "PluginManager.h"

namespace kg
{


	void PluginManager::addComponentPlugin( const std::shared_ptr<PluginFactoryInterface<Component>>& componentPluginFactory )
	{
		int id = componentPluginFactory->getId();

		if( m_componentPluginFactorys.find( id ) != m_componentPluginFactorys.end() )
			throw PluginRegistrationException( id );

		m_componentPluginFactorys[id] = componentPluginFactory;
		m_componentPluginFactorysByName[componentPluginFactory->getName()] = componentPluginFactory;
	}

	void PluginManager::addSystemPlugin( const std::shared_ptr<PluginFactoryInterface<System>>& systemPluginFactory )
	{
		int id = systemPluginFactory->getId();

		if( m_systemPluginFactorys.find( id ) != m_systemPluginFactorys.end() )
			throw PluginRegistrationException( id );

		m_systemPluginFactorys[id] = systemPluginFactory;
		m_systemPluginFactorysByName[systemPluginFactory->getName()] = systemPluginFactory;
	}

	std::tuple<int, size_t, std::shared_ptr<Component>> PluginManager::createComponentPlugin( const int& pluginId )
	{
		try
		{
			auto& componentPluginFactory = m_componentPluginFactorys.at( pluginId );

			return std::make_tuple( componentPluginFactory->getId(), componentPluginFactory->getRealTypeHashCode(), componentPluginFactory->create());
		}
		catch (std::out_of_range& e)
		{
			e;//to get rid of annoying warning C4101
			throw PluginRequestException( pluginId );
		}
	}

	std::tuple<int, size_t, std::shared_ptr<Component>> PluginManager::createComponentPlugin( const std::string& pluginName )
	{
		try
		{
			auto& componentPluginFactory = m_componentPluginFactorysByName.at( pluginName );

			return std::make_tuple( componentPluginFactory->getId(), componentPluginFactory->getRealTypeHashCode(), componentPluginFactory->create() );
		}
		catch( std::out_of_range& e )
		{
			e;//to get rid of annoying warning C4101
			throw PluginRequestException( pluginName );
		}
	}

	std::tuple<int, size_t, std::shared_ptr<System>> PluginManager::createSystemPlugin( const int& pluginId )
	{
		try
		{
			auto& systemPluginFactory = m_systemPluginFactorys.at( pluginId );

			return std::make_tuple( systemPluginFactory->getId(), systemPluginFactory->getRealTypeHashCode(), systemPluginFactory->create() );
		}
		catch( std::out_of_range& e )
		{
			e;//to get rid of annoying warning C4101
			throw PluginRequestException( pluginId );
		}
	}

	std::tuple<int, size_t, std::shared_ptr<System>> PluginManager::createSystemPlugin( const std::string& pluginName )
	{
		try
		{
			auto& systemPluginFactory = m_systemPluginFactorysByName.at( pluginName );

			return std::make_tuple( systemPluginFactory->getId(), systemPluginFactory->getRealTypeHashCode(), systemPluginFactory->create() );
		}
		catch( std::out_of_range& e )
		{
			e;//to get rid of annoying warning C4101
			throw PluginRequestException( pluginName );
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

	const std::pair<int, std::string> PluginManager::getPlugininformationByRealTypeHashCode( size_t hashCode )const
	{
		for( const auto& el : m_componentPluginFactorys )
			if( el.second->getRealTypeHashCode() == hashCode )
				return std::pair<int, std::string>( el.second->getId(), el.second->getName() );
		return std::pair<int, std::string>( -1, "getPlugininformationByRealTypeHashCode::NOT_FOUND. HashCode: " + std::to_string( hashCode ) );
	}

}
