#pragma once
#include "stdafx.h"

namespace kg
{
	class AnimationData : public Resource
	{
	public:
		virtual bool loadFromFile( const std::string& path ) override;

	};

	class Animation : public Component
	{
		std::shared_ptr<AnimationData> m_animationData;

		int currentStateDurationInMilliseconds=-1;

		boost::optional<const sf::Clock&> m_syncedClock;
		boost::optional<sf::Clock> m_unsyncedClock;

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues ) override;

		virtual void init( Engine& engine, ComponentManager& thisEntity ) override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime ) override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<size_t> getRequieredComponents() const override;

		virtual const std::string& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;


		static const std::string PLUGIN_NAME;
		
		static const std::string BLUEPRINT_SYNC;
	};
}
