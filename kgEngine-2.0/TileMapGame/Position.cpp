#include "Position.h"
using namespace std;
using namespace sf;

namespace kg
{
	void Position::preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues )
	{
		return;
	}

	void Position::init( Engine& engine, ComponentManager& thisEntity )
	{
		return;
	}

	void Position::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		return;
	}

	double Position::getUpdateImportance() const
	{
		return ( double )id::ComponentUpdateImportance::POSITION;
	}

	std::vector<size_t> Position::getRequieredComponents() const
	{
		return{ };
	}

	const std::string& Position::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id Position::getPluginId() const
{
		return ( int )id::ComponentPluginId::POSITION;
	}

	void Position::set( const sf::Vector2i& position )
	{
		m_position = position;
		s_changed( position );
	}

	const sf::Vector2i& Position::get() const
	{
		return m_position;
	}

	void Position::move( const sf::Vector2i& offset )
	{
		set( m_position + offset );
	}

	void Position::writeSaveInformation( EntitySaveInformation& writeTo )
	{
		writeTo.addInformation( { to_string( m_position.x ), to_string( m_position.y ) } );
	}

	void Position::loadSaveInformation( const EntitySaveInformation& loadFrom )
	{
		auto& info = loadFrom.getInformation();
		if( info.size() == 2 )
		{
			auto pos_x = atoi( info.at( 0 ).c_str() );
			auto pos_y = atoi( info.at( 1 ).c_str() );
			set( Vector2i( pos_x, pos_y ) );
		}
		else
			throw exception();

		return;
	}

	const std::string Position::PLUGIN_NAME = "Position";
}