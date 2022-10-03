#pragma once

#ifndef HZN_FILE_H
#define HZN_FILE_H

#include <iostream>
#include <fstream>
// Use string if you want instead of char*
//#include <string>

namespace Hzn {
	class File {
	public:
		char* path;
		char* name;
	private:
		char* content;
		std::fstream stream;
		int currentCharacter;
	public:
		void open(); // Opens the fstream
		void close(); // Closes the fstream
		void save(); // Writes the value of content to the file
		void saveAs(char* path, char* name); // Writes the value of content to the file
		void deleteFile(); // Deletes the file
		void rename(char* newName); // Renames the file to the specified name
		char readChar(); // Reads a char from the file
		char* readLine(); // Reads a line from the file
		char* read(int numberOfCharacters); // Reads the specified number of characters if possible. If not, reads to the end
		void writeChar(char c); // Append the character c to the content
		void writeString(char* s); // Appends the string s to the content
	};
}

#endif // !HZN_FILE_H
