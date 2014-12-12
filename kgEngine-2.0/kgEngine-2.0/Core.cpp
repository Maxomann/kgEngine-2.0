#include "Core.h"

using namespace std;
using namespace sf;
using namespace tr2;
using namespace sys;

namespace kg
{
	void Core::init()
	{
		/*m_engine.renderWindow.create( sf::VideoMode( 200, 100 ), "kgEngine 2.0 - Window not initialized" );
		m_engine.renderWindow.setVerticalSyncEnabled( true ); -now done in GraphicsSystem*/

		loadPackages();
		m_engine.blueprint.link();

		for( auto& el : m_engine.pluginManager.createEverySystemAvailable() )
			m_world.addSystem( get<2>( el ), get<1>( el ) );

		//init systems
		m_world.initSystemsByImportance( m_engine, m_world );
	}

	bool Core::shouldTerminate() const
	{
		return m_engine.shouldTerminate;
	}

	void Core::update()
	{
		m_engine.renderWindow.clear( Color::Red );
		sf::Time frameTime = m_frameTimeClock.restart();
		if( m_engine.isPaused )
			frameTime = sf::microseconds( 0 );

		Event event;
		while( m_engine.renderWindow.pollEvent( event ) )
		{
			m_world.forwardSfmlEventByImportance( m_engine, event );
		}

		if( !m_engine.isPaused )//if engine is not paused, update entities
			m_world.updateEntities( m_engine, m_world, frameTime);
		m_world.updateAllSystemsByImportance( m_engine, m_world, frameTime );

		//draw here
		m_engine.renderWindow.display();
	}

	void Core::loadPackages()
	{
		auto packagesFolderPath = path( "./" );

		std::vector<path> dllsToLoad;
		std::vector<path> blueprintsToParse;

		for( auto it = recursive_directory_iterator( packagesFolderPath ); it != recursive_directory_iterator(); ++it )
		{
			const auto& file = it->path();
			if( file.extension() == blueprint::file_extension )
				blueprintsToParse.push_back( file );
			else if( file.extension() == ".dll" )
				dllsToLoad.push_back( file );
		}

		//load blueprints
		for( const auto& el : blueprintsToParse )
			m_engine.blueprint.parse( el );

		for( const auto& el : dllsToLoad )
		{
#ifdef _WIN32
			HMODULE dllHandle = LoadLibrary( std::string( el ).c_str() );
			CONNECT connectFunction = ( CONNECT )GetProcAddress( dllHandle, "kgConnect" );
			if( connectFunction )
				connectFunction( m_engine.pluginManager );
#endif // _WIN32
		}
	}
}
