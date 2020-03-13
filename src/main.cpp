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

uniform float alpha;

void main()
{
    FragColor = vec4(1.0f, 0.5f, alpha, 1.0f);
}
)SHADER";

struct basic_scene: app::scene {
    virtual ~basic_scene() {}

    virtual void on_start(app& host) {
        std::cout << "starting scene\n";
        const float3 vertices[] = {
            {-0.5f, -0.5f, 0.0f},
            { 0.5f, -0.5f, 0.0f},
            {0.0f,  0.5f, 0.0f}
        };

        {
            shader vsh(shader::vertex);
            if(!vsh.compile(vertex)) throw std::runtime_error("error in vertex shader");

            shader fsh(shader::fragment);
            if(!fsh.compile(fragment)) throw std::runtime_error("error in fragment shader");

            sh.create();
            sh.attach(vsh);
            sh.attach(fsh);
            if(!sh.link()) throw std::runtime_error("error in link step");
        }
        sh.use();

        vao.create();
        vao.bind();

        vbo.create();
        vbo.bind();
        vbo.set_data(vertices);
        sh.set_attrib_ptr(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

    }

    virtual void on_end(app& host) {

    }

    virtual void update(app& host) {
        time_ += host.time_step();
        float alpha = 0.5 + std::cos(time_) / 2.0;


        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sh.use();
        sh.set_uniform("alpha", alpha);
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
private:

    float time_;

    vertex_array vao;
    buffer vbo{buffer::vbo};
    program sh;
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
