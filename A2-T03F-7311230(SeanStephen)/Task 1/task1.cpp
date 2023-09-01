#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <fstream>

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

	// declare data and memory objects
	std::vector<cl_int> data(2);
	cl::Buffer dataBuffer, input1Buffer, input2Buffer, outputBuffer;
	std::string devExt;				// string for device extensions


	// task 1 part 1 vectors
	std::vector<cl_int> vec1(8), vec2(16), outputVec(32, 0), expectedVec(32);


	try {
		// select an OpenCL device
		if (!select_one_device(&platform, &device))
		{
			// if no device selected
			quit_program("Device not selected.");
		}


		// initialise values for vec1
		// use 10 + random number

		// seed rand
		srand(time(0));

		for (int i = 0; i < 8; i++)
		{
			vec1[i] = 10 + (rand() % 10);
		}


		// initialise values for vec2
		// for 0 - 7, use the index to intialise
		// for 8 - 15, use index - 16

		for (int i = 0; i < 16; i++)
		{
			if (i < 8)
			{
				vec2[i] = i;
			}

			else
			{
				vec2[i] = i - 16;
			}
		}

		// create a context from device
		context = cl::Context(device);

		// build the program
		if(!build_program(&program, &context, "task1kernel.cl")) 
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// create a kernel
		kernel = cl::Kernel(program, "task1");

		// create command queue
		queue = cl::CommandQueue(context, device);

		// create buffers

		input1Buffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * 8, &vec1[0]);

		input2Buffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * 16, &vec2[0]);

		outputBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_int) * 32);

		std::cout << "\nDisplaying vec1\n" << std::endl;

		for (int i = 0; i < vec1.size(); i++)
		{
			std::cout << vec1[i] << " ";
		}

		std::cout << "\n\nDisplayed vec1\n" << std::endl;

		std::cout << "\nDisplaying vec2\n" << std::endl;

		for (int i = 0; i < vec2.size(); i++)
		{
			std::cout << vec2[i] << " ";
		}

		std::cout << "\n\nDisplayed vec2\n" << std::endl;

		// set kernel arguments
		kernel.setArg(0, input1Buffer);
		kernel.setArg(1, input2Buffer);
		kernel.setArg(2, outputBuffer);


		queue.enqueueTask(kernel);
		queue.finish();

		queue.enqueueReadBuffer(outputBuffer, CL_TRUE, 0, sizeof(cl_int) * 32, &outputVec[0]);
		
		std::cout << "\nDisplaying output\n" << std::endl;

		for (int i = 0; i < outputVec.size(); i++)
		{
			std::cout << outputVec[i] << " ";
		}

		std::cout << "\n\nDisplayed output\n" << std::endl;




		// actual expected array

		bool over16 = false;

		for (int i = 0; i < 8; i++)
		{
			if (vec1[i] > 16)
			{
				over16 = true;
			}
			//expectedVec.push_back(vec1[i]);
			expectedVec[i] = vec1[i];
		}

		for (int i = 0; i < vec2.size(); i++)
		{
			//expectedVec.push_back(vec2[i]);
			expectedVec[i + 8] = vec2[i];
		}

		if (over16)
		{
			for (int i = 0; i < 8; i++)
			{
				if (vec1[i] > 16)
				{
					//expectedVec.push_back(vec2[i]);
					expectedVec[i + 24] = vec2[i];
				}
				else
				{
					//expectedVec.push_back(vec2[i + 8]);
					expectedVec[i + 24] = vec2[i + 8];
				}
			}
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				if (i < 4)
				{
					//expectedVec.push_back(vec2[i]);
					expectedVec[i + 24] = vec2[i];
				}
				else
				{
					//expectedVec.push_back(vec2[i + 4]);
					expectedVec[i + 24] = vec2[i + 4];
				}
			}
		}

		std::cout << "\nDisplaying expected\n" << std::endl;

		for (int i = 0; i < expectedVec.size(); i++)
		{
			std::cout << expectedVec[i] << " ";
		}

		std::cout << "\n\nDisplayed expectedVec\n" << std::endl;

		bool correctResult = true;

		for (int i = 0; i < 32; i++)
		{
			if (outputVec[i] != expectedVec[i])
			{
				correctResult = false;
			}
		}

		std::cout << "\nResults Correct ? : " << (correctResult ? "Yes" : "No") << std::endl;

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
