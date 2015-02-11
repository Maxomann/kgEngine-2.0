#include "AnimationSystem.h"
using namespace std;
using namespace sf;

namespace kg
{


	void AnimationSystem::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		return;
	}

	void AnimationSystem::sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent )
	{
		return;
	}

	void AnimationSystem::update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime )
	{
		int passedTime = frameTime.asMilliseconds();

		for( auto& el : m_syncedTimeByStateDuration )
		{
			el.second += passedTime;
			if( el.first > el.second )
				el.second = 0;
		}

		return;
	}

	double AnimationSystem::getUpdateImportance() const
	{
		return id::SystemUpdateImportance::ANIMATION_SYSTEM;
	}

	const std::string& AnimationSystem::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id AnimationSystem::getPluginId() const
	{
		return id::SystemPluginId::ANIMATION_SYSTEM;
	}

	int AnimationSystem::getSyncedTimeForAnimationDuration( const int stateDurationInMilliseconds )
	{
		return m_syncedTimeByStateDuration[stateDurationInMilliseconds];
	}

	const std::string AnimationSystem::PLUGIN_NAME="AnimationSystem";

}
