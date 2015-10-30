#pragma once
#include "stdafx.h"
#include "System.h"
#include "ConfigFile.h"
#include "ResourceManager.h"
#include "Engine.h"

namespace kg
{
	struct Engine;
	class SaveManager;

	class DLL_EXPORT SystemManager : boost::noncopyable
	{
		std::map<size_t, System*> m_systemsByType;
		std::map<double, std::vector<std::unique_ptr<System>>> m_systemsByUpdateImportance;
	public:

		//passed reference will be nullptr after function call!
		void addSystem( std::unique_ptr<System>&& system );

		void initSystemsByImportance( Engine& engine, World& world, SaveManager& saveManager );

		template<class T>
		T* getSystem()
		{
			return static_cast< T* >(m_systemsByType.at( T::type_hash ));
		};

		void saveSystemConfigFilesByImportance( Engine& engine );

		void forwardSfmlEventByImportance( Engine& engine, World& world, SaveManager& saveManager, const sf::Event& sfEvent );
		void updateAllSystemsByImportance( Engine& engine, World& world, SaveManager& saveManager, const sf::Time& frameTime );
	};
}
