#include <GLFW/glfw3.h>
#include <string>

using namespace std;

class Shader {
private:
    GLuint ID;

public:
    Shader(string vertexShaderPath, string fragmentShaderPath);

    void use();
    GLint getUniformLocation(const char* uniformName);
};