#include "InputManager.h"
using namespace std;
using namespace sf;
using namespace thor;

namespace kg
{
	void InputManager::setAction( const int& actionId, const thor::Action& action )
	{
		m_actionMap[actionId] = action;
	}

	void InputManager::removeAction( const int& actionId )
	{
		m_actionMap.removeAction( actionId );
	}

	void InputManager::clearEvents()
	{
		m_actionMap.clearEvents();
	}

	void InputManager::forwardSfmlEvent( const sf::Event& sfEvent )
	{
		gui.handleEvent( sfEvent );//If null exception here: have you passed the gui a reference to the RenderWindow?
		m_actionMap.pushEvent( sfEvent );
	}

	void InputManager::triggerCallbacks( sf::Window& window )
	{
		m_actionMap.invokeCallbacks( m_callbackSystem, &window );
	}
}
