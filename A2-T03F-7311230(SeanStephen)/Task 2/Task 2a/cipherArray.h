#include <cmath>

#define CIPHER_ARRAY_LENGTH 26

class cipherArray
{
	char cArray[CIPHER_ARRAY_LENGTH];
	void setCipher(int starting) {
		// initialise cipher for the stated length, based on starting value from ASCII
		for (int i = 0; i < CIPHER_ARRAY_LENGTH; i++)
		{
			cArray[i] = starting + i;
		}
	}
	// used to check if input is a letter. if it isn't, returns -1
	// returns 1 if it is capital letter, 2 if small letter
	// good if we only want to change letters
	int checkLetter(int input)
	{
		if (input >= 65 && input <= 90)
			return 1;
		if (input >= 97 && input <= 122)
			return 2;
		return -1;
	}

	// gets index in cArray based on letterType, for letters only
	// checks if capital (1) / small (2)
	int getLetterIndex(int input, int letterType)
	{
		// if capital
		if (letterType == 1)
		{
			return (input - 65);
		}

		// if small
		{
			return (input - 97);
		}
	}
	// method to get value in array, allows wraparound
	// checks if encrypt (1) / decrypt (2)
	int getValue(int input, int encrypt, int cipher, int letterType)
	{
		// if encryption
		if (encrypt == 1)
		{

			// find index after considering wrap around offset with modulo
			int index = fabs((getLetterIndex(input, letterType) + cipher + CIPHER_ARRAY_LENGTH) % CIPHER_ARRAY_LENGTH);
			return cArray[index];

		}
		// decryption
		else if (encrypt == 2)
		{
			cipher *= -1;
			int index = fabs((getLetterIndex(input, letterType) + cipher + CIPHER_ARRAY_LENGTH) % CIPHER_ARRAY_LENGTH);
			return cArray[index];
		}

		// return input if neither
		return input;

	}

	// for when we want to manually set a specific value
	void manualSetValue(int input, int index)
	{
		index = index % CIPHER_ARRAY_LENGTH;

		cArray[index] = input;
	}


public:
	cipherArray(int starting) {
		setCipher(starting);
	}

	int getEncryptionLetter(int input, int encryption)
	{
		// gets result from checkLetter
		int result = checkLetter(input);

		// switch, different cases if capital, small, not letter or others
		switch (result)
		{
		case 1:
			return getValue(input, 1, encryption, 1);
			break;
		case 2:
			return getValue(input, 1, encryption, 2);
			break;
		default:
			return input;

		}
	}

	int getDecryptionLetter(int input, int encryption)
	{
		// gets result from checkLetter
		int result = checkLetter(input);

		// switch, different cases if capital, small, not letter or others
		switch (result)
		{
		case 1:
			return getValue(input, 2, encryption, 1);
			break;
		case 2:
			return getValue(input, 2, encryption, 2);
			break;
		default:
			return input;

		}
	}
};