#pragma once
#include "stdafx.h"
#include "PathNotAvailableException.h"
#include "Blueprint.h"

namespace kg
{
	DLL_EXPORT std::string aLoadFileToString( const std::string& path );

	//don't forget to DELETE char*
	DLL_EXPORT char* aLoadFileToCharPointer( const std::string& path );

	//File will be deleted before writing
	DLL_EXPORT void aSaveStringToFile( const std::string& path, const std::string& data );

	class ConfigFile
	{
		std::map<std::string, std::string> m_data;
		std::string m_path="ConfigFile -1";

	public:
		//save on close
		~ConfigFile();

		DLL_EXPORT bool loadFromFile( const std::string& path );

		DLL_EXPORT void saveToFile();//saves to the path it has been loaded from
		DLL_EXPORT void saveToFile( const std::string& path );
		DLL_EXPORT std::string toString()const;

		/*DLL_EXPORT const std::map<std::string, std::string>& getAllData(); - unused method*/
		DLL_EXPORT blueprint::Value getData( const std::string& identifier );
		//returns a reference to parameter: const std::string& data
		DLL_EXPORT const std::string& setData( const std::string& identifier, const std::string& data );
	};
}