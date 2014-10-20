#include "Graphics.h"
using namespace std;
using namespace sf;

namespace kg
{

	void Graphics::preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues )
	{
		string texturePackage;
		string texturePath;

		auto it = blueprintValues.find( BLUEPRINT_PACKAGE_NAME );
		if( it != blueprintValues.end() )
			texturePackage = it->second.toString();
		it = blueprintValues.find( BLUEPRINT_TEXTURE_PATH );
		if( it != blueprintValues.end() )
			texturePath = it->second.toString();
		m_resourceManagementReference = engine.resourceManagement.getResource<sf::Texture>( texturePackage, texturePath );
		m_sprite.setTexture( *m_resourceManagementReference.get() );

		sf::IntRect textureRect;
		it = blueprintValues.find( BLUEPRINT_TEXTURE_RECT_WIDTH );
		if( it != blueprintValues.end() )
			textureRect.width = it->second.toInt();
		it = blueprintValues.find( BLUEPRINT_TEXTURE_RECT_HEIGHT );
		if( it != blueprintValues.end() )
			textureRect.height = it->second.toInt();
		it = blueprintValues.find( BLUEPRINT_TEXTURE_RECT_TOP );
		if( it != blueprintValues.end() )
			textureRect.top = it->second.toInt();
		it = blueprintValues.find( BLUEPRINT_TEXTURE_RECT_LEFT );
		if( it != blueprintValues.end() )
			textureRect.left = it->second.toInt();
		m_sprite.setTextureRect( textureRect );
	}

	void Graphics::init( Engine& engine, ComponentManager& componentManager )
	{
		r_position = componentManager.getComponent<Position>().get();
		r_boundingBox = componentManager.getComponent<BoundingBox>().get();

		r_boundingBox->registerCallback_1<Graphics, const sf::IntRect&>(
			( int )BoundingBox::CallbackId::CHANGED,
			this,
			&Graphics::onBoundingBoxChanged );

		centerOrigin();
		scaleToBoundingBox();

		return;
	}

	void Graphics::update( Engine& engine, World& world, ComponentManager& componentManager )
	{
		return;
	}

	double Graphics::getUpdateImportance() const
	{
		return ( double )id::ComponentUpdateImportance::GRAPHICS;
	}

	std::vector<size_t> Graphics::getRequieredComponents() const
	{
		return{ typeid(Position).hash_code(), typeid(BoundingBox).hash_code() };
	}

	const std::string& Graphics::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	int Graphics::getPluginId() const
	{
		return ( int )id::ComponentPluginId::GRAPHICS;
	}

	void Graphics::draw( RenderTarget& target, RenderStates states ) const
	{
		target.draw( m_sprite, states );
	}

	void Graphics::setTextureRect( const sf::IntRect& rect )
	{
		m_sprite.setTextureRect( rect );
	}

	const sf::IntRect& Graphics::getTextureRect() const
	{
		return m_sprite.getTextureRect();
	}

	void Graphics::onBoundingBoxChanged( int callbackId, const sf::IntRect& rect )
	{
		m_sprite.setPosition(sf::Vector2f(r_position->get()));
		scaleToBoundingBox();
	}

	void Graphics::centerOrigin()
	{
		m_sprite.setOrigin( sf::Vector2f( m_sprite.getGlobalBounds().width / 2, m_sprite.getGlobalBounds().height / 2 ) );
	}

	void Graphics::scaleToBoundingBox()
	{
		auto globalBounds = m_sprite.getGlobalBounds();

 		m_sprite.scale( sf::Vector2f(
 			r_boundingBox->get().width / globalBounds.width,
 			r_boundingBox->get().height / globalBounds.height) );
	}

	const std::string Graphics::PLUGIN_NAME = "Graphics";

	const std::string Graphics::BLUEPRINT_TEXTURE_RECT_LEFT = "texrect_left";

	const std::string Graphics::BLUEPRINT_TEXTURE_RECT_TOP = "texrect_top";

	const std::string Graphics::BLUEPRINT_TEXTURE_RECT_HEIGHT = "texrect_height";

	const std::string Graphics::BLUEPRINT_TEXTURE_RECT_WIDTH = "texrect_width";

	const std::string Graphics::BLUEPRINT_TEXTURE_PATH = "path";

	const std::string Graphics::BLUEPRINT_PACKAGE_NAME = "package";

}
