#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

extern "C" void error_printer(int error_code, const char* description) {
    std::cerr << "gl_error(" << error_code << "): " << description << "\n";
}

int main(int argc, const char** argv) {
    if(!glfwInit()) return -1;
    glfwSetErrorCallback(error_printer);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* window = glfwCreateWindow(1024, 600, "Thermal", nullptr, nullptr);
    if(!window) return -1;

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
