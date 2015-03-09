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
		r_transformation = thisEntity.getComponent<Transformation>().get();

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
		return m_renderTexture.getSize();
	}

	void Camera::drawSpritesToRenderWindow( sf::RenderWindow& renderWindow, const EntityManager::EntityContainer& toDraw )
	{
		if( getRenderResolution() != m_renderTextureSize )
			m_renderTexture.create( m_renderTextureSize.x, m_renderTextureSize.y );

		map<int, map<int, map<int, std::vector<Graphics*>>>> toDrawSorted;//Z Y X
		auto& thisGlobalBounds = r_transformation->getGlobalBounds();

		//sort toDraws
		for( const auto& obj : toDraw )
		{
			auto transformationComponent = obj->getComponent<Transformation>();
			auto graphicsComponent = obj->getComponent<Graphics>();
			auto& globalBounds = transformationComponent->getGlobalBounds();

			//if toDraw is seen on camera
			if( thisGlobalBounds.intersects( globalBounds ) )
			{
				//sort
				toDrawSorted
					[transformationComponent->getZValue()]//Z
				[globalBounds.top + globalBounds.height]//Y
				[globalBounds.left]//X
				.emplace_back( graphicsComponent.get() );
			}
		}

		Sprite renderTextureSprite;
		m_renderTexture.clear( Color::Green );

		batch::SpriteBatch spriteBatch;
		/*RenderStates states( m_view.getTransform() );
		spriteBatch.setRenderStates( states );*/
		spriteBatch.setRenderTarget( m_renderTexture );

		m_renderTexture.setView( m_view );
		for( const auto& Z : toDrawSorted )
			for( const auto& Y : Z.second )
				for( const auto& X : Y.second )
					for( const auto& toDraw : X.second )
						toDraw->drawToSpriteBatch( spriteBatch );

		spriteBatch.display();
		m_renderTexture.display();


		renderTextureSprite.setTexture( m_renderTexture.getTexture() );
		auto renderTextureSpriteBounds = renderTextureSprite.getGlobalBounds();
		renderTextureSprite.scale( m_finalSize.x / renderTextureSpriteBounds.width,
								   m_finalSize.y / renderTextureSpriteBounds.height );
		renderTextureSprite.setPosition( sf::Vector2f( m_screenOffset ) );

		renderWindow.draw( renderTextureSprite );
	}

	const std::string Camera::PLUGIN_NAME = "Camera";
}
