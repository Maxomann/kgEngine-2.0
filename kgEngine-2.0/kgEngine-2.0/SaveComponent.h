#pragma once
#include "stdafx.h"
#include "id_internal.h"
#include "Entity.h"
#include "Callback.h"
#include "Blueprint.h"

namespace kg
{
	class DLL_EXPORT EntitySaveInformation
	{
		using InformationByComponentIdMap = std::map < int, std::vector<std::string> >;

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

	class DLL_EXPORT SystemSaveInformation
	{
		int m_systemPluginId;

		std::vector<std::string> m_information;

		void m_fromString( const std::string& str );

	public:
		SystemSaveInformation( const std::string& constructFromString );
		SystemSaveInformation( int systemPluginId );

		const std::vector<std::string> getInformation()const;
		void setInformation( const std::vector<std::string>& information );
		bool hasInformation()const;

		int getSystemPluginId()const;

		std::string toString()const;
	};

	class DLL_EXPORT Save : public Component, public boost::noncopyable
	{
		const blueprint::Entity& m_blueprint;

		const blueprint::Entity::Id m_blueprintEntityId;
		const Entity::Id m_uniqueEntityId;

	public:
		Save( const blueprint::Entity::Id& blueprintEntityId, const blueprint::Entity& blueprint, const Entity::Id& uniqueEntityId );

		virtual void preInit( Engine& engine,
							  const std::map<blueprint::ComponentValue::Name, const blueprint::ComponentValue*>& blueprintValues ) override;

		virtual void init( Engine& engine, const World& world, const ComponentManager& thisEntity ) override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime ) override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<Plugin::Id> getRequieredComponents()const override;

		virtual const Plugin::Name& getPluginName() const override;

		virtual Plugin::Id getPluginId()const override;

		virtual const size_t& getRTTI_hash() const override;

		static const std::string PLUGIN_NAME;

		static const size_t type_hash;

		EntitySaveInformation writeSaveInformation()const;
		void loadSaveInformation( EntitySaveInformation& information );

		const Entity::Id& getUniqueId()const;
		const blueprint::Entity::Id& getBlueprintId()const;

		const blueprint::Entity& getBlueprint()const;

	signals:
		std::map < Plugin::Id, ReturningSignal< std::vector<std::string> > > s_writeSaveInformation;//first:id second: saveInformation

		std::map<Plugin::Id, Signal<const std::vector<std::string>&>> s_loadSaveInformation;//first: componentId, second: signal
	};

	__declspec(selectany) const size_t Save::type_hash = getRuntimeTypeInfo<Save>();

	//void onLoadSaveInformation( const std::vector<std::string>& information )
	//std::vector<std::string> onWriteSaveInformation( )
}
