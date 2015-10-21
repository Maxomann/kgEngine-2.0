#pragma once
#include "stdafx.h"

namespace kg
{
	class ConsoleGameState : public GameState
	{
		tgui::ChildWindow::Ptr m_consoleWindow;
		tgui::ChatBox::Ptr m_chatBox;
		tgui::Button::Ptr m_clearButton;

		void printLogEntry( const LogEntry& entry );

	public:

		virtual void onInit() override;

		virtual void registerGui( tgui::Gui& gui ) override;

		virtual void registerInputCallbacks( InputManager& inputManager ) override;

		virtual void onUpdate( ) override;

		virtual void removeInputCallbacks( InputManager& inputManager ) override;

		virtual void removeGui( tgui::Gui& gui ) override;

		virtual void onDestroy() override;

		virtual const Plugin::Name& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;


		static const std::string PLUGIN_NAME;


	private:
		//Callbacks
		void close();
	};
}
