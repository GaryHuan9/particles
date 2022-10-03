#include <iostream>
#include <vector>
#include <CL/cl.hpp>
#include <fmt/format.h>

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
	std::cout << fmt::format("using {} from {} with {} memory and {} compute units.\n",
	                         device.getInfo<CL_DEVICE_NAME>(),
	                         device.getInfo<CL_DEVICE_VENDOR>(),
	                         device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>(),
	                         device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>());

	return true;
}

int main()
{
	cl::Device device;
	if (!find_device(device)) return 0;

	
	return 0;
}