
/************************************ AnaglyphCamera.cpp ***************************************/

#include <iostream>
#include <windows.h>
#include "AnaglyphCamera2.h"


using namespace irr;
using namespace irr::scene;

CAnaglyphCamera::CAnaglyphCamera(scene::ISceneManager* smgr, scene::ISceneNode* parent, f32 id, 
			SKeyMap* keymap, size_t keymapSize, u32 colorRight, u32 colorLeft, f32 fdx):
		ICameraSceneNode((scene::ISceneNode*)0, smgr, id), // remember, pleaaaaaase!
		// ISceneNode((scene::ISceneNode*)0, smgr), // it was once derived froi it
		m_smgr(smgr),
		m_id(id),
		m_fdx(fdx),
		m_colorRight(colorRight),
		m_colorLeft(colorLeft),
		m_keymap(keymap),
		m_nodCam(m_smgr->addEmptySceneNode(parent, id)),			// node of the right-eye camera
		m_nodTar(m_smgr->addEmptySceneNode(m_nodCam, id)),			// node of the target of the camera
		m_camX(m_smgr->addCameraSceneNode(m_nodCam)),
		m_keymapSize(keymapSize),
		m_pos(core::vector3df(0.f, 0.f, 0.f)),
		m_bRes(false)
{
	// a single attached to a dummy node:
	/***              + "nodTar" attached to "nodCam" // Target   a cube node, for debug
					 /|
					/ |   400 m distance to target
	 "Left Camera" L__R  "Right Camera" if not anaglyphic, only right camera active...
				R<-20cm->L			***/

	// m_nodCam = m_smgr->addEmptySceneNode(0, id);			// node of the right-eye camera
	m_smgr->getRootSceneNode()->addChild(m_nodCam);
	// m_nodTar = m_smgr->addEmptySceneNode(m_nodCam, id);			// node of the target of the camera
	// m_camX = m_smgr->addCameraSceneNode(m_nodCam);

	m_nodCam->setPosition(m_pos);
	m_nodTar->setPosition(core::vector3df(0.f, 0.f, -4000.f));
	m_camX->setFarValue(9000.f);
	m_camX->setName("Right Camera");
	m_nodTar->updateAbsolutePosition(); // not yet!
	m_camX->setTarget(m_nodTar->getAbsolutePosition());
	m_camX->bindTargetAndRotation(false);
	m_smgr->setActiveCamera(m_camX);
	POINT p;
	::GetCursorPos(&p);
	m_mo0 = core::position2di(p.x, p.y);
	m_ncen[0] = ::GetSystemMetrics(0)/2 ;
	m_ncen[1] = ::GetSystemMetrics(1)/2 ;
	// all done with one part of the camera...
}

CAnaglyphCamera::~CAnaglyphCamera()
{
	// don't drop the scene manager, it belongs to the calling object
	m_smgr->getRootSceneNode()->removeChild(m_nodCam);
	return;
	if(m_nodCam)	m_nodCam->drop();
	if(m_nodTar)	m_nodTar->drop();
	if(m_camX)		m_camX->drop();
}

void CAnaglyphCamera::setPosition(const core::vector3df& pos)
{
	m_nodCam->setPosition(pos);
	m_nodTar->updateAbsolutePosition();
	m_camX->setTarget(m_nodTar->getAbsolutePosition());
}

void CAnaglyphCamera::setRotation(const core::vector3df& rot)
{
	m_nodCam->setRotation(rot);
	m_nodTar->updateAbsolutePosition();
	m_camX->setTarget(m_nodTar->getAbsolutePosition());
}

const core::vector3df& CAnaglyphCamera::getTarget() const
{
	m_nodTar->updateAbsolutePosition();
	return m_nodTar->getAbsolutePosition();
}

void CAnaglyphCamera::OnRegisterSceneNode()
{
	ICameraSceneNode::OnRegisterSceneNode();
	if(IsVisible)
	{
		// m_smgr->registerNodeForRendering(this);
	}
	m_smgr->registerNodeForRendering(this);
}

void CAnaglyphCamera::bindTargetAndRotation(bool bound)
{
	// target and rotation are always bound
	return;
}

f32 CAnaglyphCamera::getAspectRatio() const
{
	return m_camX->getAspectRatio();
}

f32 CAnaglyphCamera::getFarValue() const
{
	return m_camX->getFarValue();
}

f32 CAnaglyphCamera::getFOV() const
{
	return m_camX->getFOV();
}

f32 CAnaglyphCamera::getNearValue() const
{
	return m_camX->getNearValue();
}

const core::matrix4& CAnaglyphCamera::getProjectionMatrix() const
{
	return m_camX->getProjectionMatrix();
}

bool CAnaglyphCamera::getTargetAndRotationBinding(void) const
{
	// 
	return true;
}

const core::vector3df& CAnaglyphCamera::getUpVector() const
{
	return m_camX->getUpVector();
}

const scene::SViewFrustum* CAnaglyphCamera::getViewFrustum() const
{
	// 
	return 0;		// to be implemented!
}

const core::matrix4& CAnaglyphCamera::getViewMatrix() const
{
	// 
	return m_camX->getViewMatrix();
}

const core::matrix4& CAnaglyphCamera::getViewMatrixAffector() const
{
	return m_camX->getViewMatrixAffector();
}

bool CAnaglyphCamera::isInputReceiverEnabled() const
{
	return m_camX->isInputReceiverEnabled();
}

bool CAnaglyphCamera::isOrthogonal() const
{
	return m_camX->isOrthogonal();
}

bool CAnaglyphCamera::OnEvent(const SEvent& event)
{
	// 
	return m_camX->OnEvent(event);
}

void CAnaglyphCamera::setAspectRatio(f32 aspect)
{
	// 
	m_camX->setAspectRatio(aspect);
}

void CAnaglyphCamera::setFarValue(f32 zf)
{
	// 
	m_camX->setFarValue(zf);
}

void CAnaglyphCamera::setFOV(f32 fovy)
{
	// 
	m_camX->setFOV(fovy);
}

void CAnaglyphCamera::setInputReceiverEnabled(bool enabled)
{
	// 
	m_camX->setInputReceiverEnabled(enabled);
}

void CAnaglyphCamera::setNearValue(f32 zn)
{
	// ICameraSceneNode::setNearValue(zn);
	m_camX->setNearValue(zn);
}

void CAnaglyphCamera::setProjectionMatrix(const core::matrix4&projection, bool isOrthogonal)
{
	// ICameraSceneNode::setProjectionMatrix(projection, isOrthogonal);
	m_camX->setProjectionMatrix(projection, IsOrthogonal);
}

void CAnaglyphCamera::setUpVector(const core::vector3df& pos)
{
	// 
	m_camX->setUpVector(pos);
}

void CAnaglyphCamera::setTarget(const core::vector3df& pos) // [v]
{
	// IN REVISION: IT DOESN'T WORK PROPERLY		// IN REVISION: IT DOESN'T WORK PROPERLY
	// remember that our camera is composed from two empty nodes and another camera 
	// we must rotate the parent-note of the camera so the target node to be the new target 
	/****************************************** *******************************************/
	m_nodTar->updateAbsolutePosition();
	core::vector3df& vecOldTar = m_nodTar->getAbsolutePosition()-m_nodCam->getPosition();
	core::vector3df& vecNewTar = pos-m_nodCam->getPosition();
	float fNewLen = vecNewTar.getLength();			// the new lenght
	vecOldTar.normalize();
	vecNewTar.normalize();
	core::quaternion qSet;
	qSet.rotationFromTo(vecOldTar, vecNewTar); // we get the quaterion from
	core::vector3df rotSet;
	qSet.toEuler(rotSet);
	m_nodCam->setRotation(rotSet);
	m_nodTar->setPosition(m_nodTar->getPosition()*fNewLen);
	m_nodTar->updateAbsolutePosition();
	m_camX->setTarget(m_nodTar->getAbsolutePosition());
	
	// that's all!
}

void CAnaglyphCamera::setViewMatrixAffector(const core::matrix4& affector)
{
	// 
	// ICameraSceneNode::setViewMatrixAffector(affector);
	m_camX->setViewMatrixAffector(affector);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CAnaglyphCamera::OnAnimate(u32 timeMs)
{
	// scene::ISceneNode::OnAnimate(timeMs);
	scene::ICameraSceneNode::OnAnimate(timeMs);
	m_time = timeMs;
	moveIt(m_time); // debug
	//Right eye: the camera self:
	video::IVideoDriver* driver = m_smgr->getVideoDriver();
	driver->beginScene();										// REACTIVATE!!!!!
	//clear the depth buffer
	driver->clearZBuffer();
	m_camX->setPosition(core::vector3df(m_fdx, 0, 0));
	driver->getOverrideMaterial().Material.ColorMask=m_colorLeft; 
	driver->getOverrideMaterial().EnableFlags=video::EMF_COLOR_MASK;
	driver->getOverrideMaterial().EnablePasses=scene::ESNRP_SKY_BOX+scene::ESNRP_SOLID+scene::ESNRP_TRANSPARENT+scene::ESNRP_TRANSPARENT_EFFECT+scene::ESNRP_SHADOW;

	m_smgr->drawAll(); // 
	driver->clearZBuffer();
	m_camX->setPosition(core::vector3df(0.f, 0.f, 0.f));
	driver->getOverrideMaterial().Material.ColorMask=m_colorRight;
	driver->getOverrideMaterial().EnableFlags=video::EMF_COLOR_MASK;
	driver->getOverrideMaterial().EnablePasses=scene::ESNRP_SKY_BOX+scene::ESNRP_SOLID+scene::ESNRP_TRANSPARENT+scene::ESNRP_TRANSPARENT_EFFECT+scene::ESNRP_SHADOW;

	m_smgr->drawAll(); // 
	driver->clearZBuffer();
	driver->endScene();
	
	//
	driver->getOverrideMaterial().Material.ColorMask=video::ECP_ALL;
	driver->getOverrideMaterial().EnableFlags=0;
	driver->getOverrideMaterial().EnablePasses=0;
	//
	// driver->endScene();

}

void CAnaglyphCamera::moveIt(u32 timeMs)			// MODIFIED!!!!
{	
	m_nodTar->updateAbsolutePosition();
	core::vector3df vds = (m_nodTar->getAbsolutePosition()-m_nodCam->getPosition())/(f32)timeMs/100.f;
	core::vector3df vdl = vds.crossProduct(std::move(core::vector3df(0.f, 1.f, 0.f)))/(f32)timeMs/10.f;

	for(int i = 0; i<m_keymapSize; i++)
	{
		SHORT tabKeyState = GetAsyncKeyState(m_keymap[i].KeyCode);
		if((1<<16)&tabKeyState)
		{
			// printf("%d", m_keymap[i].Action);
			if(EKA_MOVE_FORWARD==m_keymap[i].Action)	{m_nodCam->setPosition(m_nodCam->getPosition()+vds);}
			if(EKA_MOVE_BACKWARD==m_keymap[i].Action)	{m_nodCam->setPosition(m_nodCam->getPosition()-vds);}
			if(EKA_STRAFE_LEFT==m_keymap[i].Action)		{m_nodCam->setPosition(m_nodCam->getPosition()+vdl);}
			if(EKA_STRAFE_RIGHT==m_keymap[i].Action)	{m_nodCam->setPosition(m_nodCam->getPosition()-vdl);}
			if(EKA_JUMP_UP==m_keymap[i].Action)			{m_nodCam->setPosition(m_nodCam->getPosition()-vdl);}
		}
	}
	POINT p;
	::GetCursorPos(&p);
	if(0x100&GetKeyState(VK_RBUTTON))
	{
		core::position2di dxy = (core::position2di(p.x, p.y) - m_mo0);
		m_nodCam->setRotation(m_nodCam->getRotation() + core::vector3df(.1f*(f32)dxy.Y, .1f*(f32)dxy.X, 0.f));
		m_mo0 = core::position2di(p.x, p.y);
	}
	else
	{
		m_mo0 = core::position2di(p.x, p.y);
		::SetCursorPos(m_ncen[0], m_ncen[1]); // set the mouse position back to the cenete of the screen
	}
	/***********************************************
	EKA_MOVE_FORWARD = 0, EKA_MOVE_BACKWARD, EKA_STRAFE_LEFT, EKA_STRAFE_RIGHT, EKA_JUMP_UP, EKA_CROUCH, EKA_COUNT,
	************************************************/
	m_camX->setTarget(m_nodTar->getAbsolutePosition());
}

float CAnaglyphCamera::getFdx()
{
	return m_fdx;
}

void CAnaglyphCamera::setActive()
{
	// 
	m_smgr->setActiveCamera(m_camX);

}

void CAnaglyphCamera::render()
{

}

core::aabbox3df& CAnaglyphCamera::getBoundingBox() const
{
	// 
	core::aabbox3df box = m_nodCam->getBoundingBox();
	return box;
}

void CAnaglyphCamera::addChild(scene::ISceneNode* nodChild)
{
	nodChild->setParent(m_nodCam);
}

void CAnaglyphCamera::test()
{
	// 
	return;
}

void CAnaglyphCamera::debugAnaglyph(f32 fdx, u32 colorRight, u32 colorLeft)
{
	if(fdx)	m_fdx = fdx;
	if(colorRight) m_colorRight = colorRight;
	if(colorLeft) m_colorLeft = colorLeft;
}

