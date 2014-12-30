#pragma once
#include "stdafx.h"
#include "id.h"

namespace kg
{
	class Rotation : public Component
	{
		float m_rotationInDegree = 0;

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues );

		virtual void init( Engine& engine, ComponentManager& thisEntity );

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual std::vector<size_t> getRequieredComponents() const;

		virtual const std::string& getPluginName() const;

		virtual Plugin::Id getPluginId()const;

		virtual void writeSaveInformation( EntitySaveInformation& writeTo ) override;

		virtual void loadSaveInformation( const EntitySaveInformation& loadFrom ) override;



		void set( const float rotationInDegree );

		//in degree
		float get()const;

		void rotate( const float offsetInDegree );

	signals:
		Signal<const float&> s_changed;

		static const std::string PLUGIN_NAME;
	};
}
