#include "GraphicsSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GraphicsSystem::init( Engine& engine, World& world )
	{
		m_camera = std::make_shared<Entity>( world.getUniqueEntityId() );
		m_camera->addComponent<Position>( static_pointer_cast< Component >(make_shared<Position>()) );
		m_camera->addComponent<Size>( static_pointer_cast< Component >(make_shared<Size>()) );
		m_camera->addComponent<Camera>( static_pointer_cast< Component >(make_shared<Camera>()) );
		m_camera->addComponent<GlobalBounds>( static_pointer_cast< Component >(make_shared<GlobalBounds>()) );
		m_camera->initComponentsByImportance( engine );
		m_camera->getComponent<Position>()->set( sf::Vector2i( 0, 0 ) );
		m_camera->getComponent<Size>()->set( sf::Vector2i( engine.renderWindow.getSize() ) );
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

	std::shared_ptr<Entity> GraphicsSystem::getCamera()
	{
		return m_camera;
	}

	const std::string GraphicsSystem::PLUGIN_NAME = "GraphicsSystem";
}
