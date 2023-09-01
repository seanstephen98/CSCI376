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

#define LENGTH 512

#include "common.h"

int main(void) 
{
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device

	// declare data and memory objects
	cl::Buffer addBuffer;

	// this will be our base array. to create the result array
	// we just need to offset by 1, then add the input number * global id
	std::vector<cl_int> all3s(512, 3), resultVec(512);


	try {
		// select an OpenCL device
		if (!select_one_device(&platform, &device))
		{
			// if no device selected
			quit_program("Device not selected.");
		}

		// create a context from device
		context = cl::Context(device);

		// build the program
		if(!build_program(&program, &context, "arrayVec_mult.cl")) 
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// create a kernel
		kernel = cl::Kernel(program, "arrayVec_mult");

		// create command queue
		queue = cl::CommandQueue(context, device);

		// create buffer
		addBuffer = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * LENGTH, &all3s[0]);

		// get input from user
		
		int numInput;
		bool withinRange = false;

		// check if the value is between 2 and 99 inclusive
		while (!withinRange)
		{
			std::cout << "\nDear User, This program takes in a single number between 2 and 99 inclusive. Please enter a number : ";
			std::cin >> numInput;
			std::cin.ignore(100, '\n');

			if (numInput >= 2 && numInput <= 99)
			{
				withinRange = true;
			}

			else
			{
				std::cout << "\nNumber not between 2 and 99 Inclusive. Please try again." << std::endl;
			}
		}


		std::cout << "\nPlease note that you have entered : " << numInput << std::endl;

		std::cout << "\nOriginal Array : \n" << std::endl;

		for (int i = 0; i < 64; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << std::setw(4) << all3s[j + i * 4] << " ";
			}
			std::cout << std::endl;
		}

		// set kernel arguments
		kernel.setArg(0, numInput);
		kernel.setArg(1, addBuffer);


		// enqueue kernel for execution
		//queue.enqueueTask(kernel);

		// offset 1 because we want the first element to remain 1
		// globalSize 512 because we want to use the index of the array to compute how much to increment
		cl::NDRange offset(1);
		cl::NDRange globalSize(512);

		queue.enqueueNDRangeKernel(kernel, offset, globalSize);
			   
		std::cout << "\nKernel enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read from device to host memory
		queue.enqueueReadBuffer(addBuffer, CL_TRUE, 0, sizeof(cl_int) * LENGTH, &resultVec[0]);


		std::cout << "\n Our Results : \n" << std::endl;

		// check the results
		for (int i = 0; i < 64; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << std::setw(4) << resultVec[j + i * 4] << " ";
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
