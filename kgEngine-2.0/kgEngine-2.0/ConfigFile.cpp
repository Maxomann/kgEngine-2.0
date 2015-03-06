#include "ConfigFile.h"
using namespace std;
using namespace sf;
using namespace kg::blueprint;

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
				bool afterFirstValueChar = false;
				for( const auto& el : line )
				{
					if( afterBreak )
					{
						if( !afterFirstValueChar && !std::isspace( el ) )
							afterFirstValueChar = true;
						if( afterFirstValueChar )
						{
							m_data[identifier].push_back( el );
						}
					}
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

	DLL_EXPORT std::string& ConfigFile::getData( const std::string& identifier )
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
			if( el.second != "" )
			{
				str += el.first;
				str += ": ";
				str += el.second;
				str += "\n";
			}
		}
		return str;
	}

	DLL_EXPORT const std::string& ConfigFile::setData( const std::string& identifier, const std::string& data )
	{
		m_data[identifier] = data;
		return data;
	}

	/*DLL_EXPORT const std::map<std::string, std::string>& ConfigFile::getAllData()
	{
	return m_data;
	}*/

	ConfigFile::~ConfigFile()
	{
		if( m_path != "ConfigFile -1" )
			saveToFile();
	}
}
