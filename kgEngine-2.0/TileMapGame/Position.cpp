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
		auto saveComponent = thisEntity.getComponent<Save>();
		if( saveComponent )
		{
			m_connectToSignal( saveComponent->s_loadSaveInformation[( int )id::ComponentPluginId::POSITION],
							   &Position::onLoadSaveInformation );
			m_connectToSignal( saveComponent->s_writeSaveInformation[( int )id::ComponentPluginId::POSITION],
							   &Position::onWriteSaveInformation );
		}

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

	std::vector<std::string> Position::onWriteSaveInformation()
	{
		return { to_string( m_position.x ), to_string( m_position.y ) };
	}

	void Position::onLoadSaveInformation( const std::vector<std::string>& information )
	{
		if( information.size() == 2 )
		{
			auto pos_x = atoi( information.at( 0 ).c_str() );
			auto pos_y = atoi( information.at( 1 ).c_str() );
			set( Vector2i( pos_x, pos_y ) );
		}
		else
			throw exception();

		return;
	}

	const std::string Position::PLUGIN_NAME = "Position";
}
