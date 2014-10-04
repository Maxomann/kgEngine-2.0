#pragma once
#include "stdafx.h"
#include "World.h"
#include "Engine.h"

namespace kg
{
	class Foo : public kg::System
	{
	public:

		virtual void init( Engine& engine, SystemManager& systemManager )
		{
			std::cout << "init" << std::endl;
		}

		virtual void update( Engine& engine, World& world )
		{
			std::cout << "update" << std::endl;
		}

		virtual double getUpdateImportance() const
		{
			return 1.0;
		}

	};

	class Core
	{
		Engine m_engine;
		World m_client;


	public:
		void init();

		void update();
		bool shouldTerminate()const;
	};
}
