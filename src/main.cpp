#include <glue/glue.hpp>
#include <iostream>
#include <fstream>

#include "obj_loader.hpp"

using namespace amyinorbit::gl;

matrix4 translation(float dx, float dy, float dz) {
      return matrix4{
          1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1.f, 0.f,
          dx,  dy,  dz,  1.f,
      };
  }

matrix4 perspective(float fov, float aspect, float zNear, float zFar) {

    const float tanHalfFov = std::tan(fov / 2.f);
    // From GLM
    matrix4 result(0.f);
    result[0][0] = 1.f / (aspect * tanHalfFov);
	result[1][1] = 1.f / (tanHalfFov);
	result[2][2] = - (zFar + zNear) / (zFar - zNear);
	result[2][3] = - 1.f;
	result[3][2] = - (2.f * zFar * zNear) / (zFar - zNear);
    return result;
}

matrix4 orthographic(float width, float height, float zNear, float zFar) {
    auto zDiff = zFar - zNear;
    auto zSum = zFar + zNear;

    matrix4 result(1.f);
    result[0][0] =  2.f / width;
    result[1][1] = -2.f / height;
    result[2][2] =  2.f / zDiff;
    result[3][0] = -1.f;
    result[3][1] =  1.f;
    result[3][2] = -zSum/zDiff;
    return result;
}

matrix4 lookAt(const float3& camera, const float3& target, const float3& up) {
    return matrix4(1.f);
}


struct BasicScene: App::Scene {
    virtual ~BasicScene() {}

    virtual void on_start(App& app) {
        std::cout << "starting scene\n";

        auto res = app.window().framebuffer_size();

        P = perspective(M_PI/3.f, float(res.x)/float(res.y), 0.1f, 100.f);
        V = translation(0, 0, -5);


        std::ifstream obj("assets/sphere.obj");
        if(!obj.is_open()) abort();

        try {
            model = load_object(obj);
            std::cout << "size: " << model.count * sizeof(Vertex) << " {} " << sizeof(float) * model.count * 8 << "\n";

        } catch(ParseError& error) {
            std::cout << "error: " << error.what() << "\n";
            abort();
        }

        // model.data.push_back(Vertex{ float3(-0.5f, -0.5f, 0.0f), float3(), float2() });
        // model.data.push_back(Vertex{ float3( 0.5f, -0.5f, 0.0f), float3(), float2() });
        // model.data.push_back(Vertex{ float3( 0.0f,  0.5f, 0.0f), float3(), float2() });
        // model.count = 3;


        {
            auto vsh = Shader::create(Shader::vertex);
            std::ifstream vsh_source("assets/shaders/model.vsh");
            if(!vsh.compile(vsh_source))
                throw std::runtime_error("error in vertex shader: " + vsh.debug_message());

            auto fsh = Shader::create(Shader::fragment);
            std::ifstream fsh_source("assets/shaders/model.fsh");
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

        sh.set_uniform("light.direction", float3(1));
        sh.set_uniform("light.color", float3(1, 0.6, 0.6));
    }

    virtual void on_end(App& app) {

    }

    virtual void update(App& app) {

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sh.use();
        sh.set_uniform("model", M);
        sh.set_uniform("view", V);
        sh.set_uniform("proj", P);
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
