#include "DrawingLayer.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	void StandartDrawingLayer::addEntity( Entity* entity )
	{
		auto graphicsComponent = entity->getComponent<Graphics>();
		auto sprite = graphicsComponent->getSprite();

		if( graphicsComponent->isStatic() )
		{
			auto chunkPosition = *entity->getComponent<Transformation>()->getChunkPosition();

			auto vboChunk = m_staticVBOs[chunkPosition];
			if( vboChunk == m_staticVBOs.end() )
				vboChunk = m_staticVBOs.in

				auto& vbo = vboChunk[sprite->getTexture()];
			if( !vbo.isGenerated() )
				vbo.generate( STANDART_VERTEX_CAPACITY, GL_WRITE_ONLY );
			vbo.addSprite( sprite );
		}
		else
		{
			m_dynamicVBO.addSprite( sprite );
		}
	}
}