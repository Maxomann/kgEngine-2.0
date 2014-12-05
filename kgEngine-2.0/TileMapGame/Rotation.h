#pragma once
#include "stdafx.h"
#include "id.h"

namespace kg
{
	class Rotation : public Component, public CallbackSender
	{
		float m_rotationInDegree = 0;

	public:
		virtual void preInit( Engine& engine, const std::map<std::string, blueprint::Value>& blueprintValues );

		virtual void init( Engine& engine, ComponentManager& componentManager );

		virtual void update( Engine& engine, World& world, ComponentManager& componentManager );

		virtual double getUpdateImportance() const;

		virtual std::vector<size_t> getRequieredComponents() const;

		virtual const std::string& getPluginName() const;

		virtual int getPluginId() const;

		void set( const float rotationInDegree );

		//in degree
		float get()const;

		void rotate( const float offsetInDegree );

		enum class CallbackId : int
		{
			CHANGED//T: const float&
		};

		static const std::string PLUGIN_NAME;
	};
}
