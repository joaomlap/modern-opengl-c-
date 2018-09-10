#pragma once 

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader {
public:
  Shader();

  void createFromString(
    const char* vertexCode,
    const char* fragmentCode
  );
  void createFromFiles(
    const char* vertexLocation,
    const char* fragmentLocation
  );

  std::string readFile(const char* fileLocation);

  GLuint getProjectionLocation();
  GLuint getModelLocation();
  GLuint getViewLocation();

  void useShader();
  void clearShader();

  ~Shader();

private:
  GLuint shaderId, uniformProjection, uniformModel, uniformView;

  void compileShader(
    const char* vertexCode,
    const char* fragmentCode
  );
  void addShader(
    GLuint program,
    const char *shaderCode,
    GLenum shaderType
  );
};
