#pragma once
#include "stdafx.h"

namespace kg
{
	namespace blueprint
	{
		const std::string file_extension = ".blueprint";

		bool isLineEmpty( const std::string& line );

		//checks if string a contains string b
		bool contains( const std::string&a, const std::string b );


		class ParsingError : public std::exception
		{
			const std::string m_msg;

		public:
			ParsingError( unsigned int line );

			const char* what()const override;
		};

		class LinkingError : public std::exception
		{
			std::string m_msg;
		public:
			LinkingError( std::string blueprintName );

			const char* what()const override;

		};

		template<class T>
		class Command
		{
		public:
			//first:nextLine second:Object
			virtual std::pair<unsigned int, T> execute( unsigned int firstLine,
														const std::vector<std::string>& lines ) = 0;

			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines ) = 0;
		};


		class DLL_EXPORT Value
		{
			std::string m_name;
			std::string m_rawValue;

		public:
			Value();
			Value( std::string& name,
				   std::string& rawValue );

			const std::string& getName()const;

			int toInt()const;
			std::pair<int, std::string> toIntWithUnit()const;

			double toDouble()const;
			std::pair<double, std::string> toDoubleWithUnit()const;

			std::string toString()const;
			std::string toStringWithBraces()const;
		};

		class DLL_EXPORT Blueprint
		{
			std::string m_name;
			std::map<std::string, std::map<std::string, Value>> m_componentValues;
		public:
			Blueprint();
			Blueprint( std::string& name,
					   std::map<std::string, std::map<std::string, Value>>& componentValuesByNameByComponent );

			const std::pair<bool, Value> getValue( const std::string& componentName, const std::string& valueName )const;

			const std::map<std::string, Value> getComponentValues( const std::string& componentName )const;

			std::vector<std::string> getComponentNames()const;

			std::string getName()const;

		};

		class DLL_EXPORT Entity
		{
			unsigned int m_id;
			std::string m_name;
			std::vector<std::string> m_inheritsFrom;//in ascending override importance
			std::vector<Blueprint*> m_inheritedBlueprints;//in ascending override importance
			std::map<std::string, std::map<std::string, Value>> m_componentValues;
		public:
			Entity();
			Entity( const unsigned int& id,
					std::string& name,
					std::vector<std::string>& inheritsFrom,
					std::map<std::string, std::map<std::string, Value>>& componentValuesByNameByComponent );

			const std::pair<bool, Value> getValue( const std::string& componentName, const std::string& valueName )const;

			std::vector<std::string> getComponentNames()const;
			std::map<std::string, Value> getComponentValues(const std::string componentName)const;

			void connectToBlueprints( std::map<std::string, Blueprint>& blueprintsByName );

			const unsigned int getId()const;

			std::string getName()const;
		};


		///////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////


		class ComponentDeclaration : public Command < std::string >
		{
		public:
			virtual std::pair<unsigned int, std::string> execute( unsigned int firstLine,
																  const std::vector<std::string>& lines );

			//can execute on any line that is not empty and does only contain alphanumeric characters
			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines );
		};

		class ValueDeclaration : public Command < Value >
		{
		public:
			virtual std::pair<unsigned int, Value> execute( unsigned int firstLine,
															const std::vector<std::string>& lines );

			//can execute on any line that is not empty and does contain at least one double point
			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines );

		};

		class ComponentDefinition : public Command < std::pair<std::string, std::vector<Value> > >
		{
		public:
			virtual std::pair<unsigned int, std::pair<std::string, std::vector<Value> >> execute( unsigned int firstLine,
																								  const std::vector<std::string>& lines );

			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines );

		};

		class InheritanceDeclaration : public Command < std::string >
		{
		public:
			virtual std::pair<unsigned int, std::string> execute( unsigned int firstLine,
																  const std::vector<std::string>& lines );

			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines );

		};

		class BlueprintDeclaration :public Command < Blueprint >
		{
		public:
			virtual std::pair<unsigned int, Blueprint> execute( unsigned int firstLine,
																const std::vector<std::string>& lines );

			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines );

		};

		class EntityDeclaration :public Command < Entity >
		{
		public:
			virtual std::pair<unsigned int, Entity> execute( unsigned int firstLine,
															 const std::vector<std::string>& lines );

			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines );

		};

		class BlueprintManager
		{
			std::map<std::string, Blueprint> m_blueprintsByName;
			std::map<unsigned int, Entity> m_entitiesById;

		public:
			// parse a file
			// can throw parsing error
			void parse( const std::string& path );

			// can throw parsing error
			void parse( const std::vector<std::string>&lines );

			// should be called after all files have been parsed
			// links entities to blueprints
			// can throw linking error
			void link();

			DLL_EXPORT const std::map<std::string, Blueprint>& getBlueprintsByName()const;
			DLL_EXPORT const std::map<unsigned int, Entity>& getEntitiesById()const;

			DLL_EXPORT const Blueprint& getBlueprintByName( const std::string& name )const;
			DLL_EXPORT const Entity& getEntityById( const int id )const;

		};
	}
}
