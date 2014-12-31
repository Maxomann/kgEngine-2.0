#include "Rotation.h"
using namespace std;
using namespace sf;

namespace kg
{
	void Rotation::preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues )
	{
		return;
	}

	void Rotation::init( Engine& engine, ComponentManager& thisEntity )
	{
		auto saveComponent = thisEntity.getComponent<Save>();
		if( saveComponent )
		{
			m_connectToSignal( saveComponent->s_loadSaveInformation[( int )id::ComponentPluginId::ROTATION],
							   &Rotation::onLoadSaveInformation );
			m_connectToSignal( saveComponent->s_writeSaveInformation[( int )id::ComponentPluginId::ROTATION],
							   &Rotation::onWriteSaveInformation );
		}
		return;
	}

	void Rotation::update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime )
	{
		return;
	}

	double Rotation::getUpdateImportance() const
	{
		return ( double )id::ComponentUpdateImportance::ROTATION;
	}

	std::vector<size_t> Rotation::getRequieredComponents() const
	{
		return{ };
	}

	const std::string& Rotation::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id Rotation::getPluginId() const
{
		return ( int )id::ComponentPluginId::ROTATION;
	}

	void Rotation::set( const float rotationInDegree )
	{
		m_rotationInDegree = rotationInDegree;
		s_changed( rotationInDegree );
	}

	float Rotation::get() const
	{
		return m_rotationInDegree;
	}

	void Rotation::rotate( const float offsetInDegree )
	{
		set( get() + offsetInDegree );
	}

	std::vector<std::string> Rotation::onWriteSaveInformation()
	{
		return { to_string( m_rotationInDegree ) };
	}

	void Rotation::onLoadSaveInformation( const std::vector<std::string>& information )
	{
		if( information.size() == 1 )
			m_rotationInDegree = atof( information.at( 0 ).c_str() );
		else
			throw exception();
	}

	const std::string Rotation::PLUGIN_NAME = "Rotation";
}
