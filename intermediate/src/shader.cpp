#include "shader.h"

Shader::Shader() {
  shaderId = 0;
  uniformModel = 0;
  uniformProjection = 0;
  uniformView = 0;
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode) {
  compileShader(vertexCode, fragmentCode);
}

void Shader::compileShader(const char* vertexCode, const char* fragmentCode) {
  shaderId = glCreateProgram();

  if (!shaderId) {
    printf("Error: Could not create shader program!\n");
    return;
  }

  addShader(shaderId, vertexCode, GL_VERTEX_SHADER);
  addShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar eLog[1024] = {0};

  glLinkProgram(shaderId);
  glGetProgramiv(shaderId, GL_LINK_STATUS, &result);

  if (!result) {
    glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
    printf("Error: Could not link program: %s\n", eLog);

    return;
  }

  uniformModel = glGetUniformLocation(shaderId, "model");
  uniformProjection = glGetUniformLocation(shaderId, "projection");
  uniformView = glGetUniformLocation(shaderId, "view");
}

void Shader::addShader(GLuint program, const char *shaderCode, GLenum shaderType) {
  GLuint currShader = glCreateShader(shaderType);
  const GLchar *code[1];
  code[0] = shaderCode; 

  GLint codeLength[1];
  codeLength[0] = strlen(shaderCode);

  glShaderSource(currShader, 1, code, codeLength);
  glCompileShader(currShader);

  GLint result = 0;
  GLchar eLog[1024] = {0};

  glGetShaderiv(currShader, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetShaderInfoLog(currShader, sizeof(eLog), NULL, eLog);
    printf("Error: Could not compile %d shader: %s\n", shaderType, eLog);

    return;
  }

  glAttachShader(program, currShader);
}

void Shader::createFromFiles(const char* vertexLocation, const char* fragmentLocation) {
  std::string vertexString = readFile(vertexLocation);
  std::string fragmentString = readFile(fragmentLocation);

  const char* vertexCode = vertexString.c_str();
  const char* fragmentCode = fragmentString.c_str();

  compileShader(vertexCode, fragmentCode);
}

std::string Shader::readFile(const char* fileLocation) {
  std::string content;
  std::ifstream fileStream(fileLocation, std::ios::in);

  if (!fileStream.is_open()) {
    printf("Error: Failed to read %s!", fileLocation);
  }

  std::string line = "";
  while (!fileStream.eof()) {
    std::getline(fileStream, line);
    content.append(line + "\n");
  }

  fileStream.close();

  return content;
}

GLuint Shader::getProjectionLocation() {
  return uniformProjection;
}

GLuint Shader::getModelLocation() {
  return uniformModel;
}

GLuint Shader::getViewLocation() {
  return uniformView;
}

void Shader::useShader() {
  glUseProgram(shaderId);
}

void Shader::clearShader() {
  if (shaderId != 0) {
    glDeleteProgram(shaderId);
    shaderId = 0;
  }

  uniformModel = 0;
  uniformProjection = 0;
}

Shader::~Shader() {
  clearShader();
}
