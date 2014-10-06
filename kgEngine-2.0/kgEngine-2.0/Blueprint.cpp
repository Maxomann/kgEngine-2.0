#include "Blueprint.h"

namespace kg
{
	namespace blueprint
	{

		bool blueprint::isLineEmpty( const std::string& line )
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

		bool blueprint::contains( const std::string&a, const std::string b )
		{
			return a.find( b ) != std::string::npos;
		}


		blueprint::ParsingError::ParsingError( unsigned int line ) :m_line( line )
		{

		}

		const char* blueprint::ParsingError::what() const
		{
			return ("parsing error on line:" + std::to_string( m_line )).c_str();
		}


		blueprint::LinkingError::LinkingError( std::string blueprintName ) :m_blueprintName( blueprintName )
		{

		}

		const char* blueprint::LinkingError::what() const
		{
			return ("linking error to blueprint:" + m_blueprintName).c_str();
		}


		blueprint::Value::Value()
		{

		}

		blueprint::Value::Value( std::string& name, std::string& rawValue ) :m_name( name ),
			m_rawValue( rawValue )
		{

		}

		std::string blueprint::Value::toStringWithoutBraces() const
		{
			auto retVal = m_rawValue;
			retVal.erase( 0 );
			retVal.pop_back();
			return retVal;
		}

		std::string blueprint::Value::toString() const
		{
			return m_rawValue;
		}

		std::pair<double, std::string> blueprint::Value::toDoubleWithUnit() const
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

			return std::pair<double, std::string>( d, unit );
		}

		double blueprint::Value::toDouble() const
		{
			std::istringstream i( m_rawValue );
			double d = 0;
			if( !(i >> d) )
				d = 0;
			return d;
		}

		std::pair<int, std::string> blueprint::Value::toIntWithUnit() const
		{
			auto retVal = toDoubleWithUnit();
			return std::pair<int, std::string>( ( int )retVal.first, retVal.second );
		}

		int blueprint::Value::toInt() const
		{
			return ( int )toDouble();
		}

		const std::string& blueprint::Value::getName() const
		{
			return m_name;
		}


		std::string blueprint::Blueprint::getName() const
		{
			return m_name;
		}

		const std::pair<bool, Value> blueprint::Blueprint::getValue( const std::string& componentName, const std::string& valueName ) const
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

		blueprint::Blueprint::Blueprint( std::string& name, std::map<std::string, std::map<std::string, Value>>& componentValuesByNameByComponent ) :m_name( name ),
			m_componentValues( componentValuesByNameByComponent )
		{

		}

		blueprint::Blueprint::Blueprint()
		{

		}


		std::string blueprint::Entity::getName() const
		{
			return m_name;
		}

		const unsigned int blueprint::Entity::getId() const
		{
			return m_id;
		}

		void blueprint::Entity::connectToBlueprints( std::map<std::string, Blueprint>& blueprintsByName )
		{
			for( const auto& el : m_inheritsFrom )
			{
				try
				{
					m_inheritedBlueprints.push_back( &blueprintsByName.at( el ) );
				}
				catch( std::exception& e )
				{
					throw LinkingError( el );
				}
			}
		}

		const std::pair<bool, Value> blueprint::Entity::getValue( const std::string& componentName, const std::string& valueName ) const
		{
			//search for value internally
			auto it = m_componentValues.find( componentName );
			if( it != m_componentValues.end() )
			{
				auto it2 = it->second.find( valueName );
				if( it2 != it->second.end() )
					return std::pair<bool, Value>( true, it2->second );
			}
			//search for value in inherited blueprints
			for( auto it = m_inheritedBlueprints.rbegin(); it != m_inheritedBlueprints.rend(); ++it )
			{
				auto value = (*it)->getValue( componentName, valueName );
				if( value.first == true )
					return std::pair<bool, Value>( true, value.second );
			}
			return std::pair<bool, Value>( false, Value() );
		}

		blueprint::Entity::Entity( const unsigned int& id, std::string& name, std::vector<std::string>& inheritsFrom, std::map<std::string, std::map<std::string, Value>>& componentValuesByNameByComponent ) :m_id( id ),
			m_name( name ),
			m_inheritsFrom( inheritsFrom ),
			m_componentValues( componentValuesByNameByComponent )
		{

		}

		blueprint::Entity::Entity()
		{

		}


		bool blueprint::ComponentDeclaration::canExecuteOn( unsigned int line, const std::vector<std::string>& lines )
		{
			if( isLineEmpty( lines.at( line ) ) )
				return false;
			for( const char& ch : lines.at( line ) )
				if( !std::isalpha( ch ) && !std::isspace( ch ) )
					return false;
			return true;
		}

		std::pair<unsigned int, std::string> blueprint::ComponentDeclaration::execute( unsigned int firstLine, const std::vector<std::string>& lines )
		{
			auto retVal = std::make_pair<unsigned int, std::string>( firstLine + 1, std::string() );
			for( const auto& ch : lines.at( firstLine ) )
				if( !std::isspace( ch ) )
					retVal.second.push_back( ch );
			return retVal;
		}


		bool blueprint::ValueDeclaration::canExecuteOn( unsigned int line, const std::vector<std::string>& lines )
		{
			if( isLineEmpty( lines.at( line ) ) )
				return false;
			bool hasDoublePoint = false;
			for( const char& ch : lines.at( line ) )
				if( ch == ':' )
					hasDoublePoint = true;

			return hasDoublePoint;
		}

		std::pair<unsigned int, Value> blueprint::ValueDeclaration::execute( unsigned int firstLine, const std::vector<std::string>& lines )
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

			return std::make_pair<unsigned int, Value>( firstLine + 1, Value( name, value ) );
		}


		bool blueprint::ComponentDefinition::canExecuteOn( unsigned int line, const std::vector<std::string>& lines )
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

		std::pair<unsigned int, std::pair<std::string, std::vector<Value> >> blueprint::ComponentDefinition::execute( unsigned int firstLine, const std::vector<std::string>& lines )
		{
			unsigned int line = firstLine;
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

			return std::pair<unsigned int, std::pair<std::string, std::vector<Value>>>(
				line,
				std::pair<std::string, std::vector<Value>>( componentName, componentValues )
				);
		}


		bool blueprint::InheritanceDeclaration::canExecuteOn( unsigned int line, const std::vector<std::string>& lines )
		{
			if( isLineEmpty( lines.at( line ) ) )
				return false;
			else
				return contains( lines.at( line ), "INHERIT" );
		}

		std::pair<unsigned int, std::string> blueprint::InheritanceDeclaration::execute( unsigned int firstLine, const std::vector<std::string>& lines )
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

			return std::pair<unsigned int, std::string>( firstLine + 1, entityName );
		}


		bool blueprint::BlueprintDeclaration::canExecuteOn( unsigned int line, const std::vector<std::string>& lines )
		{
			if( isLineEmpty( lines.at( line ) ) )
				return false;
			else
				return contains( lines.at( line ), "BLUEPRINT" );
		}

		std::pair<unsigned int, Blueprint> blueprint::BlueprintDeclaration::execute( unsigned int firstLine, const std::vector<std::string>& lines )
		{
			unsigned int line = firstLine;
			auto lineVal = lines.at( line );

			std::string blueprintName;
			std::map<std::string, std::map<std::string, Value>> componentValues;

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

			return std::pair<unsigned int, Blueprint>( line + 1,
													   Blueprint(
													   blueprintName,
													   componentValues
													   )
													   );
		}


		bool blueprint::EntityDeclaration::canExecuteOn( unsigned int line, const std::vector<std::string>& lines )
		{
			return contains( lines.at( line ), "ENTITY" );
		}

		std::pair<unsigned int, Entity> blueprint::EntityDeclaration::execute( unsigned int firstLine, const std::vector<std::string>& lines )
		{
			unsigned int line = firstLine;
			auto lineVal = lines.at( line );

			std::string entityName;
			std::string entityId;
			std::map<std::string, std::map<std::string, Value>> componentValues;
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

			return std::pair<unsigned int, Entity>( line + 1,
													Entity( atoi( entityId.c_str() ),
													entityName,
													inheritsFrom,
													componentValues
													) );
		}


		const std::map<unsigned int, Entity>& blueprint::BlueprintManager::getEntitiesById() const
		{
			return m_entitiesById;
		}

		const std::map<std::string, Blueprint>& blueprint::BlueprintManager::getBlueprintsByName() const
		{
			return m_blueprintsByName;
		}

		void blueprint::BlueprintManager::link()
		{
			for( auto& entity : m_entitiesById )
				entity.second.connectToBlueprints( m_blueprintsByName );
		}

		void blueprint::BlueprintManager::parse( const std::vector<std::string>&lines )
		{
			unsigned int line = 0;

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

		void blueprint::BlueprintManager::parse( const std::string& path )
		{
			std::ifstream file;
			file.open( "test.blueprint" );

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

			return returnValue;
		}

		std::map<std::string, Value> Entity::getComponentValues( const std::string componentName ) const
		{
			std::map<std::string, Value> returnValue = m_componentValues.at(componentName);

			for( auto it = m_inheritedBlueprints.rbegin(); it != m_inheritedBlueprints.rend(); ++it )
			{
				auto componentValues=(*it)->getComponentValues(componentName);
				for( const auto& el : componentValues )
					returnValue[el.first] = el.second;
			}

			return returnValue;
		}


		const std::map<std::string, Value>& Blueprint::getComponentValues( const std::string& componentName ) const
		{
			return m_componentValues.at(componentName);
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