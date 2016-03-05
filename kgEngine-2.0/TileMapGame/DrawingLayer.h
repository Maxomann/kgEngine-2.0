#pragma once
#include "stdafx.h"
#include "VBO.h"
#include "ChunkVBO.h"
#include "Graphics.h"

namespace kg
{
	//SUPPORT MULTIPLE TEXTURES

	class DrawingLayer
	{
	public:
		static const int STANDART_VERTEX_CAPACITY = 400'000;
		static const int EXTENDED_VERTEX_CAPACITY = 1'000'000;
		static const int STATIC_VBO_USAGE = GL_STATIC_DRAW;
		static const int DYNAMIC_VBO_USAGE = GL_DYNAMIC_DRAW;

		virtual void addEntity( Entity* entity ) = 0;
		virtual void removeEntity( Entity* entity ) = 0;

		virtual void draw( sf::RenderTarget& target, sf::RenderStates& states, PositionXY cameraPosition, int drawDistance ) = 0;
	};

	// Sprites added in this Container MUST NOT MOVE!
	class StandartDrawingLayer : public DrawingLayer
	{
		std::vector<ChunkVBO> m_staticVBOs;

		std::vector<sf::Sprite*> m_dynamicSprites;
		SpriteVBO m_dynamicVBO;

	public:
		StandartDrawingLayer();

		virtual void addEntity( Entity* entity ) override;
		virtual void removeEntity( Entity* entity ) override;

		virtual void draw( sf::RenderTarget& target, sf::RenderStates& states, PositionXY cameraPosition, int drawDistance ) override;
	};

	class YSortedDrawingLayer : public DrawingLayer
	{
		std::vector<sf::Sprite*> m_dynamicSprites;
		SpriteVBO m_dynamicVBO;

	public:
		YSortedDrawingLayer();

		virtual void addEntity( Entity* entity ) override;
		virtual void removeEntity( Entity* entity ) override;

		virtual void draw( sf::RenderTarget& target, sf::RenderStates& states, PositionXY cameraPosition, int drawDistance ) override;
	};
}