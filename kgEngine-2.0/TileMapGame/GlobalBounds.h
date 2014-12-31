#pragma once
#include "stdafx.h"
#include "Position.h"
#include "Size.h"
#include "Rotation.h"

namespace kg
{
	class GlobalBounds : public Component, public CallbackReciever
	{
		Position* r_position = nullptr;
		Size* r_size = nullptr;
		Rotation* r_rotation = nullptr;

		void onTransformationChanged();

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues );

		virtual void init( Engine& engine, ComponentManager& thisEntity );

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual std::vector<size_t> getRequieredComponents() const;

		virtual const std::string& getPluginName() const;

		virtual Plugin::Id getPluginId()const;


		const sf::FloatRect get()const;

	signals:
		//Signal<const sf::FloatRect&> s_changed;

		static const std::string PLUGIN_NAME;
	};
}
