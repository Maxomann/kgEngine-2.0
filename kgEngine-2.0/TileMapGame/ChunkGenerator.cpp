#include "ChunkGenerator.h"
using namespace std;
using namespace sf;

namespace kg
{
	void ChunkGenerator::generateChunk( Engine& engine, World& world, const sf::Vector2i chunkPositionInChunks )
	{
		const auto count = Constants::CHUNK_SIZE / Constants::TILE_SIZE;

		for( int x = 0; x < count; ++x )
			for( int y = 0; y < count; ++y )
			{
				auto tile = world.createNewSaveableEntity( engine, world, engine.randomNumberGenerator.getRandomInt( 101, 103 ) );//grass
				auto transform = tile.getComponent<Transformation>();

				int chunkPositionInPixelX = chunkPositionInChunks.x*Constants::CHUNK_SIZE;
				int chunkPositionInPixelY = chunkPositionInChunks.y*Constants::CHUNK_SIZE;
				int tilePositionRelativeToChunkInPixelX = transform->getSize().x*x;
				int tilePositionRelativeToChunkInPixelY = transform->getSize().y*y;
				tilePositionRelativeToChunkInPixelX += transform->getSize().x / 2;
				tilePositionRelativeToChunkInPixelY += transform->getSize().y / 2;

				Vector2i tilePosition( chunkPositionInPixelX + tilePositionRelativeToChunkInPixelX,
									   chunkPositionInPixelY + tilePositionRelativeToChunkInPixelY );

				transform->setPosition( tilePosition );
				transform->setZValue( Constants::STANDART_TILE_ZVALUE );

				world.addEntity( move( tile ) );
			}
	}

	const size_t& ChunkGenerator::getRTTI_hash() const
	{
		return type_hash;
	}

	void ChunkGenerator::saveChangesToConfigFile( std::shared_ptr<ConfigFile>& configFile )
	{
		return;
	}

	void ChunkGenerator::init( Engine& engine, World& world, SaveManager& saveManager, std::shared_ptr<ConfigFile>& configFile )
	{
		return;
	}

	void ChunkGenerator::sfmlEvent( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent )
	{
		return;
	}

	void ChunkGenerator::update( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime )
	{
		return;
	}

	double ChunkGenerator::getUpdateImportance() const
	{
		return id::SystemUpdateImportance::CHUNK_GENERATOR;
	}

	const std::string& ChunkGenerator::getPluginName() const
	{
		return PLUGIN_NAME;
	}

	Plugin::Id ChunkGenerator::getPluginId() const
	{
		return id::SystemPluginId::CHUNK_GENERATOR;
	}

	const std::string ChunkGenerator::PLUGIN_NAME = "ChunkGenerator";

	const size_t ChunkGenerator::type_hash = getRuntimeTypeInfo<ChunkGenerator>();
}