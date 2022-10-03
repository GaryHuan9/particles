#include <iostream>
#include <vector>
#include <CL/cl.hpp>

int main()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

//	std::cout << std::format("Found {} platforms.\n", platforms.size());

	// Get available platforms
//	std::vector<cl::Platform> platforms;
//
//	std::cout << platforms.size() << '\n';
//	std::vector<cl::Device> devices;
//
//	platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &devices);
//
//	std::cout << devices.size() << '\n';
//
//	cl_ulong memory;
//	devices[0].getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &memory);
//
//	std::cout << memory << '\n';
//
//	char vendor[128];
//	devices[0].getInfo(CL_DEVICE_VENDOR, &vendor);
//
//	std::cout << vendor << '\n';

	return 0;
}