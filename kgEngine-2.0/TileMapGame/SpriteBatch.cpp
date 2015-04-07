#include "SpriteBatch.h"
using namespace std;
using namespace sf;

namespace kg
{
	namespace batch
	{
		const float Pi = 3.14159265;
		const unsigned int MaxCapacity = std::numeric_limits<unsigned int>::max();
		const int LookupSize = 512;

		float getSin[LookupSize];
		float getCos[LookupSize];
		bool initialized = false;

		void create_lookup()
		{
			for( int i = 0; i < LookupSize; i++ )
			{
				getSin[i] = sin( i * Pi / LookupSize * 2 );
				getCos[i] = cos( i * Pi / LookupSize * 2 );
			}
			initialized = true;
		}

		SpriteBatch::SpriteBatch( void ) : count( 0 ), capacity( 40 )
		{
			vertices.resize( capacity );

			if( !initialized )
				create_lookup();
		}

		SpriteBatch::~SpriteBatch( void )
		{ }

		void SpriteBatch::draw( const Sprite &sprite )
		{
			draw( sprite.getTexture(), Vector2i( sprite.getPosition() ), sprite.getTextureRect(), sprite.getColor(), Vector2i( sprite.getScale() ), Vector2i(sprite.getOrigin()), sprite.getRotation() );
		}

		void SpriteBatch::flush()
		{
			count = 0;
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

		void SpriteBatch::display( bool reset, bool flush )
		{
			rt->draw( &vertices[0], count * 4, PrimitiveType::Quads, state );
			if( flush ) count = 0;
			if( reset ) state = RenderStates();
		}

		int SpriteBatch::create( const Texture *texture )
		{
			if( texture != state.texture )
			{
				display( false );
				state.texture = texture;
			}

			if( count * 4 >= capacity )
			{
				//display(false);
				if( capacity < MaxCapacity )
				{
					capacity *= 2;
					if( capacity > MaxCapacity ) capacity = MaxCapacity;
					vertices.resize( capacity );
				}
			}
			return 4 * count++;
		}

		void SpriteBatch::draw(
			const Texture *texture, const Vector2i &position,
			const IntRect &rec, const Color &color, const Vector2i &scale,
			const Vector2i &origin, float rotation )
		{
			auto index = create( texture );

			int rot = static_cast< int >(rotation / 360 * LookupSize + 0.5) & (LookupSize - 1);
			float _sin = getSin[rot];
			float _cos = getCos[rot];

			//float _sin = sinf(rotation);
			//float _cos = cosf(rotation);

			auto scalex = rec.width * scale.x;
			auto scaley = rec.height * scale.y;

			Vertex *ptr = &vertices[index];

			auto pX = -origin.x * scale.x;
			auto pY = -origin.y * scale.y;

			ptr->position.x = pX * _cos - pY * _sin + position.x;
			ptr->position.y = pX * _sin + pY * _cos + position.y;
			ptr->texCoords.x = rec.left;
			ptr->texCoords.y = rec.top;
			ptr->color = color;
			ptr++;

			pX += scalex;
			ptr->position.x = pX * _cos - pY * _sin + position.x;
			ptr->position.y = pX * _sin + pY * _cos + position.y;
			ptr->texCoords.x = rec.left + rec.width;
			ptr->texCoords.y = rec.top;
			ptr->color = color;
			ptr++;

			pY += scaley;
			ptr->position.x = pX * _cos - pY * _sin + position.x;
			ptr->position.y = pX * _sin + pY * _cos + position.y;
			ptr->texCoords.x = rec.left + rec.width;
			ptr->texCoords.y = rec.top + rec.height;
			ptr->color = color;
			ptr++;

			pX -= scalex;
			ptr->position.x = pX * _cos - pY * _sin + position.x;
			ptr->position.y = pX * _sin + pY * _cos + position.y;
			ptr->texCoords.x = rec.left;
			ptr->texCoords.y = rec.top + rec.height;
			ptr->color = color;
		}

		void SpriteBatch::draw( const Texture *texture, const FloatRect &dest, const IntRect &rec, const Color &color )
		{
			draw( texture, Vector2i( dest.left, dest.top ), rec, color, Vector2i( 1, 1 ), Vector2i( 0, 0 ), 0 );
		}

	}
}