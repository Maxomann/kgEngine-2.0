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

		sf::Vertex* m_ptr;

	public:
		VBO() = default;
		VBO( unsigned int vertexCapacity, GLenum usage );
		~VBO();

		void generate( unsigned int vertexCapacity, GLenum usage );
		void destroy();

		void bind();
		void unbind();

		void map();
		void unmap();

		// retruns nullptr if !isMapped()
		sf::Vertex* ptr();

		bool isGenerated()const;
		bool isBound()const;
		bool isMapped()const;
	};
}