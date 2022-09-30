//GeeksForGeeks (2018) was used in the process of making these API's found at - https://www.geeksforgeeks.org/file-handling-c-classes/
//Thompson (2022) was used in the process of making these API's found at - https://www.guru99.com/cpp-file-read-write-open.html
#include "pch.h"
#include "FileManager.h"


namespace Hzn {

	void ProjectFile::NewFile() {
		
		std::string fileName;
		std::cout << "Enter new file name" << std::endl;
		std::cin >> fileName;
		setName(fileName);

		filestream.open(fileName, std::ios::out);

		if (!filestream) {
			std::cout << "file not created" << std::endl;
		}
		else {
			std::cout << "file created successfully" << std::endl;
			filestream.close();
		}
	}


	//'Opens' new File
	void ProjectFile::openFile() {
		std::string openFileName;
		std::cout << "Enter file name to be opened" << std::endl;
		std::cin >> openFileName;


		filestream.open(openFileName, std::ios::app);
		setName(openFileName);

		if (!filestream) {
			std::cout << "file not created" << std::endl;
		}
		else {
			std::cout << "file created successfully" << std::endl;
			filestream.close();
		}
	}

	void ProjectFile::write() {
		std::string writeToFileName;
		std::cout << "Enter file name to be opened" << std::endl;
		std::cin >> writeToFileName;


		filestream.open(writeToFileName, std::ios::out | std::ios::app);
		if (!filestream) {
			std::cout << "file not created" << std::endl;
		}
		else {

			std::cout << "Enter input to file type \"end-of-input\" to exit: ";
			while (1) {


				std::getline(std::cin, lineToFile);

				if (lineToFile == "end-of-input") {
					break;
				}

				filestream << lineToFile << std::endl;


			}

			filestream.close();
		}
	}

	void ProjectFile::toClose() {
		close == true;
		filestream.close();
	}



	void ProjectFile::readFile() {

		std::string readFileName;
		std::cout << "Enter file name to be read" << std::endl;
		std::cin >> readFileName;

		filestream.open(readFileName, std::ios::in);

		if (!filestream) {
			std::cout << "Unable to read file" << std::endl;
		}
		else {
			while (getline(filestream, lineToConsole)) {
				std::cout << lineToConsole << std::endl;
			}

			filestream.close();
		}

		
	}

	void ProjectFile::readLine() {

		std::string readLineFileName;
		std::cout << "Enter file name to read line" << std::endl;
		std::cin >> readLineFileName;

		filestream.open(readLineFileName, std::ios::in);

		if (!filestream) {
			std::cout << "Unable to read file" << std::endl;
		}
		else {
			getline(filestream, lineToConsole);
			if (lineToConsole == "") {
				getline(filestream, lineToConsole);
			}
			std::cout << lineToConsole << std::endl;
			filestream.close();
		}		
	}

	void ProjectFile::readChar() {

		std::string readCharName;
		std::cout << "Enter file name to read char" << std::endl;
		std::cin >> readCharName;

		filestream.open(readCharName, std::ios::in);

		if (!filestream) {
			std::cout << "Unable to read file" << std::endl;
		}
		else {
			filestream >> ch;
			std::cout << ch;
			filestream.close();
		}

		
	}


	void ProjectFile::deleteFile() {

		std::string deleteFileName;
		std::cout << "Enter file to be deleted \n";
		std::cin >> deleteFileName;

		int result = std::remove(deleteFileName.c_str());

		if (result == 0) {
			std::cout << "File deleted successfully" << std::endl;
		}
		else {
			std::cout << "The file was not deleted" << std::endl;
		}
	}

	void ProjectFile::save() {
		filestream << std::flush;

	}

	void ProjectFile::saveAs() {
		std::string saveAsName;
		std::cout << "Enter fileName to be saved \n";
		std::cin >> saveAsName;
		filestream.open(saveAsName);

		if (!filestream) {
			std::cout << "Unable to save file" << std::endl;
		}
		else {
			std::cout << "Successfully saved file" << std::endl;
			filestream.close();
		}
	}


	void ProjectFile::setName(std::string filename) {
		name = filename;
	}

	void ProjectFile::setPath(std::string newPath) {
		path = newPath;
	}

	std::string ProjectFile::getName() {
		return name;
	}

	std::string ProjectFile::getPath() {
		return path;
	}

	bool getBool() {
		return true;
	}

}


//code for app.cpp
//ProjectFile pf;
//int var;
//
//do {
//	std::cout << "\nEnter a new number to select API function: 0 = New file, 1 = open file, 2 = write, 3 = toClose,\n4 = readFile, 5 = readLine, 6 = readChar, 7 = deleteFile, 8 = setName, -1 = exit:\n";
//	std::cin >> var;
//	switch (var) {
//	case 0:
//		pf.NewFile();
//		break;
//	case 1:
//		pf.openFile();
//		break;
//	case 2:
//		pf.write();
//		break;
//	case 3:
//		pf.toClose();
//		break;
//	case 4:
//		pf.readFile();
//		break;
//	case 5:
//		pf.readLine();
//		break;
//	case 6:
//		pf.readChar();
//		break;
//	case 7:
//		pf.deleteFile();
//		break;
//	case 8:
//		pf.setName("test1.txt");
//		break;
//	case -1:
//		break;
//	default:
//		break;
//	}
//} while (var != -1);