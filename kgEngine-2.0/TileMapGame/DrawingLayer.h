#pragma once
#include "stdafx.h"
#include "VBO.h"
#include "Graphics.h"

namespace kg
{
	//SUPPORT MULTIPLE TEXTURES

	class DrawingLayer
	{
	public:
		static const int STANDART_VERTEX_CAPACITY = 400'000;

		virtual void addEntity( Entity* entity ) = 0;
		virtual void removeEntity( Entity* entity ) = 0;

		virtual void draw( sf::RenderTarget& target, sf::RenderStates& states, PositionXY cameraPosition, int drawDistance )const = 0;
	};

	class StandartDrawingLayer : public DrawingLayer
	{
		std::vector<std::pair<PositionXY, std::vector<SpriteVBO>>> m_staticVBOs;
		SpriteVBO m_dynamicVBO;

	public:
		virtual void addEntity( Entity* entity ) override;
		virtual void removeEntity( Entity* entity ) override;

		virtual void draw( sf::RenderTarget& target, sf::RenderStates& states, PositionXY cameraPosition, int drawDistance ) const override;
	};

	class DynamicDrawingLayer : public DrawingLayer
	{
		SpriteVBO m_dynamicVBO;

	public:
		virtual void addEntity( Entity* entity ) override;
		virtual void removeEntity( Entity* entity ) override;

		virtual void draw( sf::RenderTarget& target, sf::RenderStates& states, PositionXY cameraPosition, int drawDistance ) const override;
	};
}