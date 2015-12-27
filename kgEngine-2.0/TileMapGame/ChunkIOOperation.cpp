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

	void ChunkIOOperation::execute_main()
	{
		m_future = async( launch::async, bind( &ChunkIOOperation::execute_main_internal, this ) );
	}

	bool ChunkIOOperation::isReadyToFinish() const
	{
		return m_future._Is_ready();
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

	void ChunkLoadOperation::execute_main_internal()
	{
		m_saveInformation = saveManager.loadEntitySaveInformationFromFile( chunkToOperateOn.getSavename() );

		if( !m_saveInformation )
		{
			m_entities = chunkGenerator.generateChunk( engine, world, chunkToOperateOn.getPosition() );
		}
	}

	void ChunkLoadOperation::execute_prepare()
	{
		chunkToOperateOn.setLoadState( true );
	}

	void ChunkLoadOperation::execute_finish()
	{
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
												Position2d chunkPosition,
												Chunk& chunkToUnload )
		: ChunkIOOperation( engine,
							world,
							saveManager,
							chunkGenerator,
							chunkToUnload )
	{ }

	void ChunkUnloadOperation::execute_main_internal()
	{
		saveManager.saveEntitySaveInformationToFile( chunkToOperateOn.getSavename(), m_saveInformation );
	}

	void ChunkUnloadOperation::execute_prepare()
	{
		auto& entities = chunkToOperateOn.getEntities();

		m_saveInformation = saveManager.generateSaveInformationFromEntities( entities );
		world.removeEntities( entities );
		chunkToOperateOn.setLoadState( false );
	}

	void ChunkUnloadOperation::execute_finish()
	{ }

	ChunkSaveOperation::ChunkSaveOperation( Engine& engine,
											World& world,
											SaveManager& saveManager,
											Position2d chunkPosition,
											Chunk& chunkToSave )
		: ChunkIOOperation( engine,
							world,
							saveManager,
							chunkGenerator,
							chunkToSave )
	{ }

	void ChunkSaveOperation::execute_main_internal()
	{
		saveManager.saveEntitySaveInformationToFile( chunkToOperateOn.getSavename(), m_saveInformation );
	}

	void ChunkSaveOperation::execute_prepare()
	{
		m_saveInformation = saveManager.generateSaveInformationFromEntities( chunkToOperateOn.getEntities() );
	}

	void ChunkSaveOperation::execute_finish()
	{ }
}