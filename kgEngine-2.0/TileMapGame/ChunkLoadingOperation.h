#pragma once
#include "stdafx.h"
#include "ChunkGeneratorSystem.h"

namespace kg
{
	class ChunkLoadingOperation
	{
		SaveManager& r_saveManager;
		ChunkGeneratorSystem& r_chunkGenerator;

		sf::Vector3i m_chunkPosition;

		bool m_isComplete = false;
	public:
		ChunkLoadingOperation( sf::Vector3i chunkPosition, SaveManager& saveManager, ChunkGeneratorSystem& chunkGenerator );

		sf::Vector3i getChunkPosition()const;

		std::vector<Entity> execute();//run this in a thread

		bool isComplete()const;

		std::vector<Entity> getResult();
	};

	class ChunkUnloadingOperation
	{
		SaveManager& r_saveManager;
		ChunkGeneratorSystem& r_chunkGenerator;

		sf::Vector3i m_chunkPosition;

		bool m_isComplete = false;
	public:
		ChunkUnloadingOperation( sf::Vector3i chunkPosition, SaveManager& saveManager, ChunkGeneratorSystem& chunkGenerator );

		sf::Vector3i getChunkPosition()const;

		void execute( World& world );//run this in a thread

		bool isComplete()const;
	};
}