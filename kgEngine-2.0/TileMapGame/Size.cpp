#include "Size.h"
using namespace std;
using namespace sf;

namespace kg
{
	void Size::preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues )
	{
		auto it = blueprintValues.find( BLUEPRINT_HEIGHT );
		if( it != blueprintValues.end() )
			m_size.x = it->second.toInt();
		it = blueprintValues.find( BLUEPRINT_WIDTH );
		if( it != blueprintValues.end() )
			m_size.y = it->second.toInt();
	}

	void Size::init( Engine& engine, ComponentManager& componentManager )
	{
		return;
	}

	void Size::update( Engine& engine, World& world, ComponentManager& componentManager )
	{
		return;
	}

	double Size::getUpdateImportance() const
	{
		return ( double )id::ComponentUpdateImportance::SIZE;
	}

	std::vector<size_t> Size::getRequieredComponents() const
	{
		return{ };
	}

	const std::string& Size::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	int Size::getPluginId() const
	{
		return ( int )id::ComponentPluginId::SIZE;
	}

	void Size::set( const sf::Vector2i& size )
	{
		m_size = size;
		triggerCallback<const sf::Vector2i&>( ( int )CallbackId::CHANGED, m_size );
	}

	sf::Vector2i Size::get() const
	{
		return m_size;
	}

	const std::string Size::PLUGIN_NAME = "Size";

	const std::string Size::BLUEPRINT_WIDTH = "width";
	const std::string Size::BLUEPRINT_HEIGHT = "height";
}