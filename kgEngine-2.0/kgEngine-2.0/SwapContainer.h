#pragma once
#include "stdafx.h"

namespace kg
{
	template< class T, class Container = std::queue<T> >
	class SwapContainer
	{
	public:
		DLL_EXPORT SwapContainer() :
			m_one( new Container() ),
			m_two( new Container() )
		{};

		DLL_EXPORT void swap()
		{
			m_rwMutex.lock();
			if( m_activeContainer == m_one )
			{
				m_one = std::make_shared<Container>();
				m_inActiveContainer = m_one;
				m_activeContainer = m_two;
			}
			else
			{
				m_two = std::make_shared<Container>();
				m_inActiveContainer = m_two;
				m_activeContainer = m_one;
			}
			m_rwMutex.unlock();
		};

		DLL_EXPORT void push( T& element )
		{
			m_rwMutex.lock();
			m_inActiveContainer->push( element );
			m_rwMutex.unlock();
		};

		DLL_EXPORT std::shared_ptr<Container> getContent()
		{
			m_rwMutex.lock();
			std::shared_ptr<Container> retVal = m_activeContainer;
			m_rwMutex.unlock();
			return retVal;
		};

	private:
		std::shared_ptr<Container> m_one;
		std::shared_ptr<Container> m_two;
		std::shared_ptr<Container> m_activeContainer = m_one;
		std::shared_ptr<Container> m_inActiveContainer = m_two;

		std::mutex m_rwMutex;
	};
}