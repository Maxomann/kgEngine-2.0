#pragma once
#include "stdafx.h"
#include "id.h"

namespace kg
{
	class Rotation : public Component, public CallbackReciever
	{
		float m_rotationInDegree = 0;

		void onLoadSaveInformation( const std::vector<std::string>& information );
		std::vector<std::string> onWriteSaveInformation();

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues );

		virtual void init( Engine& engine, ComponentManager& thisEntity );

		virtual void update( Engine& engine, World& world, ComponentManager& thisEntity, const sf::Time& frameTime );

		virtual double getUpdateImportance() const;

		virtual std::vector<size_t> getRequieredComponents() const;

		virtual const std::string& getPluginName() const;

		virtual Plugin::Id getPluginId()const;



		void set( const float rotationInDegree );

		//in degree
		float get()const;

		void rotate( const float offsetInDegree );

	signals:
		Signal<const float&> s_changed;

		static const std::string PLUGIN_NAME;
	};
}
