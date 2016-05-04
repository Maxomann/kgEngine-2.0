#include "Core.h"

using namespace std;
using namespace sf;
using namespace tr2;
using namespace sys;

namespace kg
{
	Core::Core()
		:m_gameStateManager( m_engine, m_world, m_saveManager )
	{ }

	void Core::init()
	{
		loadPackages();

		for( auto& el : m_engine.pluginManager.createEverySystemAvailable() )
			m_world.addSystem( move( el ) );

		//init systems
		m_world.initSystemsByImportance( m_engine, m_world, m_saveManager );
		m_gameStateManager.init();
	}

	bool Core::shouldTerminate() const
	{
		return m_engine.shouldTerminate;
	}

	void Core::terminate()
	{
		m_gameStateManager.clear();
		m_world.saveSystemConfigFilesByImportance( m_engine );
	}

	void Core::update()
	{
		sf::Time frameTime = m_frameTimeClock.restart();
		if( frameTime.asMilliseconds() < MIN_FRAME_TIME_IN_MILLISECONDS )//ensure that frameTime is bigger than MIN_FRAME_TIME_IN_MILLISECONDS
		{
			sleep( milliseconds( MIN_FRAME_TIME_IN_MILLISECONDS ) );
			frameTime += m_frameTimeClock.restart();
		}
		if( m_engine.pauseLockManager.isPaused() )
			frameTime = sf::seconds( 0 );
		m_gameStateManager.forwardFrameTime( frameTime );

		Event event;
		m_engine.inputManager.clearEvents();
		while( m_engine.renderWindow.pollEvent( event ) )
		{
			m_world.forwardSfmlEventByImportance( m_engine, m_world, m_saveManager, event );
			m_engine.inputManager.forwardSfmlEvent( event );
		}

		m_engine.inputManager.triggerCallbacks( m_engine.renderWindow );
		if( !m_engine.pauseLockManager.isPaused() )//if game is not paused, update entities
			m_world.updateEntities( m_engine, m_world, frameTime );
		m_world.updateAllSystemsByImportance( m_engine, m_world, m_saveManager, frameTime );
		m_gameStateManager.onUpdate();

		m_world.performAddRemoveActions();
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
			m_engine.blueprint.parse( el.string() );
		m_engine.blueprint.link();

		for( const auto& el : dllsToLoad )
		{
#ifdef _WIN32
			HMODULE dllHandle = LoadLibrary( el.string().c_str() );
			if( dllHandle )
			{
				CONNECT connectFunction = ( CONNECT )GetProcAddress( dllHandle, "kgConnect" );
				if( connectFunction )
					connectFunction( m_engine.pluginManager );
			}
#endif // _WIN32
		}
	}
}