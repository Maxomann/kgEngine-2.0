#pragma once
#include "stdafx.h"
#include "Graphics.h"
#include "AnimationSystem.h"

namespace kg
{
	typedef std::map<std::string, std::pair<std::vector<std::vector<int>>, bool>> AnimationFileData;
	typedef std::map<std::string, std::pair<std::vector<std::pair<sf::IntRect, int>>, bool>> FrameDurationAndTexrectByState;

	class AnimationInterpreter
	{
	public:
		virtual FrameDurationAndTexrectByState& interpretAnimationFileData( const AnimationFileData& animationFileData )const = 0;
		FrameDurationAndTexrectByState& operator()( const AnimationFileData& animationFileData )const;
	};

	class FreeAnimation : public AnimationInterpreter
	{
	public:/*!!!!!!!!!!!!!!!!*/
		virtual FrameDurationAndTexrectByState& interpretAnimationFileData( const AnimationFileData& animationFileData ) const override;
	};

	class MultiplyAnimation : public AnimationInterpreter
	{
	public:
		virtual FrameDurationAndTexrectByState& interpretAnimationFileData( const AnimationFileData& animationFileData ) const override;
	};

	class AnimationFile : public Resource
	{
		static const std::map<std::string, std::shared_ptr<AnimationInterpreter>> INTERPRETER_BY_NAME;

		std::string m_interpreterType = "-1";
		AnimationFileData m_animationFileData;
		FrameDurationAndTexrectByState m_interpretedData;

	public:
		virtual bool loadFromFile( const std::string& path ) override;

		FrameDurationAndTexrectByState& get( std::shared_ptr<Entity>& forEntity )const;
		FrameDurationAndTexrectByState& operator()( std::shared_ptr<Entity>& forEntity )const;//equals get()

		static const std::string FREE_ANIMATION;//type of interpreter
		static const std::string MULTIPLY_ANIMATION;//type of interpreter
	};

	class Animation : public Component
	{

		AnimationSystem* r_animationSystem = nullptr;
		Graphics* r_graphicsComponent = nullptr;

		FrameDurationAndTexrectByState* m_animationData;

		std::string m_state;
		int m_frame;
		boost::optional<int> m_unsyncedTimePassed;

		void setAnimationOnGraphics();

	public:
		virtual void preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues ) override;

		virtual void init( Engine& engine, World& world, ComponentManager& thisEntity ) override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime ) override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<size_t> getRequieredComponents() const override;

		virtual const std::string& getPluginName() const override;

		virtual Plugin::Id getPluginId() const override;

		//

		//playing related:
		std::string getState()const;
		int getFrame()const;

		void setState( const std::string& state );
		void setFrame( const int frame );

		bool getCurrentStateSyncronized();

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

		static const std::string BLUEPRINT_SYNC;
	};
}
