#pragma once
#include "stdafx.h"
#include "Position.h"

namespace kg
{
	class Size : public Component
	{
		sf::Vector2i m_size;

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues );

		virtual void init( Engine& engine, ComponentManager& thisEntity );

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual std::vector<size_t> getRequieredComponents() const;//Position

		virtual const std::string& getPluginName() const;

		virtual Plugin::Id getPluginId()const;

		virtual void writeSaveInformation( EntitySaveInformation& writeTo ) override;

		virtual void loadSaveInformation( const EntitySaveInformation& loadFrom ) override;



		void set( const sf::Vector2i& size );
		sf::Vector2i get()const;

	signals:
		Signal<const sf::Vector2i&> s_changed;

		static const std::string BLUEPRINT_WIDTH;
		static const std::string BLUEPRINT_HEIGHT;

		static const std::string PLUGIN_NAME;
	};
}
