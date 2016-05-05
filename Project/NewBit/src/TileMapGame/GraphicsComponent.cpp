#include "GraphicsComponent.h"
using namespace std;
using namespace sf;

namespace kg
{
	void GraphicsComponent::preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues )
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

	void GraphicsComponent::init( Engine& engine, const World& world, const ComponentManager& thisEntity )
	{
		r_transformation = thisEntity.getComponent<TransformationComponent>();

		m_connectToSignal( r_transformation->s_position2dChanged, &GraphicsComponent::onPositionChanged );
		m_connectToSignal( r_transformation->s_sizeChanged, &GraphicsComponent::onSizeChanged );
		m_connectToSignal( r_transformation->s_rotationChanged, &GraphicsComponent::onRotationChanged );

		centerOrigin();
		scaleToObjectSize();

		auto saveComponent = thisEntity.getComponentTry<Save>();
		if( saveComponent )
		{
			m_connectToSignal( saveComponent->s_loadSaveInformation[( int )id::ComponentPluginId::GRAPHICS],
							   &GraphicsComponent::onLoadSaveInformation );
			m_connectToSignal( saveComponent->s_writeSaveInformation[( int )id::ComponentPluginId::GRAPHICS],
							   &GraphicsComponent::onWriteSaveInformation );
		}

		return;
	}

	void GraphicsComponent::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		return;
	}

	double GraphicsComponent::getUpdateImportance() const
	{
		return id::ComponentUpdateImportance::GRAPHICS;
	}

	std::vector<Plugin::Id> GraphicsComponent::getRequieredComponents() const
	{
		return{ id::ComponentPluginId::TRANSFORMATION };
	}

	const std::string& GraphicsComponent::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id GraphicsComponent::getPluginId() const
	{
		return id::ComponentPluginId::GRAPHICS;
	}

	void GraphicsComponent::setTextureRect( const sf::IntRect& rect )
	{
		m_textureRect = rect;
		recalculateTextureRect();
	}

	sf::IntRect GraphicsComponent::getTextureRect() const
	{
		auto retVal = m_textureRect;
		return retVal;
	}

	void GraphicsComponent::onSizeChanged( const sf::Vector2i& newSize )
	{
		scaleToObjectSize();
	}

	void GraphicsComponent::centerOrigin()
	{
		m_sprite.setOrigin( sf::Vector2f( m_sprite.getGlobalBounds().width / 2.f, m_sprite.getGlobalBounds().height / 2.f ) );
	}

	void GraphicsComponent::scaleToObjectSize()
	{
		auto globalBounds = m_sprite.getGlobalBounds();

		auto scale = sf::Vector2f(
			r_transformation->getSize().x / globalBounds.width,
			r_transformation->getSize().y / globalBounds.height );

		m_sprite.scale( scale );
	}

	void GraphicsComponent::onPositionChanged( const Position2d& newPosition )
	{
		m_sprite.setPosition( sf::Vector2f( newPosition.toPositionXY().toVector2i() ) );
	}

	void GraphicsComponent::onRotationChanged( const float& newRotation )
	{
		m_sprite.setRotation( newRotation );
	}

	void GraphicsComponent::draw( RenderTarget& target, RenderStates states ) const
	{
		target.draw( m_sprite, states );
	}

	void GraphicsComponent::setTextureRectOffset( const sf::IntRect& rect )
	{
		m_textureRectOffset = rect;
		recalculateTextureRect();
	}

	sf::IntRect GraphicsComponent::getTextureRectOffset() const
	{
		auto retVal = m_textureRectOffset;
		return retVal;
	}

	void GraphicsComponent::recalculateTextureRect()
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

	const size_t& GraphicsComponent::getRTTI_hash() const
	{
		return type_hash;
	}

	bool GraphicsComponent::isStatic() const
	{
		return m_isStatic;
	}

	Sprite* GraphicsComponent::getSprite()
	{
		return &m_sprite;
	}

	int GraphicsComponent::getZValue() const
	{
		return m_zValue;
	}

	void GraphicsComponent::setZValue( int zValue )
	{
		m_zValue = zValue;
	}

	void GraphicsComponent::onLoadSaveInformation( const std::vector<std::string>& information )
	{
		if( information.size() != 1 )
			throw exception();

		m_zValue = atoi( information.at( 0 ).c_str() );
	}

	std::vector<std::string> GraphicsComponent::onWriteSaveInformation()
	{
		return{ to_string( m_zValue ) };
	}

	const size_t GraphicsComponent::type_hash = getRuntimeTypeInfo<GraphicsComponent>();

	const std::string GraphicsComponent::PLUGIN_NAME = "Graphics";

	const std::string GraphicsComponent::BLUEPRINT_TEXTURE_RECT_LEFT = "texrect_left";

	const std::string GraphicsComponent::BLUEPRINT_TEXTURE_RECT_TOP = "texrect_top";

	const std::string GraphicsComponent::BLUEPRINT_TEXTURE_RECT_HEIGHT = "texrect_height";

	const std::string GraphicsComponent::BLUEPRINT_TEXTURE_RECT_WIDTH = "texrect_width";

	const std::string GraphicsComponent::BLUEPRINT_STATIC = "static";
}