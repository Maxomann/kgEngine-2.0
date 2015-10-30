#include "PauseLock.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	
	bool PauseLock::operator==( const PauseLock& other )
	{
		return this->m_id == other.m_id;
	}

	PauseLock::PauseLock( const PauseLock::Id& id )
	{
		m_id = id;
	}

	void PauseLock::setActive( bool activate )
	{
		m_isActive = activate;
	}

	void PauseLock::activate()
	{
		setActive( true );
	}

	void PauseLock::deactivate()
	{
		setActive( false );
	}

	bool PauseLock::isActive() const
	{
		return m_isActive;
	}

	PauseLock::Ptr PauseLockManager::requestLock()
	{
		PauseLock lock(idCounter);
		idCounter++;

		m_locks.push_back( move( lock ) );
		return &m_locks.back();
	}

	void PauseLockManager::freeLock( const PauseLock& lock )
	{
		m_locks.remove( lock );
	}

	bool PauseLockManager::isPaused() const
	{
		for( const auto& lock : m_locks )
			if( lock.isActive() )
				return true;
		return false;
	}

}
