#include "SpriteBatch.h"
using namespace std;
using namespace sf;

namespace kg
{
	namespace batch
	{
		static const double M_PI = 3.14159265358979323846;

		SpriteBatch::SpriteBatch( void ) : count( 0 ), active( false ), queueCount( 0 ), activeTexture( 0 )
		{ }

		SpriteBatch::~SpriteBatch( void )
		{ }

		void SpriteBatch::begin()
		{
			if( active ) throw std::exception( "SpriteBatch is already active." );
			vertices.clear();
			count = 0;
			textures.clear();
			active = true;
		}

		void SpriteBatch::end()
		{
			if( !active ) throw std::exception( "SpriteBatch is not active." );
			enqueue();
			active = false;
		}

		void SpriteBatch::draw( sf::RenderTarget& target, sf::RenderStates states ) const
		{
			int index = 0;
			for( auto item : textures )
			{
				states.texture = item.texture;
				target.draw( &vertices[index], item.count, sf::PrimitiveType::Quads, states );
				index += item.count;
			}
		}

		void SpriteBatch::draw( const sf::Sprite& sprite )
		{
			draw(
				sprite.getTexture(), sprite.getPosition(), sprite.getTextureRect(),
				sprite.getColor(), sprite.getScale(), sprite.getOrigin(), sprite.getRotation() );
		}

		void SpriteBatch::draw( const sf::Texture* texture, sf::Vector2f position, sf::IntRect rec,
								sf::Color color, sf::Vector2f scale, sf::Vector2f origin, float rotation /*= 0*/ )
		{
			int index = create( texture );
			float _sin = 0, _cos = 1;

			if( rotation != 0 )
			{
				rotation = rotation / 180 * ( float )M_PI;
				_sin = sin( rotation );
				_cos = cos( rotation );
			}

			float pX = -origin.x * scale.x;
			float pY = -origin.y * scale.y;

			scale.x *= rec.width;
			scale.y *= rec.height;

			sf::Vertex* ptr = &vertices[index];

			ptr->position.x = pX * _cos - pY * _sin + position.x;
			ptr->position.y = pX * _sin + pY * _cos + position.y;
			ptr->texCoords.x = ( float )rec.left;
			ptr->texCoords.y = ( float )rec.top;
			ptr->color = color;
			ptr++;

			pX += scale.x;
			ptr->position.x = pX * _cos - pY * _sin + position.x;
			ptr->position.y = pX * _sin + pY * _cos + position.y;
			ptr->texCoords.x = ( float )(rec.left + rec.width);
			ptr->texCoords.y = ( float )rec.top;
			ptr->color = color;
			ptr++;

			pY += scale.y;
			ptr->position.x = pX * _cos - pY * _sin + position.x;
			ptr->position.y = pX * _sin + pY * _cos + position.y;
			ptr->texCoords.x = ( float )(rec.left + rec.width);
			ptr->texCoords.y = ( float )(rec.top + rec.height);
			ptr->color = color;
			ptr++;

			pX -= scale.x;
			ptr->position.x = pX * _cos - pY * _sin + position.x;
			ptr->position.y = pX * _sin + pY * _cos + position.y;
			ptr->texCoords.x = ( float )rec.left;
			ptr->texCoords.y = ( float )(rec.top + rec.height);
			ptr->color = color;
		}

		void SpriteBatch::draw( const sf::Texture* texture, sf::FloatRect rec, sf::IntRect src, sf::Color color )
		{
			int index = create( texture );
			sf::Vertex* ptr = &vertices[index];

			ptr->position.x = rec.left;
			ptr->position.y = rec.top;
			ptr->texCoords.x = ( float )src.left;
			ptr->texCoords.y = ( float )src.top;
			ptr->color = color;
			ptr++;

			ptr->position.x = rec.left + rec.width;
			ptr->position.y = rec.top;
			ptr->texCoords.x = ( float )(src.left + src.width);
			ptr->texCoords.y = ( float )(src.top);
			ptr->color = color;
			ptr++;

			ptr->position.x = rec.left + rec.width;
			ptr->position.y = rec.top + rec.height;
			ptr->texCoords.x = ( float )(src.left + src.width);
			ptr->texCoords.y = ( float )(src.top + src.height);
			ptr->color = color;
			ptr++;

			ptr->position.x = rec.left;
			ptr->position.y = rec.top + rec.height;
			ptr->texCoords.x = ( float )src.left;
			ptr->texCoords.y = ( float )(src.top + src.height);
			ptr->color = color;
		}

		int SpriteBatch::create( const sf::Texture* texture )
		{
			if( !active ) throw std::exception( "SpriteBatch is not active." );
			if( texture != activeTexture )
			{
				enqueue();
				activeTexture = texture;
			}

			vertices.resize( vertices.size() + 4 );
			queueCount += 4;

			return 4 * count++;
		}

		void SpriteBatch::enqueue()
		{
			if( queueCount > 0 )
			{
				QueueItem qi;
				qi.texture = activeTexture;
				activeTexture = NULL;
				qi.count = queueCount;
				textures.push_back( qi );
				queueCount = 0;
			}
		}
	}
}