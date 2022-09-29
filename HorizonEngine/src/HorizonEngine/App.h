#pragma once

#include "APIMacros.h"

namespace Hzn
{
	class HZN_API App
	{
	public:
		App()
		{
		}

		virtual ~App()
		{
		}

		void run();

	private:
	};

	// to be defined by the application that implements this function
	HZN_API std::shared_ptr<App> createApp();
}
