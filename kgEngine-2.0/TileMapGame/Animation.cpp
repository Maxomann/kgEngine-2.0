#include "Animation.h"
using namespace std;
using namespace sf;

namespace kg
{

	void Animation::preInit( Engine& engine, std::map<std::string, blueprint::Value>& blueprintValues )
	{
		//load blueprint configuration
		string package;
		string filename;

		try
		{
			package = blueprintValues.at( Constants::BLUEPRINT_PACKAGE_NAME ).toString();
		}
		catch( std::exception& e )
		{
			e;
			throw BlueprintValueReadErrorException( PLUGIN_NAME, Constants::BLUEPRINT_PACKAGE_NAME );
		}
		try
		{
			filename = blueprintValues.at( Constants::BLUEPRINT_FILE_PATH ).toString();
		}
		catch( std::exception& e )
		{
			e;
			throw BlueprintValueReadErrorException( PLUGIN_NAME, Constants::BLUEPRINT_FILE_PATH );
		}

		m_animationData = engine.resourceManager.getResource<AnimationFile>( package, filename )->get();
	}

	void Animation::init( Engine& engine, World& world, ComponentManager& thisEntity )
	{
		//validate pointers
		r_animationSystem = world.getSystem<AnimationSystem>().get();
		r_graphicsComponent = thisEntity.getComponent<Graphics>().get();

		//check pointers
		if( !r_animationSystem )
			throw exception();
		if( !r_graphicsComponent )
			throw exception();
	}

	void Animation::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		
	}

	double Animation::getUpdateImportance() const
	{
		return id::ComponentUpdateImportance::ANIMATION;
	}

	std::vector<size_t> Animation::getRequieredComponents() const
	{
		return{ typeid(Graphics).hash_code() };
	}

	const std::string& Animation::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id Animation::getPluginId() const
	{
		return id::ComponentPluginId::ANIMATION;
	}

	const std::string Animation::PLUGIN_NAME = "AnimationComponent";


	const std::string AnimationFile::FREE_ANIMATION="FREE";


	const std::string AnimationFile::EASY_ANIMATION="EASY";


	FrameDurationAndTexrectByState& AnimationInterpreter::operator()( const AnimationFileData& animationFileData ) const
	{
		return interpretAnimationFileData( animationFileData );
	}


	FrameDurationAndTexrectByState& FreeAnimation::interpretAnimationFileData( const AnimationFileData& animationFileData ) const
	{
		throw std::logic_error( "The method or operation is not implemented." );
	}


	FrameDurationAndTexrectByState& EasyAnimation::interpretAnimationFileData( const AnimationFileData& animationFileData ) const
	{
		throw std::logic_error( "The method or operation is not implemented." );
	}


	bool AnimationFile::loadFromFile( const std::string& path )
	{
		return true;
	}

	FrameDurationAndTexrectByState& AnimationFile::get() const
	{
		return FrameDurationAndTexrectByState();
	}

	FrameDurationAndTexrectByState& AnimationFile::operator()() const
	{
		return FrameDurationAndTexrectByState();
	}

}
