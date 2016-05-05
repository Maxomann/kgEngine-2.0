#include "ChunkGeneratorSystem.h"
using namespace std;
using namespace sf;

namespace kg
{
	std::vector<Entity> ChunkGeneratorSystem::generateChunk( Engine& engine, World& world, const Position2d& chunkPositionInChunks )
	{
		std::vector<Entity> entities;
		const auto count = Constants::CHUNK_SIZE / Constants::TILE_SIZE;

		for( int x = 0; x < count; ++x )
			for( int y = 0; y < count; ++y )
			{
				auto tile = world.createNewSaveableEntity( engine, world, engine.randomNumberGenerator.getRandomInt( 101, 103 ) );//grass
				auto transform = tile.getComponent<TransformationComponent>();
				auto graphics = tile.getComponent<GraphicsComponent>();

				int chunkPositionInPixelX = chunkPositionInChunks.x*Constants::CHUNK_SIZE;
				int chunkPositionInPixelY = chunkPositionInChunks.y*Constants::CHUNK_SIZE;
				int tilePositionRelativeToChunkInPixelX = transform->getSize().x*x;
				int tilePositionRelativeToChunkInPixelY = transform->getSize().y*y;
				tilePositionRelativeToChunkInPixelX += transform->getSize().x / 2;
				tilePositionRelativeToChunkInPixelY += transform->getSize().y / 2;

				Position2d tilePosition( chunkPositionInPixelX + tilePositionRelativeToChunkInPixelX,
										 chunkPositionInPixelY + tilePositionRelativeToChunkInPixelY,
										 //Constants::STANDART_TILE_ZVALUE,
										 chunkPositionInChunks.worldLayer );

				transform->setPosition( tilePosition );
				graphics->setZValue( -1 );

				entities.push_back( move( tile ) );
			}

		return entities;
	}

	const size_t& ChunkGeneratorSystem::getRTTI_hash() const
	{
		return type_hash;
	}

	void ChunkGeneratorSystem::saveChangesToConfigFile( std::shared_ptr<ConfigFile>& configFile )
	{
		return;
	}

	void ChunkGeneratorSystem::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		return;
	}

	void ChunkGeneratorSystem::sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent )
	{
		return;
	}

	void ChunkGeneratorSystem::update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime )
	{
		return;
	}

	double ChunkGeneratorSystem::getUpdateImportance() const
	{
		return id::SystemUpdateImportance::CHUNK_GENERATOR;
	}

	const std::string& ChunkGeneratorSystem::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id ChunkGeneratorSystem::getPluginId() const
	{
		return id::SystemPluginId::CHUNK_GENERATOR;
	}

	const std::string ChunkGeneratorSystem::PLUGIN_NAME = "ChunkGenerator";

	const size_t ChunkGeneratorSystem::type_hash = getRuntimeTypeInfo<ChunkGeneratorSystem>();
}