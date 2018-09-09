#pragma once

#include <GL/glew.h>

class Mesh {
public:
	Mesh();

	void createMesh(
		GLfloat *vertices,
		unsigned int *indices,
		unsigned int nVertices,
		unsigned int nIndices
	);
	void renderMesh();
	void clearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};
