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

	void Camera::init( Engine& engine, World& world, ComponentManager& thisEntity )
	{
		r_transformation = thisEntity.getComponent<Transformation>();

		m_connectToSignal( r_transformation->s_positionChanged, &Camera::m_onPositionChanged );
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

	void Camera::m_onPositionChanged( const sf::Vector2i& newPosition )
	{
		m_viewMutex.lock();
		m_view.setCenter( sf::Vector2f( newPosition ) );
		m_viewMutex.unlock();
	}

	void Camera::m_onSizeChanged( const sf::Vector2i& newSize )
	{
		m_setViewSize( newSize, m_zoomFactor );
	}

	void Camera::m_setViewSize( const sf::Vector2i& size, const float& zoomFactor )
	{
		m_viewMutex.lock();
		m_view.setSize( (( double )size.x)*zoomFactor, (( double )size.y)*zoomFactor );
		m_viewMutex.unlock();
	}

	void Camera::setViewport( const sf::FloatRect& viewport )
	{
		m_viewMutex.lock();
		m_view.setViewport( viewport );
		m_viewMutex.unlock();
	}

	sf::FloatRect Camera::getViewport() const
	{
		m_viewMutex.lock();
		auto retVal = m_view.getViewport();
		m_viewMutex.unlock();
		return retVal;
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

	kg::Entity Camera::CREATE( Engine& engine, World& world, boost::mutex& drawDistanceMutex, unsigned int* drawDistancePointer )
	{
		auto camera = world.createNewTemporaryEntity<Transformation, Camera>( engine, world );
		camera.getComponent<Transformation>()->setPosition( sf::Vector2i( 0, 0 ) );
		camera.getComponent<Transformation>()->setSize( sf::Vector2i( engine.renderWindow.getSize().x, engine.renderWindow.getSize().y ) );
		auto cameraComponent = camera.getComponent<Camera>();
		cameraComponent->setViewport( FloatRect( 0.f, 0.f, 1.f, 1.f ) );
		cameraComponent->r_drawDistanceMutex = &drawDistanceMutex;
		cameraComponent->r_drawDistance = drawDistancePointer;
		return camera;
	}

	const size_t& Camera::getRTTI_hash() const
	{
		return type_hash;
	}

	void Camera::drawSpritesToRenderWindow( sf::RenderWindow& renderWindow,
											const std::vector<std::tuple<sf::Vector3i, Entity*, Graphics*>>& toDrawSorted )
	{
		m_spriteBatch.setRenderTarget( renderWindow );

		const auto thisGlobalBounds = r_transformation->getGlobalBounds();
		m_viewMutex.lock();
		const auto view_copy = m_view;
		m_viewMutex.unlock();

		renderWindow.setView( view_copy );

		r_drawDistanceMutex->lock();

		for( const auto& el : toDrawSorted )
		{
			auto spritePosition = get<0>( el );
			auto thisPosition = r_transformation->getPosition();

			auto distanceVec = sf::Vector2i( spritePosition.x - thisPosition.x, spritePosition.y - thisPosition.y );
			if( length( distanceVec ) <= *r_drawDistance )
				get<2>( el )->drawToSpriteBatch( m_spriteBatch );
		}

		r_drawDistanceMutex->unlock();

		m_spriteBatch.display();

		renderWindow.setView( renderWindow.getDefaultView() );
	}

	const std::string Camera::PLUGIN_NAME = "Camera";

	const size_t Camera::type_hash = getRuntimeTypeInfo<Camera>();

}
