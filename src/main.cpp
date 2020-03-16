#include <glue/glue.hpp>
#include <apmath/vector.hpp>
#include <iostream>
#include <fstream>

#include "obj_loader.hpp"

using namespace amyinorbit::gl;

void diagnose() {
    auto err = glGetError();
    if(err == GL_NO_ERROR) return;
    std::cerr << "gl error [" << err << "]\n";
}

matrix4 translation(float dx, float dy, float dz) {
  return matrix4{
      1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      dx,  dy,  dz,  1.f,
  };
}

matrix4 rotationX(float rx) {
    auto c = std::cos(rx);
    auto s = std::sin(rx);

    return matrix4 {
        1.f, 0.f, 0.f, 0.f,
        0.f,  c,  -s,  0.f,
        0.f,  s,   c,  0.f,
        0.f, 0.f, 0.f, 1.f,
    };
}

matrix4 rotationY(float ry) {
    auto c = std::cos(ry);
    auto s = std::sin(ry);
    return matrix4 {
         c,   0.f, -s, 0.f,
        0.f, 1.f, 0.f, 0.f,
         s,   0.f,  c, 0.f,
        0.f, 0.f, 0.f, 1.f
    };
}

matrix4 rotationZ(float rz) {
    auto c = std::cos(rz);
    auto s = std::sin(rz);
    return matrix4 {
         c,   s,  0.f, 0.f,
        -s,   c,  0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
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

matrix4 lookAt(const float3& camera, const float3& target, const float3& up = float3(0, 1, 0)) {
    auto z = normalize(camera - target);
    auto x = normalize(cross(up, z));
    auto y = normalize(cross(z, x));

    mat4 view;
    view[0] = float4(x.x, y.x, z.x, 0);
    view[1] = float4(x.y, y.y, z.y, 0);
    view[2] = float4(x.z, y.z, z.z, 0);
    view[3] = float4(-dot(x, camera), -dot(y, camera), -dot(z, camera), 1);
    return view;
}

struct BasicScene: App::Scene {
    virtual ~BasicScene() {}

    virtual void on_start(App& app) {
        std::cout << "starting scene\n";

        auto res = app.window().framebuffer_size();

        P = perspective(M_PI/3.f, float(res.x)/float(res.y), 0.1f, 100.f);
        // M = translation(0, 0, 0);
        V = lookAt(float3(0, 1, 3), float3(0));

        std::ifstream obj("assets/monkey.obj");
        if(!obj.is_open()) abort();

        try {
            model = load_object(obj);
        } catch(ParseError& error) {
            std::cout << "error: " << error.what() << "\n";
            abort();
        }

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

        Image img("assets/tex.png");
        if(!img.is_loaded()) {
            std::cout << "image not loaded\n";
        }
        tex = Tex2D::create();
        tex.bind();
        tex.upload_data(img);
        tex.gen_mipmaps();
        tex.set_mag_filter(Tex2D::Filter::linear);
        tex.set_min_filter(Tex2D::Filter::nearest);
        tex.set_wrap(Tex2D::Wrap::clamp_edge, Tex2D::Wrap::clamp_edge);
        std::cout << "image channels: " << img.channels() << "\n";

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

        sh.set_uniform("light.direction", float3(-1, 1, 1));
        sh.set_uniform("light.color", float3(1, 0.8, 0.8));
        sh.set_uniform("blend", 0.f);
        diagnose();
    }

    virtual void on_end(App& app) {

    }

    virtual void update(App& app) {

        time_ += app.time_step();
        float x = 3.f * std::cos(time_ * 2);
        float z = 3.f * std::sin(time_ * 2);

        M = rotationY(time_);

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
