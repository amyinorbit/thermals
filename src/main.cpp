#include <iostream>
#include "app.hpp"

using namespace amyinorbit::gl;

struct basic_scene: app::scene {
    virtual ~basic_scene() {}

    virtual void on_start(app& host) {

    }

    virtual void on_end(app& host) {

    }

    virtual void update(app& host) {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // gl::ClearColor(1.0, 1.0, 1.0, 1.0);
        // gl::Clear().Color().Depth();
    }
private:
};

int main(int argc, const char** argv) {

    window::attrib config;
    config.name = "Thermals";
    config.size = {1024, 600};
    config.version = {4, 1};

    try {
        app_main<basic_scene>(config);
    } catch(std::exception& e) {
        std::cerr << "fatal error: " << e.what() << "\n";
    }
}
