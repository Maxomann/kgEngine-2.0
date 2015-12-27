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

		virtual void execute_main_internal() = 0;//run this in a thread

	public:
		ChunkIOOperation( Engine& engine,
						  World& world,
						  SaveManager& saveManager,
						  ChunkGeneratorSystem& chunkGenerator,
						  Chunk& chunkToOperateOn );

		Chunk& getChunkToOperateOn()const;

		virtual void execute_prepare() = 0;// set load state of chunk here to prevent ChunkSystem from adding invalid (doubled) operation
		void execute_main();// runs execute_main_internal in a thread
		virtual void execute_finish() = 0;

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

		virtual void execute_main_internal() override;

		virtual void execute_prepare() override;

		virtual void execute_finish() override;
	};

	class ChunkUnloadOperation : public ChunkIOOperation
	{
		std::vector<EntitySaveInformation> m_saveInformation;

	public:
		ChunkUnloadOperation( Engine& engine,
							  World& world,
							  SaveManager& saveManager,
							  Position2d chunkPosition,
							  Chunk& chunkToUnload );

		virtual void execute_main_internal() override;

		virtual void execute_prepare() override;

		virtual void execute_finish() override;
	};

	class ChunkSaveOperation : public ChunkIOOperation
	{
		std::vector<EntitySaveInformation> m_saveInformation;

	public:
		ChunkSaveOperation( Engine& engine,
							World& world,
							SaveManager& saveManager,
							Position2d chunkPosition,
							Chunk& chunkToSave );

		virtual void execute_main_internal() override;

		virtual void execute_prepare() override;

		virtual void execute_finish() override;
	};
}