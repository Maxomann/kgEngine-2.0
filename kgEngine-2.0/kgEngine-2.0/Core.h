#pragma once
#include "stdafx.h"
#include "World.h"
#include "Engine.h"


namespace kg
{
	class Core
	{
		Engine m_engine;
		World m_world;

	public:
		void update();
		bool shouldTerminate()const;
	};
}