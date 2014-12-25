#pragma once
#include "stdafx.h"
#include "ComponentManager.h"
#include "Blueprint.h"
#include "Engine.h"
#include "ComponentMissingException.h"

namespace kg
{
	class EntitySaveInformation;

	class DLL_EXPORT Entity : public ComponentManager
	{
	public:
		typedef unsigned long long Id;

	private:

		const Entity::Id m_id;
		blueprint::ComponentValuesByNameByComponentMap m_additionalComponentValues;

	public:
		Entity( const Id& id );

		void initFromBlueprint( Engine& engine,
								const blueprint::Entity& entity,
								const blueprint::ComponentValuesByNameByComponentMap& additionalBlueprintValues
								= blueprint::ComponentValuesByNameByComponentMap() );//componentValuesByNameByComponent

		const blueprint::ComponentValuesByNameByComponentMap& getAdditionalComponentValues()const;

		const Id& getId()const;

		void writeSaveInformation( EntitySaveInformation& writeTo );
		void loadSaveInformation( EntitySaveInformation& loadFrom );
	};

	class DLL_EXPORT EntitySaveInformation
	{
		using InformationByComponentIdMap = std::map < int, std::vector<std::string> > ;

		unsigned int m_blueprintEntityId;
		Entity::Id m_uniqueEntityId;

		InformationByComponentIdMap m_information;

		int m_activeComponentId = -1;

		void m_fromString( const std::string& str );

	public:
		EntitySaveInformation( const std::string& constructFromString );
		EntitySaveInformation( unsigned int blueprintEntityId, Entity::Id uniqueEntityId );

		int getActiveComponentId()const;
		void setActiveComponentId( int id );

		const std::vector<std::string>& getInformation()const;

		//for active componentId
		void addInformation( const std::vector<std::string>& information );

		unsigned int getBlueprintEntityId()const;
		Entity::Id getUniqueEntityId()const;

		std::string toString()const;
	};
}
