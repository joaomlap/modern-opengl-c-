#include <cmath>
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.001f;
float currAngle = 0.0f;

// Vertex Shader
static const char *vShader = "                                        \n\
#version 410                                                          \n\
layout (location = 0) in vec3 pos;                                    \n\
                                                                      \n\
out vec4 vCol;                                                        \n\
                                                                      \n\
uniform mat4 model;                                                   \n\
uniform mat4 projection;                                              \n\
                                                                      \n\
void main() {                                                         \n\
  gl_Position = projection * model * vec4(pos.x, pos.y, pos.z, 1.0);  \n\
  vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);                          \n\
}";

// Fragment Shader
static const char *fShader = "                              \n\
#version 410                                                \n\
                                                            \n\
in vec4 vCol;                                               \n\
                                                            \n\
out vec4 colour;                                            \n\
                                                            \n\
void main() {                                               \n\
  colour = vCol;                                            \n\
}";

void createTriangle() {
  unsigned int indices[] = {0, 3, 1, 1, 3, 2, 2, 3, 0};

  GLfloat vertices[] = {
      // indice 0
      -1.0,
      -1.0f,
      0.0f,
      // indice 1
      0.0f,
      -1.0f,
      1.0f,
      // indice 2
      1.0,
      -1.0f,
      0.0f,
      // indice 3
      0.0,
      1.0f,
      0.0f,
  };

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

  glEnableVertexAttribArray(0);
}

void addShader(GLuint program, const char *shaderCode, GLenum shaderType) {
  GLuint currShader = glCreateShader(shaderType);
  const GLchar *code[0];
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

void compileShaders() {
  shader = glCreateProgram();

  if (!shader) {
    printf("Error: Could not create shader program!\n");
    return;
  }

  addShader(shader, vShader, GL_VERTEX_SHADER);
  addShader(shader, fShader, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar eLog[1024] = {0};

  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &result);

  if (!result) {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error: Could not link program: %s\n", eLog);

    return;
  }

  glValidateProgram(shader);
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

  if (!result) {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error: Could not validate program: %s\n", eLog);

    return;
  }

  uniformModel = glGetUniformLocation(shader, "model");
  uniformProjection = glGetUniformLocation(shader, "projection");
}

int main() {
  if (!glfwInit()) {
    printf("Error: GLFW initialisation failed!\n");
    glfwTerminate();

    return 1;
  }

  // Setup GLfw window properties
  // OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Core profile means no backwards compatibility
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Allow forward compatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL João", NULL, NULL);

  if (!mainWindow) {
    printf("Error: GLFW window creation failed!\n");
    glfwTerminate();

    return 1;
  }

  // Get buffer size information
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  // Set context for GLEW to use
  glfwMakeContextCurrent(mainWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    printf("Error: GLEW initialisation failed!\n");
    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    return 1;
  }

  glEnable(GL_DEPTH_TEST);

  // Setup viewport size
  glViewport(0, 0, bufferWidth, bufferHeight);

  createTriangle();
  compileShaders();

  GLfloat aspectRactio = (GLfloat)bufferWidth / (GLfloat)bufferHeight;
  glm::mat4 projection = glm::perspective(45.0f, aspectRactio, 0.1f, 100.0f);

  // Loop until window close
  while (!glfwWindowShouldClose(mainWindow)) {
    // Get and handle user input events
    glfwPollEvents();

    if (direction) {
      triOffset += triIncrement;
    } else {
      triOffset -= triIncrement;
    }

    if (abs(triOffset) >= triMaxOffset) {
      direction = !direction;
    }

    currAngle += 0.1f;
    if (currAngle >= 360) {
      currAngle -= 360;
    }

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
    model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4, 0.4, 1));

    // bind uniform variable
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBindVertexArray(0);

    // glUseProgram(0);

    glfwSwapBuffers(mainWindow);
  }

  // Cleanup
  glDeleteBuffers(1, &IBO);
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteProgram(shader);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}
