#pragma once

#include <irrlicht.h>

class Scene {
private:
	irr::IrrlichtDevice* m_device;
	irr::scene::ISceneManager* smgr;

public:
	Scene(irr::IrrlichtDevice* device);
	~Scene();



	void update();
	void draw();
};