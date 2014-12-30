#pragma once
#include "stdafx.h"
#include "id_internal.h"
#include "Entity.h"
#include "Callback.h"

namespace kg
{
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

	class DLL_EXPORT Saveable : public Component, public boost::noncopyable
	{
		const unsigned int m_blueprintEntityId;
		const Entity::Id m_uniqueEntityId;

	public:
		Saveable( const unsigned int& blueprintEntityId, const Entity::Id& uniqueEntityId );

		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues ) override;

		virtual void init( Engine& engine, ComponentManager& thisEntity ) override;

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime ) override;

		virtual double getUpdateImportance() const override;

		virtual std::vector<size_t> getRequieredComponents() const override;

		virtual const std::string& getPluginName() const override;

		virtual Plugin::Id getPluginId()const override;

		static const std::string PLUGIN_NAME;

		EntitySaveInformation writeSaveInformation()const;
		void loadSaveInformation( EntitySaveInformation& information );

		const Entity::Id& getUniqueId()const;

	signals:
		std::map < Plugin::Id,//first: componentId, second: signal
			boost::signals2::signal< std::vector<std::string>()> > s_writeSaveInformation;//first:id second: saveInformation

		std::map<Plugin::Id, Signal<const std::vector<std::string>&>> s_loadSaveInformation;//first: componentId, second: signal
	};
}
