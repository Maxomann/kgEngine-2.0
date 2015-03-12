#pragma once
#include "stdafx.h"
#include "Graphics.h"
#include "SpriteBatch.h"

namespace kg
{

	class Camera : public Component, public CallbackReciever
	{
		mutable std::mutex m_renderTextureSizeMutex;
		mutable std::mutex m_viewMutex;
		mutable std::mutex m_finalSizeAndScreenOffsetMutex;

		Transformation* r_transformation;

		sf::View m_view;
		sf::Vector2u m_finalSize;
		sf::Vector2i m_screenOffset;

		sf::Vector2u m_renderTextureSize;
		sf::RenderTexture m_renderTexture;

		void onPositionChanged( const sf::Vector2i& newPosition );
		void onSizeChanged( const sf::Vector2i& newSize );

	public:
		virtual void preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues )override;

		virtual void init( Engine& engine, World& world, ComponentManager& thisEntity )override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<size_t> getRequieredComponents() const override;

		virtual const std::string& getPluginName() const override;

		virtual Plugin::Id getPluginId()const override;

		// the size in pixel that this camera will render on the screen
		// for a window of size 800x600 choose sf::Vector2u(800,600)
		// if this is the only camera you want to draw to the window.
		// This is not the size of the bounding box. The bounding box is the
		// size of the area rendered from the world. Final size is the size that the rendered
		// area will get scaled to before being drawn to the screen
		void setFinalSize( const sf::Vector2u& size );
		sf::Vector2u getFinalSize()const;

		// the offset that will be applied to the rendered image of the camera before drawing on the screen
		void setScreenOffset( const sf::Vector2i& offset );
		sf::Vector2i getScreenOffset()const;

		// the size in pixel the camera will render to its internal buffer
		// if finalSize is set to 1600*1600
		// and renderResolution is set to 800*800
		// the image will get scaled up by the factor 2x.
		// The lower this value is set the less time is needed to render the image.
		// Use it for speedup on weak systems
		void setRenderResolution( const sf::Vector2u& resolution );
		sf::Vector2u getRenderResolution()const;
		
		void drawSpritesToRenderWindow( sf::RenderWindow& renderWindow,
										const EntityManager::EntityContainer& toDraw );

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

		//creates a new camera, adds it to the world and returns a pointer to it
		static std::shared_ptr<Entity> EMPLACE_TO_WORLD( Engine& engine, World& world );
	};
}
