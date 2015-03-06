#pragma once
#include "stdafx.h"
#include "PathNotAvailableException.h"
#include "Blueprint.h"
#include "ResourceManager.h"

namespace kg
{
	class ConfigFile : public Resource
	{
		std::map<std::string, std::string> m_data;
		std::string m_path = "ConfigFile -1";

	public:
		//save on close
		~ConfigFile();

		DLL_EXPORT bool loadFromFile( const std::string& path );

		DLL_EXPORT void saveToFile();//saves to the path it has been loaded from
		DLL_EXPORT void saveToFile( const std::string& path );
		DLL_EXPORT std::string toString()const;

		/*DLL_EXPORT const std::map<std::string, std::string>& getAllData(); - unused method*/
		DLL_EXPORT std::string& getData( const std::string& identifier );
		//returns a reference to parameter: const std::string& data
		DLL_EXPORT const std::string& setData( const std::string& identifier, const std::string& data );
	};
}
