__kernel void task2cE (__global char2* input, __global char2*output,__global char* substituteCipher)
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

		// return the new index in alphabet after encryption
		afterEncrypt.s0 = substituteCipher[letterIndex];

	}

		if (beforeEncrypt.s1 >= 'A' && beforeEncrypt.s1 <= 'Z')
	{
		letterIndex = 0;
		
		// index in the alphabet
		letterIndex = beforeEncrypt.s1 - 'A';

		// return the new index in alphabet after encryption
		afterEncrypt.s1 = substituteCipher[letterIndex];

	}

	output[inputIndex] = afterEncrypt;

}

__kernel void task2cD (__global char2* input, __global char2*output, __global char* substituteCipher)
{
	int inputIndex = get_global_id(0);
	int i = 0;
	// cipher logic

	char2 beforeDecrypt  = input[inputIndex];
	char2 afterDecrypt = input[inputIndex];
	

	if (beforeDecrypt.s0 >= 'A' && beforeDecrypt.s0 <= 'Z')
	{	
		// index in the cipher
		for (i = 0; i < 26; i++)
		{
			if (beforeDecrypt.s0 == substituteCipher[i])
			{
				break;
			}
		}

		// return the index in substituteCipher + 65 to get the actual alphabet
		afterDecrypt.s0 = i + 'A';

	}

	if (beforeDecrypt.s1 >= 'A' && beforeDecrypt.s1 <= 'Z')
	{	
		// index in the cipher
		for (i = 0; i < 26; i++)
		{
			if (beforeDecrypt.s1 == substituteCipher[i])
			{
				break;
			}
		}

		// return the index in substituteCipher + 65 to get the actual alphabet
		afterDecrypt.s1 = i + 'A';

	}

	output[inputIndex] = afterDecrypt;

}