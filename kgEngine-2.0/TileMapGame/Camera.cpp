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

	void Camera::setFinalSize( const sf::Vector2u& size )
	{
		m_finalSizeAndScreenOffsetMutex.lock();
		m_finalSize = size;
		m_finalSizeAndScreenOffsetMutex.unlock();
	}

	sf::Vector2u Camera::getFinalSize() const
	{
		m_finalSizeAndScreenOffsetMutex.lock();
		auto retVal = m_finalSize;
		m_finalSizeAndScreenOffsetMutex.unlock();
		return retVal;
	}

	void Camera::setScreenOffset( const sf::Vector2i& offset )
	{
		m_finalSizeAndScreenOffsetMutex.lock();
		m_screenOffset = offset;
		m_finalSizeAndScreenOffsetMutex.unlock();
	}

	sf::Vector2i Camera::getScreenOffset() const
	{
		m_finalSizeAndScreenOffsetMutex.lock();
		auto retVal = m_screenOffset;
		m_finalSizeAndScreenOffsetMutex.unlock();
		return retVal;
	}

	std::shared_ptr<Entity> Camera::EMPLACE_TO_WORLD( Engine& engine, World& world )
	{
		auto camera = world.createNewTemporaryEntity<Transformation, Camera>( engine, world );
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
		m_renderTextureSize = resolution;
	}

	sf::Vector2u Camera::getRenderResolution() const
	{
		m_renderTextureSizeMutex.lock();
		auto retVal = m_renderTexture.getSize();
		m_renderTextureSizeMutex.unlock();
		return retVal;
	}

	void Camera::drawSpritesToRenderWindow( sf::RenderWindow& renderWindow,
											const EntityManager::EntityContainer& toDraw )
	{
		m_renderTextureSizeMutex.lock();
		if( m_renderTexture.getSize() != m_renderTextureSize )
			m_renderTexture.create( m_renderTextureSize.x, m_renderTextureSize.y );
		m_renderTextureSizeMutex.unlock();

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

		Sprite renderTextureSprite;
		m_renderTexture.clear( Color::Green );

		batch::SpriteBatch spriteBatch;
		spriteBatch.setRenderTarget( m_renderTexture );

		m_renderTexture.setView( view_copy );
		for( const auto& Z : toDrawSorted )
			for( const auto& Y : Z.second )
				for( const auto& X : Y.second )
					for( const auto& toDraw : X.second )
						toDraw->drawToSpriteBatch( spriteBatch );

		spriteBatch.display();
		m_renderTexture.display();

		renderTextureSprite.setTexture( m_renderTexture.getTexture() );
		auto renderTextureSpriteBounds = renderTextureSprite.getGlobalBounds();
		m_finalSizeAndScreenOffsetMutex.lock();
		renderTextureSprite.scale( static_cast<int>(m_finalSize.x / renderTextureSpriteBounds.width),
								   static_cast<int>(m_finalSize.y / renderTextureSpriteBounds.height));
		renderTextureSprite.setPosition( sf::Vector2f( m_screenOffset ) );
		m_finalSizeAndScreenOffsetMutex.unlock();

		renderWindow.draw( renderTextureSprite );
	}

	const std::string Camera::PLUGIN_NAME = "Camera";

	const size_t Camera::type_hash = getRuntimeTypeInfo<Camera>();

}
