#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <stdio.h>
#include <string.h>
#include <list>
#include <cmath>

// Triangle class.
class Triangle {
public:
  Triangle(GLfloat vs[]);
  glm::vec3 p1, p2, p3;
};

Triangle::Triangle(GLfloat vs[]){
  this->p1 = glm::vec3(vs[0], vs[1], vs[2]);
  this->p2 = glm::vec3(vs[3], vs[4], vs[5]);
  this->p3 = glm::vec3(vs[6], vs[7], vs[8]);
}

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

short iterations = 8;
bool updateIter = false;

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

void createTriangle() {
  GLfloat vertices[] = {-1.0f, -1.0f, 0.0f,
                         1.0f, -1.0f, 0.0f,
                         0.0f,  1.0f, 0.0f};

  Triangle t(vertices);
  std::list<Triangle> lt;
  lt.push_back(t);

  // subdivide triangle
  for (int i = 0; i < iterations; ++i){
    short s = lt.size();

    for (int j = 0; j < s; ++j){
      glm::vec3 p12 = (lt.front().p1 + lt.front().p2) / 2.0f;
      glm::vec3 p13 = (lt.front().p1 + lt.front().p3) / 2.0f;
      glm::vec3 p23 = (lt.front().p2 + lt.front().p3) / 2.0f;

      GLfloat t1[] = {lt.front().p1.x, lt.front().p1.y, lt.front().p1.z,
                      p12.x, p12.y, p12.z,
                      p13.x, p13.y, p13.z};
      GLfloat t2[] = {p12.x, p12.y, p12.z,
                      lt.front().p2.x, lt.front().p2.y, lt.front().p2.z,
                      p23.x, p23.y, p23.z};
      GLfloat t3[] = {p13.x, p13.y, p13.z,
                      p23.x, p23.y, p23.z,
                      lt.front().p3.x, lt.front().p3.y, lt.front().p3.z};

      lt.push_back(Triangle(t1));
      lt.push_back(Triangle(t2));
      lt.push_back(Triangle(t3));
      lt.pop_front();
    }
  }

  // transform triangle vector in an array
  short listSize = lt.size();
  GLfloat vertexArray[listSize * 9];
  for (int i = 0; i < listSize; ++i){
    vertexArray[i * 9 + 0] = lt.front().p1.x;
    vertexArray[i * 9 + 1] = lt.front().p1.y;
    vertexArray[i * 9 + 2] = lt.front().p1.z;
    vertexArray[i * 9 + 3] = lt.front().p2.x;
    vertexArray[i * 9 + 4] = lt.front().p2.y;
    vertexArray[i * 9 + 5] = lt.front().p2.z;
    vertexArray[i * 9 + 6] = lt.front().p3.x;
    vertexArray[i * 9 + 7] = lt.front().p3.y;
    vertexArray[i * 9 + 8] = lt.front().p3.z;
    lt.pop_front();
  }

  // binding
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // unbinding
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void addShader(GLuint theProgram, const char *shaderFile, GLenum shaderType) {
  GLuint theShader = glCreateShader(shaderType);

  const GLchar *theCode[1];
  theCode[0] = readShaderSource(shaderFile);

  glShaderSource(theShader, 1, theCode, NULL);
  glCompileShader(theShader);

  GLint result = 0;
  GLchar eLog[1024] = {0};

  glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
    printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
    return;
  }

  glAttachShader(theProgram, theShader);
}

void compileShaders() {
  shader = glCreateProgram();

  if (!shader) {
    printf("Error creating shader program!");
    return;
  }

  addShader(shader, "shaders/basic.vs", GL_VERTEX_SHADER);
  addShader(shader, "shaders/basic.fs", GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar eLog[1024] = { 0 };

  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error linking program: '%s'\n", eLog);
    return;
  }

  glValidateProgram(shader);
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error validating program: '%s'\n", eLog);
    return;
  }
}

void updateKeyboard(GLFWwindow *window, int key, int scancode, int action,
                    int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        if (iterations < 9) { // greater number of iterations cause segmentation fault
          iterations++;
          updateIter = true;
        }
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        if (iterations > 1) {
          iterations--;
          updateIter = true;
        }
    }
}


int main() {
  // Initialise GLFW
  if (!glfwInit()) {
    printf("GLFW initialisation failed!");
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
    printf("GLFW window creation failed!");
    glfwTerminate();
    return 1;
  }

  // Get Buffer size information
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  // Set context for GLEW to use
  glfwMakeContextCurrent(mainWindow);
  glfwSetKeyCallback(mainWindow, updateKeyboard);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    printf("GLEW initialisation failed!");
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 1;
  }

  // Setup Viewport size
  glViewport(0, 0, bufferWidth, bufferHeight);

  createTriangle();
  compileShaders();

  // Loop until window closed
  while (!glfwWindowShouldClose(mainWindow)) {
    // Get + Handle user input events
    glfwPollEvents();

    if (updateIter) {
      createTriangle();
      updateIter = false;
    }

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind
    glUseProgram(shader);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, std::pow(3, iterations + 1));

    // unbind
    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapBuffers(mainWindow);
  }

  return 0;
}
