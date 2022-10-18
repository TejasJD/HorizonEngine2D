#pragma once

#ifndef HZN_FDWINDOWS_H
#define HZN_FDWINDOWS_H

#include <string>

namespace Hzn
{
	class FileDialogs {


	public:
	static std::string	openFile();
	static std::string	saveFile();

	};
}
#endif // !HZN_IMGUI_LAYER_H