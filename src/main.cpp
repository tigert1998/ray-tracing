#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <fstream>
#include <memory>

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

using namespace glm;
using std::make_shared;
using std::string;
using std::weak_ptr;
using std::shared_ptr;
using std::cout;
using std::cerr;
using std::endl;
using std::function;
using std::vector;
using std::pair;
using boost::format;
using boost::none;

int width = 1024, height = 512, samples = 100;

shared_ptr<Camera> camera_ptr;
HitableList object_list;
function<double()> dice;
vector<vec3> pixels;

namespace color {
    vec3 WHITE = vec3(1, 1, 1);
    vec3 BLACK = vec3(0, 0, 0);
    vec3 SAPPHIRE = vec3(0.5, 0.7, 1);
    vec3 LIGHT_GREEN = vec3(0.4, 1, 0.4);
}

void Init(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Not sufficient arguments." << endl;
        exit(1);
    }
    try {
        samples = std::stoi(argv[1]);
        if (samples <= 0) throw std::invalid_argument("");
    } catch (...) {
        cerr << "Invalid argument." << endl;
        exit(1);
    }
    std::default_random_engine engine(time(nullptr));
    std::uniform_real_distribution<double> dis(-1, 1);
    dice = std::bind(dis, engine);

    camera_ptr = make_shared<Camera>(vec3(-0.9, 0.9, -1.6), vec3(0, 0, 0), vec3(0, 1, 0), pi<double>() * 0.7, double(width) / height);
    object_list.list().push_back(make_shared<Sphere>(vec3(1, 0, 0), 1, make_shared<Metal>(vec3(0.8, 0.8, 0.8))));
    object_list.list().push_back(make_shared<Sphere>(vec3(-1, 0, 0), 1, make_shared<Bump>(vec3(1, 0.5, 0.5))));
    object_list.list().push_back(make_shared<Sphere>(vec3(0, -1e3 - 1, 0), 1e3, make_shared<Lambertian>(dice, vec3(0.8, 0.8, 0))));

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

void Render(vector<vec3> &pixels) {
    for (int i = 0; i < height; i++) {
        cout << format("\rRendering %.2lf%%") % (100. * i / height) << std::flush;
        for (int j = 0; j < width; j++) {
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
            pixels[i * width + j] = color;
        }
    }
}

int main(int argc, char **argv) {
    Init(argc, argv);
    Render(pixels);
    Output(pixels);
    return 0;
}
