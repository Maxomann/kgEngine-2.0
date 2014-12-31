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

	void Size::init( Engine& engine, ComponentManager& thisEntity )
	{
		auto saveComponent = thisEntity.getComponent<Save>();
		if( saveComponent )
		{
			m_connectToSignal( saveComponent->s_loadSaveInformation[( int )id::ComponentPluginId::SIZE],
							   &Size::onLoadSaveInformation );
			m_connectToSignal( saveComponent->s_writeSaveInformation[( int )id::ComponentPluginId::SIZE],
							   &Size::onWriteSaveInformation );
		}
		return;
	}

	void Size::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
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

	Plugin::Id Size::getPluginId() const
{
		return ( int )id::ComponentPluginId::SIZE;
	}

	void Size::set( const sf::Vector2i& size )
	{
		m_size = size;
		s_changed( size );
	}

	sf::Vector2i Size::get() const
	{
		return m_size;
	}

	std::vector<std::string> Size::onWriteSaveInformation()
	{
		return { to_string( m_size.x ), to_string( m_size.y ) };
	}

	void Size::onLoadSaveInformation( const std::vector<std::string>& information )
	{
		if( information.size() == 2 )
		{
			auto size_x = atoi( information.at( 0 ).c_str() );
			auto size_y = atoi( information.at( 1 ).c_str() );
			set( sf::Vector2i( size_x, size_y ) );
		}
		else
			throw exception();

		return;
	}

	const std::string Size::PLUGIN_NAME = "Size";

	const std::string Size::BLUEPRINT_WIDTH = "width";
	const std::string Size::BLUEPRINT_HEIGHT = "height";
}