#include "DrawingLayerContainer.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	void DrawingLayerContainer::initDrawingLayerPointer( std::unique_ptr<DrawingLayer>& pointer, int zValue )
	{
		if( pointer == nullptr )
		{
			if( zValue == YSORTED_DRAWING_LAYER_ZVALUE )
				pointer = make_unique<YSortedDrawingLayer>();
			else
				pointer = make_unique<StandartDrawingLayer>();
		}
	}

	void DrawingLayerContainer::addEntities( const EntityManager::EntityPointerContainer& entities )
	{
		for( auto& entity : entities )
		{
			auto worldLayer = entity->getComponent<TransformationComponent>()->getPosition().worldLayer;
			auto zValue = entity->getComponent<GraphicsComponent>()->getZValue();
			auto& el = m_drawingLayersByZValueByWorldLayer[worldLayer][zValue];

			initDrawingLayerPointer( el, zValue );
			el->addEntity( entity );
		}
	}

	void DrawingLayerContainer::removeEntities( const EntityManager::EntityPointerContainer& entities )
	{
		for( auto& entity : entities )
		{
			auto worldLayer = entity->getComponent<TransformationComponent>()->getPosition().worldLayer;
			auto zValue = entity->getComponent<GraphicsComponent>()->getZValue();
			auto& el = m_drawingLayersByZValueByWorldLayer[worldLayer][zValue];

			//remove
			el->removeEntity( entity );
		}
	}

	void DrawingLayerContainer::addEntities_try( const EntityManager::EntityPointerContainer& entities )
	{
		EntityManager::EntityPointerContainer validEntities;

		for( auto& entity : entities )
			if( entity->hasComponent<GraphicsComponent>() )
				validEntities.push_back( entity );

		addEntities( validEntities );
	}

	void DrawingLayerContainer::removeEntities_try( const EntityManager::EntityPointerContainer& entities )
	{
		EntityManager::EntityPointerContainer validEntities;

		for( auto& entity : entities )
			if( entity->hasComponent<GraphicsComponent>() )
				validEntities.push_back( entity );

		removeEntities( validEntities );
	}

	void DrawingLayerContainer::draw( sf::RenderTarget& target, sf::RenderStates& states, Position2d cameraPosition, int drawDistance )
	{
		auto& worldLayer = cameraPosition.worldLayer;

		auto it = m_drawingLayersByZValueByWorldLayer.find( worldLayer );

		if( it != m_drawingLayersByZValueByWorldLayer.end() )
		{
			for( const auto& zValueLayer : it->second )
				zValueLayer.second->draw( target, states, cameraPosition.toPositionXY(), drawDistance );
		}
	}
}