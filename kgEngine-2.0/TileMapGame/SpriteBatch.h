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
			void draw( const sf::Texture *texture, const sf::Vector2i &position,
					   const sf::IntRect &rec, const sf::Color &color, const sf::Vector2i &scale,
					   const sf::Vector2i &origin, float rotation = 0 );

			void draw( const sf::Texture *texture, const sf::FloatRect &dest, const sf::IntRect &rec, const sf::Color &color );

			void flush();
			void setRenderStates( const sf::RenderStates &states );
			void setRenderTarget( sf::RenderTarget &rt );

		private:
			sf::RenderTarget *rt;
			sf::RenderStates state;
			std::vector<sf::Vertex> vertices;
			int count;

			int create( const sf::Texture *texture );

		};
	}
}
