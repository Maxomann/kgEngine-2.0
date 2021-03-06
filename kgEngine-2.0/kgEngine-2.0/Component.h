#pragma once
#include "stdafx.h"
#include "Plugin.h"
#include "Blueprint.h"

namespace kg
{
	struct Engine;
	class ComponentManager;
	class World;
	class EntitySaveInformation;

	class DLL_EXPORT Component : public PluginRTTI, public sf::NonCopyable
	{
	public:
		virtual ~Component()
		{ };

		// get values from blueprint Entity
		// will NOT BE CALLED, if this entity is temporary
		virtual void preInit( Engine& engine,
							  const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues ) = 0;

		// it is guaranteed that all components of the entity are loaded at this point
		// their init function is not guaranteed to have been called
		virtual void init( Engine& engine, const World& world, const ComponentManager& thisEntity ) = 0;

		// components get updated before systems
		// frame time is 0 when isPaused = true
		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime ) = 0;

		//the lower the importance, the earlier this component gets updated
		virtual double getUpdateImportance()const = 0;

		virtual std::vector<Plugin::Id> getRequieredComponents()const = 0;
	};
}
