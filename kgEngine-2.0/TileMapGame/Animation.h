#pragma once
#include "stdafx.h"
#include "Graphics.h"
#include "AnimationSystem.h"

namespace kg
{

	typedef std::map<std::string, std::pair<std::vector<std::vector<int>>, bool>> AnimationFileData;
	typedef std::map<std::string, std::vector<int>> FrameDurationByState;

	class AnimationImplementation
	{
		boost::optional<int> m_unsyncedTimePassed;

		AnimationFileData& m_animationData;

	protected:
		virtual FrameDurationByState& getFrameDurationByState() = 0;
		virtual void setAnimationOnGraphics( Graphics* graphicsComponent ) = 0;

		AnimationFileData& getAnimationData();

	public:
		void update( AnimationSystem* animationSystem, Graphics* graphicsComponent, int frameTimeInMilliseconds );

		std::string getState()const;
		int getFrame()const;

		void setState( const std::string& state );
		void setFrame( const int frame );

		bool getCurrentStateSyncronized();
	};

	class AnimationFile : public Resource
	{
		AnimationFileData m_animationData;

	public:
		virtual bool loadFromFile( const std::string& path ) override;

		std::unique_ptr<AnimationImplementation> get()const;
		std::unique_ptr<AnimationImplementation> operator()();//equals get()
	};

	class FreeAnimation : public AnimationImplementation
	{
	public:
		
	};

	class EasyAnimation : public AnimationImplementation
	{
	public:

	};


	class Animation : public Component
	{
		AnimationSystem* r_animationSystem=nullptr;
		Graphics* r_graphicsComponent = nullptr;

		std::unique_ptr<AnimationImplementation> m_animationImplementation;


	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues ) override;

		virtual void init( Engine& engine, World& world, ComponentManager& thisEntity ) override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime ) override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<size_t> getRequieredComponents() const override;

		virtual const std::string& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;


		static const std::string PLUGIN_NAME;
		
		static const std::string BLUEPRINT_SYNC;
	};
}
