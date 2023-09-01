#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "common.h"


int main(void) 
{
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device


	try {
		// select an OpenCL device
		if (!select_one_device(&platform, &device))
		{
			// if no device selected
			quit_program("Device not selected.");
		}

		// create a context from device
		context = cl::Context(device);


		// create command queue
		queue = cl::CommandQueue(context, device);


		// create and initialise our vector of alphabets

		std::vector<cl_uchar> alphVec;

		// we can make use of the ASCII table to initialise our vector easily
		// z-a in ASCII is 122 to 97
		// Z-A in ASCII is 90 to 65

		cl_int i = 0;

		for (i = 122; i >= 97; i--)
		{
			alphVec.push_back(i);
		}

		for (i = 90; i >= 65; i--)
		{
			alphVec.push_back(i);
		}

		// create and initialise our vector of unsigned ints

		std::vector<cl_uint> uIntVec;

		for (i = 0; i <= 1023; i++)
		{
			uIntVec.push_back(i);
		}


		// creating our three OpenCL buffer objects ,and initialising alphBuffer

		// alphabet buffer
		cl::Buffer alphBuffer;
		alphBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_uchar) * alphVec.size(), &alphVec[0]);

		// unsigned char buffer
		cl::Buffer uCharBuffer;
		uCharBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_uchar) * 52);

		// unsigned int buffer
		cl::Buffer uIntBuffer;
		uIntBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint) * 1024);


		// enqueue two OpenCL commands

		// copy first buffer into second buffer
		queue.enqueueCopyBuffer(alphBuffer, uCharBuffer, 0, 0, sizeof(cl_uchar) * 52);

		queue.enqueueWriteBuffer(uIntBuffer, CL_TRUE, 0, sizeof(cl_uint) * 1024, &uIntVec[0]);


		// build the program
		if (!build_program(&program, &context, "task2.cl"))
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		kernel = cl::Kernel(program, "task2");

		kernel.setArg(0, 12.45f);
		kernel.setArg(1, uCharBuffer);
		kernel.setArg(2, uIntBuffer);

		std::cout << "\n\tEnqueue-ing kernel ...\n" << std::endl;

		std::cout << "\t." << std::endl;
		std::cout << "\t." << std::endl;
		std::cout << "\t." << std::endl;
		queue.enqueueTask(kernel);

		std::cout << "\n\tReturned from kernel !!!\n" << std::endl;

		// read contents from buffers

		std::cout << "Reading the contents of the two buffers now :" << std::endl;

		// two placeholder buffers to read from
		std::vector<cl_uchar> dispCharBuffer(52);
		std::vector<cl_uint> dispIntBuffer(1024);

		queue.enqueueReadBuffer(uCharBuffer, CL_TRUE, 0, sizeof(cl_uchar) * 52, &dispCharBuffer[0]);

		std::cout << "\nDone reading Second Buffer. These are its contents in a 4 * 13 matrix :\n" << std::endl;

		for ( i = 0; i < 4; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				std::cout << dispCharBuffer[(j * 1) + (i * 13)] << " ";
			}
			std::cout << std::endl;
		}

		
		queue.enqueueReadBuffer(uIntBuffer, CL_TRUE, 0, sizeof(cl_uint) * 1024, &dispIntBuffer[0]);

		std::cout << "\nDone reading Third Buffer. These are its contents in a 64 * 16 matrix:\n" << std::endl;


		for (i = 0; i < 64; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				std::cout << std::setw(4) << dispIntBuffer[(j * 1) + (i * 16)] << " ";
			}
			std::cout << std::endl;
		}

		

	}
	// catch any OpenCL function errors
	catch (cl::Error e) {
		// call function to handle errors
		handle_error(e);
	}

#ifdef _WIN32
	// wait for a keypress on Windows OS before exiting
	std::cout << "\npress a key to quit...";
	std::cin.ignore();
#endif

	return 0;
}
