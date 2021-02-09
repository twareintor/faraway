/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "triad.h"


ClsTriad::ClsTriad(IrrlichtDevice* device, scene::ISceneNode* nodPar):
	m_driver(device->getVideoDriver()), 
	m_nodZero(device->getSceneManager()->addEmptySceneNode(nodPar)), 
	m_mshX(device->getSceneManager()->addArrowMesh("Arrow", video::SColor(128, 128, 128, 128), video::SColor(128, 128, 0, 0))),
	m_mshY(device->getSceneManager()->addArrowMesh("Arrow", video::SColor(128, 128, 128, 128), video::SColor(128, 0, 128, 0))),
	m_mshZ(device->getSceneManager()->addArrowMesh("Arrow", video::SColor(128, 128, 128, 128), video::SColor(128, 0, 0, 128))),
	m_nodX(device->getSceneManager()->addMeshSceneNode(m_mshX, m_nodZero)),
	m_nodY(device->getSceneManager()->addMeshSceneNode(m_mshY, m_nodZero)),
	m_nodZ(device->getSceneManager()->addMeshSceneNode(m_mshZ, m_nodZero)),

	m_dummy(false)
{
	wchar_t szAxis[] = L"X\0Y\0Z\0";
	scene::IBillboardTextSceneNode *bilX, *bilY, *bilZ;
	gui::IGUIFont* font = device->getGUIEnvironment()->getFont("d:/media/fontcourier.bmp");

	bilX = device->getSceneManager()->addBillboardTextSceneNode(font, szAxis + 0, m_nodX, 
		core::dimension2df(1.5f, 2.4f), core::vector3df(0.f, 1.2f, 0.f), -1, video::SColor(128, 255, 0, 0));
	bilY = device->getSceneManager()->addBillboardTextSceneNode(font, szAxis + 2, m_nodY, 
		core::dimension2df(1.5f, 2.4f), core::vector3df(0.f, 1.2f, 0.f), -1, video::SColor(128, 0, 255, 0));
	bilZ = device->getSceneManager()->addBillboardTextSceneNode(font, szAxis + 4, m_nodZ, 
		core::dimension2df(1.5f, 2.4f), core::vector3df(0.f, 1.2f, 0.f), -1, video::SColor(128, 0, 0, 255));

	bilX->setMaterialFlag(video::EMF_LIGHTING, false);
	bilY->setMaterialFlag(video::EMF_LIGHTING, false);
	bilZ->setMaterialFlag(video::EMF_LIGHTING, false);

	bilX->setMaterialFlag(video::EMF_ZBUFFER, false);
	bilY->setMaterialFlag(video::EMF_ZBUFFER, false);
	bilZ->setMaterialFlag(video::EMF_ZBUFFER, false);

	m_nodX->setMaterialFlag(video::EMF_LIGHTING, false);
	m_nodY->setMaterialFlag(video::EMF_LIGHTING, false);
	m_nodZ->setMaterialFlag(video::EMF_LIGHTING, false);

	m_nodX->setMaterialFlag(video::EMF_ZBUFFER, false);
	m_nodY->setMaterialFlag(video::EMF_ZBUFFER, false);
	m_nodZ->setMaterialFlag(video::EMF_ZBUFFER, false);

	m_nodX->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	m_nodY->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	m_nodZ->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

	m_nodX->setScale(core::vector3df(2.5f, 9.2f, 2.5f));
	m_nodY->setScale(core::vector3df(2.5f, 9.2f, 2.5f));
	m_nodZ->setScale(core::vector3df(2.5f, 9.2f, 2.5f));

	m_nodX->setRotation(core::vector3df(+00.f, +00.f, +90.f));
	m_nodY->setRotation(core::vector3df(+00.f, +00.f, +00.f));
	m_nodZ->setRotation(core::vector3df(+90.f, +00.f, +00.f));

	// this->SetParent(nodPar);
	return;
}

void ClsTriad::setParent(scene::ISceneNode* nodPar)
{
	m_nodZero->setParent(nodPar);
	return;
}

void ClsTriad::setVisible(BOOL bVis)
{
	m_bVis = bVis;
	m_nodZero->setVisible(m_bVis);

	return;
}

void ClsTriad::setPosition(core::vector3df pos)
{
	m_nodZero->setPosition(pos);
	return;
}

void ClsTriad::setRotation(core::vector3df rot)
{
	m_nodZero->setRotation(rot);
	return;
}


BOOL ClsTriad::isVisible()
{
	return m_bVis;
}


















/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
