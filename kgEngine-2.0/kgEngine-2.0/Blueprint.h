#pragma once
#include "stdafx.h"

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

		//checks if string a contains string b
		bool contains( const std::string&a, const std::string b )
		{
			return a.find( b ) != std::string::npos;
		}


		class ParsingError : public std::exception
		{
			unsigned int m_line;

		public:
			ParsingError( unsigned int line )
				:m_line( line )
			{ };

			const char* what()const override
			{
				return ("parsing error on line:"+std::to_string( m_line )).c_str();
			};
		};

		class LinkingError : public std::exception
		{
			std::string m_blueprintName;
		public:
			LinkingError( std::string blueprintName )
				:m_blueprintName( blueprintName )
			{ };

			const char* what()const override
			{
				return ("linking error to blueprint:" + m_blueprintName).c_str();
			};

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


		class Value
		{
			std::string m_name;
			std::string m_rawValue;

		public:
			Value()
			{ };
			Value( std::string& name,
				   std::string& rawValue )
				   :m_name(name),
				   m_rawValue(rawValue)
			{ };

			const std::string& getName()const
			{
				return m_name;
			};

			int toInt()const
			{
				( int )toDouble();
			};
			std::pair<int, std::string> toIntWithUnit()const
			{
				auto retVal = toDoubleWithUnit();
				return std::pair<int, std::string>( ( int )retVal.first, retVal.second );
			};

			double toDouble()const
			{
				std::istringstream i( m_rawValue );
				double d = 0;
				if( !(i >> d) )
					d = 0;
				return d;
			};
			std::pair<double, std::string> toDoubleWithUnit()const
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
			};

			std::string toString()const
			{
				return m_rawValue;
			};
			std::string toStringWithoutBraces()const
			{
				auto retVal = m_rawValue;
				retVal.erase( 0 );
				retVal.pop_back();
				return retVal;
			};
		};

		class Blueprint
		{
			std::string m_name;
			std::map<std::string, std::map<std::string, Value>> m_componentValues;
		public:
			Blueprint()
			{ };
			Blueprint( std::string& name,
					   std::map<std::string, std::map<std::string, Value>>& componentValuesByNameByComponent )
					   :m_name( name ),
					   m_componentValues( componentValuesByNameByComponent )
			{ };

			const std::pair<bool, Value> getValue( const std::string& componentName, const std::string& valueName )const
			{
				auto it = m_componentValues.find( componentName );
				if( it != m_componentValues.end() )
				{
					auto it2=it->second.find( valueName );
					if( it2 != it->second.end() )
						return std::pair<bool, Value>( true, it2->second );
				}
				return std::pair<bool, Value>( false, Value() );
			}

			std::string getName()const
			{
				return m_name;
			};

		};

		class Entity
		{
			unsigned int m_id;
			std::string m_name;
			std::vector<std::string> m_inheritsFrom;//in ascending override importance
			std::vector<Blueprint*> m_inheritedBlueprints;//in ascending override importance
			std::map<std::string, std::map<std::string, Value>> m_componentValues;
		public:
			Entity()
			{ };
			Entity( const unsigned int& id,
					std::string& name,
					std::vector<std::string>& inheritsFrom,
					std::map<std::string, std::map<std::string, Value>>& componentValuesByNameByComponent )
					:m_id( id ),
					m_name( name ),
					m_inheritsFrom( inheritsFrom ),
					m_componentValues( componentValuesByNameByComponent )
			{ };

			const std::pair<bool, Value> getValue( const std::string& componentName, const std::string& valueName )const
			{
				//search for value internally
				auto it = m_componentValues.find( componentName );
				if( it != m_componentValues.end() )
				{
					auto it2 = it->second.find( valueName );
					if( it2 != it->second.end() )
						return std::pair<bool, Value>(true, it2->second);
				}
				//search for value in inherited blueprints
				for( auto it = m_inheritedBlueprints.rbegin(); it != m_inheritedBlueprints.rend(); ++it )
				{
					auto value = (*it)->getValue( componentName, valueName );
					if( value.first == true )
						return std::pair<bool, Value>( true, value.second);
				}
				return std::pair<bool, Value>( false, Value() );
			}

			void connectToBlueprints( std::map<std::string, Blueprint>& blueprintsByName )
			{
				for( const auto& el : m_inheritsFrom )
				{
					try
					{
						m_inheritedBlueprints.push_back( &blueprintsByName.at( el ));
					}
					catch( std::exception& e )
					{
						throw LinkingError( el );
					}
				}
					
			};

			std::string getName()const
			{
				return m_name;
			};
		};


		///////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////


		class ComponentDeclaration : public Command < std::string >
		{
		public:
			virtual std::pair<unsigned int, std::string> execute( unsigned int firstLine,
																  const std::vector<std::string>& lines )
			{
				auto retVal = std::make_pair<unsigned int, std::string>( firstLine + 1, std::string() );
				for( const auto& ch : lines.at( firstLine ) )
					if( !std::isspace( ch ) )
						retVal.second.push_back( ch );
				return retVal;
			}

			//can execute on any line that is not empty and does only contain alphanumeric characters
			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines )
			{
				if( isLineEmpty( lines.at( line ) ) )
					return false;
				for( const char& ch : lines.at( line ) )
					if( !std::isalpha( ch ) && !std::isspace( ch ) )
						return false;
				return true;
			}
		};

		class ValueDeclaration : public Command < Value >
		{
		public:
			virtual std::pair<unsigned int, Value> execute( unsigned int firstLine,
															const std::vector<std::string>& lines )
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

			//can execute on any line that is not empty and does contain at least one double point
			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines )
			{
				if( isLineEmpty( lines.at( line ) ) )
					return false;
				bool hasDoublePoint = false;
				for( const char& ch : lines.at( line ) )
					if( ch == ':' )
						hasDoublePoint = true;

				return hasDoublePoint;
			}

		};

		class ComponentDefinition : public Command < std::pair<std::string, std::vector<Value> > >
		{
		public:
			virtual std::pair<unsigned int, std::pair<std::string, std::vector<Value> >> execute( unsigned int firstLine,
																								  const std::vector<std::string>& lines )
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
						if( isLineEmpty( lines.at(line) ) )
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
					std::pair<std::string, std::vector<Value>>(componentName, componentValues )
					);
			}

			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines )
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

		};

		class InheritanceDeclaration : public Command < std::string >
		{
		public:
			virtual std::pair<unsigned int, std::string> execute( unsigned int firstLine,
																  const std::vector<std::string>& lines )
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

			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines )
			{
				if( isLineEmpty( lines.at( line ) ) )
					return false;
				else
					return contains( lines.at( line ), "INHERIT" );
			}

		};

		class BlueprintDeclaration :public Command < Blueprint >
		{
		public:
			virtual std::pair<unsigned int, Blueprint> execute( unsigned int firstLine,
																const std::vector<std::string>& lines )
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
						if( !std::isspace( ch ) && std::isalpha( ch ) )
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
								componentValues[retVal.second.first][el.getName()]=el;
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

				return std::pair<unsigned int, Blueprint>( line+1,
														   Blueprint(
														   blueprintName,
														   componentValues
														   )
														   );
			}

			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines )
			{
				if( isLineEmpty( lines.at( line ) ) )
					return false;
				else
					return contains( lines.at( line ), "BLUEPRINT" );
			}

		};

		class EntityDeclaration :public Command < Entity >
		{
		public:
			virtual std::pair<unsigned int, Entity> execute( unsigned int firstLine,
															 const std::vector<std::string>& lines )
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
						if( !std::isspace( ch ) && std::isalpha( ch ) )
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

			virtual bool canExecuteOn( unsigned int line,
									   const std::vector<std::string>& lines )
			{
				return contains( lines.at( line ), "ENTITY" );
			}

		};

		class BlueprintManager
		{
			std::map<std::string, Blueprint> m_blueprintsByName;
			std::map<std::string, Entity> m_entitiesByName;

		public:
			// can throw parsing error
			void parse( const std::vector<std::string>&lines )
			{
				unsigned int line = 0;

				while( line < lines.size() )
				{
					if( isLineEmpty( lines.at( line ) ))
					{
						line++;
					}
					else if( EntityDeclaration().canExecuteOn( line, lines ) )
					{
						auto retVal = EntityDeclaration().execute( line, lines );
						line = retVal.first;
						m_entitiesByName[retVal.second.getName()] = retVal.second;
					}
					else if( BlueprintDeclaration().canExecuteOn( line, lines ) )
					{
						auto retVal = BlueprintDeclaration().execute( line, lines );
						line = retVal.first;
						m_blueprintsByName[retVal.second.getName()] = retVal.second;
					}
				}
			};

			// should be called after all files have been parsed
			// links entities to blueprints
			// can throw linking error
			void link()
			{
				for( auto& entity : m_entitiesByName )
						entity.second.connectToBlueprints( m_blueprintsByName );	
			}

			const std::map<std::string, Blueprint>& getBlueprintsByName()const
			{
				return m_blueprintsByName;
			};
			const std::map<std::string, Entity>& getEntitiesByName()const
			{
				return m_entitiesByName;
			};

		};
	}
}