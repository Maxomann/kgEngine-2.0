#include "ChunkSystem.h"
using namespace std;
using namespace sf;

namespace kg
{


	void ChunkSystem::init( Engine& engine, World& world )
	{
		world.registerCallback_1( ( int )EntityManager::CallbackId::ENTITY_ADDED,
								  this,
								  &ChunkSystem::m_onEntityAddedToWorld );
		return;
	}

	void ChunkSystem::sfmlEvent( const sf::Event& sfEvent )
	{
		return;
	}

	void ChunkSystem::update( Engine& engine, World& world )
	{
		return;
	}

	double ChunkSystem::getUpdateImportance() const
	{
		return ( double )id::SystemUpdateImportance::CHUNK_SYSTEM;
	}

	const std::string& ChunkSystem::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	int ChunkSystem::getPluginId() const
	{
		return ( int )id::SystemPluginId::CHUNK_SYSTEM;
	}

	void ChunkSystem::m_onEntityAddedToWorld( int callbackId, std::shared_ptr<Entity>& entity )
	{

	}

	const std::string ChunkSystem::PLUGIN_NAME = "ChunkSystem";
}
