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

			void display( bool reset = true );
			void drawToDynamicBuffer( const sf::Sprite &sprite );
			void drawToDynamicBuffer( const sf::Texture *texture, const sf::Vector2i &position,
									  const sf::IntRect &rec, const sf::Color &color, const sf::Vector2i &scale,
									  const sf::Vector2i &origin, float rotation = 0 );

			void drawToDynamicBuffer( const sf::Texture *texture, const sf::FloatRect &dest, const sf::IntRect &rec, const sf::Color &color );

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

			GLuint m_dynamicVbo = 0;
			bool m_isDynamicVBOinit = false;
			bool m_isDynamicBufferBound = false;
			void initVBO();
			void destroyVBO();

			sf::Vertex* m_bufferPtr = nullptr;
		};
	}
}
