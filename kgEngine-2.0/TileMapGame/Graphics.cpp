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
		it = blueprintValues.find( BLUEPRINT_STATIC );
		if( it != blueprintValues.end() )
			m_isStatic = it->second->asBool();
		setTextureRect( textureRect );
	}

	void Graphics::init( Engine& engine, const World& world, const ComponentManager& thisEntity )
	{
		r_transformation = thisEntity.getComponent<Transformation>();

		m_connectToSignal( r_transformation->s_position2dChanged, &Graphics::onPositionChanged );
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

	std::vector<Plugin::Id> Graphics::getRequieredComponents() const
	{
		return{ id::ComponentPluginId::TRANSFORMATION };
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
		m_textureRect = rect;
		recalculateTextureRect();
	}

	sf::IntRect Graphics::getTextureRect() const
	{
		auto retVal = m_textureRect;
		return retVal;
	}

	void Graphics::onSizeChanged( const sf::Vector2i& newSize )
	{
		scaleToObjectSize();
	}

	void Graphics::centerOrigin()
	{
		m_sprite.setOrigin( sf::Vector2f( m_sprite.getGlobalBounds().width / 2.f, m_sprite.getGlobalBounds().height / 2.f ) );
	}

	void Graphics::scaleToObjectSize()
	{
		auto globalBounds = m_sprite.getGlobalBounds();

		auto scale = sf::Vector2f(
			r_transformation->getSize().x / globalBounds.width,
			r_transformation->getSize().y / globalBounds.height );

		m_sprite.scale( scale );
	}

	void Graphics::onPositionChanged( const Position2d& newPosition )
	{
		m_sprite.setPosition( sf::Vector2f( newPosition.toPositionXY().toVector2i() ) );
	}

	void Graphics::onRotationChanged( const float& newRotation )
	{
		m_sprite.setRotation( newRotation );
	}

	void Graphics::drawToSpriteBatch( batch::SpriteBatch& spriteBatch )
	{
		spriteBatch.drawToDynamicBuffer( { &m_sprite } );
	}

	void Graphics::draw( RenderTarget& target, RenderStates states ) const
	{
		target.draw( m_sprite, states );
	}

	void Graphics::setTextureRectOffset( const sf::IntRect& rect )
	{
		m_textureRectOffset = rect;
		recalculateTextureRect();
	}

	sf::IntRect Graphics::getTextureRectOffset() const
	{
		auto retVal = m_textureRectOffset;
		return retVal;
	}

	void Graphics::recalculateTextureRect()
	{
		sf::IntRect finalRect;
		finalRect.left = m_textureRect.left + m_textureRectOffset.left;
		finalRect.top = m_textureRect.top + m_textureRectOffset.top;

		if( m_textureRectOffset.width != 0 )
			finalRect.width = m_textureRectOffset.width;
		else
			finalRect.width = m_textureRect.width;

		if( m_textureRectOffset.height != 0 )
			finalRect.height = m_textureRectOffset.height;
		else
			finalRect.height = m_textureRect.height;

		m_sprite.setTextureRect( finalRect );
	}

	const size_t& Graphics::getRTTI_hash() const
	{
		return type_hash;
	}

	bool Graphics::isStatic() const
	{
		return m_isStatic;
	}

	Sprite* Graphics::getSprite()
	{
		return &m_sprite;
	}

	float Graphics::getZValue() const
	{
		return (m_sprite.getPosition().y) + (m_sprite.getGlobalBounds().height / 2.f);
	}

	const size_t Graphics::type_hash = getRuntimeTypeInfo<Graphics>();

	const std::string Graphics::PLUGIN_NAME = "Graphics";

	const std::string Graphics::BLUEPRINT_TEXTURE_RECT_LEFT = "texrect_left";

	const std::string Graphics::BLUEPRINT_TEXTURE_RECT_TOP = "texrect_top";

	const std::string Graphics::BLUEPRINT_TEXTURE_RECT_HEIGHT = "texrect_height";

	const std::string Graphics::BLUEPRINT_TEXTURE_RECT_WIDTH = "texrect_width";

	const std::string Graphics::BLUEPRINT_STATIC = "static";
}