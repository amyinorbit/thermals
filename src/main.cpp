#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "app.hpp"

using namespace amyinorbit::gl;

void update() {

}

int main(int argc, const char** argv) {

    window::attrib config;
    config.name = "Thermals";
    config.size = {1024, 600};
    config.version = {4, 1};

    app_main(config, &update);
}
