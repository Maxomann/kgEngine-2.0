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
		m_syncedTime += frameTime;
		if( m_syncedTime > Constants::ANIMATION_MAX_SYNCED_TIME )
			m_syncedTime = sf::Time();
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

	AnimationHandler* AnimationSystem::getAnimationHandler( Engine& engine, const std::string& package, const std::string& path )
	{
		auto& el = m_animationHandlersByPackageByPath[package][path];
		if( el == nullptr )
		{
			auto animationFile = engine.resourceManager.getResource<AnimationFile>( package, path );
			el = animationFile->getAnimationHandler();
			el->updateSyncedTimeRef( m_syncedTime );
		}

		return el.get();
	}

	const std::string AnimationSystem::PLUGIN_NAME = "AnimationSystem";

	const size_t AnimationSystem::type_hash = getRuntimeTypeInfo<AnimationSystem>();

	void AnimationHandler::updateSyncedTimeRef( const sf::Time& time )
	{
		r_syncedTime = &time;
	}

	bool AnimationFile::loadFromFile( const std::string& path )
	{
		auto file = readFileToVector( path );


		return true;
	}

	std::shared_ptr<AnimationHandler> AnimationFile::getAnimationHandler() const
	{
		return m_animationHandler;
	}

}
