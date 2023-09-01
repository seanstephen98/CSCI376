__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | 
      CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST; 

// 3x3 Vertical Sobel edge detection filter
__constant float vSobelFilter[9] = {-1.0, -2.0, -1.0, 
									 0.0,  0.0,  0.0, 
									 1.0,  2.0,  1.0};
// 3x3 Blurring filter
__constant float BlurringFilter[9] = {	1.0/9, 1.0/9, 1.0/9, 
										1.0/9, 1.0/9, 1.0/9, 
										1.0/9, 1.0/9, 1.0/9};
// 3x3 Sharpening filter
__constant float SharpeningFilter[9] = { 0.0, -1.0, 0.0, 
										-1.0, 5.0, -1.0, 
										 0.0, -1.0, 0.0};

// 7x7 Gaussian Blur filter
__constant float gaussianFilter77[49] = {0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036,
										0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363,
										0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446,
										0.002291, 0.023226, 0.092651, 0.146768, 0.092651, 0.023226, 0.002291,
										0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446,
										0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363,
										0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036};

// 1x7 Gaussian Blur filter
__constant float gaussianFilter17[7] = {0.00598, 0.060626, 0.241843, 0.383103, 0.241843, 0.060626, 0.00598};



__kernel void simple_conv(read_only image2d_t src_image,
					write_only image2d_t dst_image) {

   /* Get work-item’s row and column position */
   int column = get_global_id(0); 
   int row = get_global_id(1);

   /* Accumulated pixel value */
   float4 sum = (float4)(0.0);

   /* Filter's current index */
   int filter_index =  0;

   int2 coord;
   float4 pixel;

   /* Iterate over the rows */
   for(int i = -1; i <= 1; i++) {
	  coord.y =  row + i;

      /* Iterate over the columns */
	  for(int j = -1; j <= 1; j++) {
         coord.x = column + j;

		 /* Read value pixel from the image */ 		
		 pixel = read_imagef(src_image, sampler, coord);
		 /* Acculumate weighted sum */ 		
		 sum.xyz += pixel.xyz * SharpeningFilter[filter_index++];
	  }
   }

   /* Write new pixel value to output */
   coord = (int2)(column, row); 
   write_imagef(dst_image, coord, sum);
}

__kernel void task3a(read_only image2d_t src_image,
						write_only image2d_t dst_image) {

   // global id 0 for x, global id 1 for y
   int2 coord = (int2)(get_global_id(0), get_global_id(1));

   // get pixel xyz with coordinates
   float4 pixel = read_imagef(src_image, sampler, coord);

   // get luminance based on given formula
   float luminance = (0.299f * pixel.x) + (0.587f * pixel.y) + (0.114 * pixel.z);

   // set each RGB to the same value - luminance
   pixel.xyz = luminance;

   // write out the result
   write_imagef(dst_image, coord, pixel);
}

__kernel void task3bi(	read_only image2d_t src_image,
						write_only image2d_t dst_image) {

   // global id 0 for x, global id 1 for y
   int2 coord = (int2)(get_global_id(0), get_global_id(1));

    /* Accumulated pixel value */
    float4 sum = (float4)(0.0);

     /* Filter's current index */
   int filter_index =  0;

    int2 tempCoord;
	float4 pixel;

    for(int i = -3; i <= 3; i++){
        tempCoord.y = coord.y + i;
        for (int j = -3; j <= 3; j++){
            tempCoord.x = coord.x + j;
            /* Read value pixel from the image */ 		
			pixel = read_imagef(src_image, sampler, tempCoord);
			/* Acculumate weighted sum */ 		
			sum.xyz += pixel.xyz * gaussianFilter77[filter_index++];

        }
    }
   
    /* Write new pixel value to output */
    write_imagef(dst_image, coord, sum);
}

__kernel void task3bii(	read_only image2d_t src_image,
						write_only image2d_t dst_image, int pass) {

   // global id 0 for x, global id 1 for y
   int2 coord = (int2)(get_global_id(0), get_global_id(1));

    /* Accumulated pixel value */
    float4 sum = (float4)(0.0);

     /* Filter's current index */
   int filter_index =  0;

    int2 tempCoord;
	float4 pixel;

	// if pass = 1, horizontal
	// if pass = 2, vertical

	if (pass == 1)
	{
		tempCoord.y = coord.y;

		for (int j = -3; j <= 3; j++){
			tempCoord.x = coord.x + j;
			/* Read value pixel from the image */ 		
			pixel = read_imagef(src_image, sampler, tempCoord);
			/* Acculumate weighted sum */ 		
			sum.xyz += pixel.xyz * gaussianFilter17[filter_index++];
		}
    }    
	if (pass == 2)
	{
		tempCoord.x = coord.x;

		for (int j = -3; j <= 3; j++){
			tempCoord.y = coord.y + j;
			/* Read value pixel from the image */ 		
			pixel = read_imagef(src_image, sampler, tempCoord);
			/* Acculumate weighted sum */ 		
			sum.xyz += pixel.xyz * gaussianFilter17[filter_index++];
		}
        
	}
   
    /* Write new pixel value to output */
    write_imagef(dst_image, coord, sum);
}

__kernel void task3c2(read_only image2d_t src_image,
						write_only image2d_t dst_image) {

   // global id 0 for x, global id 1 for y
   int2 coord = (int2)(get_global_id(0), get_global_id(1));

   // get pixel xyz with coordinates
   float4 pixel = read_imagef(src_image, sampler, coord);

   // get luminance based on given formula
   float luminance = (0.299f * pixel.x) + (0.587f * pixel.y) + (0.114 * pixel.z);

   // set ONLY G, GREEN to the luminance value
   pixel.x = 0.0f;
   pixel.y = luminance;
   pixel.z = 0.0f;

   // write out the result
   write_imagef(dst_image, coord, pixel);
}

__kernel void task3c4(read_only image2d_t src_image,
						write_only image2d_t dst_image, float threshold) {

   // global id 0 for x, global id 1 for y
   int2 coord = (int2)(get_global_id(0), get_global_id(1));

   // get pixel xyz with coordinates
   float4 pixel = read_imagef(src_image, sampler, coord);

   // change to 0 if less than threshold
//   if (pixel.x < threshold)
//   {
//		pixel.x = 0.0f;
//   }
   if (pixel.y < threshold)
   {
		pixel.y = 0.0f;
   }
//   if (pixel.z < threshold)
//   {
//		pixel.z = 0.0f;
//   }
   

   // write out the result
   write_imagef(dst_image, coord, pixel);
}

__kernel void task3c5 (read_only image2d_t src_image,
						write_only image2d_t dst_image, read_only image2d_t third_image) {

   // global id 0 for x, global id 1 for y
   int2 coord = (int2)(get_global_id(0), get_global_id(1));

   // get pixel xyz with coordinates
   float4 pixelSrc = read_imagef(src_image, sampler, coord);
   float4 pixel3rd = read_imagef(third_image, sampler, coord);

   // add the two pixels values
   pixelSrc.y += pixel3rd.y;

   // if more than maximum colour value 1, set to 1
//   if (pixelSrc.x > 1.0f)
//   {
//		pixelSrc.x = 1.0f;
//   }
   if (pixelSrc.y > 1.0f)
   {
		pixelSrc.y = 1.0f;
   }
//   if (pixelSrc.z > 1.0f)
//   {
//		pixelSrc.z = 1.0f;
//   }
   
   // write out the result
   write_imagef(dst_image, coord, pixelSrc);
}

__kernel void task3c6 (read_only image2d_t src_image,
						write_only image2d_t dst_image, read_only image2d_t third_image) {

   // global id 0 for x, global id 1 for y
   int2 coord = (int2)(get_global_id(0), get_global_id(1));

   // get pixel xyz with coordinates
   float4 pixelSrc = read_imagef(src_image, sampler, coord);
   float4 pixel3rd = read_imagef(third_image, sampler, coord);

   // multiply the two pixels values
	//pixelSrc *= pixel3rd;

   //pixelSrc.x = pixelSrc.x * pixel3rd.x;
   pixelSrc.y = pixelSrc.y * pixel3rd.y;
   //pixelSrc.z = pixelSrc.z * pixel3rd.z;

   // if more than maximum colour value 1, set to 1
//   if (pixelSrc.x > 1.0f)
//   {
//		pixelSrc.x = 1.0f;
//   }
   if (pixelSrc.y > 1.0f)
   {
		pixelSrc.y = 1.0f;
   }
//   if (pixelSrc.z > 1.0f)
//   {
//		pixelSrc.z = 1.0f;
//   }
   
   // write out the result
   write_imagef(dst_image, coord, pixelSrc);
}