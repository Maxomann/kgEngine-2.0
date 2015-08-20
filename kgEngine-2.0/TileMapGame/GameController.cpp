#include "GameController.h"
using namespace std;
using namespace sf;
using Action = thor::Action;

namespace kg
{
	void GameController::m_onSavegameOpened( Engine& engine )
	{
		m_cameraZoomFactor = 1;
	}

	void GameController::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		r_engine = &engine;
		r_world = &world;
		r_saveManager = &saveManager;
		r_graphicsSystem = world.getSystem<GraphicsSystem>();

		m_connectToSignal( saveManager.s_savegameOpened, &GameController::m_onSavegameOpened );
		registerInputCallbacks( engine.inputManager );

		saveManager.openSavegame( engine, world, "MyFirstSavegameEver" );

		return;
	}

	void GameController::sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent )
	{
		if( sfEvent.type == Event::Closed )
			shutDown();
		return;
	}

	void GameController::update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime )
	{
		lastFrameTimeInMilliseconds = frameTime.asMilliseconds();

		if( !engine.isPaused )
		{

		}

		return;
	}

	double GameController::getUpdateImportance() const
	{
		return ( double )id::SystemUpdateImportance::GAME_CONTROLLER;
	}

	const std::string& GameController::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id GameController::getPluginId() const
	{
		return ( int )id::SystemPluginId::GAME_CONTROLLER;
	}

	const size_t& GameController::getRTTI_hash() const
	{
		return type_hash;
	}

	void GameController::saveOpenSavegame( Engine& engine, World& world, SaveManager& saveManager )
	{
		world.getSystem<ChunkSystem>()->saveAllLoadedChunks( engine, world, saveManager );
		saveManager.saveSystems( world );
	}

	void GameController::movePlayer( sf::Vector2i distance )
	{
		if( m_camera.expired() )
			m_camera = r_graphicsSystem->getCamera( 0 );
		auto camera = m_camera.lock();

		camera->getComponent<Transformation>()->move( distance );
	}

	const std::string GameController::PLUGIN_NAME = "GameControllerSystem";

	const size_t GameController::type_hash = getRuntimeTypeInfo<GameController>();


	void GameController::registerInputCallbacks( InputManager& inputManager )
	{
		// Keys
		Action shift( Keyboard::LShift, Action::Hold );
		Action control( Keyboard::LControl, Action::Hold );

		Action w( Keyboard::W, Action::Hold );
		Action a( Keyboard::A, Action::Hold );
		Action s( Keyboard::S, Action::Hold );
		Action d( Keyboard::D, Action::Hold );

		Action add( Keyboard::Add, Action::Hold );
		Action subtract( Keyboard::Subtract, Action::Hold );

		Action escapePress( Keyboard::Escape, Action::PressOnce );
		Action f5Press( Keyboard::F5, Action::PressOnce );


		// Events
		Action eventClose( Event::Closed );


		// Set the actions and register callbacks
		inputManager.setAction( id::Input::SHUT_DOWN,
								escapePress || eventClose,
								bind( &GameController::shutDown, this ) );
		inputManager.setAction( id::Input::RELOAD_SAVE, f5Press,
								bind( &GameController::reloadSave, this ) );

		inputManager.setAction( id::Input::MOVE_UP, w && !shift && !control,
								bind( &GameController::moveUp, this ) );
		inputManager.setAction( id::Input::MOVE_DOWN, s && !shift && !control,
								bind( &GameController::moveDown, this ) );
		inputManager.setAction( id::Input::MOVE_LEFT, a && !shift && !control,
								bind( &GameController::moveLeft, this ) );
		inputManager.setAction( id::Input::MOVE_RIGHT, d && !shift && !control,
								bind( &GameController::moveRight, this ) );

		inputManager.setAction( id::Input::MOVE_UP_FAST, w && shift && !control,
								bind( &GameController::moveUpFast, this ) );
		inputManager.setAction( id::Input::MOVE_DOWN_FAST, s && shift && !control,
								bind( &GameController::moveDownFast, this ) );
		inputManager.setAction( id::Input::MOVE_LEFT_FAST, a && shift && !control,
								bind( &GameController::moveLeftFast, this ) );
		inputManager.setAction( id::Input::MOVE_RIGHT_FAST, d && shift && !control,
								bind( &GameController::moveRightFast, this ) );

		inputManager.setAction( id::Input::MOVE_UP_SLOW, w && !shift && control,
								bind( &GameController::moveUpSlow, this ) );
		inputManager.setAction( id::Input::MOVE_DOWN_SLOW, s && !shift && control,
								bind( &GameController::moveDownSlow, this ) );
		inputManager.setAction( id::Input::MOVE_LEFT_SLOW, a && !shift && control,
								bind( &GameController::moveLeftSlow, this ) );
		inputManager.setAction( id::Input::MOVE_RIGHT_SLOW, d && !shift && control,
								bind( &GameController::moveRightSlow, this ) );

		inputManager.setAction( id::Input::ZOOM_IN, add,
								bind( &GameController::zoomIn, this ) );
		inputManager.setAction( id::Input::ZOOM_OUT, subtract,
								bind( &GameController::zoomOut, this ) );

	}

	void GameController::shutDown()
	{
		saveOpenSavegame( *r_engine, *r_world, *r_saveManager );
		r_engine->shouldTerminate = true;
	}

	void GameController::reloadSave()
	{
		saveOpenSavegame( *r_engine, *r_world, *r_saveManager );
		r_saveManager->openSavegame( *r_engine, *r_world, "MyFirstSavegameEver" );
	}

	void GameController::pause()
	{
		r_engine->isPaused = !r_engine->isPaused;
	}

	void GameController::moveUp()
	{
		sf::Vector2i cameraMovement( 0, -10.0 / 16.0 * lastFrameTimeInMilliseconds );
		movePlayer( cameraMovement );
	}

	void GameController::moveDown()
	{
		sf::Vector2i cameraMovement( 0, 10.0 / 16.0 * lastFrameTimeInMilliseconds );
		movePlayer( cameraMovement );
	}

	void GameController::moveLeft()
	{
		sf::Vector2i cameraMovement( -10.0 / 16.0 * lastFrameTimeInMilliseconds, 0 );
		movePlayer( cameraMovement );
	}

	void GameController::moveRight()
	{
		sf::Vector2i cameraMovement( 10.0 / 16.0 * lastFrameTimeInMilliseconds, 0 );
		movePlayer( cameraMovement );
	}

	void GameController::moveUpFast()
	{
		sf::Vector2i cameraMovement( 0, -10.0 / 16.0 * lastFrameTimeInMilliseconds * 4 );
		movePlayer( cameraMovement );
	}

	void GameController::moveDownFast()
	{
		sf::Vector2i cameraMovement( 0, 10.0 / 16.0 * lastFrameTimeInMilliseconds * 4 );
		movePlayer( cameraMovement );
	}

	void GameController::moveLeftFast()
	{
		sf::Vector2i cameraMovement( -10.0 / 16.0 * lastFrameTimeInMilliseconds * 4, 0 );
		movePlayer( cameraMovement );
	}

	void GameController::moveRightFast()
	{
		sf::Vector2i cameraMovement( 10.0 / 16.0 * lastFrameTimeInMilliseconds * 4, 0 );
		movePlayer( cameraMovement );
	}

	void GameController::moveUpSlow()
	{
		sf::Vector2i cameraMovement( 0, -10.0 / 16.0 * lastFrameTimeInMilliseconds / 2 );
		movePlayer( cameraMovement );
	}

	void GameController::moveDownSlow()
	{
		sf::Vector2i cameraMovement( 0, 10.0 / 16.0 * lastFrameTimeInMilliseconds / 2 );
		movePlayer( cameraMovement );
	}

	void GameController::moveLeftSlow()
	{
		sf::Vector2i cameraMovement( -10.0 / 16.0 * lastFrameTimeInMilliseconds / 2, 0 );
		movePlayer( cameraMovement );
	}

	void GameController::moveRightSlow()
	{
		sf::Vector2i cameraMovement( 10.0 / 16.0 * lastFrameTimeInMilliseconds / 2, 0 );
		movePlayer( cameraMovement );
	}

	void GameController::zoomIn()
	{
		if( m_camera.expired() )
			m_camera = r_graphicsSystem->getCamera( 0 );
		auto camera = m_camera.lock();

		m_cameraZoomFactor -= 0.01*lastFrameTimeInMilliseconds;

		if( m_cameraZoomFactor < 0 )
			m_cameraZoomFactor = 0;
		camera->getComponent<Transformation>()->setSize( Vector2i( (double)1280.0 * m_cameraZoomFactor, (double)720.0 * m_cameraZoomFactor ) );
	}

	void GameController::zoomOut()
	{
		if( m_camera.expired() )
			m_camera = r_graphicsSystem->getCamera( 0 );
		auto camera = m_camera.lock();

		m_cameraZoomFactor += 0.01*lastFrameTimeInMilliseconds;

		if( m_cameraZoomFactor < 0 )
			m_cameraZoomFactor = 0;
		camera->getComponent<Transformation>()->setSize( Vector2i( (double)1280.0 * m_cameraZoomFactor, (double)720.0 * m_cameraZoomFactor ) );
	}

}
