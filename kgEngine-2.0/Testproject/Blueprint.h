#pragma once
#include "stdafx.h"

namespace kg
{
	namespace blueprint
	{
		const std::string file_extension = ".blueprint";
		const std::string boostEmptyCharFilter = "\t ";
		typedef unsigned int Line;

		const std::string ENTITY = "ENTITY";
		const std::string BLUEPRINT = "BLUEPRINT";
		const std::string COMPONENT = "COMPONENT";
		const std::string EXTENSION = "EXTENSION";
		const std::string DESTINATION = "DEST";
		const std::string END = "END";
		const std::string INHERIT = "INHERIT";


		bool isLineEmptyOrComment( const std::string& line );

		bool contains( const std::string&a, const std::string b );

		std::vector<std::string> split( const std::string& str, const std::string& splitAt );

		std::vector<std::string> splitAtFirstOccurence( const std::string str, const char splitAt );


		class ParsingError : public std::exception
		{
			const std::string m_msg;

		public:
			ParsingError( const std::string& file, Line line, const std::string error_message = "parsing error" );

			const char* what()const override;
		};

		class EmptyLineNotRecognisedError : public ParsingError
		{
		public:
			EmptyLineNotRecognisedError( const std::string& file, Line line );
		};

		class LinkingError : public std::exception
		{
			std::string m_msg;
		public:
			LinkingError( const std::string& componentContainerName, const std::string error_message );

			const char* what()const override;
		};

		class CircularInheritanceError : public LinkingError
		{
		public:
			CircularInheritanceError( const std::vector<std::string*>& linkingStack );

			static std::string linkingStackToString( const std::vector<std::string*>& linkingStack );
		};

		class NoOverridePossibleError : public std::exception
		{
		public:
			const char* what()const override;
		};

		class ComponentValueConversionNotAvailable : public std::exception
		{
		public:
			ComponentValueConversionNotAvailable();

			const char* what()const override;
		};

		template<class T>
		class Command : public boost::noncopyable
		{
		public:
			//first:nextLine to parse second:Object
			virtual std::pair<Line, T> execute( const std::string& file,
												Line firstLine,
												const std::vector<std::string>& lines ) = 0;

			virtual bool canExecuteOn( const std::string& file,
									   Line firstLine,
									   const std::vector<std::string>& lines ) = 0;
		};

		template<>
		class Command < void >
		{
		public:
			virtual bool canExecuteOn( const std::string& file,
									   Line firstLine,
									   const std::vector<std::string>& lines ) = 0;
		};

		class ComponentValue
		{
		public:
			typedef std::string Name;
		private:
			const Name m_name;
			const std::string m_rawValue;

			boost::optional<bool> m_asBool;
			boost::optional<int> m_asInt;
			boost::optional<std::pair<int, std::string>> m_asIntWithUnit;
			boost::optional<double> m_asDouble;
			boost::optional<std::pair<double, std::string>> m_asDoubleWithUnit;
			boost::optional<std::string> m_asString;

			/*boost::optional<std::vector<ComponentValue>> m_asVectorOfValues;*/

			void parseRawValue();

		public:
			ComponentValue( const Name& name, const std::string& rawValue );

			const Name& getName()const;
			const std::string& getRawValue()const;

			bool asBool()const;

			const int& asInt()const;
			const std::pair<int, std::string>& asIntWithUnit()const;

			const double& asDouble()const;
			const std::pair<double, std::string>& asDoubleWithUnit()const;

			const std::string& asString()const;

			/*const std::vector<ComponentValue>& asContainer()const;*/
		};

		class Component : public boost::noncopyable
		{
		public:
			typedef std::string Name;
		private:
			const Name m_name;
			std::map<ComponentValue::Name, ComponentValue> m_componentValuesByName;
			std::map<ComponentValue::Name, const ComponentValue*> m_componentValueReferencesByName;

		public:
			Component( Component&& component );
			Component( const Name& name );/*no component values yet*/

			void addComponentValue( const ComponentValue& componentValue );

			void overrideComponentValueReference( const ComponentValue* componentValue );
			void overrideLocalComponentValueReferences();

			const std::map<ComponentValue::Name, const ComponentValue*>& getComponentValueReferencesByName()const;
			const std::map<ComponentValue::Name, ComponentValue>& getComponentValuesByName()const;

			const std::string& getName()const;
		};

		class ComponentContainer : public boost::noncopyable
		{
		public:
			typedef std::string Name;
		private:
			const Name m_name;

			std::map<std::string, Component> m_components;
			std::vector<Name> m_inheritsFromNames;
			std::vector<Name> m_isOverriddenByNames;

			bool m_isCollectingInheritReferences = false;
			bool m_hasCollectedInheritReferences = false;

			void overrideLocalComponentValueReferences();

		public:
			ComponentContainer( ComponentContainer&& container );
			ComponentContainer( const Name& name );

			/*
			Adds the information of component to this object.
			If the passed component has the same name as one of the already added Components
			the information of both will be combined.
			Throws NoOverridePossibleError if component information gets overridden.
			Use overrideComponentInformation for that.
			*/
			void addComponentInformation( const Component& component );

			void overrideComponentInformationReferences( const Component& component );

			void addInheritsFrom( const Name& componentContainerToInheritFrom );

			void addIsOverriddenBy( const Name& componentContainerThatOverrides );

			const Name& getName()const;

			void collectInheritReferences( std::vector<std::string*> linkingStack, std::map<ComponentContainer::Name, ComponentContainer*>& componentContainersByName );

			const std::map<std::string, Component>& getComponentsByName()const;
		};

		class Blueprint : public ComponentContainer
		{

		public:
			Blueprint( Blueprint&& blueprint );
			Blueprint( const ComponentContainer::Name& name );
		};

		class Entity : public ComponentContainer
		{
		public:
			typedef unsigned int Id;
		private:
			Id m_id;
		public:
			Entity( Entity&& entity );
			Entity( const ComponentContainer::Name& name, const Id& id );

			const Id& getId()const;
		};

		class ComponentContainerExtension : public ComponentContainer
		{
			std::vector<ComponentContainer::Name> m_destinations;

		public:
			ComponentContainerExtension( ComponentContainerExtension&& componentContainerExtension );
			ComponentContainerExtension( const ComponentContainer::Name& name );

			void addDestination( const ComponentContainer::Name& destination );

			void writeInheritanceInfoToDestinations( std::map<ComponentContainer::Name, ComponentContainer*>& componentContainersByName )const;
		};


		///////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////

		class EndOfBlockDeclaration : public Command < void >
		{
		public:
			virtual bool canExecuteOn( const std::string& file,
									   Line firstLine,
									   const std::vector<std::string>& lines ) override;
		};


		class ComponentDeclaration : public Command < Component >
		{
		public:
			virtual std::pair<Line, Component> execute( const std::string& file,
														Line firstLine,
														const std::vector<std::string>& lines ) override;

			virtual bool canExecuteOn( const std::string& file,
									   Line firstLine,
									   const std::vector<std::string>& lines ) override;

		};

		class ComponentValueDeclaration : public Command < ComponentValue >
		{
		public:
			virtual std::pair<Line, ComponentValue> execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

			virtual bool canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

		};

		class ComponentDefinition : public Command < Component >
		{
		public:
			virtual std::pair<Line, Component> execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

			virtual bool canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

		};


		class InheritanceDeclaration : public Command < ComponentContainer::Name >
		{
		public:
			virtual std::pair<Line, ComponentContainer::Name> execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

			virtual bool canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

		};

		class BlueprintDeclaration : public Command < Blueprint >
		{
		public:
			virtual std::pair<Line, Blueprint> execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

			virtual bool canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

		};

		class EntityDeclaration : public Command < Entity >
		{
		public:
			virtual std::pair<Line, Entity> execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

			virtual bool canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

		};

		class ComponentContainerExtensionDestinationDeclaration : public Command < ComponentContainer::Name >
		{
		public:
			virtual std::pair<Line, ComponentContainer::Name> execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

			virtual bool canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

		};

		class ComponentContainerExtensionDeclaration : public Command < ComponentContainerExtension >
		{
		public:
			virtual std::pair<Line, ComponentContainerExtension> execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

			virtual bool canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines ) override;

		};


		class BlueprintManager : public boost::noncopyable
		{
			std::map<ComponentContainer::Name, ComponentContainerExtension> m_componentContainerExtensionsByName;
			std::map<ComponentContainer::Name, Blueprint> m_blueprintsByName;
			std::map<Entity::Id, Entity> m_entitiesById;
			std::map<ComponentContainer::Name, ComponentContainer*> m_componentContainersByName;

		public:
			// parse a file
			// can throw parsing error
			void parse( const std::string& path );

			// can throw parsing error
			void parse( const std::string& file, const std::vector<std::string>&lines );

			// should be called after all files have been parsed
			// links entities to blueprints
			// can throw linking error
			void link();

			//all references from getBlueprintsByName, getEntitiesById, getBlueprintByName, getEntityById get invalidated
			void clear();

			const std::map<ComponentContainer::Name, Blueprint>& getBlueprintsByName()const;
			const std::map<Entity::Id, Entity>& getEntitiesById()const;
			const std::map<ComponentContainer::Name, ComponentContainerExtension>& getComponentContainerExtensionsByName()const;

			const Blueprint& getBlueprintByName( const ComponentContainer::Name name )const;
			const Entity& getEntityById( const Entity::Id& id )const;
		};
	}
}
