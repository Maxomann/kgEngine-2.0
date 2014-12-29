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
		const unsigned int m_blueprintId;
		const bool m_isConstructedFromBlueprint;

		void initFromBlueprint( Engine& engine,
								const blueprint::Entity& entity );//componentValuesByNameByComponent
		void loadSaveInformation( EntitySaveInformation& loadFrom );

	public:
		// Entity has to be initialized manually when this constructor is called
		// This Entity will not be saved. (To be saved it needs to be constructed from a blueprint)
		Entity( const Id& id );
		Entity( Engine& engine,
				const Id& id,
				const blueprint::Entity& entity );
		Entity( Engine& engine,
				const Id& id,
				const blueprint::Entity& entity,
				EntitySaveInformation& loadFrom );


		const Id& getId()const;
		//entities can only be saved if this returns true
		bool isConstructedFromBlueprint()const;
		//check isConstructedFromBlueprint() before. if false, this value can be ignored
		unsigned int getBlueprintId()const;

		void writeSaveInformation( EntitySaveInformation& writeTo );
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
