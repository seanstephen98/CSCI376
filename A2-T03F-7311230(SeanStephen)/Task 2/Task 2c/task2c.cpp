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

// callback functions
void CL_CALLBACK kernel_complete(cl_event e, cl_int status, void* data) 
{
	// display the data
	printf("%s", (char*)data);
}

void CL_CALLBACK check_data(cl_event e, cl_int status, void* data) 
{
	cl_bool check;
	cl_float *bufferData;

	// check the data whether the contents are all 5.0
	bufferData = (cl_float*)data;
	check = CL_TRUE;
	for (int i = 0; i < 4096; i++) {
		if (bufferData[i] != 5.0) {
			check = CL_FALSE;
			break;
		}
	}
	if (check)
		std::cout << "The data is accurate." << std::endl;
	else
		std::cout << "The data is not accurate." << std::endl;
}

int main(void) 
{
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device

	// declare data and memory objects
	std::vector<cl_char> fileReadVec, outputVec;
	cl::Buffer inBuffer, outBuffer, substituteCipherBuffer;

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
		if (!build_program(&program, &context, "task2c.cl"))
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// create 2cE kernel
		kernel = cl::Kernel(program, "task2cE");

		// create command queue
		queue = cl::CommandQueue(context, device);

		// explicit declaration of substitute cipher
		std::vector<cl_char> substituteCipherVec = { 'C', 'I', 'S', 'Q', 'V',
													 'N', 'F', 'O', 'W', 'A',
													 'X', 'M', 'T', 'G', 'U',
													 'H', 'P', 'B', 'K', 'L',
													 'R', 'E', 'Y', 'D', 'Z',
													 'J' };
		substituteCipherBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * substituteCipherVec.size(), &substituteCipherVec[0]);


		// read file

		std::ifstream plainText("plaintext.txt");
		if (plainText.is_open())
		{
			
			char temp;
			int tempCount = 0;
			while (plainText.get(temp))
			{
				fileReadVec.push_back(toupper(temp));
			}
			plainText.close();

			std::cout << "\nDone Reading plaintext.txt" << std::endl;

			//std::cout << "\n" << std::endl;
			//for (int i = 0; i < fileReadVec.size(); i++)
			//{
			//	std::cout << fileReadVec[i];
			//}


			// encryption

				// create buffers
				inBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * fileReadVec.size(), &fileReadVec[0]);
				outBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_char) * fileReadVec.size());

				// set kernel args, enqueue

				kernel.setArg(0, inBuffer);
				kernel.setArg(1, outBuffer);
				kernel.setArg(2, substituteCipherBuffer);

				cl::NDRange offset(0);
				cl::NDRange globalSize(fileReadVec.size());

				queue.enqueueNDRangeKernel(kernel, offset, globalSize);

				// read results
				outputVec.resize(fileReadVec.size());

				queue.enqueueReadBuffer(outBuffer, CL_TRUE, 0, sizeof(cl_char) * fileReadVec.size(), &outputVec[0]);

				// output kernel results to file

				std::ofstream encryptFile("ciphertext.txt");
				if (encryptFile.is_open())
				{
					for (int i = 0; i < outputVec.size(); i++)
					{
						encryptFile << outputVec[i];
					}

					encryptFile.close();
					std::cout << "\nCreated ciphertext.txt" << std::endl;
				}
				else
				{
					std::cout << "\nCannot open ciphertext.txt, please fix it and try again" << std::endl;
				}

			// decryption


				// recreate buffers, now with the encrypted as source
				inBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * fileReadVec.size(), &outputVec[0]);
				outBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_char) * fileReadVec.size());

				// create 2cD kernel
				kernel = cl::Kernel(program, "task2cD");

				// set kernel args, enqueue

				kernel.setArg(0, inBuffer);
				kernel.setArg(1, outBuffer);
				kernel.setArg(2, substituteCipherBuffer);

				queue.enqueueNDRangeKernel(kernel, offset, globalSize);

				// read results

				queue.enqueueReadBuffer(outBuffer, CL_TRUE, 0, sizeof(cl_char) * fileReadVec.size(), &outputVec[0]);

				// output kernel results to file

				std::ofstream decryptFile("decrypt.txt");
				if (decryptFile.is_open())
				{
					for (int i = 0; i < outputVec.size(); i++)
					{
						decryptFile << outputVec[i];
					}

					decryptFile.close();
					std::cout << "\nCreated decrypt.txt" << std::endl;

				}
				else
				{
					std::cout << "\nCannot open decrypt.txt, please fix it and try again" << std::endl;
				}

		}
		else
		{
			std::cout << "\nCannot open plaintext.txt, please fix it and try again" << std::endl;
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
