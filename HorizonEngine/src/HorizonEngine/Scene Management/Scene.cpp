#include "Scene.h"

namespace Hzn {
	Scene::Scene(ProjectFile* projectFile) {
		file = projectFile;
		componentGroups = new std::map<std::string, std::vector<std::shared_ptr<Component>>*>();
		gameObjects = new std::vector<std::shared_ptr<GameObject>>();
	}

	Scene::~Scene() {

	}

	void Scene::open() {
		/*std::shared_ptr<std::vector<std::string>> content = std::make_shared<std::vector<std::string>>(); // file->getContent();
		content->push_back("name:Test Name\n");
		content->push_back("gameObject:GO Name\n");
		content->push_back("component:Transform\n");
		content->push_back("values:[\n");
		content->push_back("\tposition:vec2:0.000000,0.000000\n");
		content->push_back("\trotation:float:0.000000\n");
		content->push_back("\tscale:vec2:1.000000,1.000000\n");
		content->push_back("\tright:vec2:1.000000,0.000000\n");
		content->push_back("\tup:vec2:0.000000,1.000000\n");
		content->push_back("\tparent:Transform:Test GO\n");
		content->push_back("\troot:Transform:Test GO\n");
		content->push_back("\tsiblingIndex:int:0\n");
		content->push_back("\tchildrenCount:int:0\n");
		content->push_back("]\n");
		content->push_back("component:BoxCollider2D\n");
		content->push_back("values:[\n");
		content->push_back("\ttransform:Transform:GO Name\n");
		content->push_back("\tsize:vec2:1.000000,1.000000\n");
		content->push_back("\toffset:vec2:0.000000,0.000000\n");
		content->push_back("]\n");
		content->push_back("gameObject:Test GO\n");
		content->push_back("component:Transform\n");
		content->push_back("values:[\n");
		content->push_back("\tposition:vec2:0.000000,0.000000\n");
		content->push_back("\trotation:float:0.000000\n");
		content->push_back("\tscale:vec2:1.000000,1.000000\n");
		content->push_back("\tright:vec2:1.000000,0.000000\n");
		content->push_back("\tup:vec2:0.000000,1.000000\n");
		content->push_back("\tparent:Transform:NULL\n");
		content->push_back("\troot:Transform:NULL\n");
		content->push_back("\tsiblingIndex:int:0\n");
		content->push_back("\tchildrenCount:int:0\n");
		content->push_back("]\n");
		content->push_back("component:BoxCollider2D\n");
		content->push_back("values:[\n");
		content->push_back("\ttransform:Transform:Test GO\n");
		content->push_back("\tsize:vec2:1.000000,1.000000\n");
		content->push_back("\toffset:vec2:0.000000,0.000000\n");
		content->push_back("]\n");
		*/

		// Each line in the file is one of the following:
		// name:SceneName
		// gameObject:GameObjectName
		// component:ComponentType
		// fieldName:fieldType:fieldValue

		// Get the content from the file
		std::shared_ptr<std::vector<std::string>> content = file->getContent();

		// Create a map where the key is a component and the value is a vector of
		// strings, each representing a value in the component
		std::map<std::shared_ptr<Component>, std::shared_ptr<std::vector<std::string>>>* valuesMap = new std::map<std::shared_ptr<Component>, std::shared_ptr<std::vector<std::string>>>();

		std::string values = "";
		std::shared_ptr<GameObject> currentGameObject = NULL;
		std::shared_ptr<Component> currentComponent;
		bool addingValues = false;
		for (int i = 0; i < content->size(); i++) {
			if (content->at(i).find(":") != std::string::npos) {
				std::string key = content->at(i).substr(0, content->at(i).find(":"));
				std::string value = content->at(i).substr(content->at(i).find(":") + 1, content->at(i).size() - (key.size() + 2));
				// Set scene name
				if (key.compare("name") == 0) {
					name = value;
				}
				// Create new gameObject in the scene
				else if (key.compare("gameObject") == 0) {
					// Add current game object to the scene
					if (currentGameObject != NULL) {
						addGameObject(std::make_shared<GameObject>(currentGameObject));
					}
					currentGameObject = std::make_shared<GameObject>(value);
				}
				// Create new component for the current game object
				else if (key.compare("component") == 0) {
					std::shared_ptr<Component> c(FACTORY(Component).create(value));
					currentComponent = std::shared_ptr<Component>(c);

					currentGameObject->addComponent(currentComponent);
				}
				// start setting values for the current component
				else if (key.compare("values") == 0) {
					addingValues = true;
				}
				// add current vaue for the current component
				else if (addingValues) {
					size_t start = key.find_first_not_of("\t");
					std::string fieldName = key.substr(start);

					auto it = valuesMap->find(currentComponent);
					if (it != valuesMap->end()) {
						it->second->push_back(fieldName + ":" + value);
					}
					else {
						std::shared_ptr<std::vector<std::string>> v = std::make_shared<std::vector<std::string>>();
						v->push_back({ fieldName + ":" + value });
						valuesMap->insert({ currentComponent, v });
					}
				}
			}
			// finish setting values for the current component
			else {
				addingValues = false;
				values = "";
			}
		}

		// Add the last game object to the scene
		addGameObject(std::make_shared<GameObject>(currentGameObject));
		
		// Loop through the map and set components' values
		for (std::map<std::shared_ptr<Component>, std::shared_ptr<std::vector<std::string>>>::iterator it = valuesMap->begin(); it != valuesMap->end(); it++)
		{
			for (int i = 0; i < it->second->size(); i++) {
				// Value will be a gameObject's scene id if the field is a component
				std::string name = "", typeName = "", value = "";
				size_t index = it->second->at(i).find(":");
				if (index == std::string::npos) {
					std::cout << "Error loading field name!" << std::endl;
					return;
				}
				// Set field name
				name = it->second->at(i).substr(0, index);

				std::string temp = it->second->at(i).substr(index + 1, it->second->at(i).size() - (name.size() + 1));
				index = temp.find(":");
				if (index == std::string::npos) {
					std::cout << "Error loading field type!" << std::endl;
					return;
				}
				// Set field type
				typeName = temp.substr(0, index);
				// Set field value
				value = temp.substr(index + 1, std::string::npos);

				// Create value of the appropriate type
				if (typeName == "int") {
					it->first->setField(name, std::stoi(value));
				}
				else if (typeName == "float") {
					it->first->setField(name, std::stof(value));
				}
				else if (typeName == "double") {
					it->first->setField(name, std::stod(value));
				}
				else if (typeName == "char") {
					it->first->setField(name, value.at(0));
				}
				else if (typeName == "string") {
					it->first->setField(name, value);
				}
				else if (typeName == "bool") {
					if (value.compare("true")) {
						it->first->setField(name, true);
					} else {
						it->first->setField(name, false);
					}
				}
				else if (typeName == "vec2") {
					std::string X = value.substr(0, value.find(","));
					std::string Y = value.substr(value.find(",") + 1, value.size() - X.size() - 1);
					float x = std::stof(X);
					float y = std::stof(Y);
					it->first->setField(name, glm::vec2(x, y));
				}
				else if (typeName == "GameObject") {
					it->first->setField(name, findGameObject(value));
				}
				else { // If we get to here, then it's a component
					if (value.compare("NULL") != 0) {
						it->first->setField(name, findGameObject(value)->getComponent(typeName));
					}
				}
			}
		}
	}

	void Scene::close() {

	}

	void Scene::save() {
		std::shared_ptr<std::vector<std::string>> content = std::make_shared<std::vector<std::string>>();
		content->push_back("name:" + name + "\n");
		for (int i = 0; i < gameObjects->size(); i++) {
			std::shared_ptr<GameObject> gameObject = gameObjects->at(i);
			content->push_back("gameObject:" + gameObject->name + "\n");
			std::vector<std::shared_ptr<Component>>* components = gameObject->getComponents();
			for (int j = 0; j < components->size(); j++) {
				std::vector<std::string>* componentString = components->at(j)->stringify();
				for (int k = 0; k < componentString->size(); k++) {
					content->push_back(componentString->at(k));
				}
			}
		}

		file->setContent(std::shared_ptr<std::vector<std::string>>(content));

		// Test
		/*
		for (int i = 0; i < content->size(); i++) {
			std::cout << content->at(i);
		}
		*/
	}

	void Scene::addGameObject(std::shared_ptr<GameObject> gameObject) {
		std::shared_ptr<Transform> parent;
		try {
			parent = std::any_cast<std::shared_ptr<Transform>>(gameObject->transform->getField("parent"));
		} catch (const std::bad_any_cast& e) { 
			parent = NULL;
		}

		if (parent == NULL) {
			gameObjects->push_back(gameObject);
		}
		
		std::vector<std::shared_ptr<Component>>* components = gameObject->getComponents();
		for (int i = 0; i < components->size(); i++) {
			bool found = false;
			std::string firstType = "";
			for (std::map<std::string, std::vector<std::shared_ptr<Component>>*>::iterator it = componentGroups->begin(); it != componentGroups->end(); ++it) {
				if (components->at(i)->getComponentType().compare(it->first) == 0) {
					it->second->push_back(components->at(i));
					found = true;
					break;
				}

				if (found) {
					break;
				}
			}

			if (!found) {
				std::vector<std::shared_ptr<Component>>* newComponentList = new std::vector<std::shared_ptr<Component>>();
				newComponentList->push_back(components->at(i));
				componentGroups->insert({ components->at(i)->getComponentType(), newComponentList });
			}
		}
	}

	void Scene::removeGameObject(std::shared_ptr<GameObject> gameObject) {
		for (int i = 0; i < gameObjects->size(); i++) {
			if (gameObjects->at(i)->name.compare(gameObject->name) == 0) {
				gameObjects->erase(std::next(gameObjects->begin(), i));
				return;
			}
		}
	}

	std::shared_ptr<GameObject> Scene::findGameObject(std::string name) {
		for (int i = 0; i < gameObjects->size(); i++) {
			if (gameObjects->at(i)->name.compare(name) == 0) {
				return std::shared_ptr<GameObject>(gameObjects->at(i));
			}
		}

		return nullptr;
	}

	std::vector<std::shared_ptr<GameObject>>* Scene::getObjects() {
		return gameObjects;
	}
}