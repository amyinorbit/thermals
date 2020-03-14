#include <glue/glue.hpp>
#include <iostream>
#include <fstream>
#include "app.hpp"

using namespace amyinorbit::gl;

struct BasicScene: App::Scene {
    virtual ~BasicScene() {}

    virtual void on_start(App& app) {
        std::cout << "starting scene\n";

        float vertices[] = {
            // positions
             0.8f,  0.8f, 0.0f,
             0.8f, -0.8f, 0.0f,
            -0.8f, -0.8f, 0.0f,
            -0.8f,  0.8f, 0.0f,
        };

        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        {
            auto vsh = Shader::create(Shader::vertex);
            std::ifstream vsh_source("shaders/plane.vsh");
            if(!vsh.compile(vsh_source))
                throw std::runtime_error("error in vertex shader: " + vsh.debug_message());

            auto fsh = Shader::create(Shader::fragment);
            std::ifstream fsh_source("shaders/raymarch_1.fsh");
            if(!fsh.compile(fsh_source))
                throw std::runtime_error("error in fragment shader: " + fsh.debug_message());

            sh = Program::create();
            sh.attach(vsh);
            sh.attach(fsh);
            if(!sh.link())
                throw std::runtime_error("error in link step: " + sh.debug_message());
        }
        sh.use();

        vao = VertexArray::create();
        vbo = Buffer::create(Buffer::vbo);
        ebo = Buffer::create(Buffer::ebo);


        vao.bind();
        vbo.bind();
        vbo.set_data(vertices);
        ebo.bind();
        ebo.set_data(indices);

        Program::AttrDescr<float> pos;
        pos.count = 3;
        pos.stride = 3;

        sh.set_attrib_ptr<float>(0, pos);
        sh.enable_attrib(0);
    }

    virtual void on_end(App& app) {

    }

    virtual void update(App& app) {
        time_ += app.time_step();
        // float alpha = 0.5 + std::cos(time_) / 2.0;


        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tex.bind();
        sh.use();
        auto res = app.window().framebuffer_size();
        sh.set_uniform("resolution", float2(res.x, res.y));
        vao.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
private:

    float time_;

    VertexArray vao;
    Buffer vbo, ebo;
    Program sh;
    Tex2D tex;
};

int main(int argc, const char** argv) {

    Window::Attrib config;
    config.name = "Thermals";
    config.size = {1024, 600};
    config.version = {4, 1};

    try {
        app_main<BasicScene>(config);
    } catch(std::exception& e) {
        std::cerr << "fatal error: " << e.what() << "\n";
        return -1;
    }
}
