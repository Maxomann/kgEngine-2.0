#include "DrawingLayer.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	StandartDrawingLayer::StandartDrawingLayer()
	{
		m_dynamicVBO.generate( MEDIUM_VERTEX_CAPACITY, DYNAMIC_VBO_USAGE );
	}

	void StandartDrawingLayer::addEntity( Entity* entity )
	{
		auto graphicsComponent = entity->getComponent<GraphicsComponent>();
		auto sprite = graphicsComponent->getSprite();

		if( graphicsComponent->isStatic() )
		{
			auto chunkPosition = *entity->getComponent<TransformationComponent>()->getLastChunkPosition();

			// find vbo for chunk
			auto it = find_if( m_staticVBOs.begin(), m_staticVBOs.end(), [&]( const ChunkVBO& el )
			{
				if( el.getChunkPosition() == chunkPosition.toPositionXY() && el.getTexture() == sprite->getTexture() )
					return true;
				else
					return false;
			} );

			//found
			if( it != m_staticVBOs.end() )
			{
				it->addSprite( sprite );
			}
			//not found
			else
			{
				m_staticVBOs.emplace_back();
				auto& vbo = m_staticVBOs.back();

				vbo.setChunkPosition( chunkPosition.toPositionXY() );
				vbo.generate( STANDART_VERTEX_CAPACITY, STATIC_VBO_USAGE );
				vbo.addSprite( sprite );
			}
		}
		else
		{
			m_dynamicSprites.push_back( sprite );
		}
	}
	void StandartDrawingLayer::removeEntity( Entity* entity )
	{
		auto graphicsComponent = entity->getComponent<GraphicsComponent>();
		auto sprite = graphicsComponent->getSprite();

		if( graphicsComponent->isStatic() )
		{
			auto chunkPosition = *entity->getComponent<TransformationComponent>()->getLastChunkPosition();

			// find vbo for chunk
			auto it = find_if( m_staticVBOs.begin(), m_staticVBOs.end(), [&]( const ChunkVBO& el )
			{
				if( el.getChunkPosition() == chunkPosition.toPositionXY() && el.getTexture() == sprite->getTexture() )
					return true;
				else
					return false;
			} );

			//found
			if( it != m_staticVBOs.end() )
			{
				it->removeSprite( sprite );
				if( it->getSpriteCount() == 0 )
					m_staticVBOs.erase( it );
			}
			//not found
			else
			{
				throw exception();
			}
		}
		else
		{
			auto condition = [&]( const sf::Sprite* el )
			{
				return el == sprite;
			};

			m_dynamicSprites.erase( remove_if( m_dynamicSprites.begin(), m_dynamicSprites.end(), condition ), m_dynamicSprites.end() );
		}
	}
	void StandartDrawingLayer::draw( sf::RenderTarget & target,
									 sf::RenderStates & states,
									 PositionXY cameraPosition,
									 int drawDistance )
	{
		for( auto& el : m_staticVBOs )
			el.draw( target, states );

		for( auto& sprite : m_dynamicSprites )
		{
			auto spritePosition = sprite->getPosition();

			auto distanceVec = sf::Vector2i( spritePosition.x - cameraPosition.x, spritePosition.y - cameraPosition.y );
			if( length( distanceVec ) <= drawDistance )
				m_dynamicVBO.addSprite( sprite );
		}
		m_dynamicVBO.draw( target, states );
		m_dynamicVBO.clear();
	}

	YSortedDrawingLayer::YSortedDrawingLayer()
	{
		m_dynamicVBO.generate( EXTENDED_VERTEX_CAPACITY, DYNAMIC_VBO_USAGE );
	}

	void YSortedDrawingLayer::addEntity( Entity * entity )
	{
		m_dynamicSprites.push_back( entity->getComponent<GraphicsComponent>()->getSprite() );
	}
	void YSortedDrawingLayer::removeEntity( Entity* entity )
	{
#ifdef _DEBUG
		unsigned int size_before = m_dynamicSprites.size();
#endif

		Sprite* sprite = entity->getComponent<GraphicsComponent>()->getSprite();

		auto condition = [&]( const sf::Sprite* el )
		{
			return el == sprite;
		};

		m_dynamicSprites.erase( remove_if( m_dynamicSprites.begin(), m_dynamicSprites.end(), condition ), m_dynamicSprites.end() );

#ifdef _DEBUG
		if( size_before <= m_dynamicSprites.size() )
			throw exception();
#endif
	}
	void YSortedDrawingLayer::draw( sf::RenderTarget & target, sf::RenderStates & states, PositionXY cameraPosition, int drawDistance )
	{
		for( auto& sprite : m_dynamicSprites )
		{
			auto spritePosition = sprite->getPosition();

			auto distanceVec = sf::Vector2i( spritePosition.x - cameraPosition.x, spritePosition.y - cameraPosition.y );
			if( length( distanceVec ) <= drawDistance )
				m_dynamicVBO.addSprite( sprite );
		}

		//Y Value SORTING:
		m_dynamicVBO.sortSprites( [](
			const Sprite* lhs,
			const Sprite* rhs )
		{
			const auto& zValueLeft = lhs->getPosition().y + (lhs->getGlobalBounds().height / 2.f);
			const auto& zValueRight = rhs->getPosition().y + (rhs->getGlobalBounds().height / 2.f);

			if( zValueRight > zValueLeft )
				return true;

			return false;
		} );

		m_dynamicVBO.draw( target, states );
		m_dynamicVBO.clear();
	}
}