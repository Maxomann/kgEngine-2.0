#include "AnimationSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	void AnimationSystem::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		for( const auto& el : engine.pluginManager.getEveryUserDefinedPluginFactory<AnimationHandler>() )
			m_AnimationHandlerPluginsbyName[el->getName()] = el;

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
			auto animationFile = engine.resourceManager.getResource<AnimationFile>( package, path, m_AnimationHandlerPluginsbyName );
			el = animationFile->getAnimationHandler();
			el->updateSyncedTimeRef( m_syncedTime );
		}

		return el.get();
	}

	void AnimationSystem::saveChangesToConfigFile( std::shared_ptr<ConfigFile>& configFile )
	{
		return;
	}

	const size_t& AnimationSystem::getRTTI_hash() const
	{
		return type_hash;
	}

	const std::string AnimationSystem::PLUGIN_NAME = "AnimationSystem";

	const size_t AnimationSystem::type_hash = getRuntimeTypeInfo<AnimationSystem>();

	void AnimationHandler::updateSyncedTimeRef( const sf::Time& time )
	{
		r_syncedTime = &time;
	}

	bool AnimationFile::loadFromFile( const std::string& path,
									  std::map<std::string, std::shared_ptr<PluginFactoryInterface<AnimationHandler>>>& animationHandlers )
	{
		auto file = readFileToVector( path );

		m_animationHandler = animationHandlers[file.at( 0 )]->create();

		if( m_animationHandler == nullptr )
			throw exception();

		m_animationHandler->load( file );

		return true;
	}

	std::shared_ptr<AnimationHandler> AnimationFile::getAnimationHandler() const
	{
		return m_animationHandler;
	}

	void EasyAnimationHandler::load( const std::vector<std::string> lines )
	{
		texrect_x = boost::lexical_cast< int >(lines.at( 1 ));
		texrect_y = boost::lexical_cast< int >(lines.at( 2 ));
		durationPerFrame = boost::lexical_cast< int >(lines.at( 3 ));

		int state_count = 0;

		for( unsigned int i = 4; i < lines.size(); ++i )
		{
			std::vector<std::string> vec;
			boost::split( vec, lines.at( i ), boost::is_any_of( "," ) );

			auto& name = vec.at( 0 );
			auto& frames = vec.at( 1 );
			auto& sync = vec.at( 2 );

			name.pop_back();
			name.erase( name.begin() );

			m_states[name] = std::make_tuple( state_count, boost::lexical_cast< int >(frames), boost::lexical_cast< bool >(sync) );
			state_count++;
		}
	}

	sf::IntRect EasyAnimationHandler::getTextureRect( const std::string& state, const sf::Time& internalTime ) const
	{
		auto& stateref = m_states.at( state );

		int frame;
		sf::Int32 timeAsMilliseconds;

		if( std::get<2>( stateref ) )
		{
			auto stateDuration = getStateDuration( state ).asMilliseconds();
			timeAsMilliseconds = r_syncedTime->asMilliseconds() % stateDuration;
		}
		else
		{
			timeAsMilliseconds = internalTime.asMilliseconds();
		}

		frame = (timeAsMilliseconds - (timeAsMilliseconds%durationPerFrame)) / durationPerFrame;

		return sf::IntRect( frame*texrect_x, get<0>( stateref )*texrect_y, texrect_x, texrect_y );
	}

	sf::Time EasyAnimationHandler::getStateDuration( const std::string& state ) const
	{
		return sf::milliseconds( std::get<1>( m_states.at( state ) )*durationPerFrame );
	}

	bool EasyAnimationHandler::getStateSynchronized( const std::string& state ) const
	{
		return std::get<2>( m_states.at( state ) );
	}

	const std::string& EasyAnimationHandler::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id EasyAnimationHandler::getPluginId() const
	{
		return id::SpecialPluginId::ANIMATION_HANDLER_EASY;
	}

	const std::string EasyAnimationHandler::PLUGIN_NAME = "EASY";
}