#include <iostream>
#include "app.hpp"

#include <glue/glue.hpp>

using namespace amyinorbit::gl;

const char* vertex = R"SHADER(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)SHADER";

const char* fragment = R"SHADER(
#version 330 core
out vec4 FragColor;

// uniform float alpha;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.5f, 1.0f);
}
)SHADER";

struct BasicScene: App::Scene {
    virtual ~BasicScene() {}

    virtual void on_start(App& app) {
        std::cout << "starting scene\n";
        const float3 vertices[] = {
            {-0.5f, -0.5f, 0.0f},
            { 0.5f, -0.5f, 0.0f},
            {0.0f,  0.5f, 0.0f}
        };

        {
            auto vsh = Shader::create(Shader::vertex);
            if(!vsh.compile(vertex))
                throw std::runtime_error("error in vertex shader: " + vsh.debug_message());

            auto fsh = Shader::create(Shader::fragment);
            if(!fsh.compile(fragment))
                throw std::runtime_error("error in fragment shader: " + fsh.debug_message());

            sh = Program::create();
            sh.attach(vsh);
            sh.attach(fsh);
            if(!sh.link())
                throw std::runtime_error("error in link step: " + sh.debug_message());
        }
        sh.use();

        vao = VertexArray::create();
        vao.bind();

        vbo = Buffer::create(Buffer::vbo);
        vbo.bind();
        vbo.set_data(vertices);
        sh.set_attrib_ptr(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

    }

    virtual void on_end(App& app) {

    }

    virtual void update(App& app) {
        time_ += app.time_step();
        float alpha = 0.5 + std::cos(time_) / 2.0;


        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sh.use();
        // sh.set_uniform("alpha", alpha);
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
private:

    float time_;

    VertexArray vao;
    Buffer vbo;
    Program sh;
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
