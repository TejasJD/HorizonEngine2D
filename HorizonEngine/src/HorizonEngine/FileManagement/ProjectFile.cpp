
#include "pch.h"
#include "ProjectFile.h"

namespace Hzn {
	/// <summary>
	/// open a file and load the contents to a vector string.
	/// 
	/// First perform a check that the file exists.
	/// Next, open the file path and check for error handling.
	/// Push back the content into a vector string.
	/// Finally, return content to the call.
	///  
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
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


	/// <summary>
	/// Write the contents to a file and save.
	/// Open a file stream with the file path
	/// check for error handling.
	/// Write the contents of the content vector to the file and close
	/// </summary>
	void ProjectFile::saveToFile() {
		out.open(getPath(), std::ios::out);

		//check error handling
		out_check();

		for (int i = 0; i < content.size(); i++) {
			out << content[i];
		}
		out.close();
	}


	/// <summary>
	/// File manipulation methods treat the lines as index 0.
	/// return a specific line from content vec.
	/// </summary>
	/// <param name="row"></param>
	/// <returns></returns>
	std::string ProjectFile::getALine(int row) {
		return content[row];
	}


	/// <summary>
	/// return next char from content vec
	/// </summary>
	/// <param name="row"></param>
	/// <param name="colNum"></param>
	/// <returns></returns>
	char ProjectFile::nextChar(int row, int colNum) {
		return content[row][colNum];
	}


	/// <summary>
	/// return next n chars from content vec
	/// </summary>
	/// <param name="row"></param>
	/// <param name="colNum"></param>
	/// <param name="numberOfCharacters"></param>
	/// <returns></returns>
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



	/// <summary>
	/// return next line from content vec
	/// </summary>
	/// <param name="row"></param>
	/// <returns></returns>
	std::string ProjectFile::nextLine(int row) {
		return content[row];
	}



	/// <summary>
	/// return next n lines from content vec
	/// </summary>
	/// <param name="row"></param>
	/// <param name="numberOfLines"></param>
	/// <returns></returns>
	std::vector<std::string> ProjectFile::nextLines(int row, int numberOfLines) {

		std::vector<std::string> vecOfLines;
		std::string line;

		for (int i = row; i < numberOfLines; i++) {
			line = content[i];
			vecOfLines.push_back(line);
		}
		return vecOfLines;
	}


	/// <summary>
	/// Save the contents of content vec to a new file
	/// </summary>
	/// <param name="newPath"></param>
	/// <param name="contentIntoFile"></param>
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

	/// <summary>
	/// Delete a specified file
	/// </summary>
	/// <param name="filePathForDelete"></param>
	void ProjectFile::deleteFile(std::string filePathForDelete) {

		int result = std::remove(filePathForDelete.c_str());

		if (result == 0) {
			std::cout << "File deleted successfully" << std::endl;
		}
		else {
			std::cout << "The file was not deleted" << std::endl;
		}
	}

	/// <summary>
	/// make a folder
	/// </summary>
	/// <param name="path"></param>
	void ProjectFile::makeDir(std::string path) {
		int result = mkdir(path.c_str());

		if (result == -1) {
			std::cout << "Error folder not created" << std::endl;
		}
		else {
			std::cout << "Folder created successfully" << std::endl;
		}
	}

	/// <summary>
	/// remove a folder
	/// </summary>
	/// <param name="path"></param>
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
