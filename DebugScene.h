#pragma once

#include <irrlicht.h>

using namespace irr;

class CDebugScene
{
private:
	video::IVideoDriver* m_driver;
	scene::ISceneCollisionManager* m_colman;
protected:
public:
	CDebugScene();
	CDebugScene(IrrlichtDevice* device, video::IVideoDriver* driver);
	~CDebugScene();
};