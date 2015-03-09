#pragma once
#include "stdafx.h"

namespace kg
{
	namespace batch
	{
		class SpriteBatch
		{
		public:
			SpriteBatch( void );
			~SpriteBatch( void );


			void display( bool reset = true, bool flush = true );
			void draw( const sf::Sprite &sprite );
			void draw( const sf::Texture *texture, const sf::Vector2f &position,
					   const sf::IntRect &rec, const sf::Color &color, const sf::Vector2f &scale,
					   const sf::Vector2f &origin, float rotation = 0 );

			void draw( const sf::Texture *texture, const sf::FloatRect &dest, const sf::IntRect &rec, const sf::Color &color );

			void flush();
			void setRenderStates( const sf::RenderStates &states );
			void setRenderTarget( sf::RenderTarget &rt );

		private:
			sf::RenderTarget *rt;
			sf::RenderStates state;
			std::vector<sf::Vertex> vertices;
			int count;
			int capacity;

			int create( const sf::Texture *texture );

		};
	}
}
