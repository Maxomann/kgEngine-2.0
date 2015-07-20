#pragma once
#include "stdafx.h"

namespace kg
{
	class AnimationHandler : public Plugin, public sf::NonCopyable
	{
	protected:
		const sf::Time* r_syncedTime;

	public:
		void updateSyncedTimeRef( const sf::Time& time );
		
		virtual void load( const std::vector<std::string> lines )=0;//ignore lines.at(0)! it is the name of the animation handler

		virtual sf::IntRect getTextureRect( const std::string& state, const sf::Time& internalTime )const = 0;

		virtual sf::Time getStateDuration( const std::string& state )const = 0;

		virtual bool getStateSynchronized( const std::string& state )const = 0;
	};

	class AnimationFile
	{
		std::shared_ptr<AnimationHandler> m_animationHandler;

	public:
		bool loadFromFile( const std::string& path, std::map<std::string, std::shared_ptr<PluginFactoryInterface<AnimationHandler>>>& animationHandlers );

		std::shared_ptr<AnimationHandler> getAnimationHandler()const;
	};

	class AnimationSystem : public System
	{
		std::map<std::string, std::shared_ptr<PluginFactoryInterface<AnimationHandler>>> m_AnimationHandlerPluginsbyName;

		std::map<std::string, std::map<std::string, std::shared_ptr<AnimationHandler>>> m_animationHandlersByPackageByPath;

		sf::Time m_syncedTime;

	public:
		virtual void init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile ) override;

		virtual void sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent ) override;

		virtual void update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime ) override;

		virtual double getUpdateImportance() const override;

		virtual const Plugin::Name& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;

		virtual const size_t& getRTTI_hash() const override;


		AnimationHandler* getAnimationHandler( Engine& engine, const std::string& package, const std::string& path );


		static const std::string PLUGIN_NAME;

		static const size_t type_hash;
	};

	

	class EasyAnimationHandler : public AnimationHandler
	{
		int texrect_x;
		int texrect_y;
		int durationPerFrame;

		std::map<std::string, std::tuple<int, int, bool>> m_states;//tuple: state_count, frames, sync

	public:
		virtual void load( const std::vector<std::string> lines ) override;

		virtual sf::IntRect getTextureRect( const std::string& state, const sf::Time& internalTime ) const override;

		virtual sf::Time getStateDuration( const std::string& state ) const override;

		virtual bool getStateSynchronized( const std::string& state ) const override;

		virtual const Plugin::Name& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;


		static const std::string PLUGIN_NAME;
	};
}
