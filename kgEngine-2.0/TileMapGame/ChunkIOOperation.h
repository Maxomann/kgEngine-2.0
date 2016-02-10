#pragma once
#include "stdafx.h"
#include "ChunkGeneratorSystem.h"
#include "Position.h"
#include "Chunk.h"

namespace kg
{
	class ChunkIOOperation
	{
	protected:
		Engine& engine;
		World& world;
		SaveManager& saveManager;
		ChunkGeneratorSystem& chunkGenerator;

		Chunk& chunkToOperateOn;

	public:
		ChunkIOOperation( Engine& engine,
						  World& world,
						  SaveManager& saveManager,
						  ChunkGeneratorSystem& chunkGenerator,
						  Chunk& chunkToOperateOn );

		Chunk& getChunkToOperateOn()const;

		virtual void execute() = 0;
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

		virtual void execute() override;
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

		virtual void execute() override;
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

		virtual void execute() override;
	};
}