#include "Renderer.hpp"

ptl::Renderer::Renderer() : particle_texture(LoadTexture("particle.png"))
{
}

ptl::Renderer::~Renderer()
{
	UnloadTexture(particle_texture);
}

void ptl::Renderer::draw(size_t length, const float* x_positions, const float* y_positions, const float* radii) const
{
	rlDrawRenderBatchActive();

	size_t current_index = 0;

	while (current_index < length)
	{
		size_t batch_size = std::min(static_cast<size_t>(RL_DEFAULT_BATCH_DRAWCALLS), length - current_index);

		rlSetTexture(particle_texture.id);
		rlBegin(RL_QUADS);
		rlColor4ub(255, 255, 255, 127);
		rlNormal3f(0.0f, 0.0f, 1.0f);

		size_t old_index = current_index;
		current_index += batch_size;

		for (size_t i = old_index; i < current_index; ++i)
		{
			float radius = radii[i];
			float x_position = x_positions[i];
			float y_position = y_positions[i];

			Vector2 corner0{ x_position - radius, y_position - radius };
			Vector2 corner1{ x_position + radius, y_position + radius };

			rlTexCoord2f(0.0f, 0.0f);
			rlVertex2f(corner0.x, corner0.y);

			rlTexCoord2f(0.0f, 1.0f);
			rlVertex2f(corner0.x, corner1.y);

			rlTexCoord2f(1.0f, 1.0f);
			rlVertex2f(corner1.x, corner1.y);

			rlTexCoord2f(1.0f, 0.0f);
			rlVertex2f(corner1.x, corner0.y);
		}

		rlEnd();
	}
}
