#include "Core.h"

namespace kg
{


	bool Core::shouldTerminate() const
	{
		return m_engine.m_shouldTerminate;
	}

	void Core::update()
	{
		m_world.updateEntities( m_engine, m_world );
		m_world.updateAllSystemsByImportance( m_engine, m_world );
	}

}
