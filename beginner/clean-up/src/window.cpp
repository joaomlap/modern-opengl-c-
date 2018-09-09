#include "window.h"

Window::Window() {
  width = 800;
  height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
  width = windowWidth;
  height = windowHeight;
}

int Window::initialise() {
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

  mainWindow = glfwCreateWindow(width, height, "OpenGL João", NULL, NULL);

  if (!mainWindow) {
    printf("Error: GLFW window creation failed!\n");
    glfwTerminate();

    return 1;
  }

  // Get buffer size information
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

  return 0;
}

Window::~Window() {
  glfwDestroyWindow(mainWindow);
  glfwTerminate();
}
