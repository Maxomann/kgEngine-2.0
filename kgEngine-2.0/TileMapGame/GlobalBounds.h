#pragma once
#include "stdafx.h"
#include "Position.h"
#include "Size.h"
#include "Rotation.h"

namespace kg
{
	class GlobalBounds : public Component, public CallbackReciever, public CallbackSender
	{
		Position* r_position = nullptr;
		Size* r_size = nullptr;
		Rotation* r_rotation = nullptr;

		void onTransformationChanged( int callbackId );

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues );

		virtual void init( Engine& engine, ComponentManager& componentManager );

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager );

		virtual double getUpdateImportance() const;

		virtual std::vector<size_t> getRequieredComponents() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;

		const sf::FloatRect get()const;

		enum class CallbackId : int
		{
			CHANGED//T: const sf::FloatRect&
		};

		static const std::string PLUGIN_NAME;
	};
}
