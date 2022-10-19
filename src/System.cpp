#include "System.hpp"

static std::string load_kernel(std::string file_name)
{
	std::ifstream file(file_name);
	std::string kernel{ std::istreambuf_iterator<char>(file),
	                    std::istreambuf_iterator<char>() };

	std::cout << kernel << '\n';

	return kernel;
}

ptl::System::System(cl::Device& device)
	: context(device), queue(context, device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE),
	  program(context, load_kernel("kernel.cl")),
	  x_positions_buffer(context, CL_MEM_READ_WRITE, buffer_capacity * sizeof(float)),
	  y_positions_buffer(context, CL_MEM_READ_WRITE, buffer_capacity * sizeof(float)),
	  x_velocities_buffer(context, CL_MEM_READ_WRITE, buffer_capacity * sizeof(float)),
	  y_velocities_buffer(context, CL_MEM_READ_WRITE, buffer_capacity * sizeof(float)),
	  masses_buffer(context, CL_MEM_READ_ONLY, buffer_capacity * sizeof(float))
{
	if (program.build({ device }, "-cl-denorms-are-zero -cl-fast-relaxed-math") != CL_SUCCESS)
	{
		std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
		exit(1);
	}

	update_positions = cl::Kernel(program, "update_positions");
	update_velocities = cl::Kernel(program, "update_velocities");

	update_kernel_arguments();
}

void ptl::System::update(float delta_time, bool write_back)
{
	update_buffers();

	cl::NDRange global(buffer_size);
	update_positions.setArg(0, delta_time);
	update_velocities.setArg(0, delta_time);

	cl::Event velocities_event;
	std::vector<cl::Event> update_events(1);

	queue.enqueueNDRangeKernel(update_positions, cl::NullRange, global, cl::NullRange, nullptr, &update_events[0]);
	queue.enqueueNDRangeKernel(update_velocities, cl::NullRange, global, cl::NullRange, &update_events, &velocities_event);

	if (write_back)
	{
		update_events.push_back(velocities_event);
		std::vector<cl::Event> read_events(2);

		queue.enqueueReadBuffer(x_positions_buffer, CL_TRUE, 0, buffer_size * sizeof(float), x_positions.data(), &update_events, &read_events[0]);
		queue.enqueueReadBuffer(y_positions_buffer, CL_TRUE, 0, buffer_size * sizeof(float), y_positions.data(), &update_events, &read_events[1]);

		cl::Event::waitForEvents(read_events);
	}
	else velocities_event.wait();
}

void ptl::System::update_buffers()
{
	if (masses_insert.empty()) return;
	size_t insert_size = masses_insert.size();
	size_t new_size = buffer_size + insert_size;

	if (buffer_capacity < new_size)
	{
		do buffer_capacity *= 2;
		while (buffer_capacity < new_size);

		cl::Buffer x_positions_buffer_new(context, CL_MEM_READ_WRITE, buffer_capacity * sizeof(float));
		cl::Buffer y_positions_buffer_new(context, CL_MEM_READ_WRITE, buffer_capacity * sizeof(float));
		cl::Buffer x_velocities_buffer_new(context, CL_MEM_READ_WRITE, buffer_capacity * sizeof(float));
		cl::Buffer y_velocities_buffer_new(context, CL_MEM_READ_WRITE, buffer_capacity * sizeof(float));
		cl::Buffer masses_buffer_new(context, CL_MEM_READ_ONLY, buffer_capacity * sizeof(float));

		std::vector<cl::Event> copy_events(5);

		queue.enqueueCopyBuffer(x_positions_buffer, x_positions_buffer_new, 0, 0, buffer_size * sizeof(float), nullptr, &copy_events[0]);
		queue.enqueueCopyBuffer(y_positions_buffer, y_positions_buffer_new, 0, 0, buffer_size * sizeof(float), nullptr, &copy_events[1]);
		queue.enqueueCopyBuffer(x_velocities_buffer, x_velocities_buffer_new, 0, 0, buffer_size * sizeof(float), nullptr, &copy_events[2]);
		queue.enqueueCopyBuffer(y_velocities_buffer, y_velocities_buffer_new, 0, 0, buffer_size * sizeof(float), nullptr, &copy_events[3]);
		queue.enqueueCopyBuffer(masses_buffer, masses_buffer_new, 0, 0, buffer_size * sizeof(float), nullptr, &copy_events[4]);

		cl::Event::waitForEvents(copy_events);

		x_positions_buffer = x_positions_buffer_new;
		y_positions_buffer = y_positions_buffer_new;
		x_velocities_buffer = x_velocities_buffer_new;
		y_velocities_buffer = y_velocities_buffer_new;
		masses_buffer = masses_buffer_new;

		update_kernel_arguments();
	}

	std::vector<cl::Event> write_events(5);

	queue.enqueueWriteBuffer(x_positions_buffer, CL_FALSE, buffer_size * sizeof(float), insert_size * sizeof(float), x_positions_insert.data(), nullptr, &write_events[0]);
	queue.enqueueWriteBuffer(y_positions_buffer, CL_FALSE, buffer_size * sizeof(float), insert_size * sizeof(float), y_positions_insert.data(), nullptr, &write_events[1]);
	queue.enqueueWriteBuffer(x_velocities_buffer, CL_FALSE, buffer_size * sizeof(float), insert_size * sizeof(float), x_velocities_insert.data(), nullptr, &write_events[2]);
	queue.enqueueWriteBuffer(y_velocities_buffer, CL_FALSE, buffer_size * sizeof(float), insert_size * sizeof(float), y_velocities_insert.data(), nullptr, &write_events[3]);
	queue.enqueueWriteBuffer(masses_buffer, CL_FALSE, buffer_size * sizeof(float), insert_size * sizeof(float), masses_insert.data(), nullptr, &write_events[4]);

	cl::Event::waitForEvents(write_events);

	x_positions_insert.clear();
	y_positions_insert.clear();
	x_velocities_insert.clear();
	y_velocities_insert.clear();
	masses_insert.clear();
	buffer_size = new_size;
}

void ptl::System::update_kernel_arguments()
{
	update_positions.setArg(1, x_positions_buffer);
	update_positions.setArg(2, y_positions_buffer);
	update_positions.setArg(3, x_velocities_buffer);
	update_positions.setArg(4, y_velocities_buffer);

	update_velocities.setArg(1, x_positions_buffer);
	update_velocities.setArg(2, y_positions_buffer);
	update_velocities.setArg(3, x_velocities_buffer);
	update_velocities.setArg(4, y_velocities_buffer);
	update_velocities.setArg(5, masses_buffer);
}
