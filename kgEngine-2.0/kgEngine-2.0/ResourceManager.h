#pragma once
#include"stdafx.h"
#include "PathNotAvailableException.h"

namespace kg
{
	class DLL_EXPORT Resource
	{
	public:
		virtual bool loadFromFile( const std::string& path ) = 0;
	};

	class DLL_EXPORT ResourceManager
	{
	private:
		std::unordered_map< std::string, std::unordered_map<size_t, std::shared_ptr<void>> >m_resources;

		// Template argument is the type, NOT A POINTER TO IT!!!
		// getResource<sf::Texture>  == OK
		// getResource<sf::Texture*> == NOT WORKING
		//
		// class T must have a function like this:
		// void bool T::loadFromFile(const std::string& path)
		// YOU CAN INHERIT Resource FOR THAT
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

		// same as getResource but loadFromFile will be called in every case
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

		// Template argument is the type, NOT A POINTER TO IT!!!
		// getResource<sf::Texture>  == OK
		// getResource<sf::Texture*> == NOT WORKING
		//
		// class T must have a function like this:
		// void bool T::loadFromFile(const std::string& path)
		// YOU CAN INHERIT Resource FOR THAT
		//
		// resourcePath is relative to the packages 'Resource' folder
		template< class T >
		std::shared_ptr<T> getResource( const std::string& packageName, const std::string& resourcePath )
		{
			return getResource<T>( "./Packages/" + packageName + "/Resource/" + resourcePath );
		}

		// same as getResource but loadFromFile will be called in every case
		//
		// resourcePath is relative to the packages 'Resource' folder
		template< class T >
		std::shared_ptr<T> reloadResource( const std::string& packageName, const std::string& resourcePath )
		{
			return reloadResource<T>( "./Packages/" + packageName + "/Resource/" + resourcePath );
		}

		template< class T >
		std::shared_ptr<T> getConfigFile( const std::string& systemName )
		{
			return reloadResource<T>( "./Config/" + systemName + ".ini" );
		}
	};
}
