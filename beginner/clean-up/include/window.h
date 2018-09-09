#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
  Window();

  Window(GLint windowWidth, GLint windowHeight);

  int initialise();

  GLfloat getBufferWidth() { return bufferWidth; }
  GLfloat getBufferHeight() { return bufferHeight; }

  bool shouldClose() {
    return glfwWindowShouldClose(mainWindow);
  }

  void swapBuffers() {
    return glfwSwapBuffers(mainWindow);
  }

  ~Window();

private:
  GLFWwindow *mainWindow;

  GLint width, height;
  int bufferWidth, bufferHeight;
};
