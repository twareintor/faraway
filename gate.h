

#if !defined(GATE_H_23A99040D492A934C5928202903B1)
#define		 GATE_H_23A99040D492A934C5928202903B1


#include <irrlicht.h>

using namespace irr;

class CxGate: public scene::ISceneNode
{
private:
	scene::ISceneManager*	m_smgr;
	scene::ISceneNode*		m_nodFrame; // an empty node as mounting point of the gate (lowest middle-point)
	scene::ISceneNode*		m_nodElem; // the element self
	float					m_fApp;

	bool m_bCmdOpen, m_bCmdClose, m_bIsOpened, m_bIsClosed;

	bool m_bRes;

public:
	// CxGate() = delete; // in order not to call it anymroe
	CxGate(
		scene::ISceneManager* smgr,		// the scene manager, we'll get the video drivers and all from it
										/*** IrrlichtDevice*	device,		****/			// device
		scene::ISceneNode* nodPar = 0,			// parent (can be 0)
		s32 id = -1,
		scene::ISceneNode* nodFrame = 0,
		scene::ISceneNode* nodElem = 0,
		scene::IMetaTriangleSelector* met = 0,
		core::vector3df posLeft = core::vector3df(-1.f, 0.f, 0.f),		// left
		core::vector3df posRight = core::vector3df(0.f, 0.f, 1.f),		// right
		core::vector3df posTop = core::vector3df(0.f, 1.f, 0.f),		// top, dimensions of installation points
		float fThickness = 1.f,
		video::SMaterial* matFrame = 0,			// material of the frame
		video::SMaterial* matElem = 0,			// material of the flaps

		bool bRes = false				// reserved
		);
	virtual ~CxGate();
	virtual void OnRegisterSceneNode();
	virtual void OnAnimate(u32 timeMs);
	virtual void render();
	virtual const core::aabbox3df& getBoundingBox() const override; 
	virtual const core::aabbox3df getTransformedBoundingBox() const override; 
	virtual void open();
	virtual void close();
	virtual bool isOpen();
	virtual bool isClosed();


};














#endif //   GATE_H_23A99040D492A934C5928202903B1