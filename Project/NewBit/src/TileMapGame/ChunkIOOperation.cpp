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

	void ChunkIOOperation::execute_init()
	{
		execute_init_internal();
	}

	void ChunkIOOperation::execute_main()
	{
		m_future = async( launch::async, bind( &ChunkIOOperation::execute_main_internal, this ) );
	}

	bool ChunkIOOperation::execute_finish_try()
	{
		if( isReadyToFinish() )
		{
			execute_finish();
			return true;
		}
		else
			return false;
	}

	void ChunkIOOperation::execute_finish()
	{
		m_future.wait();
		execute_finish_internal();
	}

	void ChunkIOOperation::execute_abort()
	{
		if( m_future.valid() )
			m_future.wait();
		execute_abort_internal();
	}

	bool ChunkIOOperation::isReadyToFinish() const
	{
		return m_future.wait_for( chrono::milliseconds( 0 ) ) == future_status::ready;
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

	void ChunkLoadOperation::execute_init_internal()
	{
		if( chunkToOperateOn.getState() != Chunk::State::UNLOADED )
			throw exception();
		chunkToOperateOn.setState( Chunk::State::LOADING );
	}

	void ChunkLoadOperation::execute_main_internal()
	{
		m_saveInformation = saveManager.loadEntitySaveInformationFromFile( chunkToOperateOn.getSavename() );
		if( !m_saveInformation )
		{
			m_entities = chunkGenerator.generateChunk( engine, world, chunkToOperateOn.getPosition() );
		}
	}

	void ChunkLoadOperation::execute_finish_internal()
	{
		if( m_saveInformation )
			world.addEntities( saveManager.generateEntitiesFromSaveInformation( engine, world, *m_saveInformation ) );
		else if( m_entities )
		{
			auto temp = move( *m_entities );
			world.addEntities( move( temp ) );
		}
		else
			throw exception();

		chunkToOperateOn.setState( Chunk::State::LOADED );
	}

	void ChunkLoadOperation::execute_abort_internal()
	{
		chunkToOperateOn.setState( Chunk::State::UNLOADED );
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

	void ChunkUnloadOperation::execute_init_internal()
	{
		if( chunkToOperateOn.getState() != Chunk::State::LOADED )
			throw exception();

		m_saveInformation = saveManager.generateSaveInformationFromEntities( chunkToOperateOn.getEntities() );

		chunkToOperateOn.setState( Chunk::State::UNLOADING );
	}

	void ChunkUnloadOperation::execute_main_internal()
	{
		saveManager.saveEntitySaveInformationToFile( chunkToOperateOn.getSavename(), m_saveInformation );
	}

	void ChunkUnloadOperation::execute_finish_internal()
	{
		auto entities = chunkToOperateOn.getEntities();
		m_saveInformation = saveManager.generateSaveInformationFromEntities( entities );
		world.removeEntities( entities );

		chunkToOperateOn.setState( Chunk::State::UNLOADED );
	}

	void ChunkUnloadOperation::execute_abort_internal()
	{
		chunkToOperateOn.setState( Chunk::State::LOADED );
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

	void ChunkSaveOperation::execute_init_internal()
	{
		if( chunkToOperateOn.getState() != Chunk::State::LOADED )
			throw exception();
		m_saveInformation = saveManager.generateSaveInformationFromEntities( chunkToOperateOn.getEntities() );
	}

	void ChunkSaveOperation::execute_main_internal()
	{
		saveManager.saveEntitySaveInformationToFile( chunkToOperateOn.getSavename(), m_saveInformation );
	}

	void ChunkSaveOperation::execute_finish_internal()
	{ }

	void ChunkSaveOperation::execute_abort_internal()
	{
		return;
	}
}