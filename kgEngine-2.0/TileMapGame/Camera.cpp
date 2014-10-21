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

	void Camera::init( Engine& engine, ComponentManager& componentManager )
	{
		r_position = componentManager.getComponent<Position>().get();
		r_boundingBox = componentManager.getComponent<BoundingBox>().get();

		r_position->registerCallback_1<Camera, const sf::Vector2i&>(
			( int )Position::CallbackId::CHANGED,
			this,
			&Camera::onPositionChanged );
		r_boundingBox->registerCallback_1<Camera, const sf::Vector2i&>(
			( int )BoundingBox::CallbackId::SIZE_CHANGED,
			this,
			&Camera::onBoundingBoxChanged );
	}

	void Camera::update( Engine& engine, World& world, ComponentManager& componentManager )
	{
		m_texture.clear( Color::Green );
		m_texture.setView( m_view );

		auto toDraw = world.getEntitiesThatHaveComponent<Graphics>();
		map<int, std::vector<shared_ptr<Entity>>> sortedByFeetPosition;
		for( auto& el : toDraw )
		{
			auto feetPosition = el->getComponent<BoundingBox>()->getFeetPosition();
			sortedByFeetPosition[feetPosition].push_back( el );
		}

		for( const auto& el : sortedByFeetPosition )
			for( const auto& entity : el.second )
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
		return{ typeid(Position).hash_code(), typeid(BoundingBox).hash_code() };
	}

	const std::string& Camera::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	int Camera::getPluginId() const
	{
		return ( int )id::ComponentPluginId::CAMERA;
	}

	void Camera::draw( RenderTarget& target, RenderStates states ) const
	{
		Sprite sprite;
		sprite.setTexture(m_texture.getTexture());
		sprite.setPosition( sf::Vector2f(m_screenOffset ));
		sprite.scale( sf::Vector2f(
			m_finalSize.x / sprite.getGlobalBounds().width,
			m_finalSize.y / sprite.getGlobalBounds().height
			) );

		target.draw( sprite );
	}

	void Camera::onPositionChanged( int callbackId, const sf::Vector2i& newPosition )
	{
		m_view.setCenter( sf::Vector2f( newPosition ) );
	}

	void Camera::onBoundingBoxChanged( int callbacId, const sf::Vector2i& newSize )
	{
		m_view.setSize( sf::Vector2f( newSize ) );
		m_texture.create( newSize.x, newSize.y );
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

	const std::string Camera::PLUGIN_NAME = "Camera";

}
