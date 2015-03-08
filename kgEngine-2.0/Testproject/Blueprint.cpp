#include "Blueprint.h"
using namespace std;

namespace kg
{
	namespace blueprint
	{
		bool isLineEmptyOrComment( const std::string& line )
		{
			if( line.size() == NULL )
				return true;
			else
				for( const auto& ch : line )
					if( !std::isspace( ch ) )
						if( ch == '#'/*line is commented out. handle as empty*/ )
							return true;
						else
							return false;
			return true;
		}

		bool contains( const std::string&a, const std::string b )
		{
			return a.find( b ) != std::string::npos;
		}

		vector<string> split( const std::string& str, const std::string& splitAt )
		{
			vector<string> strs;
			boost::split( strs, str, boost::is_any_of( splitAt ) );

			strs.erase(
				remove_if( strs.begin(), strs.end(), []( const string& s )
			{
				return (s.size() == 0);
			} ),
				end( strs ) );

			return strs;
		}

		std::vector<std::string> splitAtFirstOccurence( const std::string str, const char splitAt )
		{
			string::size_type pos;
			pos = str.find( splitAt );
			if( pos == string::npos )
				return{ str };
			else
			{
				vector<string> retVal;
				retVal.push_back( str.substr( 0, pos ) );
				retVal.push_back( str.substr( pos + 1 ) );
				return retVal;
			}
		}

		ParsingError::ParsingError( const std::string& file, Line line, const std::string error_message )
			: m_msg( "ParsingError: file: " + file + "; line: " + to_string( line ) + "; error: " + error_message + ";" )
		{ }

		const char* ParsingError::what() const
		{
			return m_msg.c_str();
		}

		LinkingError::LinkingError( const std::string& componentContainerName, const std::string error_message ) :m_msg( "LinkingError: containerName: " + componentContainerName + "; error: " + error_message )
		{ }

		const char* LinkingError::what() const
		{
			return m_msg.c_str();
		}

		bool EndOfBlockDeclaration::canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			const auto& line = lines.at( firstLine );
			if( !isLineEmptyOrComment( line ) )
			{
				auto strs = split( line, boostEmptyCharFilter );

				if( strs.size() > 0 )
				{
					if( strs.at( 0 ) == END )
					{
						return true;
					}
				}
				else
					throw EmptyLineNotRecognisedError( file, firstLine );
			}

			return false;
		}

		EmptyLineNotRecognisedError::EmptyLineNotRecognisedError( const std::string& file, Line line )
			: ParsingError( file, line, "EmptyLineNotRecognised" )
		{ }

		std::pair<Line, Component> ComponentDeclaration::execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			auto strs = split( lines.at( firstLine ), boostEmptyCharFilter );
			return make_pair( firstLine + 1, Component( strs.at( 0 ) ) );
		}

		bool ComponentDeclaration::canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			const auto& line = lines.at( firstLine );
			if( !isLineEmptyOrComment( line ) )
			{
				auto strs = split( line, boostEmptyCharFilter );
				if( strs.size() != 1 )
					return false;
				else
					return true;
			}

			return false;
		}

		std::pair<Line, ComponentValue> ComponentValueDeclaration::execute( const std::string& file,
																			Line firstLine,
																			const std::vector<std::string>& lines )
		{
			const auto& line = lines.at( firstLine );
			auto strs = splitAtFirstOccurence( line, ':' );

			auto& str = strs.at( 0 );/*remove spaces from name*/
			str.erase( remove_if( str.begin(), str.end(), isspace ), str.end() );

			return make_pair( firstLine + 1, ComponentValue( str, strs.at( 1 ) ) );
		}

		bool ComponentValueDeclaration::canExecuteOn( const std::string& file,
													  Line firstLine,
													  const std::vector<std::string>& lines )
		{
			const auto& line = lines.at( firstLine );
			if( !isLineEmptyOrComment( line ) )
				return find( line.begin(), line.end(), ':' ) != line.end();
			return false;
		}

		std::pair<Line, Component> ComponentDefinition::execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			Line currentLine = firstLine;

			auto strs = split( lines.at( firstLine ), boostEmptyCharFilter );
			Component component( strs.at( 1 ) );/*strs.at( 1 ): name*/

			currentLine++;

			ComponentValueDeclaration cvd;
			bool cvdCanExecute = cvd.canExecuteOn( file, currentLine, lines );
			bool isEmpty = isLineEmptyOrComment( lines.at( currentLine ) );
			while( cvdCanExecute || isEmpty )
			{
				if( isEmpty )
					currentLine++;
				else if( cvdCanExecute )
				{
					auto cvdRetVal = cvd.execute( file, currentLine, lines );
					component.addComponentValue( cvdRetVal.second );

					currentLine = cvdRetVal.first;
				}

				cvdCanExecute = cvd.canExecuteOn( file, currentLine, lines );
				isEmpty = isLineEmptyOrComment( lines.at( currentLine ) );
			}
			EndOfBlockDeclaration eobd;
			if( eobd.canExecuteOn( file, currentLine, lines ) )
				return make_pair( currentLine + 1, move( component ) );
			else
				throw ParsingError( file, currentLine );
		}

		bool ComponentDefinition::canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			const auto& line = lines.at( firstLine );
			if( !isLineEmptyOrComment( line ) )
			{
				auto strs = split( line, boostEmptyCharFilter );

				if( strs.at( 0 ) == COMPONENT )
				{
					if( strs.size() == 2 )
						return true;
					else
						throw ParsingError( file, firstLine );
				}
			}
			return false;
		}

		std::pair<Line, ComponentContainer::Name> InheritanceDeclaration::execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			auto strs = split( lines.at( firstLine ), boostEmptyCharFilter );
			return make_pair( firstLine + 1, strs.at( 1 ) );
		}

		bool InheritanceDeclaration::canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			const auto& line = lines.at( firstLine );
			if( !isLineEmptyOrComment( line ) )
			{
				auto strs = split( line, boostEmptyCharFilter );

				if( strs.at( 0 ) == INHERIT )
				{
					if( strs.size() == 2 )
						return true;
					else
						throw ParsingError( file, firstLine );
				}
			}
			return false;
		}

		std::pair<Line, Blueprint> BlueprintDeclaration::execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			Line currentLine = firstLine;
			auto strs = split( lines.at( currentLine ), boostEmptyCharFilter );
			Blueprint blueprint( strs.at( 1 ) );

			currentLine++;

			ComponentDeclaration cdec;
			ComponentDefinition cdef;
			InheritanceDeclaration idec;
			EndOfBlockDeclaration eobd;
			bool cdecCanExecute = cdec.canExecuteOn( file, currentLine, lines );
			bool cdefCanExecute = cdef.canExecuteOn( file, currentLine, lines );
			bool idecCanExecute = idec.canExecuteOn( file, currentLine, lines );
			bool eobdCanExecute = eobd.canExecuteOn( file, currentLine, lines );
			bool isEmpty = isLineEmptyOrComment( lines.at( currentLine ) );
			while( cdecCanExecute || cdefCanExecute || idecCanExecute || isEmpty )
			{
				if( isEmpty )
					currentLine++;
				else if( eobdCanExecute )
					break;
				else if( cdefCanExecute )
				{
					auto retVal = cdef.execute( file, currentLine, lines );
					blueprint.addComponentInformation( retVal.second );

					currentLine = retVal.first;
				}
				else if( cdecCanExecute )
				{
					auto retVal = cdec.execute( file, currentLine, lines );
					blueprint.addComponentInformation( retVal.second );

					currentLine = retVal.first;
				}
				else if( idecCanExecute )
				{
					auto retVal = idec.execute( file, currentLine, lines );
					blueprint.addInheritsFrom( retVal.second );

					currentLine = retVal.first;
				}

				cdecCanExecute = cdec.canExecuteOn( file, currentLine, lines );
				cdefCanExecute = cdef.canExecuteOn( file, currentLine, lines );
				idecCanExecute = idec.canExecuteOn( file, currentLine, lines );
				eobdCanExecute = eobd.canExecuteOn( file, currentLine, lines );
				isEmpty = isLineEmptyOrComment( lines.at( currentLine ) );
			}
			if( eobd.canExecuteOn( file, currentLine, lines ) )
				return make_pair( currentLine + 1, move( blueprint ) );
			else
				throw ParsingError( file, currentLine );
		}

		bool BlueprintDeclaration::canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			const auto& line = lines.at( firstLine );
			if( !isLineEmptyOrComment( line ) )
			{
				auto strs = split( line, boostEmptyCharFilter );

				if( strs.at( 0 ) == BLUEPRINT )
				{
					if( strs.size() == 2 )
						return true;
					else
						throw ParsingError( file, firstLine );
				}
			}
			return false;
		}

		std::pair<Line, Entity> EntityDeclaration::execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			Line currentLine = firstLine;
			auto strs = split( lines.at( currentLine ), boostEmptyCharFilter + ':' );
			Entity entity( strs.at( 1 ), static_cast< Entity::Id >(atoll( strs.at( 2 ).c_str() )) );

			currentLine++;

			ComponentDeclaration cdec;
			ComponentDefinition cdef;
			InheritanceDeclaration idec;
			EndOfBlockDeclaration eobd;
			bool cdecCanExecute = cdec.canExecuteOn( file, currentLine, lines );
			bool cdefCanExecute = cdef.canExecuteOn( file, currentLine, lines );
			bool idecCanExecute = idec.canExecuteOn( file, currentLine, lines );
			bool eobdCanExecute = eobd.canExecuteOn( file, currentLine, lines );
			bool isEmpty = isLineEmptyOrComment( lines.at( currentLine ) );
			while( cdecCanExecute || cdefCanExecute || idecCanExecute || isEmpty )
			{
				if( isEmpty )
					currentLine++;
				else if( eobdCanExecute )
					break;
				else if( cdefCanExecute )
				{
					auto retVal = cdef.execute( file, currentLine, lines );
					entity.addComponentInformation( retVal.second );

					currentLine = retVal.first;
				}
				else if( cdecCanExecute )
				{
					auto retVal = cdec.execute( file, currentLine, lines );
					entity.addComponentInformation( retVal.second );

					currentLine = retVal.first;
				}
				else if( idecCanExecute )
				{
					auto retVal = idec.execute( file, currentLine, lines );
					entity.addInheritsFrom( retVal.second );

					currentLine = retVal.first;
				}

				cdecCanExecute = cdec.canExecuteOn( file, currentLine, lines );
				cdefCanExecute = cdef.canExecuteOn( file, currentLine, lines );
				idecCanExecute = idec.canExecuteOn( file, currentLine, lines );
				eobdCanExecute = eobd.canExecuteOn( file, currentLine, lines );
				isEmpty = isLineEmptyOrComment( lines.at( currentLine ) );
			}
			if( eobd.canExecuteOn( file, currentLine, lines ) )
				return make_pair( currentLine + 1, move( entity ) );
			else
				throw ParsingError( file, currentLine );
		}

		bool EntityDeclaration::canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			const auto& line = lines.at( firstLine );
			if( !isLineEmptyOrComment( line ) )
			{
				auto strs = split( line, boostEmptyCharFilter + ':' );

				if( strs.at( 0 ) == ENTITY )
				{
					if( strs.size() == 3 )
						return true;
					else
						throw ParsingError( file, firstLine );
				}
			}
			return false;
		}

		std::pair<Line, ComponentContainer::Name> ComponentContainerExtensionDestinationDeclaration::execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			auto strs = split( lines.at( firstLine ), boostEmptyCharFilter );
			return make_pair( firstLine + 1, strs.at( 1 ) );
		}

		bool ComponentContainerExtensionDestinationDeclaration::canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			const auto& line = lines.at( firstLine );
			if( !isLineEmptyOrComment( line ) )
			{
				auto strs = split( line, boostEmptyCharFilter );

				if( strs.at( 0 ) == DESTINATION )
				{
					if( strs.size() == 2 )
						return true;
					else
						throw ParsingError( file, firstLine );
				}
			}
			return false;
		}

		std::pair<Line, ComponentContainerExtension> ComponentContainerExtensionDeclaration::execute( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			Line currentLine = firstLine;
			auto strs = split( lines.at( currentLine ), boostEmptyCharFilter );
			ComponentContainerExtension componentContainerExtension( strs.at( 1 ) );

			currentLine++;

			ComponentDeclaration cdec;
			ComponentDefinition cdef;
			InheritanceDeclaration idec;
			ComponentContainerExtensionDestinationDeclaration cedd;
			EndOfBlockDeclaration eobd;
			bool cdecCanExecute = cdec.canExecuteOn( file, currentLine, lines );
			bool cdefCanExecute = cdef.canExecuteOn( file, currentLine, lines );
			bool idecCanExecute = idec.canExecuteOn( file, currentLine, lines );
			bool ceddCanExecute = cedd.canExecuteOn( file, currentLine, lines );
			bool eobdCanExecute = eobd.canExecuteOn( file, currentLine, lines );
			bool isEmpty = isLineEmptyOrComment( lines.at( currentLine ) );
			while( cdecCanExecute || cdefCanExecute || idecCanExecute || ceddCanExecute || isEmpty )
			{
				if( isEmpty )
					currentLine++;
				else if( eobdCanExecute )
					break;
				else if( cdefCanExecute )
				{
					auto retVal = cdef.execute( file, currentLine, lines );
					componentContainerExtension.addComponentInformation( retVal.second );

					currentLine = retVal.first;
				}
				else if( cdecCanExecute )
				{
					auto retVal = cdec.execute( file, currentLine, lines );
					componentContainerExtension.addComponentInformation( retVal.second );

					currentLine = retVal.first;
				}
				else if( idecCanExecute )
				{
					auto retVal = idec.execute( file, currentLine, lines );
					componentContainerExtension.addInheritsFrom( retVal.second );

					currentLine = retVal.first;
				}
				else if( ceddCanExecute )
				{
					auto retVal = cedd.execute( file, currentLine, lines );
					componentContainerExtension.addDestination( retVal.second );

					currentLine = retVal.first;
				}

				cdecCanExecute = cdec.canExecuteOn( file, currentLine, lines );
				cdefCanExecute = cdef.canExecuteOn( file, currentLine, lines );
				idecCanExecute = idec.canExecuteOn( file, currentLine, lines );
				ceddCanExecute = cedd.canExecuteOn( file, currentLine, lines );
				eobdCanExecute = eobd.canExecuteOn( file, currentLine, lines );
				isEmpty = isLineEmptyOrComment( lines.at( currentLine ) );
			}
			if( eobd.canExecuteOn( file, currentLine, lines ) )
				return make_pair( currentLine + 1, move( componentContainerExtension ) );
			else
				throw ParsingError( file, currentLine );
		}

		bool ComponentContainerExtensionDeclaration::canExecuteOn( const std::string& file, Line firstLine, const std::vector<std::string>& lines )
		{
			const auto& line = lines.at( firstLine );
			if( !isLineEmptyOrComment( line ) )
			{
				auto strs = split( line, boostEmptyCharFilter );

				if( strs.at( 0 ) == EXTENSION )
				{
					if( strs.size() == 2 )
						return true;
					else
						throw ParsingError( file, firstLine );
				}
			}
			return false;
		}

		void BlueprintManager::parse( const std::string& path )
		{
			std::ifstream file;
			file.open( path );
			if( !file.is_open() )
				throw ParsingError( path, 0, "file does not exist" );

			std::vector<std::string> lines;

			std::string line;
			while( getline( file, line ) )
				lines.push_back( move( line ) );

			parse( path, lines );
		}

		void BlueprintManager::parse( const std::string& file, const std::vector<std::string>&lines )
		{
			Line line = 0;

			BlueprintDeclaration bdec;
			EntityDeclaration edec;
			ComponentContainerExtensionDeclaration cced;

			while( line < lines.size() )
			{
				if( isLineEmptyOrComment( lines.at( line ) ) )
				{
					line++;
				}
				else if( bdec.canExecuteOn( file, line, lines ) )
				{
					auto retVal = bdec.execute( file, line, lines );
					line = retVal.first;
					auto name = retVal.second.getName();
					m_blueprintsByName.emplace( make_pair( name, move( retVal.second ) ) );

					m_componentContainersByName.emplace(
						std::pair<ComponentContainer::Name, ComponentContainer*>(
						name,
						&m_blueprintsByName.at( name ) ) );
				}
				else if( edec.canExecuteOn( file, line, lines ) )
				{
					auto retVal = edec.execute( file, line, lines );
					line = retVal.first;
					auto id = retVal.second.getId();
					auto name = retVal.second.getName();
					m_entitiesById.emplace( make_pair( id, move( retVal.second ) ) );

					m_componentContainersByName.emplace(
						std::pair<ComponentContainer::Name, ComponentContainer*>(
						name,
						&m_entitiesById.at( id ) ) );
				}
				else if( cced.canExecuteOn( file, line, lines ) )
				{
					auto retVal = cced.execute( file, line, lines );
					line = retVal.first;
					auto name = retVal.second.getName();
					m_componentContainerExtensionsByName.emplace( make_pair( name, move( retVal.second ) ) );

					m_componentContainersByName.emplace(
						std::pair<ComponentContainer::Name, ComponentContainer*>(
						name,
						&m_componentContainerExtensionsByName.at( name ) ) );
				}
			}
		}

		void BlueprintManager::link()
		{
			/*Add inheritance info to Extension destinations*/
			for( const auto& el : m_componentContainerExtensionsByName )
				el.second.writeInheritanceInfoToDestinations( m_componentContainersByName );

			std::vector<std::string*> linkingStack;

			/*Collect inherit references*/
			for( auto& el : m_componentContainersByName )
				el.second->collectInheritReferences( linkingStack, m_componentContainersByName );
		}

		void BlueprintManager::clear()
		{
			m_componentContainerExtensionsByName.clear();
			m_blueprintsByName.clear();
			m_entitiesById.clear();
			m_componentContainersByName.clear();
		}

		const std::map<ComponentContainer::Name, ComponentContainerExtension>& BlueprintManager::getComponentContainerExtensionsByName() const
		{
			return m_componentContainerExtensionsByName;
		}

		const std::map<Entity::Id, Entity>& BlueprintManager::getEntitiesById() const
		{
			return m_entitiesById;
		}

		const std::map<ComponentContainer::Name, Blueprint>& BlueprintManager::getBlueprintsByName() const
		{
			return m_blueprintsByName;
		}

		const Blueprint& BlueprintManager::getBlueprintByName( const ComponentContainer::Name name ) const
		{
			return m_blueprintsByName.at( name );
		}

		const Entity& BlueprintManager::getEntityById( const Entity::Id& id ) const
		{
			return m_entitiesById.at( id );
		}

		CircularInheritanceError::CircularInheritanceError( const std::vector<std::string*>& linkingStack )
			: LinkingError( *(*linkingStack.cend()), "CircularInheritance stack: " + linkingStackToString( linkingStack ) )
		{ }

		std::string CircularInheritanceError::linkingStackToString( const std::vector<std::string*>& linkingStack )
		{
			std::string retVal;
			for( const auto& el : linkingStack )
			{
				retVal += *el;
				retVal += " - ";
			}
			retVal.pop_back();
			return retVal;
		}

		ComponentContainer::ComponentContainer( const Name& name )
			:m_name( name )
		{ }

		ComponentContainer::ComponentContainer( ComponentContainer&& container )
			: m_name( move( container.m_name ) ),
			m_components( move( container.m_components ) ),
			m_inheritsFromNames( move( container.m_inheritsFromNames ) ),
			m_isCollectingInheritReferences( move( container.m_isCollectingInheritReferences ) ),
			m_hasCollectedInheritReferences( move( container.m_hasCollectedInheritReferences ) )

		{
		}

		void ComponentContainer::addComponentInformation( const Component& component )
		{
			auto& name = component.getName();
			auto it = m_components.find( name );
			if( it == end( m_components ) )
			{
				auto retVal = m_components.emplace( make_pair( name, Component( name ) ) );
				it = retVal.first;
			}
			auto& componentValuesToAdd = component.getComponentValuesByName();
			for( const auto& val : componentValuesToAdd )
				it->second.addComponentValue( val.second );
		}

		void ComponentContainer::overrideComponentInformationReferences( const Component& component )
		{
			auto& name = component.getName();
			auto it = m_components.find( name );
			if( it == end( m_components ) )
			{
				auto retVal = m_components.emplace( make_pair( name, Component( name ) ) );
				it = retVal.first;
			}
			auto& componentValuesToAdd = component.getComponentValueReferencesByName();
			for( const auto& val : componentValuesToAdd )
				it->second.overrideComponentValueReference( val.second );
		}

		void ComponentContainer::addInheritsFrom( const Name& componentContainerToInheritFrom )
		{
			m_inheritsFromNames.push_back( componentContainerToInheritFrom );
		}

		void ComponentContainer::collectInheritReferences(
			std::vector<std::string*> linkingStack,
			std::map<ComponentContainer::Name, ComponentContainer*>& componentContainersByName )
		{
			if( !m_hasCollectedInheritReferences )
			{
				linkingStack.push_back( &static_cast< string >(m_name) );

				if( m_isCollectingInheritReferences == true )
					throw CircularInheritanceError( linkingStack );
				else
				{
					m_isCollectingInheritReferences = true;
					for( const auto& name : m_inheritsFromNames )
					{
						auto* currentContainer = componentContainersByName.at( name );
						currentContainer->collectInheritReferences( linkingStack, componentContainersByName );

						for( const auto& comp : currentContainer->getComponentsByName() )
							overrideComponentInformationReferences( comp.second );
					}

					overrideLocalComponentValueReferences();

					for( const auto& name : m_isOverriddenByNames )
					{
						auto* currentContainer = componentContainersByName.at( name );
						currentContainer->collectInheritReferences( linkingStack, componentContainersByName );

						for( const auto& comp : currentContainer->getComponentsByName() )
							overrideComponentInformationReferences( comp.second );
					}
				}

				linkingStack.pop_back();
				m_isCollectingInheritReferences = false;
				m_hasCollectedInheritReferences = true;
			}
		}

		const std::map<std::string, Component>& ComponentContainer::getComponentsByName() const
		{
			return m_components;
		}

		const ComponentContainer::Name& ComponentContainer::getName() const
		{
			return m_name;
		}

		void ComponentContainer::overrideLocalComponentValueReferences()
		{
			for( auto& el : m_components )
				el.second.overrideLocalComponentValueReferences();
		}

		void ComponentContainer::addIsOverriddenBy( const Name& componentContainerThatOverrides )
		{
			m_isOverriddenByNames.push_back( componentContainerThatOverrides );
		}

		Component::Component( const Name& name )
			: m_name( name )
		{ }

		Component::Component( Component&& component )
			: m_name( move( component.m_name ) ),
			m_componentValuesByName( move( component.m_componentValuesByName ) ),
			m_componentValueReferencesByName( move( component.m_componentValueReferencesByName ) )
		{
		}

		const std::string& Component::getName() const
		{
			return m_name;
		}

		void Component::addComponentValue( const ComponentValue& componentValue )
		{
			auto& name = componentValue.getName();
			auto retVal = m_componentValuesByName.emplace( make_pair( name, componentValue ) );/*Copies component Value*/
			auto retVal2 = m_componentValueReferencesByName.emplace( make_pair( name, &(retVal.first->second) ) );
			if( !retVal.second || !retVal2.second )
				throw NoOverridePossibleError();
		}

		const std::map<ComponentValue::Name, const ComponentValue*>& Component::getComponentValueReferencesByName() const
		{
			return m_componentValueReferencesByName;
		}

		void Component::overrideComponentValueReference( const ComponentValue* componentValue )
		{
			auto& name = componentValue->getName();
			m_componentValueReferencesByName[name] = componentValue;
		}

		const std::map<ComponentValue::Name, ComponentValue>& Component::getComponentValuesByName() const
		{
			return m_componentValuesByName;
		}

		void Component::overrideLocalComponentValueReferences()
		{
			for( const auto& el : m_componentValuesByName )
				m_componentValueReferencesByName[el.first] = &el.second;
		}

		ComponentValue::ComponentValue( const Name& name, const std::string& rawValue )
			: m_name( name ),
			m_rawValue( rawValue )
		{
			parseRawValue();
		}

		const ComponentValue::Name& ComponentValue::getName() const
		{
			return m_name;
		}

		const std::string& ComponentValue::getRawValue() const
		{
			return m_rawValue;
		}

		bool ComponentValue::asBool() const
		{
			if( m_asBool )
				return *m_asBool;
			else
				throw ComponentValueConversionNotAvailable();
		}

		const int& ComponentValue::asInt() const
		{
			if( m_asInt )
				return *m_asInt;
			else
				throw ComponentValueConversionNotAvailable();
		}

		const std::pair<int, std::string>& ComponentValue::asIntWithUnit() const
		{
			if( m_asIntWithUnit )
				return *m_asIntWithUnit;
			else
				throw ComponentValueConversionNotAvailable();
		}

		const double& ComponentValue::asDouble() const
		{
			if( m_asDouble )
				return *m_asDouble;
			else
				throw ComponentValueConversionNotAvailable();
		}

		const std::pair<double, std::string>& ComponentValue::asDoubleWithUnit() const
		{
			if( m_asDoubleWithUnit )
				return *m_asDoubleWithUnit;
			else
				throw ComponentValueConversionNotAvailable();
		}

		const std::string& ComponentValue::asString() const
		{
			if( m_asString )
				return *m_asString;
			else
				throw ComponentValueConversionNotAvailable();
		}

		void ComponentValue::parseRawValue()
		{
			/*bool*/
			if( atoi( m_rawValue.c_str() ) != 0 )
			{
				m_asBool = true;
			}
			else
			{
				auto rawValueCopy = m_rawValue;
				std::transform( rawValueCopy.begin(), rawValueCopy.end(), rawValueCopy.begin(), ::tolower );
				if( rawValueCopy == "true" )
					m_asBool = true;
			}
			m_asBool = false;

			/*doubleWithUnit*/
			std::string number;
			std::string unit;

			bool afterNumber = false;
			for( const auto& ch : m_rawValue )
			{
				if( !afterNumber )
				{
					if( std::isdigit( ch ) || ch == '.' )
						number.push_back( ch );
					else
					{
						afterNumber = true;
						unit.push_back( ch );
					}
				}
				else
				{
					unit.push_back( ch );
				}
			}

			std::istringstream i( number );
			double d = 0;
			if( (i >> d) )
				m_asDoubleWithUnit = std::pair<double, std::string>( d, unit );

			/*double*/
			if( m_asDoubleWithUnit )
				m_asDouble = m_asDoubleWithUnit->first;

			/*intWithUnit*/
			if( m_asDoubleWithUnit )
				m_asIntWithUnit = std::pair<int, std::string>(
				( int )m_asDoubleWithUnit->first,
				m_asDoubleWithUnit->second );

			/*int*/
			if( m_asIntWithUnit )
				m_asInt = m_asIntWithUnit->first;

			/*string*/
			if( m_rawValue.at( 0 ) == '"' && *m_rawValue.rbegin() == '"' )
			{
				std::string retVal = m_rawValue;
				retVal.erase( 0, 1 );
				retVal.pop_back();
				m_asString = retVal;
			}
			else
				m_asString = m_rawValue;
		}

		const char* NoOverridePossibleError::what() const
		{
			return "No override possible";
		}

		Blueprint::Blueprint( const ComponentContainer::Name& name )
			:ComponentContainer( name )
		{
		}

		Blueprint::Blueprint( Blueprint&& blueprint )
			: ComponentContainer( move( blueprint ) )
		{
		}

		Entity::Entity( const ComponentContainer::Name& name, const Id& id )
			: ComponentContainer( name ),
			m_id( id )
		{
		}

		Entity::Entity( Entity&& entity )
			: ComponentContainer( move( entity ) ),
			m_id( move( entity.m_id ) )
		{
		}

		const Entity::Id& Entity::getId() const
		{
			return m_id;
		}

		ComponentContainerExtension::ComponentContainerExtension( const ComponentContainer::Name& name )
			:ComponentContainer( name )
		{
		}

		ComponentContainerExtension::ComponentContainerExtension( ComponentContainerExtension&& componentContainerExtension )
			: ComponentContainer( move( componentContainerExtension ) ),
			m_destinations( move( componentContainerExtension.m_destinations ) )
		{
		}

		void ComponentContainerExtension::addDestination( const ComponentContainer::Name& destination )
		{
			m_destinations.push_back( destination );
		}

		void ComponentContainerExtension::writeInheritanceInfoToDestinations(
			std::map<ComponentContainer::Name, ComponentContainer*>& componentContainersByName ) const
		{
			for( const auto& dest : m_destinations )
				componentContainersByName.at( dest )->addIsOverriddenBy( getName() );
		}

		ComponentValueConversionNotAvailable::ComponentValueConversionNotAvailable()
		{
		}

		const char* ComponentValueConversionNotAvailable::what() const
		{
			return "ComponentValueConversionNotAvailable";
		}
	}
}