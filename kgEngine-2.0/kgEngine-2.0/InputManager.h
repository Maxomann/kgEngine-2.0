#pragma once
#include "stdafx.h"

namespace kg
{
	class DLL_EXPORT InputManager
	{

		thor::ActionMap<int> m_actionMap;
		thor::ActionMap<int>::CallbackSystem m_callbackSystem;

	public:
		// Has to be initialized manually in GraphicsSystem.
		// Event forwarding is done by InputManager. DON'T forward them twice!
		// Also gui.draw() has to be called manually. This is to enable multithreading.
		tgui::Gui gui;


		void setAction( const int& actionId, const thor::Action& action );
		template<class T>
		void setAction( const int& actionId, const thor::Action& action, const T& callback );

		void removeAction( const int& actionId );

		template<class T>
		void connect( const int& actionId, const T& callback );

		void clearEvents();
		void forwardSfmlEvent( const sf::Event& sfEvent );
		void triggerCallbacks( sf::Window& window );
	};

	template<class T>
	void kg::InputManager::setAction( const int& actionId, const thor::Action& action, const T& callback )
	{
		setAction( actionId, action );
		connect( actionId, callback );
	}

	template<class T>
	void kg::InputManager::connect( const int& actionId, const T& callback )
	{
		m_callbackSystem.connect( actionId, callback );
	}

}
