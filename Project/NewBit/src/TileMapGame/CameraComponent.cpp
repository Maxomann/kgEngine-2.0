#include "CameraComponent.h"
using namespace std;
using namespace sf;

namespace kg
{
	void CameraComponent::preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues )
	{
		//Should never be called since camera creates itself
		throw new bad_function_call();
	}

	void CameraComponent::init( Engine& engine, const World& world, const ComponentManager& thisEntity )
	{
		r_transformation = thisEntity.getComponent<TransformationComponent>();

		m_connectToSignal( r_transformation->s_position2dChanged, &CameraComponent::m_onPositionChanged );
		m_connectToSignal( r_transformation->s_sizeChanged, &CameraComponent::m_onSizeChanged );
	}

	void CameraComponent::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		return;
	}

	double CameraComponent::getUpdateImportance() const
	{
		return ( double )id::ComponentUpdateImportance::CAMERA;
	}

	std::vector<Plugin::Id> CameraComponent::getRequieredComponents() const
	{
		return{ id::ComponentPluginId::TRANSFORMATION };
	}

	const std::string& CameraComponent::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id CameraComponent::getPluginId() const
	{
		return ( int )id::ComponentPluginId::CAMERA;
	}

	void CameraComponent::m_onPositionChanged( const Position2d& newPosition )
	{
		m_view.setCenter( Vector2f( newPosition.toPositionXY().toVector2i() ) );
	}

	void CameraComponent::m_onSizeChanged( const sf::Vector2i& newSize )
	{
		m_setViewSize( newSize, m_zoomFactor );
	}

	void CameraComponent::m_setViewSize( const sf::Vector2i& size, const float& zoomFactor )
	{
		m_view.setSize( (( double )size.x)*zoomFactor, (( double )size.y)*zoomFactor );
	}

	void CameraComponent::setViewport( const sf::FloatRect& viewport )
	{
		m_view.setViewport( viewport );
	}

	sf::FloatRect CameraComponent::getViewport() const
	{
		return m_view.getViewport();
	}

	void CameraComponent::setZoomFactor( const float& zoomFactor )
	{
		m_zoomFactor = zoomFactor;
		auto size = r_transformation->getSize();
		m_setViewSize( size, m_zoomFactor );
	}

	const float& CameraComponent::getZoomFactor() const
	{
		return m_zoomFactor;
	}

	void CameraComponent::setDrawDistance( const unsigned int& drawDistance )
	{
		m_drawDistance = drawDistance;
	}

	const unsigned int& CameraComponent::getDrawDistance() const
	{
		return m_drawDistance;
	}

	kg::Entity CameraComponent::CREATE( Engine& engine, World& world, const unsigned int& drawDistancePointer )
	{
		auto camera = world.createNewTemporaryEntity<TransformationComponent, CameraComponent>( engine, world );
		camera.getComponent<TransformationComponent>()->setPositionXY( PositionXY( 0, 0 ) );
		camera.getComponent<TransformationComponent>()->setSize( sf::Vector2i( engine.renderWindow.getSize().x, engine.renderWindow.getSize().y ) );
		auto cameraComponent = camera.getComponent<CameraComponent>();
		cameraComponent->setViewport( FloatRect( 0.f, 0.f, 1.f, 1.f ) );
		cameraComponent->setDrawDistance( drawDistancePointer );
		return camera;
	}

	const size_t& CameraComponent::getRTTI_hash() const
	{
		return type_hash;
	}

	void CameraComponent::drawSpritesToRenderWindow( sf::RenderWindow& renderWindow,
													 DrawingLayerContainer& drawingLayerContainer )
	{
		const auto thisGlobalBounds = r_transformation->getGlobalBounds();

		renderWindow.setView( m_view );

		auto thisPosition = r_transformation->getPosition();

		drawingLayerContainer.draw( renderWindow, RenderStates(), thisPosition, m_drawDistance );

		renderWindow.setView( renderWindow.getDefaultView() );
	}

	const std::string CameraComponent::PLUGIN_NAME = "Camera";

	const size_t CameraComponent::type_hash = getRuntimeTypeInfo<CameraComponent>();
}