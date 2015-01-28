#pragma once
#include "stdafx.h"

namespace kg
{
	namespace blueprint
	{
		class Value;
		typedef std::map<std::string, std::map<std::string, Value>> ComponentValuesByNameByComponentMap;

		typedef unsigned int Id;
		typedef unsigned int Line;

		const std::string file_extension = ".blueprint";

		bool isLineEmpty( const std::string& line );

		//checks if string a contains string b
		bool contains( const std::string&a, const std::string b );

		class ParsingError : public std::exception
		{
			const std::string m_msg;

		public:
			ParsingError( Line line );

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
			//first:nextLine to parse second:Object
			virtual std::pair<Line, T> execute( Line firstLine,
														const std::vector<std::string>& lines ) = 0;

			virtual bool canExecuteOn( Line line,
									   const std::vector<std::string>& lines ) = 0;
		};

		class DLL_EXPORT Value
		{
			/*const*/ std::string m_name = "standard constructor has been called - object not initialized";
			/*const*/ std::string m_rawValue = "standard constructor has been called - object not initialized";
			bool m_isValid = false;

		public:
			Value();
			Value( const std::string& name,
				   const std::string& rawValue );

			const std::string& getName()const;

			bool toBool()const;

			int toInt()const;
			std::pair<int, std::string> toIntWithUnit()const;

			double toDouble()const;
			std::pair<double, std::string> toDoubleWithUnit()const;

			std::string toString()const;

			std::string getRawValue()const;

			bool isValid()const;
			operator bool()const;

			Value& operator=(const std::string& rhs);
		};

		class DLL_EXPORT Blueprint
		{
			std::string m_name;
			ComponentValuesByNameByComponentMap m_componentValues;
		public:
			Blueprint();
			Blueprint( std::string& name,
					   ComponentValuesByNameByComponentMap& componentValuesByNameByComponent );

			const std::pair<bool, Value> getValue( const std::string& componentName, const std::string& valueName )const;

			const std::map<std::string, Value> getComponentValues( const std::string& componentName )const;

			std::vector<std::string> getComponentNames()const;

			std::string getName()const;
		};

		class DLL_EXPORT Entity
		{
			Id m_id;
			std::string m_name;
			std::vector<std::string> m_inheritsFrom;//in ascending override importance
			std::vector<Blueprint*> m_inheritedBlueprints;//in ascending override importance
			ComponentValuesByNameByComponentMap m_componentValues;//componentValuesByNameByComponent
		public:
			Entity();
			Entity( const Id& id,
					std::string& name,
					std::vector<std::string>& inheritsFrom,
					ComponentValuesByNameByComponentMap& componentValuesByNameByComponent );

			const std::pair<bool, Value> getValue( const std::string& componentName, const std::string& valueName )const;

			std::vector<std::string> getComponentNames()const;
			std::map<std::string, Value> getComponentValues( const std::string componentName )const;

			void connectToBlueprints( std::map<std::string, Blueprint>& blueprintsByName );

			const Id getId()const;

			std::string getName()const;

			const std::vector<Blueprint*>& getInheritedBlueprints()const;
		};

		///////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////

		class ComponentDeclaration : public Command < std::string >
		{
		public:
			virtual std::pair<Line, std::string> execute( Line firstLine,
																  const std::vector<std::string>& lines );

			//can execute on any line that is not empty and does only contain alphanumeric characters
			virtual bool canExecuteOn( Line line,
									   const std::vector<std::string>& lines );
		};

		class ValueDeclaration : public Command < Value >
		{
		public:
			virtual std::pair<Line, Value> execute( Line firstLine,
															const std::vector<std::string>& lines );

			//can execute on any line that is not empty and does contain at least one double point
			virtual bool canExecuteOn( Line line,
									   const std::vector<std::string>& lines );
		};

		class ComponentDefinition : public Command < std::pair<std::string, std::vector<Value> > >
		{
		public:
			virtual std::pair<Line, std::pair<std::string, std::vector<Value> >> execute( Line firstLine,
																								  const std::vector<std::string>& lines );

			virtual bool canExecuteOn( Line line,
									   const std::vector<std::string>& lines );
		};

		class InheritanceDeclaration : public Command < std::string >
		{
		public:
			virtual std::pair<Line, std::string> execute( Line firstLine,
																  const std::vector<std::string>& lines );

			virtual bool canExecuteOn( Line line,
									   const std::vector<std::string>& lines );
		};

		class BlueprintDeclaration :public Command < Blueprint >
		{
		public:
			virtual std::pair<Line, Blueprint> execute( Line firstLine,
																const std::vector<std::string>& lines );

			virtual bool canExecuteOn( Line line,
									   const std::vector<std::string>& lines );
		};

		class EntityDeclaration :public Command < Entity >
		{
		public:
			virtual std::pair<Line, Entity> execute( Line firstLine,
															 const std::vector<std::string>& lines );

			virtual bool canExecuteOn( Line line,
									   const std::vector<std::string>& lines );
		};

		class BlueprintManager
		{
			std::map<std::string, Blueprint> m_blueprintsByName;
			std::map<Id, Entity> m_entitiesById;

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

			//all references from getBlueprintsByName, getEntitiesById, getBlueprintByName, getEntityById get invalidated
			//void reparse()

			DLL_EXPORT const std::map<std::string, Blueprint>& getBlueprintsByName()const;
			DLL_EXPORT const std::map<Id, Entity>& getEntitiesById()const;

			DLL_EXPORT const Blueprint& getBlueprintByName( const std::string& name )const;
			DLL_EXPORT const Entity& getEntityById( const int id )const;
		};
	}
}
