#include "Animation.h"
using namespace std;
using namespace sf;

namespace kg
{

	void Animation::preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues )
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
		try
		{
			if( blueprintValues.at( BLUEPRINT_SYNC ).toBool() )
				m_unsyncedClock = sf::Clock();
		}
		catch( std::exception& e )
		{
			e;
			throw BlueprintValueReadErrorException( PLUGIN_NAME, BLUEPRINT_SYNC );
		}

		m_animationData = engine.resourceManager.getResource<AnimationData>( package, filename );
	}

	void Animation::init( Engine& engine, ComponentManager& thisEntity )
	{
		throw std::logic_error( "The method or operation is not implemented." );
	}

	void Animation::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		throw std::logic_error( "The method or operation is not implemented." );
	}

	double Animation::getUpdateImportance() const
	{
		throw std::logic_error( "The method or operation is not implemented." );
	}

	std::vector<size_t> Animation::getRequieredComponents() const
	{
		throw std::logic_error( "The method or operation is not implemented." );
	}

	const std::string& Animation::getPluginName() const
	{
		throw std::logic_error( "The method or operation is not implemented." );
	}

	Plugin::Id Animation::getPluginId() const
	{
		throw std::logic_error( "The method or operation is not implemented." );
	}

	const std::string Animation::PLUGIN_NAME = "AnimationComponent";

	const std::string Animation::BLUEPRINT_SYNC = "sync";


	bool AnimationData::loadFromFile( const std::string& path )
	{
		throw std::logic_error( "The method or operation is not implemented." );
	}

}
