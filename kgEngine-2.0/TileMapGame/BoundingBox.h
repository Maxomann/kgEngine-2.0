#pragma once
#include "stdafx.h"
#include "Position.h"

namespace kg
{
	class BoundingBox : public Component, public CallbackSender, public CallbackReciever
	{
		Position* r_position;

		sf::IntRect m_boundingBox;

		void onPositionChanged(int CallbackId, const sf::Vector2i& position);
		void setCenter( const sf::Vector2i& position );

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues );

		virtual void init( Engine& engine, ComponentManager& componentManager );

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager );

		virtual double getUpdateImportance() const;

		virtual std::vector<size_t> getRequieredComponents() const;//Position

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;


		void setSize( const sf::Vector2i& size );
		sf::Vector2i getSize()const;
		const sf::IntRect& get()const;

		int getFeetPosition();//returns top+height

		enum class CallbackId : int
		{
			CHANGED//T: const sf:IntRect&
		};

		static const std::string BLUEPRINT_WIDTH;
		static const std::string BLUEPRINT_HEIGHT;

		static const std::string PLUGIN_NAME;

	};
}
