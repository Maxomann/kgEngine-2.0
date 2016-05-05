#include "SingleplayerGameState.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	Entity* SingleplayerGameState::m_getValidCamera()
	{
		return r_graphicsSystem->getCamera( 0 );
	}

	void SingleplayerGameState::movePlayer( sf::Vector2i distance )
	{
		auto camera = m_getValidCamera();

		camera->getComponent<TransformationComponent>()->moveXY( distance );
	}

	void SingleplayerGameState::m_onSavegameOpened( Engine& engine, World& world )
	{
		m_cameraZoomFactor = 1;
	}

	void SingleplayerGameState::onInit()
	{
		r_graphicsSystem = r_world->getSystem<GraphicsSystem>();
		r_defaultGameState = static_cast< DefaultGameState* >(r_gameStateManager->getFirstInstanceOf( id::DEFAULT_GAMESTATE_ID ));

		m_connectToSignal_safe( r_saveManager->s_savegameOpened, &SingleplayerGameState::m_onSavegameOpened );

		r_saveManager->openSavegame( *r_engine, *r_world, "MyFirstSavegameEver" );

		m_pauseLock = r_engine->pauseLockManager.requestLock();
	}

	void SingleplayerGameState::registerGui( tgui::Gui& gui )
	{
		return;
	}

	void SingleplayerGameState::registerInputCallbacks( InputManager& inputManager )
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

		Action pPress( Keyboard::P, Action::PressOnce );

		// Events
		Action eventClose( Event::Closed );

		// Set the actions and register callbacks
		inputManager.setAction( id::Input::RELOAD_SAVE, f5Press,
								bind( &SingleplayerGameState::reloadSave, this ) );

		inputManager.setAction( id::Input::MOVE_UP, w && !shift && !control,
								bind( &SingleplayerGameState::moveUp, this ) );
		inputManager.setAction( id::Input::MOVE_DOWN, s && !shift && !control,
								bind( &SingleplayerGameState::moveDown, this ) );
		inputManager.setAction( id::Input::MOVE_LEFT, a && !shift && !control,
								bind( &SingleplayerGameState::moveLeft, this ) );
		inputManager.setAction( id::Input::MOVE_RIGHT, d && !shift && !control,
								bind( &SingleplayerGameState::moveRight, this ) );

		inputManager.setAction( id::Input::MOVE_UP_FAST, w && shift && !control,
								bind( &SingleplayerGameState::moveUpFast, this ) );
		inputManager.setAction( id::Input::MOVE_DOWN_FAST, s && shift && !control,
								bind( &SingleplayerGameState::moveDownFast, this ) );
		inputManager.setAction( id::Input::MOVE_LEFT_FAST, a && shift && !control,
								bind( &SingleplayerGameState::moveLeftFast, this ) );
		inputManager.setAction( id::Input::MOVE_RIGHT_FAST, d && shift && !control,
								bind( &SingleplayerGameState::moveRightFast, this ) );

		inputManager.setAction( id::Input::MOVE_UP_SLOW, w && !shift && control,
								bind( &SingleplayerGameState::moveUpSlow, this ) );
		inputManager.setAction( id::Input::MOVE_DOWN_SLOW, s && !shift && control,
								bind( &SingleplayerGameState::moveDownSlow, this ) );
		inputManager.setAction( id::Input::MOVE_LEFT_SLOW, a && !shift && control,
								bind( &SingleplayerGameState::moveLeftSlow, this ) );
		inputManager.setAction( id::Input::MOVE_RIGHT_SLOW, d && !shift && control,
								bind( &SingleplayerGameState::moveRightSlow, this ) );

		inputManager.setAction( id::Input::ZOOM_IN, add,
								bind( &SingleplayerGameState::zoomIn, this ) );
		inputManager.setAction( id::Input::ZOOM_OUT, subtract,
								bind( &SingleplayerGameState::zoomOut, this ) );

		inputManager.setAction( id::Input::PAUSE, pPress,
								bind( &SingleplayerGameState::pause, this ) );
	}

	void SingleplayerGameState::onUpdate()
	{
		return;
	}

	void SingleplayerGameState::removeInputCallbacks( InputManager& inputManager )
	{
		inputManager.removeAction( id::Input::RELOAD_SAVE );

		inputManager.removeAction( id::Input::MOVE_UP );
		inputManager.removeAction( id::Input::MOVE_DOWN );
		inputManager.removeAction( id::Input::MOVE_LEFT );
		inputManager.removeAction( id::Input::MOVE_RIGHT );

		inputManager.removeAction( id::Input::MOVE_UP_FAST );
		inputManager.removeAction( id::Input::MOVE_DOWN_FAST );
		inputManager.removeAction( id::Input::MOVE_LEFT_FAST );
		inputManager.removeAction( id::Input::MOVE_RIGHT_FAST );

		inputManager.removeAction( id::Input::MOVE_UP_SLOW );
		inputManager.removeAction( id::Input::MOVE_DOWN_SLOW );
		inputManager.removeAction( id::Input::MOVE_LEFT_SLOW );
		inputManager.removeAction( id::Input::MOVE_RIGHT_SLOW );

		inputManager.removeAction( id::Input::ZOOM_IN );
		inputManager.removeAction( id::Input::ZOOM_OUT );
	}

	void SingleplayerGameState::removeGui( tgui::Gui& gui )
	{
		return;
	}

	void SingleplayerGameState::onDestroy()
	{
		return;
	}

	const Plugin::Name& SingleplayerGameState::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id SingleplayerGameState::getPluginId() const
	{
		return id::GameStatePluginId::SINGLEPLAYER;
	}

	void SingleplayerGameState::reloadSave()
	{
		r_defaultGameState->saveOpenSavegame();
		r_saveManager->openSavegame( *r_engine, *r_world, "MyFirstSavegameEver" );

		r_engine->console.info( "Savegame reloaded" );
	}

	void SingleplayerGameState::pause()
	{
		m_pauseLock->setActive( !m_pauseLock->isActive() );
	}

	void SingleplayerGameState::moveUp()
	{
		sf::Vector2i cameraMovement( 0, -10.0 / 16.0 * frameTime.asMilliseconds() );
		movePlayer( cameraMovement );
	}

	void SingleplayerGameState::moveDown()
	{
		sf::Vector2i cameraMovement( 0, 10.0 / 16.0 * frameTime.asMilliseconds() );
		movePlayer( cameraMovement );
	}

	void SingleplayerGameState::moveLeft()
	{
		sf::Vector2i cameraMovement( -10.0 / 16.0 * frameTime.asMilliseconds(), 0 );
		movePlayer( cameraMovement );
	}

	void SingleplayerGameState::moveRight()
	{
		sf::Vector2i cameraMovement( 10.0 / 16.0 * frameTime.asMilliseconds(), 0 );
		movePlayer( cameraMovement );
	}

	void SingleplayerGameState::moveUpFast()
	{
		sf::Vector2i cameraMovement( 0, -10.0 / 16.0 * frameTime.asMilliseconds() * 4 );
		movePlayer( cameraMovement );
	}

	void SingleplayerGameState::moveDownFast()
	{
		sf::Vector2i cameraMovement( 0, 10.0 / 16.0 * frameTime.asMilliseconds() * 4 );
		movePlayer( cameraMovement );
	}

	void SingleplayerGameState::moveLeftFast()
	{
		sf::Vector2i cameraMovement( -10.0 / 16.0 * frameTime.asMilliseconds() * 4, 0 );
		movePlayer( cameraMovement );
	}

	void SingleplayerGameState::moveRightFast()
	{
		sf::Vector2i cameraMovement( 10.0 / 16.0 * frameTime.asMilliseconds() * 4, 0 );
		movePlayer( cameraMovement );
	}

	void SingleplayerGameState::moveUpSlow()
	{
		sf::Vector2i cameraMovement( 0, -10.0 / 16.0 * frameTime.asMilliseconds() / 2 );
		movePlayer( cameraMovement );
	}

	void SingleplayerGameState::moveDownSlow()
	{
		sf::Vector2i cameraMovement( 0, 10.0 / 16.0 * frameTime.asMilliseconds() / 2 );
		movePlayer( cameraMovement );
	}

	void SingleplayerGameState::moveLeftSlow()
	{
		sf::Vector2i cameraMovement( -10.0 / 16.0 * frameTime.asMilliseconds() / 2, 0 );
		movePlayer( cameraMovement );
	}

	void SingleplayerGameState::moveRightSlow()
	{
		sf::Vector2i cameraMovement( 10.0 / 16.0 * frameTime.asMilliseconds() / 2, 0 );
		movePlayer( cameraMovement );
	}

	void SingleplayerGameState::zoomIn()
	{
		auto camera = m_getValidCamera();

		m_cameraZoomFactor -= 0.01*frameTime.asMilliseconds();

		if( m_cameraZoomFactor < 0 )
			m_cameraZoomFactor = 0;
		camera->getComponent<CameraComponent>()->setZoomFactor( m_cameraZoomFactor );
	}

	void SingleplayerGameState::zoomOut()
	{
		auto camera = m_getValidCamera();

		m_cameraZoomFactor += 0.01*frameTime.asMilliseconds();

		if( m_cameraZoomFactor < 0 )
			m_cameraZoomFactor = 0;
		camera->getComponent<CameraComponent>()->setZoomFactor( m_cameraZoomFactor );
	}

	const std::string SingleplayerGameState::PLUGIN_NAME = "GameState Singleplayer";
}