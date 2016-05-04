#pragma once
#include "stdafx.h"

namespace kg
{
	class PauseLockManager;

	class DLL_EXPORT PauseLock
	{
	public:
		typedef unsigned int Id;
		typedef PauseLock* Ptr;
		friend class PauseLockManager;

	private:
		Id m_id;
		bool m_isActive = false;

		PauseLock( const Id& id );

	public:
		~PauseLock() = default;
		PauseLock( const PauseLock& pauseLock ) = delete;
		PauseLock( PauseLock&& pauseLock ) = default;

		PauseLock& operator=( const PauseLock& pauseLock ) = delete;
		PauseLock& operator=( PauseLock&& pauseLock ) = default;

		void setActive( bool activate );

		void activate();
		void deactivate();

		bool isActive()const;

		bool operator==( const PauseLock& other );
	};

	class DLL_EXPORT PauseLockManager
	{
		PauseLock::Id idCounter = 0;

		std::list<PauseLock> m_locks;//list because refs must not be invalidated

	public:
		PauseLockManager() = default;
		~PauseLockManager() = default;
		PauseLockManager( const PauseLockManager& pauseLockManager ) = delete;
		PauseLockManager( PauseLockManager&& pauseLockManager ) = default;

		PauseLockManager& operator=( const PauseLockManager& pauseLockManager ) = delete;
		PauseLockManager& operator=( PauseLockManager&& pauseLockManager ) = default;

		//lock will not be activated
		PauseLock::Ptr requestLock();
		void freeLock( const PauseLock& lock );

		bool isPaused()const;
	};
}
