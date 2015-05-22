#pragma once
#include "stdafx.h"
#include "Graphics.h"
#include "SpriteBatch.h"

namespace kg
{

	class Camera : public Component, public CallbackReciever
	{
		mutable boost::mutex m_viewMutex;

		Transformation* r_transformation;

		sf::View m_view;
		sf::Vector2u m_finalSize;

		void onPositionChanged( const sf::Vector2i& newPosition );
		void onSizeChanged( const sf::Vector2i& newSize );

		batch::SpriteBatch m_spriteBatch;

	public:
		virtual void preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues )override;

		virtual void init( Engine& engine, World& world, ComponentManager& thisEntity )override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<Plugin::Id> getRequieredComponents()const override;

		virtual const Plugin::Name& getPluginName() const override;

		virtual Plugin::Id getPluginId()const override;

		virtual const size_t& getRTTI_hash() const override;


		// the viewport of the camera on the screen
		void setViewport( const sf::FloatRect& viewport );
		sf::FloatRect getViewport()const;
		
		void drawSpritesToRenderWindow( sf::RenderWindow& renderWindow,
										const std::vector<std::tuple<sf::Vector3i, std::shared_ptr<Entity>, Graphics*>>& toDrawSorted );

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

		//creates a new camera, adds it to the world and returns a pointer to it
		static std::shared_ptr<Entity> EMPLACE_TO_WORLD( Engine& engine, World& world );


	};
}
