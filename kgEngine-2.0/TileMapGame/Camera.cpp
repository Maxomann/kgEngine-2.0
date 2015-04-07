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

	std::vector<size_t> Camera::getRequieredComponents() const
	{
		return{ Transformation::type_hash };
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

	void Camera::drawSpritesToRenderWindow( sf::RenderWindow& renderWindow,
											const EntityManager::EntityContainer& toDraw )
	{
		batch::SpriteBatch spriteBatch;
		spriteBatch.setRenderTarget( renderWindow );

		map<int, map<int, map<int, std::vector<Graphics*>>>> toDrawSorted;//Z Y X
		auto thisGlobalBounds = r_transformation->getGlobalBounds();
		m_viewMutex.lock();
		auto view_copy = m_view;
		m_viewMutex.unlock();

		//sort toDraws
		for( const auto& obj : toDraw )
		{
			auto transformationComponent = obj->getComponent<Transformation>();
			auto graphicsComponent = obj->getComponent<Graphics>();
			auto globalBounds = transformationComponent->getGlobalBounds();

			//if toDraw is seen on camera
			if( thisGlobalBounds.intersects( globalBounds ) )
			{
				//sort
				toDrawSorted
					[transformationComponent->getZValue()]//Z
				[globalBounds.top + globalBounds.height]//Y
				[globalBounds.left]//X
				.emplace_back( graphicsComponent );
			}
		}


		renderWindow.setView( view_copy );

		for( const auto& Z : toDrawSorted )
			for( const auto& Y : Z.second )
				for( const auto& X : Y.second )
					for( const auto& toDraw : X.second )
						toDraw->drawToSpriteBatch( spriteBatch );
		//renderWindow.draw( *toDraw );
		spriteBatch.display();

		renderWindow.setView( renderWindow.getDefaultView() );
	}

	const std::string Camera::PLUGIN_NAME = "Camera";

	const size_t Camera::type_hash = getRuntimeTypeInfo<Camera>();

}
