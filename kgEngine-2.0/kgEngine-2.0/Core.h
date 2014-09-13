#pragma once
#include "stdafx.h"
#include "World.h"
#include "Engine.h"


namespace kg
{
	class Core
	{
		Engine m_engine;
		World m_client;
private:
		World m_world;


	public:
		void init();

		void update();
		bool shouldTerminate()const;
	};
}