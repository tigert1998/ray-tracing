#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <fstream>
#include <memory>
#include <thread>

#include <boost/format.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "camera.h"
#include "hitable_list.h"
#include "sphere.h"
#include "lambertian.h"
#include "bump.h"
#include "metal.h"
#include "dielectric.h"
#include "checker_texture.h"
#include "constant_texture.h"

using namespace glm;
using std::make_shared, std::string, std::weak_ptr, std::shared_ptr, std::cout;
using std::cerr, std::endl, std::function, std::vector, std::pair;
using std::thread;
using boost::format, boost::none;

int width = 1200, height = 800, samples = 100, number_of_threads = 1;

shared_ptr<Camera> camera_ptr;
HitableList object_list;
function<double()> dice;
vector<vec3> pixels;

namespace color {
    vec3 WHITE = vec3(1, 1, 1);
    vec3 BLACK = vec3(0, 0, 0);
    vec3 SAPPHIRE = vec3(0.5, 0.7, 1);
    vec3 LIGHT_GREEN = vec3(0.4, 1, 0.4);
    vec3 GREY = vec3(0.69, 0.69, 0.69);
}

void SetupStage() {
    // 0 -> dielectric
    // 1 -> lambertian
    // 2 -> metal
    using namespace color;
    static auto generate_color = [] () -> vec3 {
        vec3 res;
        for (int i = 0; i < 3; i++) res[i] = (dice() + 1) * 0.5;
        return res;
    };
    static auto generate_position = [] () -> vec3 {
        vec3 res;
        res.x = dice() * 20;
        res.y = dice() * 0.5 + 1.5;
        res.z = dice() * 5;
        return res;
    };
    static constexpr int total = 20;
    vector<vec3> positions;
    for (int i = 0; i < total; i++) {
        while (true) {
            auto p = generate_position();
            for (const vec3 &prev_p: positions) {
                if (distance(prev_p, p) <= p.y + prev_p.y) goto fail_test;
            }
            positions.push_back(p);
            break;
fail_test:;
        }
    }

    auto checker_texture = make_shared<CheckerTexture>(
        make_shared<ConstantTexture>(vec3(0.2, 0.3, 0.1)),
        make_shared<ConstantTexture>(vec3(0.9, 0.9, 0.9))
    );
    auto stage = make_shared<Sphere>(vec3(0, -1e3, 0), 1e3, make_shared<Lambertian>(dice, checker_texture));
    object_list.list().push_back(stage);
    for (int i = 0; i < total; i++) {
        int t = floor((dice() + 1) * 3 / 2);
        auto p = positions[i], c = generate_color();
        shared_ptr<Material> material;
        if (t == 0) {
            double index = dice() * 0.3 + 1.4;
            material = make_shared<Dielectric>(sqrt(c), index);
        } else if (t == 1) {
            material = make_shared<Lambertian>(dice, make_shared<ConstantTexture>(c));
        } else {
            material = make_shared<Metal>(c);
        }
        auto object = make_shared<Sphere>(p, p.y, material);
        object_list.list().push_back(object);
    }
}

void Init(int argc, char **argv) {
    if (argc <= 1) {
        cerr << "Not sufficient arguments." << endl;
        exit(1);
    }
    try {
        samples = std::stoi(argv[1]);
        if (samples <= 0) throw std::invalid_argument("");
        if (argc >= 3) {
            number_of_threads = std::stoi(argv[2]);
            if (number_of_threads <= 0) throw std::invalid_argument("");
        }
    } catch (...) {
        cerr << "Invalid argument." << endl;
        exit(1);
    }
    auto seed = time(nullptr);
    std::default_random_engine engine(seed);
    std::uniform_real_distribution<double> dis(-1, 1);
    dice = std::bind(dis, engine);

    camera_ptr = make_shared<Camera>(vec3(0, 5, 8), vec3(0, 2, 0), vec3(0, 1, 0), pi<double>() * 0.7, double(width) / height);
    SetupStage();

    pixels.resize(width * height);
}

vec3 Trace(const Ray &ray, int depth) {
    auto record = object_list.Hit(ray, pair<double, double>(1e-3, DBL_MAX));
    if (record == none) {
        float ratio = ray.direction().y * 0.5 + 0.5;
        auto result = color::SAPPHIRE * ratio + color::WHITE * (1 - ratio);
        return result;
    }
    auto material_ptr = record->material_ptr.lock();
    auto attenuation_reflection_pair = material_ptr->Scatter(ray, record.value());
    if (attenuation_reflection_pair == none) {
        return color::BLACK;
    }
    auto attenuation = attenuation_reflection_pair->first;
    auto reflection = attenuation_reflection_pair->second;
    auto result = attenuation * Trace(reflection, depth + 1);
    return result;
}

void Output(vector<vec3> &pixels) {
    string title = "image.ppm";
    cout << "\nWriting to " << title << "..." << endl;
    std::fstream fs(title, std::ios::out);
    fs << format("P3\n%1% %2%\n255\n") % width % height;
    for (int i = 0; i < width * height; i++) {
        auto color = pixels[i];
        for (int j = 0; j < 3; j++)
            fs << int(color[j] * 255) << " ";
    }
}

vec3 RenderPixel(int i, int j) {
    // 0 <= i < height
    // 0 <= j < width
    auto color = vec3(0);
    for (int k = 0; k < samples; k++) {
        double x = (dice() + double(j)) / double(width);
        double y = (dice() + double(height - i)) / double(height);
        x = 2 * x - 1;
        y = 2 * y - 1;
        auto ray = camera_ptr->ray_at(x, y);
        auto temp = Trace(ray, 0);
        color += temp;
    }
    color /= float(samples);
    return color;
}

void Render(vector<vec3> &pixels) {
    int current = 0;
    auto update_info = [&] () {
        current++;
        if (current % width == 0) {
            double p = 100. * current / width / height;
            cout << format("\rRendering %.2lf%%") % p << std::flush;
        }
    };
    auto render_segment = [&] (int l, int r) {
        for (int i = l; i <= r; i++) {
            auto color = RenderPixel(i / width, i % width);
            pixels[i] = color;
            update_info();
        }
    };
    vector<thread> threads;
    int single_thread_work_load = ((width * height + number_of_threads - 1) / number_of_threads);
    for (int i = 0; i < number_of_threads; i++) {
        int l = single_thread_work_load * i;
        int r = std::min(single_thread_work_load * (i + 1), width * height);
        threads.emplace_back(render_segment, l, r - 1);
    }
    for (thread &t: threads) t.join();
}

int main(int argc, char **argv) {
    Init(argc, argv);
    Render(pixels);
    Output(pixels);
    return 0;
}
