#pragma once
#include "stdafx.h"

namespace kg
{
	class AnimationSystem : public System
	{
		std::map<int, int> m_syncedTimeByStateDuration;

	public:
		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile ) override;

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent ) override;

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime ) override;

		virtual double getUpdateImportance() const override;

		virtual const std::string& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;


		int getSyncedTimeForAnimationDuration( const int stateDurationInMilliseconds );


		static const std::string PLUGIN_NAME;
	};
}