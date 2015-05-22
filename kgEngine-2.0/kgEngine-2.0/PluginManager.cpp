#include "PluginManager.h"

namespace kg
{
	std::vector<std::shared_ptr<System>> PluginManager::createEverySystemAvailable() const
	{
		std::vector<std::shared_ptr<System>> retVal;

		for( const auto& el : m_systemPluginFactorysById )
			retVal.push_back( el.second->create() );

		return retVal;
	}
}