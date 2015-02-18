#include "Blueprint.h"

namespace kg
{
	namespace blueprint
	{
		bool isLineEmpty( const std::string& line )
		{
			if( line.size() == NULL )
				return true;
			else if( line.at( 0 ) == '#' )
				return true;//line is commented out. handle as empty
			else
				for( const auto& ch : line )
					if( !std::isspace( ch ) )
						return false;
			return true;
		}

		bool contains( const std::string&a, const std::string b )
		{
			return a.find( b ) != std::string::npos;
		}

		ParsingError::ParsingError( Line line )
			:m_msg( "parsing error on line:" + std::to_string( line ) )
		{ }

		const char* ParsingError::what() const
		{
			return m_msg.c_str();
		}

		LinkingError::LinkingError( std::string blueprintName )
			:m_msg( "linking error to blueprint:" + blueprintName )
		{ }

		const char* LinkingError::what() const
		{
			return m_msg.c_str();
		}

		Value::Value()
		{ }

		Value::Value( const std::string& name, const std::string& rawValue ) :
			m_name( name ),
			m_rawValue( rawValue )
		{
			for( const auto& el : rawValue )
			{
				if( !std::isspace( el ) )
				{
					m_isValid = true;
					break;
				}
			}
		}

		std::string Value::getRawValue() const
		{
			return m_rawValue;
		}

		std::string Value::toString()
		{
			if(!m_asString)
			{
				if( m_rawValue.at( 0 ) == '"' && *m_rawValue.rbegin() == '"' )
				{
					std::string retVal = m_rawValue;
					retVal.erase( 0, 1 );
					retVal.pop_back();
					m_asString= retVal;
				}
				else
					m_asString= m_rawValue;
			}
			return *m_asString;
		}

		std::pair<double, std::string> Value::toDoubleWithUnit()
		{
			if( !m_asDoubleWithUnit )
			{
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
				if( !(i >> d) )
					d = 0;

				m_asDoubleWithUnit = std::pair<double, std::string>( d, unit );
			}
			return *m_asDoubleWithUnit;
		}

		double Value::toDouble()
		{
			if(!m_asDouble)
				m_asDouble= toDoubleWithUnit().first;
			return *m_asDouble;
		}

		std::pair<int, std::string> Value::toIntWithUnit()
		{
			if(!m_asIntWithUnit )
			{
				auto retVal = toDoubleWithUnit();
				m_asIntWithUnit= std::pair<int, std::string>( ( int )retVal.first, retVal.second );
			}
			return *m_asIntWithUnit;
		}

		int Value::toInt()
		{
			if(!m_asInt )
				m_asInt=( int )toDouble();
			return *m_asInt;
		}

		const std::string& Value::getName() const
		{
			return m_name;
		}

		bool Value::isValid() const
		{
			return m_isValid;
		}

		Value::operator bool() const
		{
			return isValid();
		}

		bool Value::toBool()
		{
			if(!m_asBool )
			{
				if( atoi( m_rawValue.c_str() ) != 0 )
				{
					m_asBool = true;
				}
				else
				{
					auto rawValueCopy = m_rawValue;
					std::transform( rawValueCopy.begin(), rawValueCopy.end(), rawValueCopy.begin(), ::tolower );
					if( rawValueCopy == "true" )
						m_asBool= true;
				}
				m_asBool=false;
			}
			return *m_asBool;
		}

		Value& Value::operator=(const std::string& rhs)
		{
			m_rawValue = rhs;

			return *this;
		}

		std::string Blueprint::getName() const
		{
			return m_name;
		}

		const std::pair<bool, Value> Blueprint::getValue( const std::string& componentName, const std::string& valueName ) const
		{
			auto it = m_componentValues.find( componentName );
			if( it != m_componentValues.end() )
			{
				auto it2 = it->second.find( valueName );
				if( it2 != it->second.end() )
					return std::pair<bool, Value>( true, it2->second );
			}
			return std::pair<bool, Value>( false, Value() );
		}

		Blueprint::Blueprint( std::string& name, ComponentValuesByNameByComponentMap& componentValuesByNameByComponent ) :m_name( name ),
			m_componentValues( componentValuesByNameByComponent )
		{ }

		Blueprint::Blueprint()
		{ }

		std::string Entity::getName() const
		{
			return m_name;
		}

		const Id Entity::getId() const
		{
			return m_id;
		}

		void Entity::connectToBlueprints( std::map<std::string, Blueprint>& blueprintsByName )
		{
			for( const auto& el : m_inheritsFrom )
			{
				try
				{
					m_inheritedBlueprints.push_back( &blueprintsByName.at( el ) );
				}
				catch( std::exception& e )
				{
					e;
					throw LinkingError( el );
				}
			}
		}

		const std::pair<bool, Value> Entity::getValue( const std::string& componentName, const std::string& valueName ) const
		{
			//search for value in inherited blueprints
			for( auto it = m_inheritedBlueprints.rbegin(); it != m_inheritedBlueprints.rend(); ++it )
			{
				auto value = (*it)->getValue( componentName, valueName );
				if( value.first == true )
					return std::pair<bool, Value>( true, value.second );
			}
			//search for value internally
			auto it = m_componentValues.find( componentName );
			if( it != m_componentValues.end() )
			{
				auto it2 = it->second.find( valueName );
				if( it2 != it->second.end() )
					return std::pair<bool, Value>( true, it2->second );
			}
			return std::pair<bool, Value>( false, Value() );
		}

		Entity::Entity( const Id& id, std::string& name, std::vector<std::string>& inheritsFrom, ComponentValuesByNameByComponentMap& componentValuesByNameByComponent ) :m_id( id ),
			m_name( name ),
			m_inheritsFrom( inheritsFrom ),
			m_componentValues( componentValuesByNameByComponent )
		{ }

		Entity::Entity()
		{ }

		bool ComponentDeclaration::canExecuteOn( Line line, const std::vector<std::string>& lines )
		{
			if( isLineEmpty( lines.at( line ) ) )
				return false;
			for( const char& ch : lines.at( line ) )
				if( !std::isalpha( ch ) && !std::isspace( ch ) )
					return false;
			return true;
		}

		std::pair<Line, std::string> ComponentDeclaration::execute( Line firstLine, const std::vector<std::string>& lines )
		{
			auto retVal = std::make_pair<Line, std::string>( firstLine + 1, std::string() );
			for( const auto& ch : lines.at( firstLine ) )
				if( !std::isspace( ch ) )
					retVal.second.push_back( ch );
			return retVal;
		}

		bool ValueDeclaration::canExecuteOn( Line line, const std::vector<std::string>& lines )
		{
			if( isLineEmpty( lines.at( line ) ) )
				return false;
			bool hasDoublePoint = false;
			for( const char& ch : lines.at( line ) )
				if( ch == ':' )
					hasDoublePoint = true;

			return hasDoublePoint;
		}

		std::pair<Line, Value> ValueDeclaration::execute( Line firstLine, const std::vector<std::string>& lines )
		{
			std::string name;
			std::string value;

			bool afterDoublePoint = false;
			for( const auto& ch : lines.at( firstLine ) )
			{
				if( !afterDoublePoint )
				{
					if( ch == ':' )
						afterDoublePoint = true;
					else if( !std::isspace( ch ) )
						name.push_back( ch );
				}
				else
				{
					value.push_back( ch );
				}
			}

			return std::make_pair<Line, Value>( firstLine + 1, Value( name, value ) );
		}

		bool ComponentDefinition::canExecuteOn( Line line, const std::vector<std::string>& lines )
		{
			auto& lineValue = lines.at( line );
			if( isLineEmpty( lineValue ) )
				return false;
			for( const auto& ch : lineValue )
				if( !std::isspace( ch ) && !std::isalpha( ch ) )
					return false;
			if( !contains( lineValue, "COMPONENT" ) )
				return false;
			return true;
		}

		std::pair<Line, std::pair<std::string, std::vector<Value> >> ComponentDefinition::execute( Line firstLine, const std::vector<std::string>& lines )
		{
			Line line = firstLine;
			auto lineVal = lines.at( line );

			std::string componentName;
			std::vector<Value> componentValues;

			bool afterFirstSpaces = false;
			bool afterComponent = false;
			bool afterSecondSpace = false;
			for( const auto& ch : lineVal )
			{
				if( !afterFirstSpaces )
				{
					if( !std::isspace( ch ) )
						afterFirstSpaces = true;
				}
				else if( !afterComponent )
				{
					if( std::isspace( ch ) )
						afterComponent = true;
				}
				else if( !afterSecondSpace )
				{
					if( !std::isspace( ch ) && std::isalpha( ch ) )
						componentName.push_back( ch );
				}
			}

			line++;
			while( true )
			{
				lineVal = lines.at( line );

				if( contains( lineVal, "END" ) )
					break;
				else
				{
					if( isLineEmpty( lines.at( line ) ) )
					{
						line++;
					}
					else
					{
						auto retVal = ValueDeclaration().execute( line, lines );
						line = retVal.first;
						componentValues.push_back( retVal.second );
					}
				}
			}
			line++;

			return std::pair<Line, std::pair<std::string, std::vector<Value>>>(
				line,
				std::pair<std::string, std::vector<Value>>( componentName, componentValues )
				);
		}

		bool InheritanceDeclaration::canExecuteOn( Line line, const std::vector<std::string>& lines )
		{
			if( isLineEmpty( lines.at( line ) ) )
				return false;
			else
				return contains( lines.at( line ), "INHERIT" );
		}

		std::pair<Line, std::string> InheritanceDeclaration::execute( Line firstLine, const std::vector<std::string>& lines )
		{
			auto& lineVal = lines.at( firstLine );

			std::string entityName;

			bool afterFirstSpaces = false;
			bool afterInherit = false;
			for( const auto& ch : lineVal )
			{
				if( !afterFirstSpaces )
				{
					if( !std::isspace( ch ) )
						afterFirstSpaces = true;
				}
				else if( !afterInherit )
				{
					if( std::isspace( ch ) )
						afterInherit = true;
				}
				else
				{
					if( !std::isspace( ch ) && std::isalpha( ch ) )
						entityName.push_back( ch );
				}
			}

			return std::pair<Line, std::string>( firstLine + 1, entityName );
		}

		bool BlueprintDeclaration::canExecuteOn( Line line, const std::vector<std::string>& lines )
		{
			if( isLineEmpty( lines.at( line ) ) )
				return false;
			else
				return contains( lines.at( line ), "BLUEPRINT" );
		}

		std::pair<Line, Blueprint> BlueprintDeclaration::execute( Line firstLine, const std::vector<std::string>& lines )
		{
			Line line = firstLine;
			auto lineVal = lines.at( line );

			std::string blueprintName;
			ComponentValuesByNameByComponentMap componentValues;

			bool afterFirstSpaces = false;
			bool afterComponent = false;
			bool afterSecondSpace = false;
			for( const auto& ch : lineVal )
			{
				if( !afterFirstSpaces )
				{
					if( !std::isspace( ch ) )
						afterFirstSpaces = true;
				}
				else if( !afterComponent )
				{
					if( std::isspace( ch ) )
						afterComponent = true;
				}
				else if( !afterSecondSpace )
				{
					if( !std::isspace( ch ) && std::isalnum( ch ) )
					{
						afterSecondSpace = true;
						blueprintName.push_back( ch );
					}
				}
				else
				{
					blueprintName.push_back( ch );
				}
			}

			line++;
			while( true )
			{
				lineVal = lines.at( line );

				if( contains( lineVal, "END" ) )
					break;
				else
				{
					if( isLineEmpty( lineVal ) )
					{
						line++;
					}
					else if( ComponentDefinition().canExecuteOn( line, lines ) )
					{
						auto retVal = ComponentDefinition().execute( line, lines );
						line = retVal.first;
						for( auto& el : retVal.second.second )
						{
							componentValues[retVal.second.first][el.getName()] = el;
						}
					}
					else if( ComponentDeclaration().canExecuteOn( line, lines ) )
					{
						auto retVal = ComponentDeclaration().execute( line, lines );
						line = retVal.first;
						componentValues[retVal.second];
					}
					else
						throw ParsingError( line );
				}
			}

			return std::pair<Line, Blueprint>( line + 1,
													   Blueprint(
													   blueprintName,
													   componentValues
													   )
													   );
		}

		bool EntityDeclaration::canExecuteOn( Line line, const std::vector<std::string>& lines )
		{
			return contains( lines.at( line ), "ENTITY" );
		}

		std::pair<Line, Entity> EntityDeclaration::execute( Line firstLine, const std::vector<std::string>& lines )
		{
			Line line = firstLine;
			auto lineVal = lines.at( line );

			std::string entityName;
			std::string entityId;
			ComponentValuesByNameByComponentMap componentValues;
			std::vector<std::string> inheritsFrom;

			bool afterFirstSpaces = false;
			bool afterComponent = false;
			bool afterSecondSpace = false;
			bool afterDoublePoint = false;
			for( const auto& ch : lineVal )
			{
				if( !afterFirstSpaces )
				{
					if( !std::isspace( ch ) )
						afterFirstSpaces = true;
				}
				else if( !afterComponent )
				{
					if( std::isspace( ch ) )
						afterComponent = true;
				}
				else if( !afterSecondSpace )
				{
					if( !std::isspace( ch ) && std::isalnum( ch ) )
					{
						afterSecondSpace = true;
						entityName.push_back( ch );
					}
				}
				else if( !afterDoublePoint )
				{
					if( ch == ':' )
						afterDoublePoint = true;
					else
						entityName.push_back( ch );
				}
				else
				{
					if( std::isdigit( ch ) )
						entityId.push_back( ch );
				}
			}

			if( entityId == "" )
				throw ParsingError( line );

			line++;
			while( true )
			{
				lineVal = lines.at( line );

				if( contains( lineVal, "END" ) )
					break;
				else
				{
					if( isLineEmpty( lineVal ) )
					{
						line++;
					}
					else if( InheritanceDeclaration().canExecuteOn( line, lines ) )
					{
						auto retVal = InheritanceDeclaration().execute( line, lines );
						line = retVal.first;
						inheritsFrom.push_back( retVal.second );
					}
					else if( ComponentDefinition().canExecuteOn( line, lines ) )
					{
						auto retVal = ComponentDefinition().execute( line, lines );
						line = retVal.first;
						for( auto& el : retVal.second.second )
						{
							componentValues[retVal.second.first][el.getName()] = el;
						}
					}
					else if( ComponentDeclaration().canExecuteOn( line, lines ) )
					{
						auto retVal = ComponentDeclaration().execute( line, lines );
						line = retVal.first;
						componentValues[retVal.second];
					}
					else
						throw ParsingError( line );
				}
			}

			return std::pair<Line, Entity>( line + 1,
													Entity( atoi( entityId.c_str() ),
													entityName,
													inheritsFrom,
													componentValues
													) );
		}

		const std::map<Id, Entity>& BlueprintManager::getEntitiesById() const
		{
			return m_entitiesById;
		}

		const std::map<std::string, Blueprint>& BlueprintManager::getBlueprintsByName() const
		{
			return m_blueprintsByName;
		}

		void BlueprintManager::link()
		{
			for( auto& entity : m_entitiesById )
				entity.second.connectToBlueprints( m_blueprintsByName );
		}

		void BlueprintManager::parse( const std::vector<std::string>&lines )
		{
			Line line = 0;

			while( line < lines.size() )
			{
				if( isLineEmpty( lines.at( line ) ) )
				{
					line++;
				}
				else if( EntityDeclaration().canExecuteOn( line, lines ) )
				{
					auto retVal = EntityDeclaration().execute( line, lines );
					line = retVal.first;
					m_entitiesById[retVal.second.getId()] = retVal.second;
				}
				else if( BlueprintDeclaration().canExecuteOn( line, lines ) )
				{
					auto retVal = BlueprintDeclaration().execute( line, lines );
					line = retVal.first;
					m_blueprintsByName[retVal.second.getName()] = retVal.second;
				}
			}
		}

		void BlueprintManager::parse( const std::string& path )
		{
			std::ifstream file;
			file.open( path );

			std::vector<std::string> lines;

			std::string line;
			while( getline( file, line ) )
			{
				lines.push_back( line );
			}

			parse( lines );
		}

		std::vector<std::string> Entity::getComponentNames() const
		{
			std::vector<std::string> returnValue;

			for( const auto& el : m_componentValues )
				returnValue.push_back( el.first );
			for( const auto& el : m_inheritedBlueprints )
				for( const auto& componentName : el->getComponentNames() )
					if( std::find( returnValue.begin(), returnValue.end(), componentName ) == returnValue.end() )
						returnValue.push_back( componentName );

			return returnValue;
		}

		std::map<std::string, Value> Entity::getComponentValues( const std::string componentName ) const
		{
			std::map<std::string, Value> returnValue;
			if( m_componentValues.find( componentName ) != m_componentValues.end() )
				returnValue = m_componentValues.at( componentName );

			for( auto it = m_inheritedBlueprints.rbegin(); it != m_inheritedBlueprints.rend(); ++it )
			{
				auto componentValues = (*it)->getComponentValues( componentName );
				for( const auto& el : componentValues )
					returnValue[el.first] = el.second;
			}

			return returnValue;
		}

		const std::vector<Blueprint*>& Entity::getInheritedBlueprints() const
		{
			return m_inheritedBlueprints;
		}

		bool Entity::inheritsFrom( const std::string& blueprintName )
		{
			for( const auto& el : m_inheritsFrom )
				if( el == blueprintName )
					return true;
			return false;
		}

		const std::map<std::string, Value> Blueprint::getComponentValues( const std::string& componentName ) const
		{
			if( m_componentValues.find( componentName ) != m_componentValues.end() )
				return m_componentValues.at( componentName );
			else
				return std::map<std::string, Value>();
		}

		std::vector<std::string> Blueprint::getComponentNames() const
		{
			std::vector<std::string> returnValue;

			for( const auto& el : m_componentValues )
				returnValue.push_back( el.first );

			return returnValue;
		}

		const Blueprint& BlueprintManager::getBlueprintByName( const std::string& name ) const
		{
			return m_blueprintsByName.at( name );
		}

		const Entity& BlueprintManager::getEntityById( const int id ) const
		{
			return m_entitiesById.at( id );
		}
	}
}
