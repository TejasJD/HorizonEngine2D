
#include "pch.h"
#include "ProjectFile.h"

namespace Hzn {
	//open a file and load the contents to a vector string
	std::vector<std::string> ProjectFile::openFile(std::string filePath) {
		std::string line;

		// Create file if not exisiting
		out.open(filePath, std::ios::app);
		out_check();
		out.close();

		//open the file here
		in.open(filePath);
		
		//Check error handling
		in_check();

		//take in a line and push_back onto content vec.
		while (std::getline(in, line)) {
			content.push_back(line);
		}

		in.close();

		//return reference to content
		return content;
	}

	//Make a new blank file
	void ProjectFile::mkNewfile(std::string newPath) {
		out.open(newPath, std::ios::out);
		
		//check error handling
		out_check();
		out.close();
	}

	//Write the contents to a file and save
	void ProjectFile::saveToFile() {
		out.open(getPath(), std::ios::out);

		//check error handling
		out_check();

		for (int i = 0; i < content.size(); i++) {
			out << content[i];
		}
		out.close();
	}


	//File manipulation method treat the lines as index 0
	//return a specific line from content vec
	std::string ProjectFile::getALine(int row) {
		return content[row];
	}


	//return next char from content vec
	char ProjectFile::nextChar(int row, int colNum) {
		return content[row][colNum];
	}


	//return next n chars from content vec
	std::vector<std::string> ProjectFile::nextChars(int row, int colNum, int numberOfCharacters) {
		std::vector<std::string> vecOfChars;
		std::string line;

		for (int i = 0, j = colNum; i < numberOfCharacters; i++, j++) {
			if (content[row][j] == ' ') {
				line = content[row][j];
				vecOfChars.push_back(line);
			}
			else {
				line = content[row][j];
				vecOfChars.push_back(line);
			}
		}
		return vecOfChars;
	}



	//return next line from content vec
	std::string ProjectFile::nextLine(int row) {
		return content[row];
	}



	//return next n lines from content vec
	std::vector<std::string> ProjectFile::nextLines(int row, int numberOfLines) {

		std::vector<std::string> vecOfLines;
		std::string line;

		for (int i = row; i < numberOfLines; i++) {
			line = content[i];
			vecOfLines.push_back(line);
		}
		return vecOfLines;
	}


	//Save the contents of content vec to a new file
	void ProjectFile::saveAs(std::string newPath, std::vector<std::string> contentIntoFile) {

		//As of now we assume all physical paths exist
		setPath(newPath);
		out.open(getPath(), std::ios::out);
		out_check();

		for (int i = 0; i < content.size(); i++) {
			out << content[i];
		}
		std::cout << "file written to " << getPath() << std::endl;

		out.close();
	}

	//Delete a specified file
	void ProjectFile::deleteFile(std::string filePathForDelete) {

		int result = std::remove(filePathForDelete.c_str());

		if (result == 0) {
			std::cout << "File deleted successfully" << std::endl;
		}
		else {
			std::cout << "The file was not deleted" << std::endl;
		}
	}

	//make a folder
	void ProjectFile::makeDir(std::string path) {
		int result = mkdir(path.c_str());

		if (result == -1) {
			std::cout << "Error folder not created" << std::endl;
		}
		else {
			std::cout << "Folder created successfully" << std::endl;
		}
	}

	//remove a folder
	void ProjectFile::deleteDir(std::string path) {
		int result = rmdir(path.c_str());

		if (result == -1) {
			std::cout << "Error folder not deleted" << std::endl;
		}
		else {
			std::cout << "Folder deleted successfully" << std::endl;
		}
	}
}
