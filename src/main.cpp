#include <random>
#include <fstream>
#include <CL/cl.hpp>

#include "main.hpp"
#include "Application.hpp"
#include "System.hpp"

bool find_device(cl::Device& device)
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	std::cout << fmt::format("Found {} platform(s): ", platforms.size());

	if (platforms.empty())
	{
		std::cout << "exiting...\n";
		return false;
	}

	cl::Platform& platform = platforms[0];
	std::cout << fmt::format("using {}.\n", platform.getInfo<CL_PLATFORM_NAME>());

	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	std::cout << fmt::format("Found {} devices(s): ", devices.size());

	if (devices.empty())
	{
		std::cout << "exiting...\n";
		return false;
	}

	device = cl::Device{ devices[0] };
	std::cout << fmt::format("using {} from {} with {} memory and {} compute units on {}.\n",
	                         device.getInfo<CL_DEVICE_NAME>(),
	                         device.getInfo<CL_DEVICE_VENDOR>(),
	                         device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>(),
	                         device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>(),
	                         device.getInfo<CL_DEVICE_VERSION>());

	return true;
}

void spawn_solar_system(ptl::System& system)
{
	float x_center;
	float y_center;

	ptl::Application::get_center(x_center, y_center);

	system.insert(x_center + 0.0f, y_center, 0.0f, 0.0f, 1.989E5f, 10.0f);      //Sun
	system.insert(x_center + 57.909f, y_center, 0.0f, 47.36E-6f, 0.33011E-1f);  //A planet similar to mercury
	system.insert(x_center + 108.209f, y_center, 0.0f, 35.02E-6f, 4.8675E-1f);  //A planet similar to venus
	system.insert(x_center + 149.596f, y_center, 0.0f, 29.78E-6f, 5.9724E-1f);  //A planet similar to earth
	system.insert(x_center + 227.923f, y_center, 0.0f, 24.07E-6f, 0.64171E-1f); //A planet similar to mars
	system.insert(x_center + 778.570f, y_center, 0.0f, 13E-6f, 1898.19E-1f);    //A planet similar to jupiter
	system.insert(x_center + 1433.529f, y_center, 0.0f, 9.68E-6f, 568.34E-1f);  //A planet similar to saturn
	system.insert(x_center + 2872.463f, y_center, 0.0f, 6.80E-6f, 86.813E-1f);  //A planet similar to uranus
	system.insert(x_center + 4495.060f, y_center, 0.0f, 5.43E-6f, 102.413E-1f); //A planet similar to neptune
}

void spawn_golden_ratio(ptl::System& system)
{
	float x_center;
	float y_center;

	ptl::Application::get_center(x_center, y_center);

	constexpr int32_t SpawnCount = 25000;
	constexpr float SpawnRadius = 400.0f;
	constexpr float AngleMultiplier = 2.0f - 2.0f / (float)std::numbers::phi;

	for (int32_t i = 0; i < SpawnCount; ++i)
	{
		float r = std::sqrt((float)i / SpawnCount) * SpawnRadius;
		float theta = (float)std::numbers::pi * AngleMultiplier * (float)i;

		float x = r * std::cos(theta);
		float y = r * std::sin(theta);

		system.insert(x_center + x, y_center + y, 0.0f, 0.0f, SpawnRadius / (r + 0.001f));
	}
}

int main()
{
	cl::Device device;
	if (!find_device(device)) return 0;

	cl::Context context(device);
	cl::CommandQueue queue(context, device);

	std::uniform_real_distribution<float> speed_distribution(-0.00005f, 0.00005f);
	std::uniform_real_distribution<float> mass_distribution(0.001f, 0.02f);
	std::random_device prng;

	ptl::Application application;
	ptl::System system(device);

//	spawn_solar_system(system);
	spawn_golden_ratio(system);

	float delta_time;

	bool modified_time = true;
	int32_t time_divisions = 4;
	float time_scale = 5.0f;
	float time_multiplier;
	bool paused = true;

	system.update();

	while (application.update(delta_time))
	{
		application.text(fmt::format("FPS: {}", 1.0f / delta_time).c_str());
		application.text(fmt::format("Count: {}", system.size()).c_str());

		modified_time |= application.slider("Time Division", time_divisions, 1, 64);
		modified_time |= application.slider("Time Scale", time_scale, -2.0f, 20.0f);
		if (application.button(paused ? "Resume" : "Pause")) paused = !paused;

		if (modified_time)
		{
			time_multiplier = std::exp(time_scale) / (float)time_divisions;
			modified_time = false;
		}

		application.text(fmt::format("Time Division: {}", time_divisions).c_str());
		application.text(fmt::format("Time Multiplier: {:#g}", time_multiplier * (float)time_divisions).c_str());
		if (paused) application.text("Paused");

		if (delta_time < 0.1f && !paused) //Skip frames if needed
		{
			for (int32_t i = time_divisions - 1; i >= 0; --i) system.update(delta_time * time_multiplier, i == 0);
		}

		application.draw(system.size(), system.get_x_positions(), system.get_y_positions(), system.get_radii());

//		int32_t x;
//		int32_t y;
//
//		if (!ptl::Application::last_click(x, y)) continue;
//
//		system.insert(static_cast<float>(x), static_cast<float>(y),
//		              speed_distribution(prng), speed_distribution(prng),
//		              mass_distribution(prng), 100.0f);
	}

	return 0;
}