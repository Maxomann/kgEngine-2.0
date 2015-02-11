#pragma once
#include "stdafx.h"
#include "Transformation.h"

namespace kg
{
	struct GraphicsStateInformation
	{
		sf::FloatRect globalBounds;
		std::shared_ptr<sf::Drawable> drawable;
		int zValue;
	};


	class Graphics : public Component, public CallbackReciever//, public sf::Drawable
	{
		Transformation* r_transformation = nullptr;

		std::shared_ptr<sf::Texture> m_resourceManagementReference;
		std::shared_ptr<sf::Sprite> m_sprite = std::make_shared<sf::Sprite>();

		void onPositionChanged( const sf::Vector2i& newPosition );
		void onSizeChanged( const sf::Vector2i& newSize );
		void onRotationChanged( const float& newRotation );

		//only works when position is {0,0} and rotation is 0
		void centerOrigin();
		void scaleToObjectSize();

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues )override;

		virtual void init( Engine& engine, World& world, ComponentManager& thisEntity )override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<size_t> getRequieredComponents() const override;

		virtual const std::string& getPluginName() const override;

		virtual Plugin::Id getPluginId()const override;


		GraphicsStateInformation getStateInformation()const;

		void setTextureRect( const sf::IntRect& rect );
		const sf::IntRect& getTextureRect()const;


		/*void drawToSpriteBatch(batch::SpriteBatch& spriteBatch)const;*/
		//virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

		static const std::string BLUEPRINT_TEXTURE_RECT_WIDTH;
		static const std::string BLUEPRINT_TEXTURE_RECT_HEIGHT;
		static const std::string BLUEPRINT_TEXTURE_RECT_TOP;
		static const std::string BLUEPRINT_TEXTURE_RECT_LEFT;

		static const std::string PLUGIN_NAME;
	};
}
