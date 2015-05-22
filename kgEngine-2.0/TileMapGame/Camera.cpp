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

		m_connectToSignal( r_transformation->s_positionChanged, &Camera::onPositionChanged );
		m_connectToSignal( r_transformation->s_sizeChanged, &Camera::onSizeChanged );
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

	void Camera::onPositionChanged( const sf::Vector2i& newPosition )
	{
		m_viewMutex.lock();
		m_view.setCenter( sf::Vector2f( newPosition ) );
		m_viewMutex.unlock();
	}

	void Camera::onSizeChanged( const sf::Vector2i& newSize )
	{
		m_viewMutex.lock();
		m_view.setSize( sf::Vector2f( newSize ) );
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

	std::shared_ptr<Entity> Camera::EMPLACE_TO_WORLD( Engine& engine, World& world )
	{
		auto camera = world.createNewTemporaryEntity<Transformation, Camera>( engine, world );
		camera->getComponent<Transformation>()->setPosition( sf::Vector2i( 0, 0 ) );
		camera->getComponent<Transformation>()->setSize( sf::Vector2i( engine.renderWindow.getSize().x, engine.renderWindow.getSize().y ) );
		camera->getComponent<Camera>()->setViewport( FloatRect( 0.f, 0.f, 1.f, 1.f ) );
		world.addEntity( camera );
		return camera;
	}

	const size_t& Camera::getRTTI_hash() const
	{
		return type_hash;
	}

	void Camera::drawSpritesToRenderWindow( sf::RenderWindow& renderWindow,
											const vector<tuple<Vector3i, std::shared_ptr<Entity>, Graphics*>>& toDrawSorted )
	{
		m_spriteBatch.setRenderTarget( renderWindow );

		const auto thisGlobalBounds = r_transformation->getGlobalBounds();
		m_viewMutex.lock();
		const auto view_copy = m_view;
		m_viewMutex.unlock();

		renderWindow.setView( view_copy );

		for( const auto& el : toDrawSorted )
			get<2>( el )->drawToSpriteBatch( m_spriteBatch );
			//renderWindow.draw( *toDraw.second );
		m_spriteBatch.display();

		renderWindow.setView( renderWindow.getDefaultView() );
	}

	const std::string Camera::PLUGIN_NAME = "Camera";

	const size_t Camera::type_hash = getRuntimeTypeInfo<Camera>();

}
