#include "Position.h"
using namespace std;
using namespace sf;

namespace kg
{

	void Position::preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues )
	{
		return;
	}

	void Position::init( Engine& engine, ComponentManager& componentManager )
	{
		return;
	}

	void Position::update( Engine& engine, World& world, ComponentManager& componentManager )
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

	int Position::getPluginId() const
	{
		return ( int )id::ComponentPluginId::POSITION;
	}

	void Position::set( const sf::Vector2i& position )
	{
		m_position = position;
		triggerCallback<const sf::Vector2i&>( ( int )CallbackId::CHANGED, m_position );
	}

	const sf::Vector2i& Position::get() const
	{
		return m_position;
	}

	void Position::move( const sf::Vector2i& offset )
	{
		set( m_position + offset );
	}

	const std::string Position::PLUGIN_NAME = "Position";

}
