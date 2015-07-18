#pragma once
#include "stdafx.h"

namespace kg
{
	template<class Object>
	class Node;

	template< class Object >
	class QuadTreeUser
	{
		Node<Object>* m_node = nullptr;

	public:
		virtual ~QuadTreeUser();

		Node<Object>* getNode()const;
		void setNode( Node<Object>* node );
	};

	template< class Object >
	kg::QuadTreeUser<Object>::~QuadTreeUser()
	{

	}

	template< class Object >
	Node<Object>* kg::QuadTreeUser<Object>::getNode() const
	{
		return m_node;
	}

	template< class Object >
	void kg::QuadTreeUser<Object>::setNode( Node<Object>* node )
	{
		m_node = node;
	}

	template< class Object >
	class Node
	{
	public:
		typedef std::function<bool( Node<Object>&, Object& )> ContainRule;

	private:
		std::vector<Node<Object>> m_subNodes;
		bool m_hasSplit = false;
		std::vector<Object> m_objects;

		const sf::Uint16 max_entities_until_split;

		ContainRule contain_rule;

		void split();
	public:
		Node( const sf::Uint16& max_entities_until_split, ContainRule contain_rule );

		void addObject( Object& object );
		void removeObject( Object& object );
	};

	template< class Object >
	kg::Node<Object>::Node( const sf::Uint16& max_entities_until_split, ContainRule contain_rule )
		:max_entities_until_split( max_entities_until_split ),
		contain_rule( contain_rule )
	{ }

	template< class Object >
	void kg::Node<Object>::addObject( Object& object )
	{
		if( m_hasSplit )
		{
			for( auto& node : m_subNodes )
			{
				if( contain_rule( node, object ) )
				{
					node.addObject( object );
					return;
				}
			}
			object.setNode( this );
			m_objects.push_back( object );
		}
		else
		{
			object.setNode( this );
			m_objects.push_back( object );
			if( m_objects.size() > max_entities_until_split )
				split();
		}
	}

	template< class Object >
	void kg::Node<Object>::removeObject( Object& object )
	{
		m_object.erase( std::remove( std::begin( m_objects ), std::end( m_objects ), object ), std::end( m_objects ) );
	}

	template< class Object >
	void kg::Node<Object>::split()
	{
		if( m_hasSplit )
			throw exception();

		m_subNodes = decltype(m_subNodes)(4, Node( max_entities_until_split, contain_rule ));
		std::vector<decltype(m_objects)::iterator> toRemove;
		for( auto it = begin( m_objects ); it != end( m_objects );++it )
		{
			for( auto& node : m_subNodes )
			{
				if( contain_rule( node, *it ) )
				{
					node.addObject( *it );
					toRemove.push_back( it );
					break;
				}
			}
		}
		for( auto it = toRemove.rbegin(); it != toRemove.rend(); ++it )
			m_objects.erase( std::remove( begin( m_objects ), end( m_objects ), **it ), end(m_objects) );

		m_hasSplit = true;
	}

	template< class Object >
	using QuadTree = Node<Object>;
}
