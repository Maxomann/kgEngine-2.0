#pragma once
#include "stdafx.h"
#include "Plugin.h"

namespace kg
{
	struct Engine;
	class SystemManager;
	class World;
	class ConfigFile;

	class DLL_EXPORT SystemSaveInformation
	{
		int m_systemId;

		std::vector<std::string> m_information;

		void m_fromString( const std::string& str );

	public:
		SystemSaveInformation( const std::string& constructFromString );
		SystemSaveInformation( int systemId );

		const std::vector<std::string> getInformation()const;
		void setInformation( const std::vector<std::string>& information );
		bool hasInformation()const;

		int getSystemId()const;

		std::string toString()const;

	};

	class DLL_EXPORT System : public Plugin
	{
	public:
		// other systems are not guaranteed to be initialized
		// configFile is only used for values of this system
		virtual void init( Engine& engine, World& world, std::shared_ptr<ConfigFile>& configFile ) = 0;

		// sfml event loop is forwarded before update() is called
		virtual void sfmlEvent( Engine& engine, const sf::Event& sfEvent ) = 0;

		// components get updated before systems
		// frame time is 0 when Engine::isPaused == true
		virtual void update( Engine& engine, World& world, const sf::Time& frameTime ) = 0;

		// the lower the importance, the earlier this component gets updated, initialized, sfmlForwarded
		virtual double getUpdateImportance()const = 0;

		virtual void writeSaveInformation( SystemSaveInformation& writeTo ) = 0;

		virtual void loadSaveInformation( const SystemSaveInformation& loadFrom ) = 0;
	};
}
