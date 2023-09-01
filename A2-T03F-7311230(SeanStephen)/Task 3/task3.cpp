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
#include "bmpfuncs.h"

int main(void) 
{
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device

	// declare data and memory objects
	unsigned char* inputImage;
	unsigned char* outputImage;
	unsigned char* thirdImage;
	int imgWidth, imgHeight, imageSize;

	cl::ImageFormat imgFormat;
	cl::Image2D inputImgBuffer, outputImgBuffer, thirdImgBuffer;

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
		if(!build_program(&program, &context, "simple_conv.cl")) 
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}


		// create command queue
		queue = cl::CommandQueue(context, device);

		// 3a
			// create a kernel
			kernel = cl::Kernel(program, "task3a");		

			// read input image
			inputImage = read_BMP_RGB_to_RGBA("mandrill.bmp", &imgWidth, &imgHeight);

			// allocate memory for output image
			imageSize = imgWidth * imgHeight * 4;
			outputImage = new unsigned char[imageSize];

			// image format
			imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);

			// create image objects
			inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
			outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

			// set kernel arguments
			kernel.setArg(0, inputImgBuffer);
			kernel.setArg(1, outputImgBuffer);
		
			// enqueue kernel
			cl::NDRange offset(0, 0);
			cl::NDRange globalSize(imgWidth, imgHeight);

			queue.enqueueNDRangeKernel(kernel, offset, globalSize);

			std::cout << "Kernel enqueued." << std::endl;
			std::cout << "--------------------" << std::endl;

			// enqueue command to read image from device to host memory
			cl::size_t<3> origin, region;
			origin[0] = origin[1] = origin[2] = 0;
			region[0] = imgWidth;
			region[1] = imgHeight;
			region[2] = 1;

			queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

			// output results to image file
			write_BMP_RGBA_to_RGB("3a.bmp", outputImage, imgWidth, imgHeight);

			std::cout << "Done with Task 3a." << std::endl;

			// deallocate memory
			free(inputImage);
			free(outputImage);

		// 3b
		//	3bi
			// create a kernel
			kernel = cl::Kernel(program, "task3bi");

			// read input image
			inputImage = read_BMP_RGB_to_RGBA("mandrill.bmp", &imgWidth, &imgHeight);

			// allocate memory for output image
			imageSize = imgWidth * imgHeight * 4;
			outputImage = new unsigned char[imageSize];

			// image format
			imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);

			// create image objects
			inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
			outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

			// set kernel arguments
			kernel.setArg(0, inputImgBuffer);
			kernel.setArg(1, outputImgBuffer);

			queue.enqueueNDRangeKernel(kernel, offset, globalSize);

			std::cout << "Kernel enqueued." << std::endl;
			std::cout << "--------------------" << std::endl;

			// enqueue command to read image from device to host memory
			//cl::size_t<3> origin, region;
			origin[0] = origin[1] = origin[2] = 0;
			region[0] = imgWidth;
			region[1] = imgHeight;
			region[2] = 1;

			queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

			// output results to image file
			write_BMP_RGBA_to_RGB("3bi.bmp", outputImage, imgWidth, imgHeight);

			std::cout << "Done with Task 3bi." << std::endl;

			// deallocate memory
			free(inputImage);
			free(outputImage);

		//	3bii
			// create a kernel
			kernel = cl::Kernel(program, "task3bii");

			// read input image
			inputImage = read_BMP_RGB_to_RGBA("mandrill.bmp", &imgWidth, &imgHeight);

			// allocate memory for output image
			imageSize = imgWidth * imgHeight * 4;
			outputImage = new unsigned char[imageSize];

			// image format
			imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);

			// create image objects
			inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
			outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

			// set kernel arguments for horizontal pass
			kernel.setArg(0, inputImgBuffer);
			kernel.setArg(1, outputImgBuffer);
			kernel.setArg(2, 1);

			queue.enqueueNDRangeKernel(kernel, offset, globalSize);

			std::cout << "Kernel enqueued, 3bii pass 1." << std::endl;
			std::cout << "--------------------" << std::endl;

			// set kernel arguments for vertical pass
			kernel.setArg(0, inputImgBuffer);
			kernel.setArg(1, outputImgBuffer);
			kernel.setArg(2, 2);

			queue.enqueueNDRangeKernel(kernel, offset, globalSize);

			std::cout << "Kernel enqueued, 3bii pass 2." << std::endl;
			std::cout << "--------------------" << std::endl;

			// enqueue command to read image from device to host memory
			//cl::size_t<3> origin, region;
			origin[0] = origin[1] = origin[2] = 0;
			region[0] = imgWidth;
			region[1] = imgHeight;
			region[2] = 1;

			queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

			// output results to image file
			write_BMP_RGBA_to_RGB("3bii.bmp", outputImage, imgWidth, imgHeight);

			std::cout << "Done with Task 3bii." << std::endl;

			// deallocate memory
			free(inputImage);
			free(outputImage);

		//	3c2
			// create a kernel
			kernel = cl::Kernel(program, "task3c2");

			// read input image
			inputImage = read_BMP_RGB_to_RGBA("mandrill.bmp", &imgWidth, &imgHeight);

			// allocate memory for output image
			imageSize = imgWidth * imgHeight * 4;
			outputImage = new unsigned char[imageSize];

			// image format
			imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);

			// create image objects
			inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
			outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

			// set kernel arguments for horizontal pass
			kernel.setArg(0, inputImgBuffer);
			kernel.setArg(1, outputImgBuffer);

			queue.enqueueNDRangeKernel(kernel, offset, globalSize);

			std::cout << "Kernel enqueued, 3c2." << std::endl;
			std::cout << "--------------------" << std::endl;

			// enqueue command to read image from device to host memory
			//cl::size_t<3> origin, region;
			origin[0] = origin[1] = origin[2] = 0;
			region[0] = imgWidth;
			region[1] = imgHeight;
			region[2] = 1;

			queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

			// output results to image file
			write_BMP_RGBA_to_RGB("Task3c1.bmp", outputImage, imgWidth, imgHeight);

			std::cout << "Done with Task 3c2." << std::endl;

			// deallocate memory
			free(inputImage);
			free(outputImage);


		//	3c3
		// horizontal pass
			// create a kernel
			kernel = cl::Kernel(program, "task3bii");

			// read input image
			inputImage = read_BMP_RGB_to_RGBA("Task3c1.bmp", &imgWidth, &imgHeight);

			// allocate memory for output image
			imageSize = imgWidth * imgHeight * 4;
			outputImage = new unsigned char[imageSize];

			// image format
			imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);

			// create image objects
			inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
			outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

			// set kernel arguments for horizontal pass
			kernel.setArg(0, inputImgBuffer);
			kernel.setArg(1, outputImgBuffer);
			kernel.setArg(2, 1);

			queue.enqueueNDRangeKernel(kernel, offset, globalSize);

			std::cout << "Kernel enqueued, 3c3, horizontal pass." << std::endl;
			std::cout << "--------------------" << std::endl;

			// enqueue command to read image from device to host memory
			//cl::size_t<3> origin, region;
			origin[0] = origin[1] = origin[2] = 0;
			region[0] = imgWidth;
			region[1] = imgHeight;
			region[2] = 1;

			queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

			// output results to image file
			write_BMP_RGBA_to_RGB("Task3c2.bmp", outputImage, imgWidth, imgHeight);

			std::cout << "Done with Task 3c3, horizontal pass." << std::endl;

			// deallocate memory
			free(inputImage);
			free(outputImage);

		// vertical pass
			// create a kernel
			//kernel = cl::Kernel(program, "task3bii");

			// read input image
			inputImage = read_BMP_RGB_to_RGBA("Task3c2.bmp", &imgWidth, &imgHeight);

			// allocate memory for output image
			imageSize = imgWidth * imgHeight * 4;
			outputImage = new unsigned char[imageSize];

			// image format
			imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);

			// create image objects
			inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
			outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

			// set kernel arguments for horizontal pass
			kernel.setArg(0, inputImgBuffer);
			kernel.setArg(1, outputImgBuffer);
			kernel.setArg(2, 2);

			queue.enqueueNDRangeKernel(kernel, offset, globalSize);

			std::cout << "Kernel enqueued, 3c3, vertical pass." << std::endl;
			std::cout << "--------------------" << std::endl;

			// enqueue command to read image from device to host memory
			//cl::size_t<3> origin, region;
			origin[0] = origin[1] = origin[2] = 0;
			region[0] = imgWidth;
			region[1] = imgHeight;
			region[2] = 1;

			queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

			// output results to image file
			write_BMP_RGBA_to_RGB("Task3c3.bmp", outputImage, imgWidth, imgHeight);

			std::cout << "Done with Task 3c3, vertical pass." << std::endl;

			// deallocate memory
			free(inputImage);
			free(outputImage);

		// 3c4
		
			bool stayInLoop = true;
			int input = 0;
			// get input from user
			while (stayInLoop == true) {
				std::cout << "Please enter a valid threshold value for Task 3c4 (0 - 255) only : ";
				std::cin >> input;
				if (input >= 0 && input <= 255) {
					stayInLoop = false;
				}
			}
			std::cin.ignore(100, '\n');
			
			// get the 0 - 1 value to use in device
			float threshold = input / 255.0f;
			std::cout << threshold << std::endl;
			
			// create a kernel
			kernel = cl::Kernel(program, "task3c4");

			// read input image
			inputImage = read_BMP_RGB_to_RGBA("Task3c3.bmp", &imgWidth, &imgHeight);

			// allocate memory for output image
			imageSize = imgWidth * imgHeight * 4;
			outputImage = new unsigned char[imageSize];

			// image format
			imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);

			// create image objects
			inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
			outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

			// set kernel arguments for horizontal pass
			kernel.setArg(0, inputImgBuffer);
			kernel.setArg(1, outputImgBuffer);
			kernel.setArg(2, threshold);

			queue.enqueueNDRangeKernel(kernel, offset, globalSize);

			std::cout << "\nKernel enqueued, 3c4." << std::endl;
			std::cout << "--------------------" << std::endl;

			// enqueue command to read image from device to host memory
			//cl::size_t<3> origin, region;
			origin[0] = origin[1] = origin[2] = 0;
			region[0] = imgWidth;
			region[1] = imgHeight;
			region[2] = 1;

			queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

			// output results to image file
			write_BMP_RGBA_to_RGB("Task3c4.bmp", outputImage, imgWidth, imgHeight);

			std::cout << "Done with Task 3c4." << std::endl;

			// deallocate memory
			free(inputImage);
			free(outputImage);

		// 3c5

			// create a kernel
			kernel = cl::Kernel(program, "task3c5");

			// read input image
			inputImage = read_BMP_RGB_to_RGBA("Task3c3.bmp", &imgWidth, &imgHeight);
			thirdImage = read_BMP_RGB_to_RGBA("Task3c4.bmp", &imgWidth, &imgHeight);

			// allocate memory for output image
			imageSize = imgWidth * imgHeight * 4;
			outputImage = new unsigned char[imageSize];

			// image format
			imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);

			// create image objects
			inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
			outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);
			thirdImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)thirdImage);

			// set kernel arguments for horizontal pass
			kernel.setArg(0, inputImgBuffer);
			kernel.setArg(1, outputImgBuffer);
			kernel.setArg(2, thirdImgBuffer);

			queue.enqueueNDRangeKernel(kernel, offset, globalSize);

			std::cout << "Kernel enqueued, 3c5." << std::endl;
			std::cout << "--------------------" << std::endl;

			// enqueue command to read image from device to host memory
			//cl::size_t<3> origin, region;
			origin[0] = origin[1] = origin[2] = 0;
			region[0] = imgWidth;
			region[1] = imgHeight;
			region[2] = 1;

			queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

			// output results to image file
			write_BMP_RGBA_to_RGB("Task3c5.bmp", outputImage, imgWidth, imgHeight);

			std::cout << "Done with Task 3c5." << std::endl;

			// deallocate memory
			free(inputImage);
			free(outputImage);
			free(thirdImage);

		// 3c6
			// create a kernel
			kernel = cl::Kernel(program, "task3c6");

			// read input image
			inputImage = read_BMP_RGB_to_RGBA("Task3c5.bmp", &imgWidth, &imgHeight);
			thirdImage = read_BMP_RGB_to_RGBA("noise.bmp", &imgWidth, &imgHeight);

			// allocate memory for output image
			imageSize = imgWidth * imgHeight * 4;
			outputImage = new unsigned char[imageSize];

			// image format
			imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);

			// create image objects
			inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
			outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);
			thirdImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)thirdImage);

			// set kernel arguments for horizontal pass
			kernel.setArg(0, inputImgBuffer);
			kernel.setArg(1, outputImgBuffer);
			kernel.setArg(2, thirdImgBuffer);

			queue.enqueueNDRangeKernel(kernel, offset, globalSize);

			std::cout << "Kernel enqueued, 3c6." << std::endl;
			std::cout << "--------------------" << std::endl;

			// enqueue command to read image from device to host memory
			//cl::size_t<3> origin, region;
			origin[0] = origin[1] = origin[2] = 0;
			region[0] = imgWidth;
			region[1] = imgHeight;
			region[2] = 1;

			queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

			// output results to image file
			write_BMP_RGBA_to_RGB("Task3c6.bmp", outputImage, imgWidth, imgHeight);

			std::cout << "Done with Task 3c6." << std::endl;

			// deallocate memory
			free(inputImage);
			free(outputImage);
			free(thirdImage);

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
