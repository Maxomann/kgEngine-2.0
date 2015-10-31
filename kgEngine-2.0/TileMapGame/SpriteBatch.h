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
			sf::RenderTarget *rt = nullptr;
			sf::RenderStates state;
			int count;

			int create( const sf::Texture *texture );

			void openGlDraw( std::size_t vertexCount,
							 sf::PrimitiveType type,
							 const sf::RenderStates& states );

			GLuint m_vbo = 0;
			bool m_isVBOinit = false;
			bool m_isBufferBound = false;
			void initVBO();
			void destroyVBO();

			sf::Vertex* m_bufferPtr = nullptr;
		};
	}
}
