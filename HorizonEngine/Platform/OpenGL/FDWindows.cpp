#include <string>
#include <commdlg.h>
#include "string.h"
#include "windows.h"
#include <windows.h>
#include <string>
#include <shlobj.h>

#include "FDWindows.h"
#include "App.h"


#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace Hzn {

		std::string	FileDialogs::openFile()
		{

			/*The following codeis the win32 file dialog API (hickeys, 2021), it is used to implement a windows file dialog
			box for the user to SAVE a file from their local machine. Part of this implementation was inspired by the implementation by The Cherno (2020) in the Hazel engine*/

			OPENFILENAME ofn;				// common dialog box structure (ASCII version)
			char szFile[260] = { 0 };       // buffer for file name
			//HWND hwnd = NULL;              // owner window

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			//ofn.hwndOwner = hwnd;
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow());
			ofn.lpstrFile = szFile;
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Display the Open dialog box. 

			if (GetOpenFileName(&ofn) == TRUE) {

				return ofn.lpstrFile;
			}
			else {
				return std::string();
			}
			
		}


		std::string	FileDialogs::saveFile() {


			/*The following codeis the win32 file dialog API (hickeys, 2021), it is used to implement a windows file dialog
			box for the user to SAVE a file from their local machine. Part of this implementation was inspired by the implementation by The Cherno (2020) in the Hazel engine*/

			OPENFILENAME ofn;       // common dialog box structure (ASCII version)
			char szFile[260] = { 0 };       // buffer for file name
			//HWND hwnd = NULL;              // owner window

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			//ofn.hwndOwner = hwnd;
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)App::getApp().getAppWindow().getPlatformRawWindow());
			ofn.lpstrFile = szFile;
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Display the Open dialog box. 

			if (GetSaveFileName(&ofn) == TRUE) {

				return ofn.lpstrFile;
			}
			else {
				return std::string();
			}
		}

		std::string FileDialogs::openFolder() {
			TCHAR path[MAX_PATH];


			BROWSEINFO bi = { 0 };
			bi.lpszTitle = ("Browse for folder...");
			bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

			LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

			if (pidl != 0)
			{
				//get the name of the folder and put it in path
				SHGetPathFromIDList(pidl, path);

				//free memory used
				IMalloc* imalloc = 0;
				if (SUCCEEDED(SHGetMalloc(&imalloc)))
				{
					imalloc->Free(pidl);
					imalloc->Release();
				}

				return path;
			}

			return "";

		}
}


/*
REFERENCES:

hickeys. (2021, September 21). Using Common Dialog Boxes - Win32 apps. Microsoft.com.
		https://learn.microsoft.com/en-us/windows/win32/dlgbox/using-common-dialog-boxes#opening-a-file

The Cherno. (2020). Open/Save File Dialogs | Game Engine series [YouTube Video]. In YouTube.
		https://www.youtube.com/watch?v=zn7N7zHgCcs&t=505s&ab_channel=TheCherno
*/