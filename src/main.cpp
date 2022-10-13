#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <CL/cl.hpp>
#include <fmt/format.h>
#include <raylib.h>

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
	cl::Program::Sources sources;

	constexpr int SIZE = 10;

	std::vector<int> A_h(SIZE);
	std::vector<int> B_h(SIZE);
	std::vector<int> C_h(SIZE);

	std::mt19937 prng(42);
	std::uniform_int_distribution<std::mt19937::result_type> distribution(0, 999999);

	std::generate(A_h.begin(), A_h.end(), [&]() { return distribution(prng); });
	std::generate(B_h.begin(), B_h.end(), [&]() { return distribution(prng); });

	cl::Buffer A_d(context, CL_MEM_READ_ONLY, sizeof(int) * SIZE);
	cl::Buffer B_d(context, CL_MEM_READ_ONLY, sizeof(int) * SIZE);
	cl::Buffer C_d(context, CL_MEM_WRITE_ONLY, sizeof(int) * SIZE);

	cl::CommandQueue queue(context, device);

	queue.enqueueWriteBuffer(A_d, CL_TRUE, 0, sizeof(int) * SIZE, A_h.data());
	queue.enqueueWriteBuffer(B_d, CL_TRUE, 0, sizeof(int) * SIZE, B_h.data());

	std::ifstream file("test.cl");
	std::string kernel_code{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

	std::cout << kernel_code << std::endl;

	sources.push_back({ kernel_code.c_str(), kernel_code.size() });
	cl::Program program(context, sources);

	if (program.build({ device }) != CL_SUCCESS)
	{
		std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
		exit(1);
	}

	cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer> simple_add(cl::Kernel(program, "simple_add"));
	cl::NDRange global(SIZE);
	simple_add(cl::EnqueueArgs(queue, global), A_d, B_d, C_d).wait();

	queue.enqueueReadBuffer(C_d, CL_TRUE, 0, sizeof(int) * SIZE, C_h.data());

	fmt::print("{}", fmt::join(C_h, ", "));

//	InitWindow(800, 450, "raylib [core] example - basic window");
//
//	while (!WindowShouldClose())
//	{
//		BeginDrawing();
//		ClearBackground(RAYWHITE);
//		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
//		EndDrawing();
//	}
//
//	CloseWindow();

	return 0;
}