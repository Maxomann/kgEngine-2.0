#include "SpriteBatch.h"
using namespace std;
using namespace sf;
using namespace priv;

namespace kg
{
	namespace batch
	{
		const size_t MaxCapacity = 4'000'000;//LIMITS THE NUMBER OF SPRITES DRAWN TO 4.000.000/4 = 1.000.000!

		SpriteBatch::SpriteBatch( void )
		{ }

		SpriteBatch::~SpriteBatch( void )
		{ }

		void SpriteBatch::drawToDynamicBuffer( const std::vector<sf::Sprite*>& sprites )
		{
			if( !m_dynamicVbo.isGenerated() )
				m_dynamicVbo.generate( MaxCapacity, GL_STREAM_DRAW );

			m_dynamicVbo.addSprites( sprites );
		}

		void SpriteBatch::setRenderStates( const sf::RenderStates &states )
		{
			display( false );
			this->state = states;
		}

		void SpriteBatch::setRenderTarget( sf::RenderTarget &rt )
		{
			this->rt = &rt;
		}

		void SpriteBatch::display( bool reset )
		{
			m_dynamicVbo.draw( *rt, state );
			m_dynamicVbo.clear();

			if( reset )
				state = RenderStates();
		}
	}
}