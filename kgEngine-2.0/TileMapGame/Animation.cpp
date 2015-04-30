#include "Animation.h"
using namespace std;
using namespace sf;

namespace kg
{
	void Animation::preInit( Engine& engine, const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues )
	{
		//load blueprint configuration
		string package;
		string filename;

		try
		{
			package = blueprintValues.at( Constants::BLUEPRINT_PACKAGE_NAME )->asString();
		}
		catch( std::exception& e )
		{
			e;
			throw BlueprintValueReadErrorException( PLUGIN_NAME, Constants::BLUEPRINT_PACKAGE_NAME );
		}
		try
		{
			filename = blueprintValues.at( Constants::BLUEPRINT_FILE_PATH )->asString();
		}
		catch( std::exception& e )
		{
			e;
			throw BlueprintValueReadErrorException( PLUGIN_NAME, Constants::BLUEPRINT_FILE_PATH );
		}

		//m_animationData = &engine.resourceManager.getResource<AnimationFile>( package, filename )->get();
	}

	void Animation::init( Engine& engine, World& world, ComponentManager& thisEntity )
	{
		//validate pointers
		r_animationSystem = world.getSystem<AnimationSystem>();
		r_graphicsComponent = thisEntity.getComponent<Graphics>();

		//check pointers
		if( !r_animationSystem )
			throw exception();
		if( !r_graphicsComponent )
			throw exception();
	}

	void Animation::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{ }

	double Animation::getUpdateImportance() const
	{
		return id::ComponentUpdateImportance::ANIMATION;
	}

	std::vector<size_t> Animation::getRequieredComponents() const
	{
		return{ Graphics::type_hash };
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

	const size_t Animation::type_hash = getRuntimeTypeInfo<Animation>();

	const std::string AnimationFile::FREE_ANIMATION = "FREE";

	const std::string AnimationFile::MULTIPLY_ANIMATION = "EASY";

	FrameDurationAndTexrectByState& AnimationInterpreter::operator()( const AnimationFileData& animationFileData ) const
	{
		return interpretAnimationFileData( animationFileData );
	}

	FrameDurationAndTexrectByState& FreeAnimation::interpretAnimationFileData( const AnimationFileData& animationFileData ) const
	{
		throw std::logic_error( "The method or operation is not implemented." );
	}

	FrameDurationAndTexrectByState& MultiplyAnimation::interpretAnimationFileData( const AnimationFileData& animationFileData ) const
	{
		throw std::logic_error( "The method or operation is not implemented." );
	}

	const std::map<std::string, std::shared_ptr<AnimationInterpreter>> AnimationFile::INTERPRETER_BY_NAME =
	{
		make_pair( AnimationFile::MULTIPLY_ANIMATION, make_shared<MultiplyAnimation>() ),
		make_pair( AnimationFile::FREE_ANIMATION, make_shared<FreeAnimation>() )
	};

	bool AnimationFile::loadFromFile( const std::string& path )
	{
		ifstream file;
		file.open( path );
		if( !file.is_open() )
			return false;/*file not open*/

		vector<string> data;
		string temp;
		while( getline( file, temp ) )
			data.push_back( temp );
		auto interpreter = INTERPRETER_BY_NAME.find( data.at( 0 ) );
		if( interpreter == INTERPRETER_BY_NAME.end() )
			return false;/*interpreter not found*/


		return true;
	}

	FrameDurationAndTexrectByState& AnimationFile::get( std::shared_ptr<Entity>& forEntity ) const
	{
		return FrameDurationAndTexrectByState();
	}

	FrameDurationAndTexrectByState& AnimationFile::operator()( std::shared_ptr<Entity>& forEntity ) const
	{
		return get( forEntity );
	}
}
