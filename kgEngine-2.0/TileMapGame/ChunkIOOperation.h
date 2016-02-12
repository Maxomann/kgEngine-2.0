#pragma once
#include "stdafx.h"
#include "ChunkGeneratorSystem.h"
#include "Position.h"
#include "Chunk.h"

namespace kg
{
	class ChunkIOOperation
	{
		std::future<void> m_future;

	protected:
		Engine& engine;
		World& world;
		SaveManager& saveManager;
		ChunkGeneratorSystem& chunkGenerator;

		Chunk& chunkToOperateOn;

		virtual void execute_init_internal() = 0;
		virtual void execute_main_internal() = 0;
		virtual void execute_finish_internal() = 0;
		virtual void execute_abort_internal() = 0;

	public:
		ChunkIOOperation( Engine& engine,
						  World& world,
						  SaveManager& saveManager,
						  ChunkGeneratorSystem& chunkGenerator,
						  Chunk& chunkToOperateOn );

		Chunk& getChunkToOperateOn()const;

		void execute_init();
		void execute_main();
		bool execute_finish_try();// returns true if successful
		void execute_finish();
		void execute_abort();

		bool isReadyToFinish()const;
	};

	class ChunkLoadOperation : public ChunkIOOperation
	{
		boost::optional<std::vector<EntitySaveInformation>> m_saveInformation;
		boost::optional<std::vector<Entity>> m_entities;
	public:
		ChunkLoadOperation( Engine& engine,
							World& world,
							SaveManager& saveManager,
							ChunkGeneratorSystem& chunkGenerator,
							Chunk& chunkToLoad );

		virtual void execute_init_internal() override;

		virtual void execute_main_internal() override;

		virtual void execute_finish_internal() override;
	};

	class ChunkUnloadOperation : public ChunkIOOperation
	{
		std::vector<EntitySaveInformation> m_saveInformation;

	public:
		ChunkUnloadOperation( Engine& engine,
							  World& world,
							  SaveManager& saveManager,
							  ChunkGeneratorSystem& chunkGenerator,
							  Chunk& chunkToUnload );

		virtual void execute_init_internal() override;

		virtual void execute_main_internal() override;

		virtual void execute_finish_internal() override;
	};

	class ChunkSaveOperation : public ChunkIOOperation
	{
		std::vector<EntitySaveInformation> m_saveInformation;

	public:
		ChunkSaveOperation( Engine& engine,
							World& world,
							SaveManager& saveManager,
							ChunkGeneratorSystem& chunkGenerator,
							Chunk& chunkToSave );

		virtual void execute_init_internal() override;

		virtual void execute_main_internal() override;

		virtual void execute_finish_internal() override;
	};
}