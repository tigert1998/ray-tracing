#include <atomic>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <thread>

#define GLM_ENABLE_EXPERIMENTAL
#include "bump.h"
#include "camera.h"
#include "dielectric.h"
#include "diffuse_light.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "hitable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "parallelepiped.h"
#include "parallelogram.h"
#include "sphere.h"
#include "textures/texture"

using glm::vec3, glm::pi, glm::clamp, glm::rotateY;
using std::cerr, std::endl, std::function, std::vector, std::pair;
using std::make_shared, std::string, std::shared_ptr, std::cout;
using std::thread, std::array;

constexpr int TRACE_DEPTH_LIMIT = 10;

int width = 400, height = 400, spp = 100,
    number_of_threads = thread::hardware_concurrency();

shared_ptr<Camera> camera_ptr;
HitableList object_list;
function<double()> dice;
vector<vec3> pixels;

namespace color {
vec3 WHITE = vec3(1, 1, 1);
vec3 BLACK = vec3(0, 0, 0);
}  // namespace color

void SetupStage() {
  camera_ptr = make_shared<Camera>(vec3(278, 278, -800), vec3(278, 278, 0),
                                   vec3(0, 1, 0), pi<double>() * 2 / 9,
                                   double(width) / height);

  auto red_material_ptr = make_shared<Lambertian>(dice, vec3(0.65, 0.05, 0.05));
  auto white_material_ptr =
      make_shared<Lambertian>(dice, vec3(0.73, 0.73, 0.73));
  auto green_material_ptr =
      make_shared<Lambertian>(dice, vec3(0.12, 0.45, 0.15));
  auto light_material_ptr = make_shared<DiffuseLight>(
      make_shared<ConstantTexture>(vec3(100.f, 100.f, 100.f)));

  object_list.list().push_back(make_shared<Parallelogram>(
      array<vec3, 3>{vec3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555)},
      red_material_ptr));
  object_list.list().push_back(make_shared<Parallelogram>(
      array<vec3, 3>{vec3(555, 0, 0), vec3(555, 555, 0), vec3(555, 0, 555)},
      green_material_ptr));
  object_list.list().push_back(make_shared<Parallelogram>(
      array<vec3, 3>{vec3(213, 554, 332), vec3(213, 554, 227),
                     vec3(343, 554, 332)},
      light_material_ptr));
  object_list.list().push_back(make_shared<Parallelogram>(
      array<vec3, 3>{vec3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555)},
      white_material_ptr));
  object_list.list().push_back(make_shared<Parallelogram>(
      array<vec3, 3>{vec3(0, 0, 555), vec3(0, 555, 555), vec3(555, 0, 555)},
      white_material_ptr));
  object_list.list().push_back(make_shared<Parallelogram>(
      array<vec3, 3>{vec3(0, 555, 0), vec3(555, 555, 0), vec3(0, 555, 555)},
      white_material_ptr));
  object_list.list().push_back(make_shared<Parallelepiped>(
      vec3(165, 165, 165), white_material_ptr, [](vec3 p) -> vec3 {
        return rotateY(p, -pi<float>() * 0.1f) + vec3(130, 0, 165);
      }));
  object_list.list().push_back(make_shared<Parallelepiped>(
      vec3(165, 330, 165), white_material_ptr, [](vec3 p) -> vec3 {
        return rotateY(p, pi<float>() / 12.f) + vec3(265, 0, 295);
      }));
}

void Init(int argc, char **argv) {
  if (argc <= 1) {
    cerr << "Not sufficient arguments." << endl;
    exit(1);
  }
  try {
    spp = std::stoi(argv[1]);
    if (spp <= 0) throw std::invalid_argument("");
    cout << "spp = " << spp << "\n";
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

  SetupStage();

  pixels.resize(width * height);
}

vec3 Trace(Ray ray) {
  std::tuple<vec3, vec3> storage[TRACE_DEPTH_LIMIT];
  int storage_size = 0;

  vec3 result;
  for (int depth = 0;; depth++) {
    auto record = object_list.Hit(ray, pair<double, double>(1e-3, DBL_MAX));
    if (record == std::nullopt || depth >= TRACE_DEPTH_LIMIT) {
      result = color::BLACK;
      break;
    }
    auto material_ptr = record->material_ptr;
    auto attenuation_reflection_pair =
        material_ptr->Scatter(ray, record.value());
    auto hit_point = ray.position() + (float)record->t * ray.direction();
    auto emitted = material_ptr->Emit(0, 0, hit_point);
    if (attenuation_reflection_pair == std::nullopt) {
      result = emitted;
      break;
    }
    auto attenuation = attenuation_reflection_pair->first;
    auto reflection = attenuation_reflection_pair->second;
    storage[storage_size++] = {emitted, attenuation};
    ray = reflection;
  }

  for (int i = storage_size - 1; i >= 0; i--) {
    vec3 emitted, attenuation;
    std::tie(emitted, attenuation) = storage[i];
    result = emitted + attenuation * result;
  }
  return result;
}

void Output(vector<vec3> &pixels) {
  string title = "image.ppm";
  cout << "Writing to " << title << "..." << endl;
  std::fstream fs(title, std::ios::out);
  fs << "P3\n" << width << " " << height << "\n255\n";
  for (int i = 0; i < width * height; i++) {
    auto color = pixels[i];
    for (int j = 0; j < 3; j++) fs << int(color[j] * 255) << " ";
  }
}

vec3 RenderPixel(int i, int j) {
  // 0 <= i < height
  // 0 <= j < width
  auto color = vec3(0);
  for (int k = 0; k < spp; k++) {
    double x = (dice() + double(j)) / double(width);
    double y = (dice() + double(height - i)) / double(height);
    x = 2 * x - 1;
    y = 2 * y - 1;
    auto ray = camera_ptr->ray_at(x, y);
    auto temp = Trace(ray);
    color += temp;
  }
  color /= float(spp);
  color = clamp(color, 0.f, 1.f);
  return color;
}

void Render(vector<vec3> &pixels) {
  auto start_clk = std::chrono::system_clock::now();
  std::atomic_int current = 0, last_ratio = 0;

  auto update_info = [&]() {
    current++;
    int ratio = 100. * current / width / height;
    if (ratio > last_ratio) {
      last_ratio.store(ratio, std::memory_order_acquire);
      std::mutex mtx;
      mtx.lock();
      cout << "\rRendering " << std::setw(2) << ratio << "%" << std::flush;
      mtx.unlock();
    }
  };

  auto render_segment = [&](int l, int r) {
    for (int i = l; i <= r; i++) {
      auto color = RenderPixel(i / width, i % width);
      pixels[i] = color;
      update_info();
    }
  };
  vector<thread> threads;
  int single_thread_work_load =
      ((width * height + number_of_threads - 1) / number_of_threads);
  for (int i = 0; i < number_of_threads; i++) {
    int l = single_thread_work_load * i;
    int r = std::min(single_thread_work_load * (i + 1), width * height);
    threads.emplace_back(render_segment, l, r - 1);
  }
  for (thread &t : threads) t.join();
  auto end_clk = std::chrono::system_clock::now();
  cout << "\nRay tracing consumes: "
       << std::chrono::duration<double>(end_clk - start_clk).count() << "s\n";
}

int main(int argc, char **argv) {
  Init(argc, argv);
  Render(pixels);
  Output(pixels);
  return 0;
}
