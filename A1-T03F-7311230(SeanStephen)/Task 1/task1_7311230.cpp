#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

// functions to handle errors
#include "common.h"

// to avoid having to use prefixes
//using namespace std;
//using namespace cl;

int main(void) 
{
	std::vector<cl::Platform> platforms;	// available platforms
	std::string outputString;				// string for output
	unsigned int i, platformNum, deviceNum;						// counters
	std::vector<cl::Device> tempDevices;
	cl::Context context;
	cl::CommandQueue queue;
	cl::Program program;
	std::vector<cl::Kernel> kernels;

	// for each element in this vector, it represents one platform.
	// which is a vector that contains that platform's devices
	std::vector<std::vector<cl::Device>> deviceEachPlat;

	try {
		// get all available OpenCL platforms
		cl::Platform::get(&platforms);

		std::cout << "\nThere are " << platforms.size() << " platforms on your system." << std::endl;
		std::cout << "Here are their information : " << std::endl;

		for (i = 0; i < platforms.size(); i++)
		{
			// output the index + 1  for better readability
			std::cout << "\n\tPlatform " << i + 1 << std::endl;

			// platform name
			platforms[i].getInfo(CL_PLATFORM_NAME, &outputString);
			std::cout << "\tPlatform Name : " << outputString << std::endl;

			// get and output platform vendor name
			outputString = platforms[i].getInfo<CL_PLATFORM_VENDOR>();
			std::cout << "\tVendor: " << outputString << std::endl;

			// get and output OpenCL version supported by the platform
			outputString = platforms[i].getInfo<CL_PLATFORM_VERSION>();
			std::cout << "\tVersion: " << outputString << std::endl;

		}
		std::cout << "\nPlease input the corresponding platform you would like to select : ";
		
		std::cin >> platformNum;
		std::cin.ignore(100, '\n');

		std::cout << "\nYou have selected platform " << platformNum << std::endl;

		// we only want GPU or CPU
		platforms[platformNum - 1].getDevices(CL_DEVICE_TYPE_CPU | CL_DEVICE_TYPE_GPU, &tempDevices);

		std::cout << "\nThere are " << tempDevices.size() << " devices on your platform." << std::endl;
		std::cout << "Here are their information : " << std::endl;

		for (i = 0; i < tempDevices.size(); i++)
		{
			// output the index + 1  for better readability
			std::cout << "\n\tDevice " << i + 1 << std::endl;

			// Device name
			tempDevices[i].getInfo(CL_DEVICE_NAME, & outputString);
			std::cout << "\tDevice Name : " << outputString << std::endl;
		}

		std::cout << "\nPlease input the corresponding device you would like to select : ";

		std::cin >> deviceNum;
		std::cin.ignore(100, '\n');

		std::cout << "\nYou have selected device " << deviceNum << std::endl;

		std::cout << "Here is its information :" << std::endl;


		// platform name
		platforms[platformNum - 1].getInfo(CL_PLATFORM_NAME, &outputString);
		std::cout << "\n\tPlatform Name : " << outputString << std::endl;

		// device type
		cl_device_type devType;
		tempDevices[deviceNum - 1].getInfo(CL_DEVICE_TYPE, &devType);
		if (devType == CL_DEVICE_TYPE_CPU)
		{
			outputString = "CL_DEVICE_TYPE _CPU";
		}
		else
		{
			outputString = "CL_DEVICE_TYPE _GPU";
		}
		std::cout << "\tDevice Type : " << outputString << std::endl;

		// device name - returns string
		tempDevices[deviceNum - 1].getInfo(CL_DEVICE_NAME, &outputString);
		std::cout << "\tDevice Name : " << outputString << std::endl;

		// number of compute units - returns cl_uint
		cl_uint maxCompUnits;
		tempDevices[deviceNum - 1].getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &maxCompUnits);
		std::cout << "\tNumber of compute units : " << maxCompUnits << std::endl;

		// max work group size - returns size_t
		size_t maxWorkGSize;
		tempDevices[deviceNum - 1].getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &maxWorkGSize);
		std::cout << "\tMaximum Work Group Size : " << maxWorkGSize << std::endl;

		// max work item dimensions - returns cl_uint
		cl_uint maxWorkDimensions;
		tempDevices[deviceNum - 1].getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &maxWorkDimensions);
		std::cout << "\tMaximum Work Item Dimensions : " << maxWorkDimensions << std::endl;


		// max work item sizes - returns a vector of size_t
		std::vector<size_t> work_item_sizes;
		tempDevices[deviceNum - 1].getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &work_item_sizes);
		std::cout << "\tMaximum Work Item Sizes :";
		for (i = 0; i < work_item_sizes.size(); i++)
		{
			std::cout << "\n\t\t\t\t\tDimension " << i + 1 << ": " << work_item_sizes[i];
		}

		// preferred vector width (int) - returns cl_uint
		cl_uint prefVecWidth;
		tempDevices[deviceNum - 1].getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, &prefVecWidth);
		std::cout << "\n\tPreferred vector width for integers : " << prefVecWidth << std::endl;

		// local memory size - returns cl_ulong
		cl_ulong mem_size;
		tempDevices[deviceNum - 1].getInfo(CL_DEVICE_LOCAL_MEM_SIZE, &mem_size);
		std::cout << "\tLocal Memory Size : " << mem_size << std::endl;

		std::cout << "\n------------------------------------------------\n" << std::endl;

		// creating context with device
		context = cl::Context(tempDevices[deviceNum - 1]);

		// creating command queue with context and device
		queue = cl::CommandQueue(context, tempDevices[deviceNum - 1]);

		std::cout << "Context and Command Queue created! ~~~" << std::endl;

		// check whether device supports extensions
		// returns char[] delimited by spaces. use strtok to read through all, declare if found

		bool fp16 = false, fp64 = false, icd = false;

		char allExt[2000];
		std::vector<char> allExtensions;
		tempDevices[deviceNum - 1].getInfo(CL_DEVICE_EXTENSIONS, &allExt);

	//	for (i = 0; i < 2000; i++)
	//	{
	//		std::cout << allExt[i];
	//	}

		// converting to string because I want to use getline() to split the char[] to the different substrings within
		// tried to use strtok but im not sure why I could not get it to work

		// manually null-terminating allExt to prevent buffer overrun error
		allExt[2000 - 1] = '\0';

		std::string allExtString(allExt);

		std::stringstream sstream(allExtString);

		std::string tempString;

		std::vector<std::string> allExtStringVec;

		while (std::getline(sstream, tempString, ' '))
		{
			allExtStringVec.push_back(tempString);
		}

		// looping through vector of strings to check if the needed extensions are present

		for (i = 0; i < allExtStringVec.size(); i++)
		{
			if (allExtStringVec[i] == "cl_khr_fp16")
			{
				fp16 = true;
			}
			else if (allExtStringVec[i] == "cl_khr_fp64")
			{
				fp64 = true;
			}
			else if (allExtStringVec[i] == "cl_khr_icd")
			{
				icd = true;
			}
		}

		std::cout << "\nThe following extensions are / are not supported by the device :" << std::endl;
		std::cout << "\tcl_khr_fp16 : " << (fp16 ? "Supported" : "Not Supported") << std::endl;
		std::cout << "\tcl_khr_fp64 : " << (fp64 ? "Supported" : "Not Supported") << std::endl;
		std::cout << "\tcl_khr_icd  : " << (icd ? "Supported" : "Not Supported") << std::endl;
		std::cout << "\n" << std::endl;

		// building program
		if (!build_program(&program, &context, "task1.cl"))
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// display build logs
		std::string buildLogs = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(tempDevices[deviceNum - 1]);
		std::cout << "\nBuild Logs :" << std::endl;
		std::cout << (buildLogs.size() > 0 ? buildLogs : "No Build Logs were generated.") << std::endl;

		// find, display number of kernels, create kernels and display their names
		// this is the way to create all kernels at once
		program.createKernels(&kernels);

		// display number of kernels
		std::cout << "\nNumber of Kernels : " << kernels.size() << std::endl;

		std::cout << "\nAll Kernel Names : " << std::endl;

		// display all kernel names
		for (i = 0; i < kernels.size(); i++)
		{
			outputString = kernels[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
			std::cout << "\n\t\t" << outputString;
		}

		std::cout << "\n\nEnd of Assignment 1 Task 1" << std::endl;


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