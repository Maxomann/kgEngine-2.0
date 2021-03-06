#include "Camera.h"
using namespace std;
using namespace sf;

namespace kg
{
	void Camera::preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues )
	{
		//Should never be called since camera creates itself
		throw new bad_function_call();
	}

	void Camera::init( Engine& engine, const World& world, const ComponentManager& thisEntity )
	{
		r_transformation = thisEntity.getComponent<Transformation>();

		m_connectToSignal( r_transformation->s_position2dChanged, &Camera::m_onPositionChanged );
		m_connectToSignal( r_transformation->s_sizeChanged, &Camera::m_onSizeChanged );
	}

	void Camera::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		return;
	}

	double Camera::getUpdateImportance() const
	{
		return ( double )id::ComponentUpdateImportance::CAMERA;
	}

	std::vector<Plugin::Id> Camera::getRequieredComponents() const
	{
		return{ id::ComponentPluginId::TRANSFORMATION };
	}

	const std::string& Camera::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id Camera::getPluginId() const
	{
		return ( int )id::ComponentPluginId::CAMERA;
	}

	void Camera::m_onPositionChanged( const Position2d& newPosition )
	{
		m_view.setCenter( Vector2f( newPosition.toPositionXY().toVector2i() ) );
	}

	void Camera::m_onSizeChanged( const sf::Vector2i& newSize )
	{
		m_setViewSize( newSize, m_zoomFactor );
	}

	void Camera::m_setViewSize( const sf::Vector2i& size, const float& zoomFactor )
	{
		m_view.setSize( (( double )size.x)*zoomFactor, (( double )size.y)*zoomFactor );
	}

	void Camera::setViewport( const sf::FloatRect& viewport )
	{
		m_view.setViewport( viewport );
	}

	sf::FloatRect Camera::getViewport() const
	{
		return m_view.getViewport();
	}

	void Camera::setZoomFactor( const float& zoomFactor )
	{
		m_zoomFactor = zoomFactor;
		auto size = r_transformation->getSize();
		m_setViewSize( size, m_zoomFactor );
	}

	const float& Camera::getZoomFactor() const
	{
		return m_zoomFactor;
	}

	void Camera::setDrawDistance( const unsigned int& drawDistance )
	{
		m_drawDistance = drawDistance;
	}

	const unsigned int& Camera::getDrawDistance() const
	{
		return m_drawDistance;
	}

	kg::Entity Camera::CREATE( Engine& engine, World& world, const unsigned int& drawDistancePointer )
	{
		auto camera = world.createNewTemporaryEntity<Transformation, Camera>( engine, world );
		camera.getComponent<Transformation>()->setPositionXY( PositionXY( 0, 0 ) );
		camera.getComponent<Transformation>()->setSize( sf::Vector2i( engine.renderWindow.getSize().x, engine.renderWindow.getSize().y ) );
		auto cameraComponent = camera.getComponent<Camera>();
		cameraComponent->setViewport( FloatRect( 0.f, 0.f, 1.f, 1.f ) );
		cameraComponent->setDrawDistance( drawDistancePointer );
		return camera;
	}

	const size_t& Camera::getRTTI_hash() const
	{
		return type_hash;
	}

	void Camera::drawSpritesToRenderWindow( sf::RenderWindow& renderWindow,
											DrawingLayerContainer& drawingLayerContainer )
	{
		const auto thisGlobalBounds = r_transformation->getGlobalBounds();

		renderWindow.setView( m_view );

		auto thisPosition = r_transformation->getPosition();

		drawingLayerContainer.draw( renderWindow, RenderStates(), thisPosition, m_drawDistance );

		renderWindow.setView( renderWindow.getDefaultView() );
	}

	const std::string Camera::PLUGIN_NAME = "Camera";

	const size_t Camera::type_hash = getRuntimeTypeInfo<Camera>();
}