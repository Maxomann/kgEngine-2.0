#pragma once
#include "stdafx.h"

namespace kg
{
	class VBO
	{
	private:
		bool m_isGenerated = false;
		bool m_isBound = false;
		bool m_isMapped = false;

		GLuint m_glId;
		unsigned int m_vertexCapaxity = 0;

		sf::Vertex* m_ptr;

	public:
		VBO() = default;
		VBO( unsigned int vertexCapacity, GLenum usage );
		~VBO();

		void generate( unsigned int vertexCapacity, GLenum usage );
		void destroy();

		void bind();
		void unbind();

		void map();
		void unmap();

		// retruns nullptr if !isMapped()
		sf::Vertex* ptr();

		bool isGenerated()const;
		bool isBound()const;
		bool isMapped()const;
		unsigned int getVertexCapacity()const;

		void draw( sf::RenderTarget &rt, const sf::RenderStates& states, std::size_t vertexCount );
	};

	class SpriteVBO : public VBO
	{
		static const sf::Uint8 VERTEX_COUNT_PER_SPRITE = 4;
		std::vector<sf::Sprite*> m_sprites;

		const sf::Texture* m_texture = nullptr;

		unsigned int calculatePtrOffsetToNewElement()const;

		void checkTexture( const sf::Sprite& sprite );

		bool chacheChanged = false;
		void chacheSprite( const sf::Sprite& sprite );// buffer has to be bound and mapped
		void chacheSprite( const sf::Texture *texture, const sf::Vector2i &position,
						   const sf::IntRect &rec, const sf::Color &color, const sf::Vector2i &scale,
						   const sf::Vector2i &origin, float rotation = 0 );// buffer has to be bound and mapped

	public:
		SpriteVBO() = default;
		SpriteVBO( unsigned int vertexCapacity, GLenum usage );
		~SpriteVBO() = default;

		void recreateChache();

		void addSprite( sf::Sprite* sprite );
		void removeSprite( sf::Sprite* sprite );
		void clear();

		const sf::Texture* getTexture()const;

		void draw( sf::RenderTarget &rt, sf::RenderStates& states );
	};
}