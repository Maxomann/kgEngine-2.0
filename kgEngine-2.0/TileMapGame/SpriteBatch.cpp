#include "SpriteBatch.h"
using namespace std;
using namespace sf;
using namespace priv;

namespace kg
{
	namespace batch
	{
		const float magic_number = 0.0075;
		const double Pi = 3.14159265;
		const size_t MaxCapacity = 4'000'000;//LIMITS THE NUMBER OF SPRITES DRAWN TO 4.000.000/4 = 1.000.000!
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

		SpriteBatch::SpriteBatch( void ) : count( 0 )
		{
			if( !initialized )
				create_lookup();
		}

		SpriteBatch::~SpriteBatch( void )
		{ }

		void SpriteBatch::openGlDrawDynamicBuffer( std::size_t vertexCount,
												   PrimitiveType type,
												   const RenderStates& states )
		{
			// Nothing to draw?
			if( vertexCount == 0 )
				return;

			// GL_QUADS is unavailable on OpenGL ES
#ifdef SFML_OPENGL_ES
			if( type == Quads )
			{
				err() << "sf::Quads primitive type is not supported on OpenGL ES platforms, drawing skipped" << std::endl;
				return;
			}
#define GL_QUADS 0
#endif

			if( rt->activate( true ) )
			{
				if( m_dynamicVbo.isMapped() )
					m_dynamicVbo.unmap();

				// First set the persistent OpenGL states if it's the very first call
				if( !rt->m_cache.glStatesSet )
					rt->resetGLStates();

				rt->applyTransform( states.transform );

				// Apply the view
				if( rt->m_cache.viewChanged )
					rt->applyCurrentView();

				// Apply the blend mode
				if( states.blendMode != rt->m_cache.lastBlendMode )
					rt->applyBlendMode( states.blendMode );

				// Apply the texture
				Uint64 textureId = states.texture ? states.texture->m_cacheId : 0;
				if( textureId != rt->m_cache.lastTextureId )
					rt->applyTexture( states.texture );

				// Apply the shader
				if( states.shader )
					rt->applyShader( states.shader );

				// Setup the pointers to the vertices components
				glVertexPointer( 2, GL_FLOAT, sizeof( Vertex ), ( const void* )0 );
				glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( Vertex ), ( const void* )8 );
				glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex ), ( const void* )12 );

				// Find the OpenGL primitive type
				static const GLenum modes[] = { GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
					GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS };
				GLenum mode = modes[type];

				// Draw the primitives
				glDrawArrays( mode, 0, vertexCount );

				// Unbind the shader, if any
				if( states.shader )
					rt->applyShader( NULL );

				m_dynamicVbo.unbind();
			}
		}

		void SpriteBatch::drawToDynamicBuffer( const Sprite &sprite )
		{
			drawToDynamicBuffer( sprite.getTexture(),
								 ( Vector2i )sprite.getPosition(),
								 sprite.getTextureRect(),
								 sprite.getColor(),
								 ( Vector2i )sprite.getScale(),
								 ( Vector2i )sprite.getOrigin(),
								 sprite.getRotation() );
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
			openGlDrawDynamicBuffer( count * 4, PrimitiveType::Quads, state );

			count = 0;
			if( reset )
				state = RenderStates();
		}

		int SpriteBatch::createDynamicBuffer( const Texture *texture )
		{
			if( texture != state.texture || (count + 1) * 4 >= MaxCapacity )
			{
				if( state.texture != nullptr )
					display( false );
				else
					count = 0;
				state.texture = texture;
			}

			return 4 * count++;
		}

		void SpriteBatch::drawToDynamicBuffer(
			const Texture *texture,
			const Vector2i &position,
			const IntRect &rec,
			const Color &color,
			const Vector2i &scale,
			const Vector2i &origin,
			float rotation )
		{
			if( !m_dynamicVbo.isGenerated() )
				m_dynamicVbo.generate( MaxCapacity, GL_STREAM_DRAW );

			auto index = createDynamicBuffer( texture );
			if( !m_dynamicVbo.isBound() )
				m_dynamicVbo.bind();
			if( !m_dynamicVbo.isMapped() )
				m_dynamicVbo.map();

			int rot = static_cast< int >(rotation / 360 * LookupSize + 0.5) & (LookupSize - 1);
			float& _sin = getSin[rot];
			float& _cos = getCos[rot];

			auto scalex = rec.width * scale.x;
			auto scaley = rec.height * scale.y;

			Vertex *ptr = m_dynamicVbo.ptr() + index;

			auto pX = -origin.x * scale.x;
			auto pY = -origin.y * scale.y;

			ptr->position.x = pX * _cos - pY * _sin + position.x;
			ptr->position.y = pX * _sin + pY * _cos + position.y;
			ptr->texCoords.x = rec.left + 0.0075;
			ptr->texCoords.y = rec.top + 0.0075;
			ptr->color = color;
			ptr++;

			pX += scalex;
			ptr->position.x = pX * _cos - pY * _sin + position.x;
			ptr->position.y = pX * _sin + pY * _cos + position.y;
			ptr->texCoords.x = rec.left + rec.width - 0.0075;
			ptr->texCoords.y = rec.top + 0.0075;
			ptr->color = color;
			ptr++;

			pY += scaley;
			ptr->position.x = pX * _cos - pY * _sin + position.x;
			ptr->position.y = pX * _sin + pY * _cos + position.y;
			ptr->texCoords.x = rec.left + rec.width - 0.0075;
			ptr->texCoords.y = rec.top + rec.height - 0.0075;
			ptr->color = color;
			ptr++;

			pX -= scalex;
			ptr->position.x = pX * _cos - pY * _sin + position.x;
			ptr->position.y = pX * _sin + pY * _cos + position.y;
			ptr->texCoords.x = rec.left + 0.0075;
			ptr->texCoords.y = rec.top + rec.height - 0.0075;
			ptr->color = color;
		}

		void SpriteBatch::drawToDynamicBuffer( const Texture *texture, const FloatRect &dest, const IntRect &rec, const Color &color )
		{
			drawToDynamicBuffer( texture,
								 Vector2i( dest.left, dest.top ),
								 rec,
								 color,
								 Vector2i( 1, 1 ),
								 Vector2i( 0, 0 ), 0 );
		}
	}
}