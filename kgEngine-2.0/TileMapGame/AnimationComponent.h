#pragma once
#include "stdafx.h"
#include "Graphics.h"
#include "AnimationSystem.h"

namespace kg
{

	class AnimationComponent : public Component
	{
		struct BlueprintValues
		{
			std::string package;
			std::string	path;
		}m_blueprintValues;

		AnimationSystem* r_animationSystem = nullptr;
		AnimationHandler* r_animationHandler = nullptr;
		Graphics* r_graphicsComponent = nullptr;

		bool m_isPlaying = true;
		std::string m_state;
		sf::Time m_stateTime;
		sf::Time m_stateDuration;
		bool m_stateSynced = false;

	public:
		virtual void preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues ) override;

		virtual void init( Engine& engine, World& world, ComponentManager& thisEntity ) override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime ) override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<Plugin::Id> getRequieredComponents()const override;

		virtual const Plugin::Name& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;

		virtual const size_t& getRTTI_hash() const override;


		//playing related:
		const std::string& getState()const;
		void setState( const std::string& state );

		void start();
		void stop();
		void reset();//if m_stateSynced, m_stateTime will not be set to 0


		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

		static const std::string BLUEPRINT_SYNC;
	};
}
