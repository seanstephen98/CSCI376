__kernel void task1(__global int4* input1, __global int* input2, __global int* output) {

	// get global id
	int globalID = get_global_id(0);
	__local int8 v;
	__local int8 v1;
	__local int8 v2;
	int8 mask;
	int8 results;
	if (globalID == 0)
	{
		
		v = (int8)(input1[globalID], input1[globalID + 1]);

		//printf("input1-1 = %v4hld", input1[0]);
		v1 = vload8(0, input2);
		
		v2 = vload8(1, input2);

		mask = v > 16 ;

		printf("v = %v8hld", v);
		printf("v1 = %v8hld", v1);
		printf("v2 = %v8hld", v2);
		printf("mask = %v8hld", mask);
		
		if (any(v > 16))
		{			
			
			

			results = select(v2, v1, mask);
			

		}
		else
		{
			//*results = (int8)((int4)(v1.s0123), (int4)(v2.s0123));
			results = (int8)(v1.s0123, v2.s0123);
		}

		vstore8(v, 0, output);
		vstore8(v1, 1, output);
		vstore8(v2, 2, output);
		vstore8(results, 3, output);


	}
}
