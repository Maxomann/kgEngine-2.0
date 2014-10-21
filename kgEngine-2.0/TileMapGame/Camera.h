#pragma once
#include "stdafx.h"
#include "Graphics.h"

namespace kg
{
	class Camera : public Component, public sf::Drawable, public CallbackReciever
	{
		Position* r_position;
		BoundingBox* r_boundingBox;

		sf::View m_view;
		sf::RenderTexture m_texture;
		sf::Vector2u m_finalSize;
		sf::Vector2i m_screenOffset;

		void onPositionChanged( int callbackId, const sf::Vector2i& newPosition );
		void onBoundingBoxChanged( int callbacId, const sf::Vector2i& newSize );

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues );

		virtual void init( Engine& engine, ComponentManager& componentManager );

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager );

		virtual double getUpdateImportance() const;

		virtual std::vector<size_t> getRequieredComponents() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;


		// the size in pixel that this camera will render on the screen
		// for a window of size 800x600 choose sf::Vector2u(800,600)
		// if this is the only camera you want to draw to the window.
		// This is not the size of the bounding box. The bounding box is the
		// size of the area rendered from the world. Final size is the size that the rendered
		// area will get scaled to before being drawn to the screen
		void setFinalSize(const sf::Vector2u& size);
		const sf::Vector2u& getFinalSize()const;

		// the offset that will be applied to the rendered image of the camera before drawing on the screen
		void setScreenOffset( const sf::Vector2i& offset );
		const sf::Vector2i& getScreenOffset()const;

		virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

		static const std::string PLUGIN_NAME;
	};
}
