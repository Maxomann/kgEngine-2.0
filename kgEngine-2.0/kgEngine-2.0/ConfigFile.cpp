#include "ConfigFile.h"
using namespace std;
using namespace sf;

namespace kg
{

	DLL_EXPORT bool ConfigFile::loadFromFile( const std::string& path )
	{
		m_data.clear();
		m_path = path;

		std::ifstream file;
		file.open( path );
		if( !file.is_open() )
		{
			/* throw PathNotAvailableException( path ); do not throw exception here since the file may be created later */
		}
		else
		{

			std::string line;

			while( std::getline( file, line ) )
			{
				std::string identifier;
				bool afterBreak = false; //afer ':'
				for( const auto& el : line )
				{
					if( afterBreak )
						m_data[identifier].push_back( el );
					else if( el == ':' )
						afterBreak = true;
					else
						identifier.push_back( el );
				}
			}
			file.close();
		}
		return true;
	}

	DLL_EXPORT const std::string& ConfigFile::getData( const std::string& identifier )
	{
		return m_data[identifier];
	}

	DLL_EXPORT void ConfigFile::saveToFile()
	{
		saveToFile( m_path );
	}

	DLL_EXPORT void ConfigFile::saveToFile( const std::string& path )
	{
		std::fstream file( path, fstream::out | fstream::trunc );
		if( !file.is_open() )
			throw PathNotAvailableException( path );

		file << toString();

		file.close();
	}

	DLL_EXPORT std::string ConfigFile::toString() const
	{
		std::string str;
		for( const auto& el : m_data )
		{
			str += el.first;
			str += ":";
			str += el.second;
			str += "\n";
		}
		return str;
	}

	DLL_EXPORT void ConfigFile::setData( const std::string& identifier, const std::string& data )
	{
		m_data[identifier] = data;
	}

	DLL_EXPORT const std::map<std::string, std::string>& ConfigFile::getAllData()
	{
		return m_data;
	}

	ConfigFile::~ConfigFile()
	{
		if( m_path != "ConfigFile -1" )
			saveToFile();
	}

	DLL_EXPORT std::string aLoadFileToString( const std::string& path )
	{
		std::string returnValue;

		std::ifstream file;
		file.open( path );
		if( !file.is_open() )
			throw PathNotAvailableException( path );

		std::string line;

		while( std::getline( file, line ) )
			returnValue += line;

		file.close();

		return returnValue;
	}

	DLL_EXPORT void aSaveStringToFile( const std::string& path, const std::string& data )
	{
		std::ofstream file;
		file.open( path, std::ios::trunc );
		if( !file.is_open() )
			throw PathNotAvailableException( path );

		file << data;
		file.close();
		return;
	}

	DLL_EXPORT char* aLoadFileToCharPointer( const std::string& path )
	{
		auto str = aLoadFileToString( path );

		char* file = new char[str.length() + 1];
		strcpy( file, str.c_str() );

		return file;
	}

}
