#include <cmath>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "shader.h"
#include "window.h"
#include "camera.h"

// Window dimensions
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char *vShader = "shaders/vertex.glsl";

// Fragment Shader
static const char *fShader = "shaders/fragment.glsl";

void createObjects() {
  unsigned int indices[] = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
		0, 1, 2
  };

  GLfloat vertices[] = {
    // indice 0
    -1.0, -1.0f, 0.0f,
    // indice 1
    0.0f, -1.0f, 1.0f,
    // indice 2
    1.0, -1.0f, 0.0f,
    // indice 3
    0.0, 1.0f, 0.0f,
  };

  Mesh* obj1 = new Mesh();
  obj1->createMesh(vertices, indices, 12, 12);
  meshList.push_back(obj1);

  Mesh* obj2 = new Mesh();
  obj2->createMesh(vertices, indices, 12, 12);
  meshList.push_back(obj2);
}

void createShaders() {
  Shader *shader1 = new Shader();
  shader1->createFromFiles(vShader, fShader);
  shaderList.push_back(*shader1);
}

int main() {
  mainWindow = Window();
  mainWindow.initialise();

  createObjects();
  createShaders();
  
  camera = Camera(
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    -90.0f,
    0.0f,
    5.0f,
    0.5f
  );

  GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

  GLfloat aspectRactio = (GLfloat) mainWindow.getBufferWidth() / mainWindow.getBufferHeight();
  glm::mat4 projection = glm::perspective(45.0f, aspectRactio, 0.1f, 100.0f);

  // Loop until window close
  while (!mainWindow.shouldClose()) {
    GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
    deltaTime = now - lastTime; // (now - lastTime) * 1000 / SDL_GetPerformanceFrequency();
    lastTime = now;

    // Get and handle user input events
    glfwPollEvents();

    camera.keyControl(mainWindow.getKeys(), deltaTime);
    camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
    
    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderList[0].useShader();
    uniformModel = shaderList[0].getModelLocation();
    uniformProjection = shaderList[0].getProjectionLocation();
    uniformView = shaderList[0].getViewLocation();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.25f, -2.5f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    // bind uniform variables
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
    meshList[0]->renderMesh();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.6f, -2.5f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    meshList[1]->renderMesh();

    glUseProgram(0);

    mainWindow.swapBuffers();
  }

  return 0;
}
