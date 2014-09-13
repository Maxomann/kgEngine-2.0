#include "Core.h"

namespace kg
{


	void Core::init()
	{
		m_engine.renderWindow.create( sf::VideoMode( 1080, 720 ), "kgEngine 2.0" );
	}

	bool Core::shouldTerminate() const
	{
		return m_engine.shouldTerminate;
	}

	void Core::update()
	{
		m_client.updateEntities( m_engine, m_client );
		m_client.updateAllSystemsByImportance( m_engine, m_client );


		sf::Event event;
		while( m_engine.renderWindow.pollEvent( event ) )
		{
			// "close requested" event: we close the window
			if( event.type == sf::Event::Closed )
				m_engine.shouldTerminate = true;
		}

		m_engine.renderWindow.clear( sf::Color::Green );
		//draw here
		m_engine.renderWindow.display();
	}
}
