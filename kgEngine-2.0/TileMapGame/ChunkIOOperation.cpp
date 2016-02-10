#include "ChunkIOOperation.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	ChunkIOOperation::ChunkIOOperation( Engine& engine,
										World& world,
										SaveManager& saveManager,
										ChunkGeneratorSystem& chunkGenerator,
										Chunk& chunkToOperateOn )
		: engine( engine ),
		world( world ),
		saveManager( saveManager ),
		chunkGenerator( chunkGenerator ),
		chunkToOperateOn( chunkToOperateOn )
	{ }

	Chunk& ChunkIOOperation::getChunkToOperateOn() const
	{
		return chunkToOperateOn;
	}

	ChunkLoadOperation::ChunkLoadOperation( Engine& engine,
											World& world,
											SaveManager& saveManager,
											ChunkGeneratorSystem& chunkGenerator,
											Chunk& chunkToLoad )
		: ChunkIOOperation( engine,
							world,
							saveManager,
							chunkGenerator,
							chunkToLoad )
	{ }

	void ChunkLoadOperation::execute()
	{
		chunkToOperateOn.setLoadState( true );

		m_saveInformation = saveManager.loadEntitySaveInformationFromFile( chunkToOperateOn.getSavename() );

		if( !m_saveInformation )
		{
			m_entities = chunkGenerator.generateChunk( engine, world, chunkToOperateOn.getPosition() );
		}

		if( m_saveInformation )
			world.addEntities( saveManager.generateEntitiesFromSaveInformation( engine, world, *m_saveInformation ) );
		else if( m_entities )
			world.addEntities( move( *m_entities ) );
		else
			throw exception();
	}

	ChunkUnloadOperation::ChunkUnloadOperation( Engine& engine,
												World& world,
												SaveManager& saveManager,
												ChunkGeneratorSystem& chunkGenerator,
												Chunk& chunkToUnload )
		: ChunkIOOperation( engine,
							world,
							saveManager,
							chunkGenerator,
							chunkToUnload )
	{ }

	void ChunkUnloadOperation::execute()
	{
		auto entities = chunkToOperateOn.getEntities();

		m_saveInformation = saveManager.generateSaveInformationFromEntities( entities );
		world.removeEntities( entities );
		chunkToOperateOn.setLoadState( false );

		saveManager.saveEntitySaveInformationToFile( chunkToOperateOn.getSavename(), m_saveInformation );
	}

	ChunkSaveOperation::ChunkSaveOperation( Engine& engine,
											World& world,
											SaveManager& saveManager,
											ChunkGeneratorSystem& chunkGenerator,
											Chunk& chunkToSave )
		: ChunkIOOperation( engine,
							world,
							saveManager,
							chunkGenerator,
							chunkToSave )
	{ }

	void ChunkSaveOperation::execute()
	{
		m_saveInformation = saveManager.generateSaveInformationFromEntities( chunkToOperateOn.getEntities() );
		saveManager.saveEntitySaveInformationToFile( chunkToOperateOn.getSavename(), m_saveInformation );
	}
}