/*Throughout all the methods the error handling structure (if(!buf) / else) was influenced by Thompson (2022) */

#include "pch.h"
#include "ProjectFile.h"

namespace Hzn {
		//create new file
		void openFile() {
			
			//We open the file by using getName in append mode
			buf.open(getName(), std::ios::app);

			//error handling
			if (!buf) {
				std::cout << "file not created" << std::endl;
			}
			else {
				std::cout << "file created successfully" << std::endl;

			}
		}


		void write(std::string contentToFile) {
			//We get variable from calling class
			//Take in string var and dump it into file.
			//check error handling

			if (!buf) {
				std::cout << "file not created" << std::endl;
			}
			else {

				buf << contentToFile;
			}
		}


		//close function
		void closeFile() {
			buf.close();
		}




		//Read Functions
		
		//Read entire file 
		std::vector<std::string> readFile() {

			//Check error handling
			if (!buf) {
				std::cout << "Unable to read file" << std::endl;
			}
			else {
				//While there are still getLines, getline from buf and put into lineFromFile
				//Then push_back() lineFromFile into content vector
				while (getline(buf, setLineFromFile(newLineFromFile))) {
					getContent.push_back(getLineFromFile());
				}

				//return getContent()
				return getContent();
			}
		}
		
		//get a line, take in a lineNumber, 
		std::string getALine(int lineNumber) {
			int count = 0;

			//Check error handling
			if (!buf) {
				std::cout << "Unable to read file" << std::endl;
			}
			else {
				//while not the end of buff increase the count
				//if count == lineNumber getLine() and return lineFromFile.
				while (!buf.eof()) {
					count++;
					if (count == lineNumber) {
						std::getLine(buf, setLineFromFile(newLineFromFile));
						return getLineFromFile();
					}
				}
			}
		}

		//get the next char
		char nextChar() {

			//Check error handling
			if (!buf) {
				std::cout << "Unable to read file" << std::endl;
			}
			else {
				//get next char from buf and return
				buf >> ch;
				return ch;
			}
		}

		//get next n number of chars
		std::string nextChars(int numberOfCharacters) {

			char ch;

			//Check error handling
			if (!buf) {
				std::cout << "Unable to read file" << std::endl;
			}
			else {
				//get next n number of chars
				for (int i = 0; i < numberOfCharacters; i++) {
					buf >> ch;
					setCharString(std::string newCharString).push_back(ch);
				}
			}

			return getCharString();
		}


		//return the next line in a file
		std::shared_ptr<std::vector<std::string>> nextLine() {

			//Check error handling
			if (!buf) {
				std::cout << "Unable to read file" << std::endl;
			}
			else {
				//get the next line in file(buf) and set into LineFromFile
				//then push_back onto content and return
				getline(buf, setLineFromFile(newLineFromFile));
				return getContent().push_back(getLineFromFile());
			}
		}

		//return the next n number of line in a file
		std::vector<std::string> nextLines(int numberOfLines) {
			//Check error handling
			if (!buf) {
				std::cout << "Unable to read file" << std::endl;
			}
			else {
				//get next n number of chars
				for (int i = 0; i < numberOfLines; i++) {
					if (count == lineNumber) {
						std::getLine(buf, setLineFromFile(newLineFromFile));
						getContent().push_back(getLineFromFile());
					}
				}
				return buf.getContent();
			}
		}



		//Save functions
		void ProjectFile::save() {
			buf.flush();
		}


		void ProjectFile::saveAs(std::string newPath, std::string newName) {

			
		
			//As of now we assume all physical paths exist
			buf.open(getNewFullPath(), std::ios::out);
			if (!buf) {
				std::cout << "Unable to save file" << std::endl;
			}
			else {
				std::cout << "Successfully saved file" << std::endl;
				buf.flush();
			}
		}

		void ProjectFile::deleteFile() {
			int result = std::remove(buf.getName().c_str());

			if (result == 0) {
				std::cout << "File deleted successfully" << std::endl;
			}
			else {
				std::cout << "The file was not deleted" << std::endl;
			}
		}

}

/*reference:
	Thompson, B. (2022, August 25). C++ File Handling: How to Open, Write, Read, Close Files in 
		C++. Guru99. https://www.guru99.com/cpp-file-read-write-open.html
*/