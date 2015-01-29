#pragma once
#include "stdafx.h"

namespace kg
{
	namespace batch
	{
		struct QueueItem
		{
			unsigned int count;
			const sf::Texture* texture;
		};

		class SpriteBatch : public sf::Drawable
		{
		public:
			SpriteBatch( void );
			~SpriteBatch( void );

			void begin();
			void end();

			void draw( const sf::Sprite& sprite );
			void draw( const sf::Texture* texture, sf::Vector2f position, sf::IntRect rec, sf::Color color, sf::Vector2f scale,
					   sf::Vector2f origin, float rotation = 0 );
			void draw( const sf::Texture* texture, sf::FloatRect rec, sf::IntRect src, sf::Color color );

			virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;


		private:
			std::vector<QueueItem> textures;
			std::vector<sf::Vertex> vertices;
			int count;
			bool active;
			unsigned int queueCount;
			const sf::Texture* activeTexture;

			int create( const sf::Texture* texture );
			void enqueue();
		};
	}
}