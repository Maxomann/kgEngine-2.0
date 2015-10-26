#include "PluginManager.h"

namespace kg
{
	std::vector<std::unique_ptr<System>> PluginManager::createEverySystemAvailable() const
	{
		std::vector<std::unique_ptr<System>> retVal;

		for( const auto& el : m_systemPluginFactorysById )
			retVal.push_back( el.second->create() );

		return retVal;
	}
}