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

		bool update(float& delta_time) const;

		static void draw_circle(float x, float y, float radius);
		static bool last_click(int32_t& x, int32_t& y);
	};
}

#endif //PARTICLES_APPLICATION_HPP
