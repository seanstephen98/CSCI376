__kernel void task2b (__global char2* input, __global char2*output, int cipher)
{
	int inputIndex = get_global_id(0);
	int letterIndex = 0;

	// cipher logic

	char2 beforeEncrypt  = input[inputIndex];
	char2 afterEncrypt = input[inputIndex];
	

	if (beforeEncrypt.s0 >= 'A' && beforeEncrypt.s0 <= 'Z')
	{	
		// index in the alphabet
		letterIndex = beforeEncrypt.s0 - 'A';

		// find how much to shift
		letterIndex = (letterIndex + cipher) % 26;

		// if shift brings letterIndex out of bounds, above will be negative
		if (letterIndex < 0)
		{
			letterIndex = letterIndex + 26;
		}

		// return the new index in alphabet after encryption
		afterEncrypt.s0 = 'A' + letterIndex;

	}

		if (beforeEncrypt.s1 >= 'A' && beforeEncrypt.s1 <= 'Z')
	{
		letterIndex = 0;
		
		// index in the alphabet
		letterIndex = beforeEncrypt.s1 - 'A';

		// find how much to shift
		letterIndex = (letterIndex + cipher) % 26;

		// if shift brings letterIndex out of bounds, above will be negative
		if (letterIndex < 0)
		{
			letterIndex = letterIndex + 26;
		}

		// return the new index in alphabet after encryption
		afterEncrypt.s1 = 'A' + letterIndex;

	}

	output[inputIndex] = afterEncrypt;

}

