#include "VBO.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	void VBO::generate( unsigned int vertexCapacity, GLenum usage )
	{
		m_isGenerated = true;

		m_vertexCapaxity = vertexCapacity;
		glGenBuffers( 1, &m_glId );
		bind();
		glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex )*vertexCapacity, NULL, usage );
	}

	void VBO::destroy()
	{
		m_isGenerated = false;

		unmap();
		unbind();
		glDeleteBuffers( 1, &m_glId );
	}

	VBO::VBO( unsigned int vertexCapacity, GLenum usage )
	{
		generate( vertexCapacity, usage );
	}

	VBO::~VBO()
	{
		destroy();
	}

	void VBO::bind()
	{
		m_isBound = true;
		glBindBuffer( GL_ARRAY_BUFFER, m_glId );
	}

	void VBO::unbind()
	{
		m_isBound = false;
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	sf::Vertex* VBO::map()
	{
		m_isMapped = true;
		return ( Vertex* )glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	}

	void VBO::unmap()
	{
		m_isMapped = false;
		glUnmapBuffer( GL_ARRAY_BUFFER );
	}
}