#include "ConfigFile.h"
using namespace std;
using namespace sf;
using namespace kg::blueprint;

namespace kg
{
	DLL_EXPORT bool ConfigFile::loadFromFile( const std::string& path )
	{
		m_dataDiffersFromTextFile = false;
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
				bool afterBreak = false; //after ':'
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

	DLL_EXPORT void ConfigFile::saveToDefaultFile()
	{
		if( m_path.size() )
		{
			if( m_dataDiffersFromTextFile )
				saveToFile( m_path );
		}
		else
			throw exception();
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

	bool ConfigFile::isDataValid( const std::string& identifier ) const
	{
		auto it = m_data.find( identifier );
		if( it != m_data.end() )
			return (it->first.size() != 0);
		else
			return false;
	}
}