#include <iostream>
#include <functional>
#include <sstream>

#include <boost/signals2.hpp>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;
using namespace placeholders;


int main()
{
	RenderWindow window( VideoMode( 600, 600 ), "" );

	vector<RectangleShape> shapes;
	for( int i = 0; i < 5000; ++i )
	{
		RectangleShape shape( Vector2f( 600, 600 ) );
		shape.setFillColor( Color::Red );

		shapes.push_back( shape );
	}

	sf::Clock clock;

	sf::RenderTexture tex;
	tex.create( 600, 600 );

	Sprite texSprite;

	while( window.isOpen() )
	{
		window.setTitle(to_string(clock.restart().asMilliseconds()));

		Event sfEvent;
		while( window.pollEvent( sfEvent ) )
		{
			if( sfEvent.type == Event::Closed )
				window.close();
		}



		window.clear( Color::Green );

		tex.clear( Color::Blue );

		//draw
		for( const auto& el : shapes )
			tex.draw( el );

		tex.display();
		texSprite.setTexture( tex.getTexture() );


		window.draw( texSprite );

		window.display();
	}


	system( "pause" );
}
