#include "ConsoleGameState.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	void ConsoleGameState::printLogEntry( const LogEntry& entry )
	{
		switch( entry.type )
		{
		case LOG_TYPE::LOG:
			m_chatBox->setTextColor( sf::Color( 204, 204, 204 ) );//Grey
			break;
		case LOG_TYPE::INFO:
			m_chatBox->setTextColor( sf::Color::Green );
			break;
		case LOG_TYPE::ERR:
			m_chatBox->setTextColor( sf::Color::Red );
			break;
		}

		char time_converted[100];
		tm localTime;
		localtime_s( &localTime, &entry.timestamp );
		strftime( time_converted, sizeof( time_converted ), "%D %T", &localTime );

		m_chatBox->addLine( to_string( time_converted ) );
		m_chatBox->addLine( entry.message );
	}

	void ConsoleGameState::onInit()
	{
		m_connectToSignal_safe( r_engine->console.s_entryAdded, &ConsoleGameState::printLogEntry );

		for( const auto& el : r_engine->console.getLog() )
			printLogEntry( el );

		return;
	}

	void ConsoleGameState::registerGui( tgui::Gui& gui )
	{
		m_consoleWindow = make_shared<ChildWindow>();
		m_chatBox = make_shared<ChatBox>();

		m_consoleWindow->setSize( 800, 400 );
		m_consoleWindow->keepInParent( true );
		m_consoleWindow->connect( "closed", &ConsoleGameState::close, this );
		m_consoleWindow->setTitle( "Console" );

		m_chatBox->setSize( 800, 400 );
		m_chatBox->getRenderer()->setBackgroundColor( sf::Color::Black );

		m_consoleWindow->add( m_chatBox );
		gui.add( m_consoleWindow );
	}

	void ConsoleGameState::registerInputCallbacks( InputManager& inputManager )
	{
		return;
	}

	void ConsoleGameState::onUpdate()
	{
		return;
	}

	void ConsoleGameState::removeInputCallbacks( InputManager& inputManager )
	{
		return;
	}

	void ConsoleGameState::removeGui( tgui::Gui& gui )
	{
		gui.remove( m_consoleWindow );
	}

	void ConsoleGameState::onDestroy()
	{
		return;
	}

	const Plugin::Name& ConsoleGameState::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id ConsoleGameState::getPluginId() const
	{
		return id::GameStatePluginId::CONSOLE;
	}

	const std::string ConsoleGameState::PLUGIN_NAME = "Console GameState";

	void ConsoleGameState::close()
	{
		r_gameStateManager->removeAllInstancesOf( id::GameStatePluginId::CONSOLE );
	}
}