#include "checker_texture.h"

using std::shared_ptr;
using glm::vec3;

CheckerTexture::CheckerTexture(shared_ptr<Texture> odd_ptr, shared_ptr<Texture> even_ptr) {
	odd_ptr_ = odd_ptr;
	even_ptr_ = even_ptr;
}

glm::vec3 CheckerTexture::Value(double u, double v, const vec3 &p) const {
	auto t = sin(p.x) * sin(p.y) * sin(p.z);
	if (t <= 0)
		return odd_ptr_->Value(u, v, p);
	return even_ptr_->Value(u, v, p);
}
