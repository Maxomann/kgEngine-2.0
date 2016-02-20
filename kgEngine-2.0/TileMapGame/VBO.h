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

		void draw( const RenderStates& states, std::size_t vertexCount );
	};

	class SpriteVBO : public VBO
	{
		static const sf::Uint8 m_vertexCountPerSprite = 4;
		std::vector<sf::Sprite*> m_sprites;

		sf::Texture* texture = nullptr;

		void push_back_sprite( const sf::Sprite* sprite );// buffer has to be bound and mapped

		void recreateChache();
		void chacheSprite( const sf::Sprite& sprite );
	public:
		SpriteVBO();
		SpriteVBO( unsigned int vertexCapacity, GLenum usage );
		~SpriteVBO() = default;

		void addSprites( const std::vector<sf::Sprite*> sprites );
		void removeSprites( const std::vector<sf::Sprite*> sprites );
		void clear();

		void draw( const RenderStates& states );
	};
}