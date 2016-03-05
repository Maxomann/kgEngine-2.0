#pragma once
#include "stdafx.h"
#include "Position.h"
#include "Graphics.h"
#include "DrawingLayer.h"

namespace kg
{
	class DrawingLayerContainer
	{
		std::map<PositionType, std::map<PositionType, std::unique_ptr<DrawingLayer>>> m_drawingLayersByZValueByWorldLayer;

		static const int YSORTED_DRAWING_LAYER_ZVALUE = 0;
		void initDrawingLayerPointer( std::unique_ptr<DrawingLayer>& pointer, int zValue );

	public:

		void addEntities( const EntityManager::EntityPointerContainer& entities );
		void removeEntities( const EntityManager::EntityPointerContainer& entities );

		void addEntities_try( const EntityManager::EntityPointerContainer& entities );
		void removeEntities_try( const EntityManager::EntityPointerContainer& entities );

		virtual void draw( sf::RenderTarget& target, sf::RenderStates& states, Position2d cameraPosition, int drawDistance );
	};
}