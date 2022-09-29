#include <string>
#include <iostream>
#include <fstream>
#include <vector>



namespace FileManagement {

	class projectFile {
	private:
		char* path;
		std::string name;
		char ch;
		std::string lineToFile;
		std::string lineToConsole;
		std::fstream filestream;
		bool close = false;

	public:

		//'Opens' new File
		void open(std::string fileName) {

			filestream.open(fileName, std::ios::out);
			setName(fileName);
			if (!filestream) {
				std::cout << "file not created" << std::endl;
			}
			else {
				std::cout << "file created successfully" << std::endl;
				filestream << "test text with spaces read with readLine()";
				write();
				filestream.close();
			}

		}

		void write() {

			std::cout << "Enter input to file type \"end-of-input\" to exit";
			while (lineToFile != "end-of-input") {
				std::getline(std::cin, lineToFile);
				filestream << lineToFile << std::endl;


			}
		}

		void toClose() {
			close == true;
			filestream.close();
		}



		void readFile() {


			filestream.open(getName(), std::ios::in);

			while (getline(filestream, lineToConsole)) {
				std::cout << lineToConsole << std::endl;
			}

			filestream.close();
		}

		void readLine() {
			filestream.open(getName(), std::ios::in);
			getline(filestream, lineToConsole);
			std::cout << lineToConsole << std::endl;
		}

		void readChar() {
			filestream.open(getName(), std::ios::in);
			filestream >> ch;
			std::cout << ch;
		}




		void deleteFile() {

			std::string testFileName;
			std::cout << "Enter file to be deleted \n";
			std::cin >> testFileName;

			int result = std::remove(testFileName.c_str());
			std::cout << "\n" << result;
		}

		std::string save() {
			filestream << std::flush;

		}

		void saveAs(char* newName) {
			//filestream.open(getName())
		}


		void setName(std::string filename) {
			name = filename;
		}

		char* setPath() {

		}

		std::string getName() {
			return name;
		}

		char* getPath() {
			return path;
		}
	};

}