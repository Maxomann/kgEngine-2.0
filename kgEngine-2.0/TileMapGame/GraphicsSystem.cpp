#include "GraphicsSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GraphicsSystem::init( Engine& engine, SystemManager& systemManager )
	{
		return;
	}

	void GraphicsSystem::sfmlEvent( const sf::Event& sfEvent )
	{
		return;
	}

	void GraphicsSystem::update( Engine& engine, World& world )
	{
// 		myEntity = world.createEntity( engine, 100 );
// 		world.addEntity( myEntity );
// 		position += sf::Vector2i(1,1);
// 		myEntity->getComponent<Position>()->set(position);

		auto toDraw = world.getEntitiesThatHaveComponent<Graphics>();
		std::map<int, std::vector<std::shared_ptr<Graphics>>> componentsToDrawByFeetPosition;
		std::vector<std::shared_ptr<Graphics>> toDrawWithoutBoundingBox;
		for( const auto& el : toDraw )
		{
			if( el->hasComponent<BoundingBox>() )
			{
				componentsToDrawByFeetPosition[el->getComponent<BoundingBox>()->getFeetPosition()].push_back( el->getComponent<Graphics>() );
			}
			else
			{
				toDrawWithoutBoundingBox.push_back( el->getComponent<Graphics>() );
			}
		}

		for( const auto& el : componentsToDrawByFeetPosition )
			for(const auto& comp: el.second )
				engine.renderWindow.draw( *comp );

		for( const auto& el : toDrawWithoutBoundingBox )
			engine.renderWindow.draw( *el );
	}

	double GraphicsSystem::getUpdateImportance() const
	{
		return ( double )id::SystemUpdateImportance::GRAPHICS_SYSTEM;
	}

	const std::string& GraphicsSystem::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	int GraphicsSystem::getPluginId() const
	{
		return (int)id::SystemPluginId::GRAPHICS_SYSTEM;
	}

	const std::string GraphicsSystem::PLUGIN_NAME = "GraphicsSystem";

}
