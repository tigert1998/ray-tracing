#pragma once

#include <tuple>
#include <vector>

#include "glm/glm.hpp"

class PerlinNoise {
 public:
  PerlinNoise() = delete;
  PerlinNoise(int perm_size);
  double Noise(double x, double y, double z) const;
  double Noise(glm::highp_dvec3 position) const;
  double Noise(glm::vec3 position) const;
  glm::highp_dvec3 DerivativeNoise(double x, double y, double z) const;
  glm::highp_dvec3 DerivativeNoise(glm::highp_dvec3 position) const;
  double Turbulent(glm::vec3 position, uint32_t depth) const;

 private:
  using WeightsType = std::tuple<std::vector<double>, std::vector<double>,
                                 std::vector<double>, std::vector<double>>;
  WeightsType CalcWeights(double x, double y, double z) const;
  const int PERM_SIZE;
  std::vector<int> perm_random_;
  double Grad(int x, int y, int z, double dx, double dy, double dz) const;
  static double F(double x);
  static double DerivativeF(double x);
};
