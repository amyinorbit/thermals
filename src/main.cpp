#include <glue/glue.hpp>
#include <apmath/vector.hpp>
#include <iostream>
#include <fstream>
#include "engine/assets_lib.hpp"
#include "cloud_scene.hpp"

using namespace amyinorbit;
using namespace amyinorbit::gl;

Context& GL() {
    static Context ctx;
    return ctx;
}

int main(int argc, const char** argv) {

    Window::Attrib config;
    config.name = "Thermals";
    config.size = {1024, 600};
    config.version = {4, 1};

    try {
        AssetsLib assets("assets");
        app_main<CloudScene>(config, std::ref(assets));
    } catch(std::exception& e) {
        std::cerr << "fatal error: " << e.what() << "\n";
        return -1;
    }
}
