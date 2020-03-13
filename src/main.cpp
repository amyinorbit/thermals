#include <iostream>
#include "app.hpp"

#include <glue/glue.hpp>

using namespace amyinorbit::gl;

const char* vertex = R"SHADER(
#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)SHADER";

const char* fragment = R"SHADER(
#version 410 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}
)SHADER";

struct BasicScene: App::Scene {
    virtual ~BasicScene() {}

    virtual void on_start(App& app) {
        std::cout << "starting scene\n";

        float vertices[] = {
            // positions          // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left
        };

        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
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
        vbo = Buffer::create(Buffer::vbo);
        ebo = Buffer::create(Buffer::ebo);


        vao.bind();
        vbo.bind();
        vbo.set_data(vertices);
        ebo.bind();
        ebo.set_data(indices);

        Program::AttrDescr<float> pos;
        pos.count = 3;
        pos.stride = 5;

        sh.set_attrib_ptr<float>("aPos", pos);
        sh.enable_attrib(0);

        Program::AttrDescr<float> uvmap;
        uvmap.count = 2;
        uvmap.stride = 5;
        uvmap.offset = 3;

        sh.set_attrib_ptr<float>(1, uvmap);
        sh.enable_attrib(1);

        tex = Tex2D::create();
        tex.bind();
        tex.upload_data(Image("/home/amy/Desktop/test.png"));
        tex.gen_mipmaps();
        tex.set_wrap(Tex2D::Wrap::clamp_edge, Tex2D::Wrap::clamp_edge);
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
