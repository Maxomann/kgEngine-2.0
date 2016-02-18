#pragma once
#include "stdafx.h"

namespace kg
{
	class VBO
	{
	private:
		bool m_isGenerated = false;
		bool m_isBound = false;
		bool m_isMapped = false;

		GLuint m_glId;
		unsigned int m_vertexCapaxity = 0;

		void generate( unsigned int vertexCapacity, GLenum usage );
		void destroy();

	public:
		VBO( unsigned int vertexCapacity, GLenum usage );
		~VBO();

		void bind();
		void unbind();

		sf::Vertex* map();
		void unmap();

		bool isGenerated()const;
		bool isBound()const;
		bool isMapped()const;
	};
}