// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "common.h"
#include "cipherArray.h"

#define CIPHER_ARRAY_LENGTH 26
#define MAX_FILE_CHARS 10000

int main(void) 
{
	// initialise cipherArray object with 65, ASCII for A
	cipherArray myHelper = cipherArray(65);
	/*
	char myString[] = "ABCDEF ZYXW";
	std::cout << "Original Value : " << myString << std::endl;


	for (int i = 0; i < sizeof(myString) / sizeof(myString[0]) ; i++)
	{
		myString[i] = myHelper.getEncryptionLetter(myString[i], -4);
	}

	std::cout << "New Value : " << myString << std::endl;
	*/

	std::cout << "\nPlease enter a positive or negative integer to encrypt : ";
	int cipher = 0;
	std::cin >> cipher;
	std::cin.ignore(100, '\n');

	// allows values more than n * 2 both ways as the cipher
	cipher = cipher % CIPHER_ARRAY_LENGTH;
	
	// open file
	char fileString[MAX_FILE_CHARS];
	std::ifstream readFile;
	readFile.open("plaintext.txt");
	if (readFile.is_open())
	{
		std::cout << "\nReading File ...\n" << std::endl;
		char temp;
		int i = 0;
		// read file
		while (readFile.get(temp))
		{
			fileString[i] = temp;
			i++;
		}
		// terminate c string
		fileString[i] = '\0';

		std::cout << "\nplaintext.txt before encryption :\n\n" << fileString << std::endl;

		// encrypt character by character
		for (int j = 0; j < i; j++)
		{
			fileString[j] = myHelper.getEncryptionLetter(fileString[j], cipher);
		}

		// write out encrypted text
		std::ofstream encryptedFile("ciphertext.txt");
		if (encryptedFile.is_open())
		{
			encryptedFile.write(fileString, i);
			encryptedFile.close();

		}
		else
		{
			std::cout << "\nciphertext.txt could not be opened. Troubleshoot and try again ...\n" << std::endl;

		}

		std::cout << "\nplaintext.txt after encryption :\n\n" << fileString << std::endl;

		// decrypt character by character
		for (int j = 0; j < i; j++)
		{
			fileString[j] = myHelper.getDecryptionLetter(fileString[j], cipher);
		}

		// write out decrypted text
		std::ofstream decryptedFile("decrypted.txt");
		if (decryptedFile.is_open())
		{
			decryptedFile.write(fileString, i);
			decryptedFile.close();

		}
		else
		{
			std::cout << "\decrypted.txt could not be opened. Troubleshoot and try again ...\n" << std::endl;

		}

		std::cout << "\nplaintext.txt after decryption :\n\n" << fileString << std::endl;


	}
	else
	{
		std::cout << "\nFile could not be opened. Troubleshoot and try again ...\n" << std::endl;
	}



#ifdef _WIN32
	// wait for a keypress on Windows OS before exiting
	std::cout << "\npress a key to quit...";
	std::cin.ignore();
#endif

	return 0;
}
