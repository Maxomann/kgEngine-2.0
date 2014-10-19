#pragma once
#include "stdafx.h"
#include "Position.h"
#include "BoundingBox.h"

namespace kg
{
	class Graphics : public Component, public CallbackReciever, sf::Drawable
	{
		std::shared_ptr<sf::Texture> m_resourceManagementReference;
		sf::Sprite m_sprite;

		void onPositionChanged( int callbackId, const sf::Vector2i& position );
		void centerOrigin();

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues );

		virtual void init( Engine& engine, ComponentManager& componentManager );

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager );

		virtual double getUpdateImportance() const;

		virtual std::vector<size_t> getRequieredComponents() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;


		void setTextureRect( const sf::IntRect& rect );
		const sf::IntRect& getTextureRect()const;
		virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

		static const std::string BLUEPRINT_PACKAGE_NAME;
		static const std::string BLUEPRINT_TEXTURE_PATH;
		static const std::string BLUEPRINT_TEXTURE_RECT_WIDTH;
		static const std::string BLUEPRINT_TEXTURE_RECT_HEIGHT;
		static const std::string BLUEPRINT_TEXTURE_RECT_TOP;
		static const std::string BLUEPRINT_TEXTURE_RECT_LEFT;

	};
}
