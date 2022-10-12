#include "pch.h"
#include "App.h"
#include "Window.h"
#include "platform/windows/MSWindow.h"
#include "HorizonEngine/Events/MouseEvent.h"

#include "HorizonEngine/GameObject.h"
#include "HorizonEngine/Components/ComponentFactory.h"
#include "HorizonEngine/Components/Component.h"
#include "HorizonEngine/Components/Transform.h"
#include "HorizonEngine/Physics2D/BoxCollider2D.h"
#include "HorizonEngine/Physics2D/Rigidbody2D.h"
#include "HorizonEngine/Scene Management/Scene.h"

#include <memory>

namespace Hzn
{
	void App::run()
	{
		// Component Factory test
		REGISTER(Component, Transform);
		REGISTER(Component, BoxCollider2D);
		//std::shared_ptr<Component> t(FACTORY(Component).create("Transform"));
		// std::shared_ptr<Transform> t2 = std::dynamic_pointer_cast<Transform>(t);

		Scene* s = new Scene(NULL);
		s->open();
		std::cout << "Found objects: " << s->getObjects()->size() << std::endl;
		s->save();

		MouseMovedEvent m(1, 2);
		std::cout << m.ToString() << std::endl;
		std::cout << "Welcome to HorizonEngine!" << std::endl;
		Window* w = new MSWindow(1366, 768, "HorizonEngine");
		while (!w->shouldClose())
		{
			w->update();
			w->render();
		}
		delete w;
	}
}