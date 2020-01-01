#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimentions
const GLint WIDTH = 800, HEIGHT = 600;

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cout << "GLFW initialisation failed";
    glfwTerminate();
    return 1;
  }

  // Setup GLFW window properties
  // OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Core profile = no backwards compatibility
  glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Allow forward compatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *mainWindow =
      glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);

  if (!mainWindow) {
    std::cout << "GLFW window creation failed!";
    glfwTerminate();
    return 1;
  }

  // Get Buffer size information
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  // Set context for GLEW to use
  glfwMakeContextCurrent(mainWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW initialisation failed!";
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 1;
  }

  // Setup Viewport size
  glViewport(0, 0, bufferWidth, bufferHeight);

  // Loop until window closed
  while (!glfwWindowShouldClose(mainWindow)) {
    // Get + Handle user input events
    glfwPollEvents();

    // Clear window
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(mainWindow);
  }

  return 0;
}
