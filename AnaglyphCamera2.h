/************************************ AnaglyphCamera.h ********************************/

#pragma once

#include <irrlicht.h>


// a camera that moves left <-> right attached to a dummy node
//	rendering with different filters
/***             + "nodTar" attached to "nodCam" // Target   a cube node, for debug
				/|
			   / |   400 m distance to target
"Left Camera" L__R  "Right Camera" if not anaglyphic, only right camera active...
R<-20cm->L ***/

using namespace irr;

// TO DO: optimizations necessary!!! it doesn't need to be derived from scene::ISceneNode
//		Derive CAnaglyphCaera from scene::ICameraSceneNode to have smgr->getActiveCamera()
//		and smgr->setActiveCamera(); // you have to define all pure virtual functions

class CAnaglyphCamera: public scene::ICameraSceneNode
{
private:
	// variables:
	scene::ISceneManager*		m_smgr;
	scene::ISceneNode*			m_nodCam;		// node of the right-eye camera
	scene::ISceneNode*			m_nodTar;		// node of the target of the camera
	scene::ICameraSceneNode*	m_camX;			// the camera self
	
	f32						m_id;
	f32						m_fdx;			// distance between eyes
	u32						m_colorRight;	// color right eye // mostly, the dominant eye...
	u32						m_colorLeft;	// color left eye
	SKeyMap*				m_keymap;
	size_t					m_keymapSize;
	core::vector3df			m_pos;			// the position of the camera
	u32						m_time;			// for OnAnimate() // for moveIt()
	core::position2di		m_mo0;			// mouse position on the screen
	bool					m_bRes;			// reserved
	u32						m_ncen[2];		// center of the screen
	// functions:
	void moveIt(u32 timeMs);

public:
	CAnaglyphCamera() = delete;
	CAnaglyphCamera(scene::ISceneManager* smgr, scene::ISceneNode* parent = (scene::ISceneNode*)0, f32 id = -1, 
		SKeyMap* keymap = (SKeyMap*)0, size_t keymapSize = 0,
		u32 colorRight = video::E_COLOR_PLANE::ECP_RED+video::E_COLOR_PLANE::ECP_BLUE, 
		u32 colorLeft = video::E_COLOR_PLANE::ECP_GREEN,
		f32 fdx = 10.3f);
	virtual ~CAnaglyphCamera();
	virtual void bindTargetAndRotation(bool bound) override;
	virtual f32 getAspectRatio() const override;
	virtual f32 getFarValue() const override;
	virtual f32 getFOV() const override;
	virtual f32 getNearValue() const override;
	virtual const core::matrix4 & getProjectionMatrix() const override;
	virtual const core::vector3df& getTarget() const override;
	virtual bool getTargetAndRotationBinding(void) const override;
	virtual const core::vector3df& getUpVector() const override;
	virtual const scene::SViewFrustum* getViewFrustum() const override; 
	virtual const core::matrix4& getViewMatrix() const override;
	virtual const core::matrix4& getViewMatrixAffector() const override;
	virtual bool isInputReceiverEnabled() const override;
	virtual bool isOrthogonal() const override;
	virtual bool OnEvent(const SEvent &event) override;
	// virtual void serializeAttributes(io::IAttributes *out, io::SAttributeReadWriteOptions *options=0) const;
	virtual void setAspectRatio (f32 aspect) override;
	virtual void setFarValue (f32 zf) override;
	virtual void setFOV(f32 fovy) override;
	virtual void setInputReceiverEnabled(bool enabled) override;
	virtual void setNearValue(f32 zn) override;
	virtual void setProjectionMatrix(const core::matrix4 &projection, bool isOrthogonal=false) override;
	virtual void setRotation(const core::vector3df& rotation) override;
	virtual void setTarget(const core::vector3df &pos) override;
	virtual void setUpVector(const core::vector3df &pos) override;
	virtual void setViewMatrixAffector(const core::matrix4 &affector) override;

	virtual void setPosition(const core::vector3df& pos); // added
	void setActive();	// added
	void render();		//  added
	core::aabbox3df& getBoundingBox() const;	// added// to be removed (a camera doesn't have bounding box...)
	void addChild(scene::ISceneNode* nodChild);

	virtual void OnRegisterSceneNode();
	virtual void OnAnimate(u32 timeMs);

	void test();

	float getFdx();
	void debugAnaglyph(f32 fdx = 0, u32 colorRight = 0, u32 colorLeft = 0);
};
