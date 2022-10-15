#ifndef PARTICLES_APPLICATION_HPP
#define PARTICLES_APPLICATION_HPP

#include "main.hpp"

namespace ptl
{
	class Application
	{
	public:
		Application();
		~Application();

		bool update(float& delta_time);

		void draw(size_t length, const float* x_positions, const float* y_positions, const float* radii) const;

		void text(const char* text);
		bool button(const char* text);
		bool slider(const char* text, float& value, float min, float max);
		bool slider(const char* text, int32_t& value, int32_t min, int32_t max);

		static void get_center(float& center_x, float& center_y);
		static bool last_click(int32_t& x, int32_t& y);

	private:
		void* renderer;

		static constexpr uint32_t ElementHeight = 20;
		static constexpr uint32_t ElementWidth = 260;
		static constexpr uint32_t GapSize = 3;

		uint32_t x_current{};
		uint32_t y_current{};
	};
}

#endif //PARTICLES_APPLICATION_HPP
