#include "GraphicsSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GraphicsSystem::init( Engine& engine, World& world )
	{
		m_test = world.addEntity(world.createEntity( engine, 100 )).second;

		m_camera = std::make_shared<Entity>( world.getUniqueEntityId() );
		m_camera->addComponent<Position>( static_pointer_cast< Component >(make_shared<Position>()) );
		m_camera->addComponent<BoundingBox>( static_pointer_cast< Component >(make_shared<BoundingBox>()) );
		m_camera->addComponent<Camera>( static_pointer_cast< Component >(make_shared<Camera>()) );
		m_camera->initComponentsByImportance( engine );
		m_camera->getComponent<Position>()->set( sf::Vector2i( 0, 0 ) );
		m_camera->getComponent<BoundingBox>()->setSize( sf::Vector2i( engine.renderWindow.getSize() ) );
		m_camera->getComponent<Camera>()->setFinalSize( engine.renderWindow.getSize() );
		m_camera->getComponent<Camera>()->setScreenOffset( sf::Vector2i( 0, 0 ) );
		world.addEntity( m_camera );
		return;
	}

	void GraphicsSystem::sfmlEvent( const sf::Event& sfEvent )
	{
		return;
	}

	void GraphicsSystem::update( Engine& engine, World& world )
	{
		engine.renderWindow.draw( *m_camera->getComponent<Camera>() );
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
		return ( int )id::SystemPluginId::GRAPHICS_SYSTEM;
	}

	const std::string GraphicsSystem::PLUGIN_NAME = "GraphicsSystem";
}
