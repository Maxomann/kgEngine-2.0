#include "stdafx.h"

using namespace std;
using namespace sf;
using namespace tgui;
using namespace placeholders;

int main()
{
	// Create the window
	RenderWindow window{ { 1080, 720 }, "Window" };
	Gui gui{ window };

	// Load the font
	gui.setFont( "./DejaVuSans.ttf" );

	auto childWindow = make_shared<ChildWindow>();
	childWindow->setSize( 800, 400 );

	auto chatBox = make_shared<ChatBox>();
	chatBox->addLine( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );

	childWindow->add( chatBox );
	gui.add( childWindow );

	while( window.isOpen() )
	{
		Event event;
		while( window.pollEvent( event ) )
		{
			if( event.type == Event::Closed )
				window.close();

			// Pass the event to all the widgets
			gui.handleEvent( event );
		}

		window.clear();

		// Draw all created widgets
		gui.draw();

		window.display();

		//output GL_ERROR
		while( true )
		{
			GLenum err = glGetError();
			if( err == GL_NO_ERROR )
				break;
			cout << err << endl;
		}
	}
}
