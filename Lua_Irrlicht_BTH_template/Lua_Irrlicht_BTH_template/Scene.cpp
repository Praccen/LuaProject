#include "Scene.h"

Scene::Scene(irr::IrrlichtDevice* device) {
	m_device = device;
	smgr = device->getSceneManager();

	smgr->addCameraSceneNodeFPS(0, 100.0f, 0.5f, -1, 0, 0, false, 0.0f, false, true);
}

Scene::~Scene() {

}

void Scene::update() {
	if (m_device->isWindowActive()) {
		smgr->getActiveCamera()->setInputReceiverEnabled(true);
		m_device->getCursorControl()->setVisible(false);
	}
	else {
		smgr->getActiveCamera()->setInputReceiverEnabled(false);
		//device->getCursorControl()->setVisible(true);
	}
}

void Scene::draw() {
	smgr->drawAll();
}