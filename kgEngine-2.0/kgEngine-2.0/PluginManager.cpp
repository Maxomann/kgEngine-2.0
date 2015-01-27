#include "PluginManager.h"

namespace kg
{
	void PluginManager::addComponentPlugin( const std::shared_ptr<PluginFactoryInterface<Component>>& componentPluginFactory )
	{
		int id = componentPluginFactory->getId();

		if( m_componentPluginFactorysById.find( id ) != m_componentPluginFactorysById.end() )
			throw PluginRegistrationException( id );

		m_componentPluginFactorysById[id] = componentPluginFactory;
		m_componentPluginFactorysByName[componentPluginFactory->getName()] = componentPluginFactory;
	}

	void PluginManager::addSystemPlugin( const std::shared_ptr<PluginFactoryInterface<System>>& systemPluginFactory )
	{
		int id = systemPluginFactory->getId();

		if( m_systemPluginFactorysById.find( id ) != m_systemPluginFactorysById.end() )
			throw PluginRegistrationException( id );

		m_systemPluginFactorysById[id] = systemPluginFactory;
		m_systemPluginFactorysByName[systemPluginFactory->getName()] = systemPluginFactory;
	}

	std::vector<std::tuple<int, size_t, std::shared_ptr<System>>> PluginManager::createEverySystemAvailable()const
	{
		std::vector<std::tuple<int, size_t, std::shared_ptr<System>>> returnValue;

		for( const auto& el : m_systemPluginFactorysById )
		{
			std::tuple<int, size_t, std::shared_ptr<System>> tempRetVal;

			std::get<0>( tempRetVal ) = el.second->getId();
			std::get<1>( tempRetVal ) = el.second->getRealTypeHashCode();
			std::get<2>( tempRetVal ) = el.second->create();

			returnValue.push_back( tempRetVal );
		}

		return returnValue;
	}

	const std::pair<int, std::string> PluginManager::getPluginInformationByRealTypeHashCode( size_t hashCode )const
	{
		for( const auto& el : m_componentPluginFactorysById )
			if( el.second->getRealTypeHashCode() == hashCode )
				return std::pair<int, std::string>( el.second->getId(), el.second->getName() );
		return std::pair<int, std::string>( -1, "getPluginInformationByRealTypeHashCode::NOT_FOUND. HashCode: " + std::to_string( hashCode ) );
	}

	std::tuple<int, size_t, std::shared_ptr<Component>> PluginManager::createComponentPlugin( const int& pluginId )
	{
		try
		{
			auto& componentPluginFactory = m_componentPluginFactorysById.at( pluginId );

			return std::make_tuple( componentPluginFactory->getId(), componentPluginFactory->getRealTypeHashCode(), componentPluginFactory->create() );
		}
		catch( std::out_of_range& e )
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
			auto& systemPluginFactory = m_systemPluginFactorysById.at( pluginId );

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
}