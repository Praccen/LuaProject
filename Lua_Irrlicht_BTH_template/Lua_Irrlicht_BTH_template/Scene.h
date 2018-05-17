#pragma once

#include <irrlicht.h>

class Scene {
private:
	irr::IrrlichtDevice* m_device;
	irr::scene::ISceneManager* m_smgr;
	irr::scene::ICameraSceneNode* m_camera;

public:
	Scene(irr::IrrlichtDevice* device);
	~Scene();

	void addMesh(irr::core::vector3df vertex1, irr::core::vector3df vertex2, irr::core::vector3df vertex3);

	void update();
	void draw();
};