#pragma once
#include"stdafx.h"
#include "PathNotAvailableException.h"

namespace kg
{
	/**********************************************************************************************//**
	 * Reads file to vector.
	 *
	 * @author	Kay
	 * @date	07.02.2015
	 *
	 * @param	path	Full pathname of the file.
	 *
	 * @return	The file to vector.
	 **************************************************************************************************/

	std::vector<std::string> readFileToVector( const std::string& path );

	/**********************************************************************************************//**
	 * A helper class for creating classes that are loadable through ResourceManager.
	 *
	 * @author	Kay
	 * @date	26.01.2015
	 **************************************************************************************************/

	class DLL_EXPORT Resource
	{
	public:
		virtual bool loadFromFile( const std::string& path ) = 0;
	};

	/**********************************************************************************************//**
	 * Manager for resources.
	 *
	 * @author	Kay
	 * @date	26.01.2015
	 **************************************************************************************************/

	class DLL_EXPORT ResourceManager
	{
	private:
		/// the loaded resources.
		std::unordered_map< std::string, std::unordered_map<size_t, std::shared_ptr<void>> >m_resources;

		/**********************************************************************************************//**
		 * Template argument is the type, NOT A POINTER TO IT!!!
		 * getResource<sf::Texture>;  == OK
		 * getResource<sf::Texture*>; == NOT WORKING
		 * 
		 * class T must have a function like this: void bool T::loadFromFile(const std::string&amp; path)
		 * YOU CAN INHERIT Resource FOR THAT.
		 *
		 * @author	Kay
		 * @date	26.01.2015
		 *
		 * @tparam	T	Generic type parameter.
		 * @param	path	Full pathname of the file.
		 *
		 * @return	The resource.
		 **************************************************************************************************/

		template< class T >
		std::shared_ptr<T> getResource( const std::string& path )
		{
			const size_t typeID = typeid(T).hash_code();

			auto it = m_resources.find( path );

			//wenn Path nicht vorhanden
			if( it == m_resources.end() )
			{
				auto res = std::make_shared<T>();
				//neues element und signatur einfügen
				m_resources[path][typeID] = std::static_pointer_cast< void >(res);
				if( !res->loadFromFile( path ) )
					new PathNotAvailableException( path );
				return res;
			}
			//wenn Path vorhanden
			else
			{
				//nach Typ suchen
				auto secondIt = it->second.find( typeID );
				//wenn Path aber NICHT Typ vorhanden
				if( secondIt == it->second.end() )
				{
					auto res = std::make_shared<T>();
					//neuen Typ (kein neues Path) einfügen
					it->second[typeID] = std::static_pointer_cast< void >(res);
					if( !res->loadFromFile( path ) )
						new PathNotAvailableException( path );
					return res;
				}
				//wenn Path und Typ vorhanden
				else
				{
					return std::static_pointer_cast< T >(secondIt->second);
				}
			}
		}

		/**********************************************************************************************//**
		 * same as getResource but loadFromFile will be called in every case.
		 *
		 * @author	Kay
		 * @date	26.01.2015
		 *
		 * @tparam	T	Generic type parameter.
		 * @param	path	Full pathname of the file.
		 *
		 * @return	The resource;
		 **************************************************************************************************/

		template< class T >
		std::shared_ptr<T> reloadResource( const std::string& path )
		{
			const size_t typeID = typeid(T).hash_code();

			auto it = m_resources.find( path );

			//wenn Path nicht vorhanden
			if( it == m_resources.end() )
			{
				auto res = std::make_shared<T>();
				//neues element und signatur einfügen
				m_resources[path][typeID] = std::static_pointer_cast< void >(res);
				if( !res->loadFromFile( path ) )
					new PathNotAvailableException( path );
				return res;
			}
			//wenn Path vorhanden
			else
			{
				//nach Typ suchen
				auto secondIt = it->second.find( typeID );
				//wenn Path aber NICHT Typ vorhanden
				if( secondIt == it->second.end() )
				{
					auto res = std::make_shared<T>();
					//neuen Typ (kein neues Path) einfügen
					it->second[typeID] = std::static_pointer_cast< void >(res);
					if( !res->loadFromFile( path ) )
						new PathNotAvailableException( path );
					return res;
				}
				//wenn Path und Typ vorhanden
				else
				{
					auto obj = std::static_pointer_cast< T >(secondIt->second);
					obj->loadFromFile( path );
					return obj;
				}
			}
		}

	public:

		/**********************************************************************************************//**
		 * Template argument is the type, NOT A POINTER TO IT!!!
		 * getResource<sf::Texture>;  == OK
		 * getResource<sf::Texture*>; == NOT WORKING
		 * 
		 * class T must have a function like this: void bool T::loadFromFile(const std::string&amp; path)
		 * YOU CAN INHERIT Resource FOR THAT
		 * 
		 * resourcePath is relative to the packages 'Resource' folder.
		 *
		 * @author	Kay
		 * @date	26.01.2015
		 *
		 * @tparam	T	Generic type parameter.
		 * @param	packageName 	Name of the package.
		 * @param	resourcePath	Path realtive to the Resource folder of the packageName.
		 *
		 * @return	The resource.
		 **************************************************************************************************/

		template< class T >
		std::shared_ptr<T> getResource( const std::string& packageName, const std::string& resourcePath )
		{
			return getResource<T>( "./Packages/" + packageName + "/Resource/" + resourcePath );
		}

		/**********************************************************************************************//**
		 * same as getResource but loadFromFile will be called in every case
		 * 
		 * resourcePath is relative to the packages 'Resource' folder.
		 *
		 * @author	Kay
		 * @date	26.01.2015
		 *
		 * @tparam	T	Generic type parameter.
		 * @param	packageName 	Name of the package.
		 * @param	resourcePath	Path realtive to the Resource folder of the packageName.
		 *
		 * @return	The resource;
		 **************************************************************************************************/

		template< class T >
		std::shared_ptr<T> reloadResource( const std::string& packageName, const std::string& resourcePath )
		{
			return reloadResource<T>( "./Packages/" + packageName + "/Resource/" + resourcePath );
		}

		/**********************************************************************************************//**
		 * Gets configuration file.
		 *
		 * @author	Kay
		 * @date	26.01.2015
		 *
		 * @tparam	T	Generic type parameter.
		 * @param	systemName	Name of the system.
		 *
		 * @return	The configuration file.
		 **************************************************************************************************/

		template< class T >
		std::shared_ptr<T> getConfigFile( const std::string& systemName )
		{
			return getResource<T>( "./Config/" + systemName + ".ini" );
		}
	};

	/**********************************************************************************************//**
	 * Reads a file to vector<string>;.
	 *
	 * @author	Kay
	 * @date	07.02.2015
	 *
	 * @param	path	Full pathname of the file.
	 *
	 * @return	A vector where each line of the file is an element.
	 **************************************************************************************************/

	std::vector<std::string> readFileToVector( const std::string& path );

}
