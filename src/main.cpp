#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bits/types/FILE.h>
#include <cstdlib>
#include <glm/vec2.hpp>

#include <cstdio>
#include <iostream>

// Window dimentions
const GLint WIDTH = 800, HEIGHT = 600;
// Number of points used to draw the Sierpinski gasket
const int POINTS = 10000;

static char *readShaderSource(const char *shaderFile) {
  FILE *pFile = fopen(shaderFile, "rb");

  if (pFile == NULL) {
    return NULL;
  }

  fseek(pFile, 0L, SEEK_END);
  long size = ftell(pFile);

  rewind(pFile);
  char *buf = new char[size + 1];
  fread(buf, 1, size, pFile);
  buf[size] = '\0';
  fclose(pFile);

  return buf;
}

// Create a GLSL program from shaders
GLuint createProgram(const char *vertexShaderFile, const char *fragmentShaderFile) {
  struct Shader {
    const char *filename;
    GLenum      type;
    GLchar     *source;
  } shaders[2] = {
    {vertexShaderFile, GL_VERTEX_SHADER, NULL},
    {fragmentShaderFile, GL_FRAGMENT_SHADER, NULL}
  };

  GLuint program = glCreateProgram();

  for(int i = 0; i < 2; ++i) {
    Shader& s = shaders[i];
    s.source = readShaderSource(s.filename);
    if (shaders[i].source == NULL) {
      std::cerr << "Failed to read " << s.filename << std::endl;
      exit(EXIT_FAILURE);
    }

    GLuint shader = glCreateShader(s.type);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
      std::cerr << s.filename << " failed to compile" << std::endl;
      GLint logLength;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
      char *logMsg = new char[logLength];
      glGetShaderInfoLog(shader, logLength, NULL, logMsg);
      std::cerr << logMsg << std::endl;
      delete[] logMsg;

      exit(EXIT_FAILURE);
    }

    delete[] s.source;
    glAttachShader(program, shader);
  }

  glLinkProgram(program);

  GLint linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (!linked) {
    std::cerr << "Shader program failed to link" << std::endl;
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    char *logMsg = new char[logLength];
    glGetProgramInfoLog(program, logLength, NULL, logMsg);
    std::cerr << logMsg << std::endl;
    delete[] logMsg;

    exit(EXIT_FAILURE);
  }

  glUseProgram(program);

  return program;
}

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
