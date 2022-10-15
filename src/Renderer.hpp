#ifndef PARTICLES_RENDERER_HPP
#define PARTICLES_RENDERER_HPP

#include <raylib.h>
#include <rlgl.h>
#include "main.hpp"

namespace ptl
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void draw(size_t length, const float* x_positions, const float* y_positions, const float* radii) const;

	private:
		Texture2D particle_texture;
	};
}

#endif //PARTICLES_RENDERER_HPP
