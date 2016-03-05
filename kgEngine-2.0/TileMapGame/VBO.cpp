#include "VBO.h"
using namespace std;
using namespace sf;

namespace kg
{
	void VBO::generate( unsigned int vertexCapacity, GLenum usage )
	{
		m_isGenerated = true;

		m_vertexCapaxity = vertexCapacity;
		glGenBuffers( 1, &m_glId );
		bind();
		glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex )*vertexCapacity, NULL, usage );
		unbind();
	}

	void VBO::destroy()
	{
		if( m_isGenerated )
		{
			m_isGenerated = false;

			if( isMapped() )
				unmap();
			if( isBound() )
				unbind();
			glDeleteBuffers( 1, &m_glId );
		}
	}

	VBO::VBO( unsigned int vertexCapacity, GLenum usage )
	{
		generate( vertexCapacity, usage );
	}

	VBO::~VBO()
	{
		destroy();
	}

	void VBO::bind()
	{
		m_isBound = true;
		glBindBuffer( GL_ARRAY_BUFFER, m_glId );
	}

	void VBO::unbind()
	{
		m_isBound = false;
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	void VBO::map()
	{
		GLint result;
		glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &result );
		cout << result << endl;

		m_isMapped = true;
		m_ptr = ( Vertex* )glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
		if( m_ptr == nullptr )
		{
			GLenum err;
			while( (err = glGetError()) != GL_NO_ERROR )
			{
				cout << err << endl;
			}
			throw exception();
		}
	}

	void VBO::unmap()
	{
		m_isMapped = false;
		glUnmapBuffer( GL_ARRAY_BUFFER );
		m_ptr = nullptr;
	}

	Vertex* VBO::ptr()
	{
		return m_ptr;
	}

	bool VBO::isGenerated() const
	{
		return m_isGenerated;
	}

	bool VBO::isBound() const
	{
		return m_isBound;
	}

	bool VBO::isMapped() const
	{
		return m_isMapped;
	}

	unsigned int VBO::getVertexCapacity() const
	{
		return m_vertexCapaxity;
	}

	void VBO::draw( sf::RenderTarget &rt, const sf::RenderStates& states, std::size_t vertexCount )
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

		if( rt.activate( true ) )
		{
			if( !isBound() )
				bind();
			if( isMapped() )
				unmap();

			// First set the persistent OpenGL states if it's the very first call
			if( !rt.m_cache.glStatesSet )
				rt.resetGLStates();

			rt.applyTransform( states.transform );

			// Apply the view
			if( rt.m_cache.viewChanged )
				rt.applyCurrentView();

			// Apply the blend mode
			if( states.blendMode != rt.m_cache.lastBlendMode )
				rt.applyBlendMode( states.blendMode );

			// Apply the texture
			Uint64 textureId = states.texture ? states.texture->m_cacheId : 0;
			if( textureId != rt.m_cache.lastTextureId )
				rt.applyTexture( states.texture );

			// Apply the shader
			if( states.shader )
				rt.applyShader( states.shader );

			// Setup the pointers to the vertices components
			glVertexPointer( 2, GL_FLOAT, sizeof( Vertex ), ( const void* )0 );
			glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( Vertex ), ( const void* )8 );
			glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex ), ( const void* )12 );

			// Draw the primitives
			glDrawArrays( GL_QUADS, 0, vertexCount );

			// Unbind the shader, if any
			if( states.shader )
				rt.applyShader( NULL );

			unbind();
		}
	}

	unsigned int SpriteVBO::calculatePtrOffsetToNthElement( const unsigned int n ) const
	{
		if( n * VERTEX_COUNT_PER_SPRITE >= getVertexCapacity() )
			throw exception();

		return n * VERTEX_COUNT_PER_SPRITE;
	}

	void SpriteVBO::checkTexture( const sf::Sprite& sprite )
	{
		if( m_texture == nullptr || m_sprites.size() == 0 )
			m_texture = sprite.getTexture();
		if( m_texture != sprite.getTexture() )
			throw exception();
	}

	void SpriteVBO::recreateChache()
	{
		bind();
		map();

		int n = 0;
		for( auto& el : m_sprites )
		{
			chacheSprite( *el, n );
			n++;
		}

		unmap();
		unbind();

		chacheChanged = false;
	}

	void SpriteVBO::chacheSprite( const sf::Sprite& sprite,
								  const unsigned int n )
	{
		chacheSprite( sprite.getTexture(),
					  ( Vector2i )sprite.getPosition(),
					  sprite.getTextureRect(),
					  sprite.getColor(),
					  ( Vector2i )sprite.getScale(),
					  ( Vector2i )sprite.getOrigin(),
					  sprite.getRotation(),
					  n );
	}

	void SpriteVBO::chacheSprite( const sf::Texture *texture,
								  const sf::Vector2i &position,
								  const sf::IntRect &rec,
								  const sf::Color &color,
								  const sf::Vector2i &scale,
								  const sf::Vector2i &origin,
								  float rotation,
								  const unsigned int n )
	{
		auto offset = calculatePtrOffsetToNthElement( n );

		float _sin = sin( rotation * boost::math::constants::pi<float>() / 180 );
		float _cos = cos( rotation * boost::math::constants::pi<float>() / 180 );

		auto scalex = rec.width * scale.x;
		auto scaley = rec.height * scale.y;

		Vertex *ptr = VBO::ptr() + offset;

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

	SpriteVBO::SpriteVBO( unsigned int vertexCapacity, GLenum usage )
		: VBO( vertexCapacity, usage )
	{ }

	const sf::Texture* SpriteVBO::getTexture() const
	{
		return m_texture;
	}

	unsigned int SpriteVBO::getSpriteCount() const
	{
		return m_sprites.size();
	}

	void SpriteVBO::draw( sf::RenderTarget &rt, sf::RenderStates& states )
	{
		if( chacheChanged )
			recreateChache();

		states.texture = m_texture;
		VBO::draw( rt, states, m_sprites.size()*VERTEX_COUNT_PER_SPRITE );
	}

	void SpriteVBO::addSprite( sf::Sprite* sprite )
	{
		chacheChanged = true;

		checkTexture( *sprite );
		m_sprites.push_back( sprite );
	}

	void SpriteVBO::removeSprite( sf::Sprite* sprite )
	{
		chacheChanged = true;

		auto condition = [&]( const sf::Sprite* el )
		{
			return el == sprite;
		};

		m_sprites.erase( remove_if( m_sprites.begin(), m_sprites.end(), condition ), m_sprites.end() );
	}

	void SpriteVBO::clear()
	{
		chacheChanged = true;

		m_sprites.clear();
		m_texture = nullptr;
	}
}