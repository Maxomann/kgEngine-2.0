#include "Core.h"

namespace kg
{
	void Core::init()
	{
		//Add Components here
		m_engine.pluginManager.addComponentPlugin(
			std::make_shared<PluginFactory<Component, PositionComponent>>( ComponentId::PositionComponent, "Position" ) );
		m_engine.pluginManager.addComponentPlugin(
			std::make_shared<PluginFactory<Component, SpriteComponent>>( ComponentId::SpriteComponent, "Sprite" ) );

		//Add Systems here
		m_engine.pluginManager.addSystemPlugin(
			std::make_shared<PluginFactory<System, CameraSystem>>( SystemId::CameraSystem, "CameraSystem" ) );



		m_engine.renderWindow.create( sf::VideoMode( 1080, 720 ), "kgEngine 2.0" );

		m_engine.blueprint.parse( "./test.blueprint" );
		m_engine.blueprint.link();

		for( auto& el : m_engine.pluginManager.createEverySystemAvailable() )
			m_client.addSystem( std::get<2>( el ), std::get<1>( el ) );

		//init systems
		m_client.initSystems( m_engine );



		m_client.addEntity( EntityManager::createEntity( m_engine, 100 ) ).second->getComponent<PositionComponent>()->setPosition( { 0, 0 } );
		m_client.addEntity( EntityManager::createEntity( m_engine, 100 ) ).second->getComponent<PositionComponent>()->setPosition( { 400, 0 } );
		m_client.addEntity( EntityManager::createEntity( m_engine, 100 ) ).second->getComponent<PositionComponent>()->setPosition( { 36, 400 } );
	}

	bool Core::shouldTerminate() const
	{
		return m_engine.shouldTerminate;
	}

	void Core::update()
	{
		m_engine.renderWindow.clear( sf::Color::Green );

		m_client.updateEntities( m_engine, m_client );
		m_client.updateAllSystemsByImportance( m_engine, m_client );


		sf::Event event;
		while( m_engine.renderWindow.pollEvent( event ) )
		{
			// "close requested" event: we close the window
			if( event.type == sf::Event::Closed )
				m_engine.shouldTerminate = true;
		}

		//draw here
		m_engine.renderWindow.display();
	}
}
