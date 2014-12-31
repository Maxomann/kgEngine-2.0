#pragma once
#include "stdafx.h"
#include "Position.h"
#include "Size.h"
#include "Rotation.h"
#include "GlobalBounds.h"

namespace kg
{
	class Graphics : public Component, public CallbackReciever, public sf::Drawable
	{
		Size* r_size = nullptr;
		GlobalBounds* r_globalBounds;

		std::shared_ptr<sf::Texture> m_resourceManagementReference;
		sf::Sprite m_sprite;

		void onPositionChanged( const sf::Vector2i& newPosition );
		void onSizeChanged( const sf::Vector2i& newSize );
		void onRotationChanged( const float& newRotation );

		void centerOrigin();
		void scaleToObjectSize();

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues )override;

		virtual void init( Engine& engine, ComponentManager& thisEntity )override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<size_t> getRequieredComponents() const override;

		virtual const std::string& getPluginName() const override;

		virtual Plugin::Id getPluginId()const override;



		void setTextureRect( const sf::IntRect& rect );
		const sf::IntRect& getTextureRect()const;

		float getZValue()const;

		virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

		static const std::string BLUEPRINT_PACKAGE_NAME;
		static const std::string BLUEPRINT_TEXTURE_PATH;
		static const std::string BLUEPRINT_TEXTURE_RECT_WIDTH;
		static const std::string BLUEPRINT_TEXTURE_RECT_HEIGHT;
		static const std::string BLUEPRINT_TEXTURE_RECT_TOP;
		static const std::string BLUEPRINT_TEXTURE_RECT_LEFT;

		static const std::string PLUGIN_NAME;
	};
}
