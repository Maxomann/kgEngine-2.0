#pragma once
#include "stdafx.h"
#include "PathNotAvailableException.h"
#include "Blueprint.h"
#include "ResourceManager.h"

namespace kg
{
	class ConfigFile : public Resource
	{
		bool m_dataDiffersFromTextFile = false;

		std::map<std::string, std::string> m_data;
		std::string m_path;

		template<class T>
		DLL_EXPORT T getData( const std::string& identifier )const
		{
			auto& data = getData<std::string>( identifier );
			return boost::lexical_cast< T >(data);
		}
		template<>
		DLL_EXPORT std::string getData( const std::string& identifier )const
		{
			return m_data.at( identifier );
		}

	public:

		DLL_EXPORT bool loadFromFile( const std::string& path );

		DLL_EXPORT void saveToDefaultFile();//saves to the path it has been loaded from
		DLL_EXPORT void saveToFile( const std::string& path );
		DLL_EXPORT std::string toString()const;

		template<class T>
		DLL_EXPORT const T& setData( const std::string& identifier, const T& data )
		{
			m_dataDiffersFromTextFile = true;
			m_data[identifier] = boost::lexical_cast< std::string >(data);
			return data;
		}
		template<>
		DLL_EXPORT const std::string& setData( const std::string& identifier, const std::string& data )
		{
			m_dataDiffersFromTextFile = true;
			m_data[identifier] = data;
			return data;
		}

		DLL_EXPORT bool isDataValid( const std::string& identifier )const;

		template<class T>
		DLL_EXPORT T getDataReplaceIfInvalid( const std::string& identifier, const T& default_value )
		{
			if( isDataValid( identifier ) )
				return getData<T>( identifier );
			else
				return setData( identifier, default_value );
		}
	};
}
