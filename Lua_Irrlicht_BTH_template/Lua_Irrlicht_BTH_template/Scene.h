#pragma once

#include <iostream>
#include <irrlicht.h>

class Scene {
private:
	irr::IrrlichtDevice* m_device;
	irr::scene::ISceneManager* m_smgr;
	irr::scene::ICameraSceneNode* m_camera;

	int m_nrOfNodes = 0;
public:
	Scene(irr::IrrlichtDevice* device);
	~Scene();

	void addMesh(irr::core::vector3df vertex1, irr::core::vector3df vertex2, irr::core::vector3df vertex3);
	void addBox(irr::core::vector3df position, float size, std::string name);

	void update();
	void draw();
};