#include "Graphics.h"
using namespace std;
using namespace sf;

namespace kg
{
	void Graphics::preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues )
	{

		string texturePackage;
		string texturePath;

		auto it = blueprintValues.find( Constants::BLUEPRINT_PACKAGE_NAME );
		if( it != blueprintValues.end() )
			texturePackage = it->second->asString();
		it = blueprintValues.find( Constants::BLUEPRINT_FILE_PATH );
		if( it != blueprintValues.end() )
			texturePath = it->second->asString();
		m_resourceManagementReference = engine.resourceManager.getResource<sf::Texture>( texturePackage, texturePath );
		m_sprite.setTexture( *m_resourceManagementReference.get() );

		sf::IntRect textureRect;
		it = blueprintValues.find( BLUEPRINT_TEXTURE_RECT_WIDTH );
		if( it != blueprintValues.end() )
			textureRect.width = it->second->asInt();
		it = blueprintValues.find( BLUEPRINT_TEXTURE_RECT_HEIGHT );
		if( it != blueprintValues.end() )
			textureRect.height = it->second->asInt();
		it = blueprintValues.find( BLUEPRINT_TEXTURE_RECT_TOP );
		if( it != blueprintValues.end() )
			textureRect.top = it->second->asInt();
		it = blueprintValues.find( BLUEPRINT_TEXTURE_RECT_LEFT );
		if( it != blueprintValues.end() )
			textureRect.left = it->second->asInt();
		m_sprite.setTextureRect( textureRect );

	}

	void Graphics::init( Engine& engine, World& world, ComponentManager& thisEntity )
	{
		r_transformation = thisEntity.getComponent<Transformation>();

		m_connectToSignal( r_transformation->s_positionChanged, &Graphics::onPositionChanged );
		m_connectToSignal( r_transformation->s_sizeChanged, &Graphics::onSizeChanged );
		m_connectToSignal( r_transformation->s_rotationChanged, &Graphics::onRotationChanged );

		centerOrigin();
		scaleToObjectSize();

		return;
	}

	void Graphics::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		return;
	}

	double Graphics::getUpdateImportance() const
	{
		return id::ComponentUpdateImportance::GRAPHICS;
	}

	std::vector<size_t> Graphics::getRequieredComponents() const
	{
		return{ Transformation::type_hash };
	}

	const std::string& Graphics::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id Graphics::getPluginId() const
	{
		return id::ComponentPluginId::GRAPHICS;
	}

	void Graphics::setTextureRect( const sf::IntRect& rect )
	{
		m_mutex.lock();
		m_sprite.setTextureRect( rect );
		m_mutex.unlock();
	}

	const sf::IntRect Graphics::getTextureRect() const
{
		m_mutex.lock();
		auto retVal = m_sprite.getTextureRect();
		m_mutex.unlock();
		return retVal;
	}

	void Graphics::onSizeChanged( const sf::Vector2i& newSize )
	{
		scaleToObjectSize();
	}

	void Graphics::centerOrigin()
	{
		m_mutex.lock();

		m_sprite.setOrigin( sf::Vector2f( m_sprite.getGlobalBounds().width / 2.f, m_sprite.getGlobalBounds().height / 2.f ) );

		m_mutex.unlock();
	}

	void Graphics::scaleToObjectSize()
	{
		m_mutex.lock();

		auto globalBounds = m_sprite.getGlobalBounds();

		auto scale = sf::Vector2f(
			r_transformation->getSize().x / globalBounds.width,
			r_transformation->getSize().y / globalBounds.height );

		m_sprite.scale( scale );

		m_mutex.unlock();
	}

	void Graphics::onPositionChanged( const sf::Vector2i& newPosition )
	{
		m_mutex.lock();
		m_sprite.setPosition( sf::Vector2f( newPosition ) );
		m_mutex.unlock();
	}

	void Graphics::onRotationChanged( const float& newRotation )
	{
		m_mutex.lock();
		m_sprite.setRotation( newRotation );
		m_mutex.unlock();
	}

	void Graphics::drawToSpriteBatch( batch::SpriteBatch& spriteBatch )const
	{
		m_mutex.lock();
		spriteBatch.draw( m_sprite );
		m_mutex.unlock();
	}

	void Graphics::draw( RenderTarget& target, RenderStates states ) const
	{
		m_mutex.lock();
		target.draw( m_sprite, states );
		m_mutex.unlock();
	}

	const size_t Graphics::type_hash = getRuntimeTypeInfo<Graphics>();

	const std::string Graphics::PLUGIN_NAME = "Graphics";

	const std::string Graphics::BLUEPRINT_TEXTURE_RECT_LEFT = "texrect_left";

	const std::string Graphics::BLUEPRINT_TEXTURE_RECT_TOP = "texrect_top";

	const std::string Graphics::BLUEPRINT_TEXTURE_RECT_HEIGHT = "texrect_height";

	const std::string Graphics::BLUEPRINT_TEXTURE_RECT_WIDTH = "texrect_width";
}
