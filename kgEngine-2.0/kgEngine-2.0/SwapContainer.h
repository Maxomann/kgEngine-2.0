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
			m_two( new Container() ),
			m_activeContainer( &m_one ),
			m_inActiveContainer( &m_two )
		{ };

		DLL_EXPORT std::unique_ptr<Container>& swap()
		{
			m_rwMutex.lock();
			if( m_activeContainer == &m_one )
			{
				m_one = std::make_unique<Container>();
				m_inActiveContainer = &m_one;
				m_activeContainer = &m_two;
			}
			else
			{
				m_two = std::make_unique<Container>();
				m_inActiveContainer = &m_two;
				m_activeContainer = &m_one;
			}
			m_rwMutex.unlock();

			return *m_activeContainer;
		};

		DLL_EXPORT void push( T&& element )
		{
			m_rwMutex.lock();
			(*m_inActiveContainer)->push( std::move(element) );
			m_rwMutex.unlock();
		};

		/*DLL_EXPORT std::unique_ptr<Container>& getContent()
		{
		m_rwMutex.lock();
		*m_activeContainer;
		m_rwMutex.unlock();
		return retVal;
		};*/

	private:
		std::unique_ptr<Container> m_one;
		std::unique_ptr<Container> m_two;
		std::unique_ptr<Container>* m_activeContainer;
		std::unique_ptr<Container>* m_inActiveContainer;

		boost::mutex m_rwMutex;
	};
}
