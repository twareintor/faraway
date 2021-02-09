
#include "gate.h"
// #include "materialpack.h"

CxGate::CxGate(
	scene::ISceneManager* smgr,		// the scene manager, we'll get the device from it!
/*** IrrlichtDevice*	device,					// device ****/
	scene::ISceneNode* nodPar,			// parent (can be 0)
	s32 id, 
	scene::ISceneNode* nodFrame,
	scene::ISceneNode* nodElem,
	scene::IMetaTriangleSelector* met,
	core::vector3df posLeft,	// left
	core::vector3df posRight,	// right	
	core::vector3df posTop,		// top
	float fThickness,
	video::SMaterial* matFrame,			// material of the frame
	video::SMaterial* matElem,			// material of the flaps

	bool bRes				// reserved
	):
	scene::ISceneNode(nodPar, smgr, -1), // do I have to do it?
	m_smgr(smgr),
	m_nodFrame(0),
	m_nodElem(0), // the element self
	m_fApp(0.f),	// apperture 
	m_bCmdClose(false),
	m_bCmdOpen(false),
	m_bIsClosed(false),
	m_bIsOpened(false),

	m_bRes(false)
{
	/* the center of the gate, as well as the center of the element is in the center 
	the scale/ position/ rotation of the gate will be determined based on the
	three positions specified in constructor, 
	*/
	if(!nodFrame)
	{
		m_nodFrame = m_smgr->addEmptySceneNode(nodPar, id);

	}
	else
	{
		m_nodFrame = nodFrame;
		m_nodFrame->setParent(nodPar);
		if(matFrame)
		{
			if(matFrame)
			{
				m_nodFrame->getMaterial(0) = *matFrame;
			}
		}
		else
		{
			// TO DO: set up a default material from..
		}
	}
	if(!nodElem)
	{
		m_nodElem = m_smgr->addCubeSceneNode(1.f, nodFrame, id); // here were a bug!
		m_nodElem->setMaterialFlag(video::EMF_LIGHTING, true);
	}
	else
	{
		m_nodElem = nodElem;
		nodElem->setParent(m_nodFrame);
		nodElem->setDebugDataVisible(true);
		// TO DO: code to adapt the scale ...
	}

	// Position then the gate on the scene according to the positioning vectors:
	core::vector3df e1 = posTop-posRight;					// the sides of the triangle
	core::vector3df e2 = posLeft-posTop;
	core::vector3df e3 = posRight-posLeft;
	core::vector3df n = e2.crossProduct(e3);	n.normalize();		// helping vectors
	core::vector3df p = n.crossProduct(e3);		p.normalize();
	core::vector3df v = e3;						v.normalize();
	f32 sx = e3.getLength();					// calculate the scales:
	core::vector3df e31 = e3; e31.normalize(); 
	core::vector3df e21 = e2; e21.normalize(); 
	f32 sy = (e31.crossProduct(-e21)).getLength()*e2.getLength();
	f32 sz = fThickness;
	m_nodFrame->setScale(core::vector3df(sx, sy, sz)); // scale set
	// POSITION: 
	core::vector3df pos = posLeft+(posRight-posLeft+p*sy)*0.5f;
	m_nodFrame->setPosition(pos);
	// then rotate the element to be on the plane defined by the three reference points:
	// using quaternion: first get the rotating vector:
	core::vector3df r0 = core::vector3df(0.f, 0.f, 1.f);
	// all this stuff to orient with the alignment points...
	core::quaternion q;
	q.rotationFromTo(r0, n);
	core::vector3df veu = r0*core::DEGTORAD;
	q.toEuler(veu);
	m_nodFrame->setRotation(veu*core::RADTODEG);
	// .. till here...
	// finally add it in the collission system:
	scene::ITriangleSelector* tri = 0;
	if(nodFrame)											
	{
		scene::IMeshSceneNode* nodTest = static_cast<scene::IMeshSceneNode*>(nodFrame);
		scene::IMesh* msh = nodTest->getMesh();
		tri = m_smgr->createTriangleSelector(msh, m_nodFrame);
		if(!m_nodFrame->getTriangleSelector()) nodFrame->setTriangleSelector(tri);
		met->addTriangleSelector(tri);
		// tri->drop();			// not yet but don't forget
	}
	if(nodElem)											
	{
		scene::IMeshSceneNode* nodTest = static_cast<scene::IMeshSceneNode*>(nodElem);
		scene::IMesh* msh = nodTest->getMesh();
		tri = m_smgr->createTriangleSelector(msh, m_nodElem);
		if(!m_nodFrame->getTriangleSelector()) nodFrame->setTriangleSelector(tri);
		met->addTriangleSelector(tri);
		// tri->drop();			// not yet but don't forget
	}
	if(tri) tri->drop();
}

CxGate::~CxGate()
{

}


void CxGate::OnRegisterSceneNode()
{
	ISceneNode::OnRegisterSceneNode();
	if (IsVisible)
	{
		m_smgr->registerNodeForRendering(this);
	}
}

void CxGate::OnAnimate(u32 timeMs)
{
	// code to open and close the door
	if(m_bCmdOpen)
	{
		m_fApp+=0.5f;
		m_nodElem->setPosition(core::vector3df(0.f, m_fApp, 0.f));

	}
	m_bIsClosed = m_fApp>0.7f;
	m_bIsOpened = m_fApp<99.3f;
}

void CxGate::render()
{
	// scene::ISceneNode::render();
}

const core::aabbox3df& CxGate::getBoundingBox() const
{
	return  m_nodElem->getBoundingBox();
}

const core::aabbox3df CxGate::getTransformedBoundingBox() const
{
	return m_nodElem->getTransformedBoundingBox();
}

void CxGate::open()
{
	m_bCmdOpen = true;
}

void CxGate::close()
{
	m_bCmdClose = true;
}

bool CxGate::isOpen()
{
	return m_bIsOpened;
}

bool CxGate::isClosed()
{
	return m_bIsClosed;
}
