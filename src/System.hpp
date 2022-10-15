#ifndef PARTICLES_PARTICLE_H
#define PARTICLES_PARTICLE_H

#include "main.hpp"
#include <cl/cl.hpp>

namespace ptl
{
	class System
	{
	public:
		explicit System(cl::Device& device);

		void insert(float x_position, float y_position,
		            float x_velocity = 0.0f, float y_velocity = 0.0f,
		            float mass = 10000.0f, float radius = 2.0f)
		{
			//length of 1.0 = 10^9 m
			//mass of 1.0   = 10^25 kg

			// G * m * 10^25 / (r * 10^9)^2 = G' * m / r^2 * 10^9
			// G * 10^25 / 10^9^2 = G' * 10^9
			// G' = G * 10^25 / 10^9^3 = G * 10^-2

//			x_position *= 1E9f;
//			y_position *= 1E9f;
//			x_velocity *= 1E9f;
//			y_velocity *= 1E9f;
//			mass *= 1E25f;

			x_positions_insert.push_back(x_position);
			y_positions_insert.push_back(y_position);
			x_velocities_insert.push_back(x_velocity);
			y_velocities_insert.push_back(y_velocity);
			masses_insert.push_back(mass);

			x_positions.push_back(x_position);
			y_positions.push_back(y_position);
			radii.push_back(radius);
		}

		size_t size() const
		{
			return buffer_size;
		}

		void extract(size_t index, float& x_position, float& y_position, float& radius) const
		{
			x_position = x_positions[index];
			y_position = y_positions[index];
			radius = radii[index];
		}

		void update(float delta_time, bool write_back = true);

		void update_buffers();

	private:
		void update_kernel_arguments();

		cl::Context context;
		cl::CommandQueue queue;
		cl::Program program;

		cl::Kernel update_positions;
		cl::Kernel update_velocities;

		std::vector<float> x_positions_insert;
		std::vector<float> y_positions_insert;
		std::vector<float> x_velocities_insert;
		std::vector<float> y_velocities_insert;
		std::vector<float> masses_insert;

		std::vector<float> x_positions;
		std::vector<float> y_positions;
		std::vector<float> radii;

		size_t buffer_size{};
		size_t buffer_capacity = 64;
		cl::Buffer x_positions_buffer;
		cl::Buffer y_positions_buffer;
		cl::Buffer x_velocities_buffer;
		cl::Buffer y_velocities_buffer;
		cl::Buffer masses_buffer;
	};
}

#endif //PARTICLES_PARTICLE_H
