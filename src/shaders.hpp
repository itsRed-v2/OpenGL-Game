#include <GLFW/glfw3.h>
#include <string>

using namespace std;

GLuint createShader(string filename, GLenum type);
GLuint createProgram(string vertexShaderFile, string fragmentShaderFile);