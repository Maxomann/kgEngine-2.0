#pragma once
#include "stdafx.h"
#include "Graphics.h"
#include "SpriteBatch.h"

namespace kg
{

	class Camera : public Component, public CallbackReciever
	{
		Transformation* r_transformation;

		unsigned int m_drawDistance;

		sf::View m_view;

		void m_onPositionChanged( const sf::Vector2i& newPosition );
		void m_onSizeChanged( const sf::Vector2i& newSize );
		void m_setViewSize( const sf::Vector2i& size, const float& zoomFactor );

		batch::SpriteBatch m_spriteBatch;

		float m_zoomFactor = 1;

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
		
		void setZoomFactor( const float& zoomFactor );
		const float& getZoomFactor()const;

		void setDrawDistance( const unsigned int& drawDistance );
		const unsigned int& getDrawDistance()const;

		void drawSpritesToRenderWindow( sf::RenderWindow& renderWindow,
										const std::vector<std::tuple<sf::Vector3i, Entity*, Graphics*>>& toDrawSorted );

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

		//creates a new camera, adds it to the world and returns a pointer to it
		static Entity CREATE( Engine& engine, World& world, const unsigned int& drawDistancePointer );


	};
}
