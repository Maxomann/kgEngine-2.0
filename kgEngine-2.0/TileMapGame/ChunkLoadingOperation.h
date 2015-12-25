#pragma once
#include "stdafx.h"
#include "ChunkGeneratorSystem.h"
#include "ChunkPosition.h"
#include "Chunk.h"

namespace kg
{
	class ChunkLoadingOperation
	{
		Engine& engine;
		World& world;
		SaveManager& saveManager;
		ChunkGeneratorSystem& chunkGenerator;

		Chunk& chunkToLoad;

		bool m_isReadyToFinish = false;

		boost::optional<std::vector<EntitySaveInformation>> m_saveInformation;
		boost::optional<std::vector<Entity>> m_entities;
	public:
		ChunkLoadingOperation( Engine& engine, World& world, SaveManager& saveManager, ChunkGeneratorSystem& chunkGenerator, Chunk& chunkToLoad );

		Chunk& getChunkToLoad()const;

		void execute_main();//run this in a thread
		std::vector<Entity> execute_finish();//DO NOT run this in a thread

		bool isReadyToFinish()const;
	};

	class ChunkUnloadingOperation
	{
		Engine& engine;
		World& world;
		SaveManager& saveManager;
		ChunkGeneratorSystem& chunkGenerator;

		Chunk& chunkToLoad;

		bool m_isComplete = false;
	public:
		ChunkUnloadingOperation( Engine& engine, World& world, SaveManager& saveManager, ChunkPosition chunkPosition, std::string& chunkSavename, Chunk& chunkToUnload );

		Chunk& getChunkToLoad()const;

		void execute_prepare();//DO NOT run this in a thread
		void execute_main();//run this in a thread

		bool isComplete()const;
	};
}