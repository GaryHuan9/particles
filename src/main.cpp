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

int main()
{
	cl::Device device;
	if (!find_device(device)) return 0;

	cl::Context context(device);
	cl::CommandQueue queue(context, device);

	std::uniform_real_distribution<float> speed_distribution(-7.0f, 7.0f);
	std::uniform_real_distribution<float> mass_distribution(30.0f, 1000.0f);
	std::random_device prng;

	ptl::Application application;
	ptl::System system(device);

//	system.insert(300.0f, 300.0f, 0.0f, 0.0f, 1.989E5f, 10.0f); //Sun
//	system.insert(300.0f + 149.19f, 300.0f, 0.0f, 3E-5f, 5.972E-1f); //Earth

	system.insert(300.0f, 300.0f, 0.0f, 0.0f, 1.989E5f, 10.0f); //Sun
	system.insert(300.0f + 149.19f, 300.0f, 0.0f, 3E-5f, 5.972E-1f); //Earth

//	for (int x = 10; x < 30; ++x)
//	{
//		for (int y = 10; y < 30; ++y)
//		{
//			system.insert(static_cast<float>(x * 20), static_cast<float>(y * 20),
//			              speed_distribution(prng), speed_distribution(prng),
//			              mass_distribution(prng));
//		}
//	}

	float delta_time;

	while (application.update(delta_time))
	{
		for (int i = 240 - 1; i >= 0; --i) system.update(delta_time * 360.0f, i == 0);
//		system.update(delta_time);

		for (size_t i = 0; i < system.size(); i++)
		{
			float x_position;
			float y_position;
			float radius;

			system.extract(i, x_position, y_position, radius);
			ptl::Application::draw_circle(x_position, y_position, radius);
		}

		int32_t x;
		int32_t y;

//		if (!ptl::Application::last_click(x, y)) continue;
//
//		system.insert(static_cast<float>(x), static_cast<float>(y),
//		              speed_distribution(prng), speed_distribution(prng),
//		              mass_distribution(prng));
	}

	return 0;
}