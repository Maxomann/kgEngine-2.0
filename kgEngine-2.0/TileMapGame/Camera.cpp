#include "Camera.h"
using namespace std;
using namespace sf;

namespace kg
{
	void Camera::preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues )
	{
		//Should never be called since camera is created in GraphicsSystem
		throw new bad_function_call();
	}

	void Camera::init( Engine& engine, ComponentManager& thisEntity )
	{
		r_transformation = thisEntity.getComponent<Transformation>().get();

		m_connectToSignal( r_transformation->s_positionChanged, &Camera::onPositionChanged );
		m_connectToSignal( r_transformation->s_sizeChanged, &Camera::onSizeChanged );
	}

	void Camera::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		m_texture.clear( Color::Green );
		m_texture.setView( m_view );

		auto toDraw = world.getEntitiesThatHaveComponent<Graphics>();
		map<int, map<int, map<int, std::vector<shared_ptr<Entity>>>>> toDrawSorted;//by Z value by Y value by X value;
		for( auto& el : toDraw )
		{
			auto graphics = el->getComponent<Graphics>();
			auto toDrawTransformationComponent = el->getComponent<Transformation>();
			auto toDrawGlobalBounds = toDrawTransformationComponent->getGlobalBounds();
			auto cameraRect = r_transformation->getGlobalBounds();

			if( toDrawGlobalBounds.intersects( cameraRect ) )//only add if visible on this camera
			{
				auto toDrawPosition = toDrawTransformationComponent->getPosition();
				auto zValue = toDrawTransformationComponent->getZValue();
				toDrawSorted[zValue][toDrawPosition.y][toDrawPosition.x].push_back( el );
			}
		}

		for( const auto& Z : toDrawSorted )
			for( const auto& Y : Z.second )
				for( const auto& X : Y.second )
					for( const auto& entity : X.second )
						m_texture.draw( *entity->getComponent<Graphics>() );

		m_texture.display();

		return;
	}

	double Camera::getUpdateImportance() const
	{
		return ( double )id::ComponentUpdateImportance::CAMERA;
	}

	std::vector<size_t> Camera::getRequieredComponents() const
	{
		return{ typeid(Transformation).hash_code() };
	}

	const std::string& Camera::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id Camera::getPluginId() const
	{
		return ( int )id::ComponentPluginId::CAMERA;
	}

	void Camera::draw( RenderTarget& target, RenderStates states ) const
	{
		Sprite sprite;
		sprite.setTexture( m_texture.getTexture() );
		sprite.setPosition( sf::Vector2f( m_screenOffset ) );
		sprite.scale( sf::Vector2f(
			m_finalSize.x / sprite.getGlobalBounds().width,
			m_finalSize.y / sprite.getGlobalBounds().height
			) );

		target.draw( sprite );
	}

	void Camera::onPositionChanged( const sf::Vector2i& newPosition )
	{
		m_view.setCenter( sf::Vector2f( newPosition ) );
	}

	void Camera::onSizeChanged( const sf::Vector2i& newSize )
	{
		m_view.setSize( sf::Vector2f( newSize ) );
	}

	void Camera::setFinalSize( const sf::Vector2u& size )
	{
		m_finalSize = size;
	}

	const sf::Vector2u& Camera::getFinalSize() const
	{
		return m_finalSize;
	}

	void Camera::setScreenOffset( const sf::Vector2i& offset )
	{
		m_screenOffset = offset;
	}

	const sf::Vector2i& Camera::getScreenOffset() const
	{
		return m_screenOffset;
	}

	std::shared_ptr<Entity> Camera::EMPLACE_TO_WORLD( Engine& engine, World& world )
	{
		auto camera = world.createNewTemporaryEntity<Transformation, Camera>( engine );
		camera->getComponent<Transformation>()->setPosition( sf::Vector2i( 0, 0 ) );
		camera->getComponent<Transformation>()->setSize( sf::Vector2i( engine.renderWindow.getSize().x, engine.renderWindow.getSize().y ) );
		camera->getComponent<Camera>()->setRenderResolution( engine.renderWindow.getSize() );
		camera->getComponent<Camera>()->setFinalSize( engine.renderWindow.getSize() );
		camera->getComponent<Camera>()->setScreenOffset( sf::Vector2i( 0, 0 ) );
		world.addEntity( camera );
		return camera;
	}

	void Camera::setRenderResolution( const sf::Vector2u& resolution )
	{
		m_texture.create( resolution.x, resolution.y );
	}

	sf::Vector2u Camera::getRenderResolution() const
	{
		return m_texture.getSize();
	}

	const std::string Camera::PLUGIN_NAME = "Camera";
}
