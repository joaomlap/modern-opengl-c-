#include "window.h"

Window::Window() {
  width = 800;
  height = 600;
  lastX = 0.0f;
  lastY = 0.0f;
  xChange = 0.0f;
  yChange = 0.0f;
  mouseFirstMoved = true;

  for (size_t i = 0; i < 1024; i++) {
    keys[i] = 0;
  }
}

Window::Window(GLint windowWidth, GLint windowHeight) {
  width = windowWidth;
  height = windowHeight;
  lastX = 0.0f;
  lastY = 0.0f;
  xChange = 0.0f;
  yChange = 0.0f;
  mouseFirstMoved = true;

  for (size_t i = 0; i < 1024; i++) {
    keys[i] = 0;
  }
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

  // Handle key + mouse inputs
  createCallbacks();
  glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  GLenum result = glewInit();
  if (result != GLEW_OK) {
    printf("Error: GLEW initialisation failed: %s\n", glewGetErrorString(result));
    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    return 1;
  }

  glEnable(GL_DEPTH_TEST);

  // Setup viewport size
  glViewport(0, 0, bufferWidth, bufferHeight);

  glfwSetWindowUserPointer(mainWindow, this);

  return 0;
}

void Window::createCallbacks() {
  glfwSetKeyCallback(mainWindow, handleKeys);
  glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange() {
  GLfloat theChange = xChange;
  xChange = 0.0f;

  return theChange;
}

GLfloat Window::getYChange() {
  GLfloat theChange = yChange;
  yChange = 0.0f;

  return theChange;
}

void Window::handleKeys(
  GLFWwindow* window,
  int key,
  int code,
  int action,
  int mode
) {
  Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
      theWindow->keys[key] = true;
    }

    if (action == GLFW_RELEASE) {
      theWindow->keys[key] = false;
    }
  }
}

void Window::handleMouse(GLFWwindow *window, double xPos, double yPos) {
  Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

  if (theWindow->mouseFirstMoved) {
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
    theWindow->mouseFirstMoved = false;
  }

  theWindow->xChange = xPos - theWindow->lastX;
  theWindow->yChange = theWindow->lastY - yPos;

  theWindow->lastX = xPos;
  theWindow->lastY = yPos;
}

Window::~Window() {
  glfwDestroyWindow(mainWindow);
  glfwTerminate();
}
