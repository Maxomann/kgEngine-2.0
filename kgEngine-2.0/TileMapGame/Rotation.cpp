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

	int Rotation::getPluginId() const
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

	const std::string Rotation::PLUGIN_NAME = "Rotation";
}