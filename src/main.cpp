#include <glue/glue.hpp>
#include <glue/transform.hpp>
#include <apmath/vector.hpp>
#include <iostream>
#include <fstream>
#include "assets_lib.hpp"
#include "obj_loader.hpp"

using namespace amyinorbit;
using namespace amyinorbit::gl;

void diagnose() {
    auto err = glGetError();
    if(err == GL_NO_ERROR) return;
    std::cerr << "gl error [" << err << "]\n";
}

struct BasicScene: App::Scene {
    virtual ~BasicScene() {}

    virtual void on_start(App& app) {

        AssetsLib lib("assets");

        auto res = app.window().framebuffer_size();
        P = math::perspective(M_PI/3.f, float(res.x)/float(res.y), 0.1f, 100.f);
        // M = translation(0, 0, 0);
        V = math::look_at(float3(0, 1, 3), float3(0));

        model = lib.model("planet.obj");
        tex = lib.texture("tex.png");

        sh = Program::create();
        sh.attach(lib.shader("model.vsh", Shader::Type::vertex));
        sh.attach(lib.shader("model.fsh", Shader::Type::fragment));
        if(!sh.link())
            throw std::runtime_error("error in link step: " + sh.debug_message());

        sh.use();
        vao = VertexArray::create();
        vbo = Buffer::create(Buffer::vbo);

        vao.bind();
        vbo.bind();

        vbo.set_data(model.data);

        Program::AttrDescr<float> pos;
        pos.offset = 0;
        pos.count = 3;
        pos.stride = 8;

        Program::AttrDescr<float> norm;
        norm.offset = 3;
        norm.count = 3;
        norm.stride = 8;

        Program::AttrDescr<float> uv;
        uv.offset = 6;
        uv.count = 2;
        uv.stride = 8;

        sh.set_attrib_ptr<float>(0, pos);
        sh.enable_attrib(0);

        sh.set_attrib_ptr<float>(1, norm);
        sh.enable_attrib(1);

        sh.set_attrib_ptr<float>(2, uv);
        sh.enable_attrib(2);

        sh.set_uniform("light.direction", float3(-1, 0, 0.2));
        sh.set_uniform("light.color", float3(1, 0.8, 0.4));
        sh.set_uniform("blend", 0.f);
        diagnose();
    }

    virtual void on_end(App& app) {

    }

    virtual void update(App& app) {

        time_ += app.time_step();
        float x = 3.f * std::cos(time_ * 2);
        float z = 3.f * std::sin(time_ * 2);

        M = math::rotate(float3(1.f, 1.f, 0.f), time_);

        // float dist = 1.f + 1.f * (0.5f + std::cos(time_ / 2.f));

        // V = lookAt(dist * float3(x, 1, z), float3(0));

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sh.use();
        tex.bind(0);
        sh.set_uniform("model", M);
        sh.set_uniform("view", V);
        sh.set_uniform("proj", P);
        sh.set_uniform("tex", tex);
        diagnose();

        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, model.count);
    }
private:

    float time_;

    mat4 M = mat4::identity(), V = mat4::identity(), P = mat4::identity();

    struct {
        float3 pos;
        float3 dir;
    } camera;

    Mesh model;
    VertexArray vao;
    Buffer vbo;
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
