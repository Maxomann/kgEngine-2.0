#pragma once
#include "stdafx.h"

namespace kg
{
	class AnimationHandler
	{
	protected:
		const sf::Time* r_syncedTime;

	public:
		AnimationHandler( const std::vector<std::string> lines );

		void updateSyncedTimeRef( const sf::Time& time );

		virtual sf::IntRect& getTextureRect( const std::string& state, const sf::Time& internalTime )const = 0;

		virtual sf::Time& getStateDuration( const std::string& state )const = 0;

		virtual bool getStateSynchronized()const = 0;
	};

	class AnimationFile
	{
		std::shared_ptr<AnimationHandler> m_animationHandler;

	public:
		bool loadFromFile( const std::string& path );

		std::shared_ptr<AnimationHandler> getAnimationHandler()const;
	};

	class AnimationSystem : public System
	{
		std::map<std::string, std::map<std::string, std::shared_ptr<AnimationHandler>>> m_animationHandlersByPackageByPath;

		sf::Time m_syncedTime;

	public:
		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile ) override;

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent ) override;

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime ) override;

		virtual double getUpdateImportance() const override;

		virtual const std::string& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;


		AnimationHandler* getAnimationHandler( Engine& engine, const std::string& package, const std::string& path );


		static const std::string PLUGIN_NAME;

		static const size_t type_hash;
	};
}
