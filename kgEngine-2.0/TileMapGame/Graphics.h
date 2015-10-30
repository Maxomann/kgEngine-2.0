#pragma once
#include "stdafx.h"
#include "Transformation.h"

namespace kg
{

	class Graphics : public Component, public CallbackReciever, public sf::Drawable
	{
		Transformation* r_transformation = nullptr;

		std::shared_ptr<sf::Texture> m_resourceManagementReference;
		sf::Sprite m_sprite;

		void onPositionChanged( const sf::Vector2i& newPosition );
		void onSizeChanged( const sf::Vector2i& newSize );
		void onRotationChanged( const float& newRotation );

		//only work when position is {0,0} and rotation is 0
		void centerOrigin();
		void scaleToObjectSize();


		sf::IntRect m_textureRect;
		sf::IntRect m_textureRectOffset;

		void setTextureRect( const sf::IntRect& rect );
		sf::IntRect getTextureRect()const;

		void recalculateTextureRect();

	public:
		virtual void preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues )override;

		virtual void init( Engine& engine, World& world, ComponentManager& thisEntity )override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<Plugin::Id> getRequieredComponents()const override;

		virtual const Plugin::Name& getPluginName() const override;

		virtual Plugin::Id getPluginId()const override;

		virtual const size_t& getRTTI_hash() const override;


		void setTextureRectOffset( const sf::IntRect& rect );
		sf::IntRect getTextureRectOffset()const;


		void drawToSpriteBatch( batch::SpriteBatch& spriteBatch )const;

		virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const override;


		static const std::string BLUEPRINT_TEXTURE_RECT_WIDTH;
		static const std::string BLUEPRINT_TEXTURE_RECT_HEIGHT;
		static const std::string BLUEPRINT_TEXTURE_RECT_TOP;
		static const std::string BLUEPRINT_TEXTURE_RECT_LEFT;

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;
	};
}
