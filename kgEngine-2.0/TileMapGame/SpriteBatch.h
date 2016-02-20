#pragma once
#include "stdafx.h"
#include "VBO.h"

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
			void drawToDynamicBuffer( const std::vector<sf::Sprite*>& sprites );

			void setRenderStates( const sf::RenderStates &states );
			void setRenderTarget( sf::RenderTarget &rt );

		private:
			sf::RenderTarget *rt = nullptr;
			sf::RenderStates state;

			SpriteVBO m_dynamicVbo;
		};
	}
}
