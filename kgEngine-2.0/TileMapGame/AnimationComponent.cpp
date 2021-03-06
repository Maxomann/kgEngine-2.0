#include "AnimationComponent.h"
using namespace std;
using namespace sf;

namespace kg
{
	void AnimationComponent::preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues )
	{
		//load blueprint configuration
		try
		{
			m_blueprintValues.package = blueprintValues.at( Constants::BLUEPRINT_PACKAGE_NAME )->asString();
		}
		catch( std::exception& e )
		{
			e;
			throw BlueprintValueReadErrorException( PLUGIN_NAME, Constants::BLUEPRINT_PACKAGE_NAME );
		}
		try
		{
			m_blueprintValues.path = blueprintValues.at( Constants::BLUEPRINT_FILE_PATH )->asString();
		}
		catch( std::exception& e )
		{
			e;
			throw BlueprintValueReadErrorException( PLUGIN_NAME, Constants::BLUEPRINT_FILE_PATH );
		}
	}

	void AnimationComponent::init( Engine& engine, const World& world, const ComponentManager& thisEntity )
	{
		//validate pointers
		r_animationSystem = world.getSystem<AnimationSystem>();
		r_graphicsComponent = thisEntity.getComponent<Graphics>();

		//check pointers
		if( !r_animationSystem )
			throw exception();
		if( !r_graphicsComponent )
			throw exception();

		r_animationHandler = r_animationSystem->getAnimationHandler( engine, m_blueprintValues.package, m_blueprintValues.path );

		if( !r_animationHandler )
			throw exception();

		setState( Constants::ANIMATION_DEFAULT_STATE );
	}

	void AnimationComponent::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		if( m_isPlaying )
		{
			m_stateTime += frameTime;
			if( m_stateTime > m_stateDuration )
				m_stateTime = sf::Time();

			r_graphicsComponent->setTextureRectOffset( r_animationHandler->getTextureRect( m_state, m_stateTime ) );
		}
	}

	double AnimationComponent::getUpdateImportance() const
	{
		return id::ComponentUpdateImportance::ANIMATION;
	}

	std::vector<Plugin::Id> AnimationComponent::getRequieredComponents() const
	{
		return{ id::ComponentPluginId::GRAPHICS };
	}

	const std::string& AnimationComponent::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id AnimationComponent::getPluginId() const
	{
		return id::ComponentPluginId::ANIMATION;
	}

	const std::string& AnimationComponent::getState() const
	{
		return m_state;
	}

	void AnimationComponent::setState( const std::string& state )
	{
		m_state = state;
		m_stateTime = sf::Time();
		m_stateDuration = r_animationHandler->getStateDuration( state );
		m_stateSynced = r_animationHandler->getStateSynchronized( state );
	}

	void AnimationComponent::start()
	{
		m_isPlaying = true;
	}

	void AnimationComponent::stop()
	{
		m_isPlaying = false;
	}

	void AnimationComponent::reset()
	{
		m_isPlaying = true;
		m_stateTime = sf::Time();
	}

	const size_t& AnimationComponent::getRTTI_hash() const
	{
		return type_hash;
	}

	const std::string AnimationComponent::PLUGIN_NAME = "Animation";

	const size_t AnimationComponent::type_hash = getRuntimeTypeInfo<AnimationComponent>();
}