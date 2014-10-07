#pragma once
#include "stdafx.h"
#include "System.h"
#include "_id.h"
#include "TestComponents.h"
#include "World.h"

namespace kg
{
	class CameraSystem : public System
	{
	public:
		virtual void init( Engine& engine, SystemManager& systemManager )
		{
			return;
		}

		virtual void update( Engine& engine, World& world )
		{
			auto toDraw = world.getEntitiesThatHaveComponent<SpriteComponent>();
			for( const auto& el : toDraw )
				engine.renderWindow.draw( *(el->getComponent<SpriteComponent>() ));
		}

		virtual double getUpdateImportance() const
		{
			return SystemUpdateImportance::CameraSystem;
		}

	};
}
