/*************************************************************************************************************************/

#include <irrlicht.h>
#include <windows.h>					// 
#include <wchar.h>

#include "driverChoice.h"
#include "EventReceiverXwator.h"		// Trebuie sa plecam de pe Marte!
#include "triad.h"						// FOR DEBUG ONLY
#include "ids.h"
#include "cwsprn2.h"
#include "gate.h"						// To debug the installatin of gates in a corridor ...

extern "C"{
#include "prnMarquee.h"
}
// #include "readConsoleInput.h"	// debug!

// #define USE_ANAGLYPH_CAMERA_1092
#ifdef USE_ANAGLYPH_CAMERA_1092
#include "AnaglyphCamera2.h"
#endif

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif


#define PI					3.14159f	

#define DEBUG_SCENE			true
#define RENDER_ANAGLYPH		false

using namespace irr;

int main()
{
	int nWidth = GetSystemMetrics(0);
	int nHeight = GetSystemMetrics(1);
	// ask user for driver
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;
	MyEventReceiver	receiver;
	// create device
	IrrlichtDevice* device =
		createDevice(driverType, core::dimension2d<u32>(nWidth, nHeight), 16U, true, false, false, &receiver);	// the device self
	if (device == 0)		return 1; // could not create selected driver.
	video::IVideoDriver* driver = device->getVideoDriver();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();
	gui::IGUISkin* skin = env->getSkin();
	gui::IGUIFont* font = env->getFont("d:/media/fontcourier.bmp");
	if (font)
	{
		skin->setFont(font);
	}
	else
	{
		std::cout << "cannot create font ..." << std::endl;
	}

	// code for checking the presence and features of the joystick:
	core::array<SJoystickInfo> joystickInfo;
	if(device->activateJoysticks(joystickInfo))
	{
		std::cout << "Joystick support is enabled and " << joystickInfo.size() << " joystick(s) are present." << std::endl;

		for(u32 j = 0; j < joystickInfo.size(); ++j)
		{
			std::cout << "Joystick " << j << ":" << std::endl;
			std::cout << "\tName: '" << joystickInfo[j].Name.c_str() << "'" << std::endl;
			std::cout << "\tAxes: " << joystickInfo[j].Axes << std::endl;
			std::cout << "\tButtons: " << joystickInfo[j].Buttons << std::endl;

			std::cout << "\tHat is: ";

			switch(joystickInfo[j].PovHat)
			{
			case SJoystickInfo::POV_HAT_PRESENT:
				std::cout << "present" << std::endl;
				break;

			case SJoystickInfo::POV_HAT_ABSENT:
				std::cout << "absent" << std::endl;
				break;

			case SJoystickInfo::POV_HAT_UNKNOWN:
			default:
				std::cout << "unknown" << std::endl;
				break;
			}
		}
	}
	else
	{
		std::cout << "Joystick support is not enabled." << std::endl;
	}
	// for joystick: additional variables:
	u32 nButtonPressed = 32767;

	// a vector for the rotation of the current bone:
	core::vector3df rotDeb = core::vector3df(0.f, 0.f, 0.f);
	const f32 DEAD_ZONE = 0.1f;
	const SEvent::SJoystickEvent& joystickData = receiver.GetJoystickState();		// joystick data
	bool bFreezeJoint = false;			// freezes a joint in the current position

	// message text to be displayed as debug or program info
	LPWSTR wszMsg = (LPWSTR)GlobalAlloc(GPTR, 0x8000);				// a message text

	scene::ISceneManager* smgr = device->getSceneManager();
	smgr->setAmbientLight(video::SColorf(.2f, .2f, .1f, 1.f));
	// driver->setAmbientLight(video::SColorf(0, 204, 90, 204));

	// create skybox and skydome
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false); // remember to disable mipmaps for skybox and skydome
	scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("d:/media/~~skydomes_from_pr_hash/hemispherical_006 _wUGr.jpg"),16,8,0.95f,2.0f);
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true); // ... and to reenable it after
	// skydome->setMaterialFlag(video::EMF_FOG_ENABLE, true);

	// Add a light, so that the unselected nodes aren't completely dark.
	scene::ILightSceneNode* lgtAmb1 = 0;
	lgtAmb1 = smgr->addLightSceneNode(0, core::vector3df(10000.f, 10000.f, 10000.f), video::SColorf(0.3f, 0.3f, 0.1f, 1.0f), 80000.0f);
	lgtAmb1->setID(ID_IsNotPickable); // Make it an invalid target for selection.
									  // lgtAmb1->setVisible(false);

									  // another light: 
	scene::ILightSceneNode* lgtAmb2 = 0;
	lgtAmb2 = smgr->addLightSceneNode(0, core::vector3df(-10000.f, 10000.f, -10000.f), video::SColorf(0.3f, 0.3f, 0.1f, 1.0f), 80000.0f);
	lgtAmb2 ->setID(ID_IsNotPickable); // Make it an invalid target for selection.
	lgtAmb1->enableCastShadow(false);
	// lgtAmb2->setVisible(false);

	// driver settings again:
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
	// the mesh manipulator
	scene::IMeshManipulator* man = smgr->getMeshManipulator();

	// triangle selector, meta triangle selector// collision manager system etc.
	scene::IMetaTriangleSelector* met = 0;
	met = smgr->createMetaTriangleSelector();
	scene::ITriangleSelector* tri = 0;

	// the nodes of the game: the very start node:
	scene::ISceneNode* nodCity01 = 0;
	scene::IAnimatedMesh* mshCity01 = smgr->getMesh("D:/media/City_Textured/textured_city000.obj"); // this works!!!
	if(mshCity01)
	{
		nodCity01 = smgr->addOctreeSceneNode(mshCity01, 0, IDFlag_IsPickable|IDFlag_IsHighlightable);
		nodCity01->setScale(core::vector3df(100.f, 100.f, 100.f));
		// nodCity01->setPosition(core::vector3df(0.f, 0.f, 0.f));			// aligned with the terrain at its scale!!!
		nodCity01->setMaterialFlag(video::EMF_LIGHTING, true);
		nodCity01->setMaterialFlag(video::EMF_GOURAUD_SHADING, true);
		nodCity01->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
		nodCity01->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
		nodCity01->setName("City01");
		if(met)
		{
			tri = smgr->createTriangleSelector(mshCity01, nodCity01);
			if(!nodCity01->getTriangleSelector()) nodCity01->setTriangleSelector(tri);
			met->addTriangleSelector(tri);
		}
	}

	// GATE FOR DEBUGGING		// gate(s) to debug the class

	core::vector3df	posGateDeb[3] = {
		core::vector3df(112.918, 11.7729, 70.6053), core::vector3df(11192.9, 1287.75, 3383.48), core::vector3df(10578.4, 3086.12, 5688.72)
	};
	scene::ISceneNode* nodFra = smgr->addCubeSceneNode(1.f, 0, IDFlag_IsPickable);
	nodFra->setMaterialFlag(video::EMF_LIGHTING, true);

	CxGate* gat = new CxGate(smgr, nodCity01, IDFlag_IsPickable, nodFra, (scene::ISceneNode*)0, met,
		posGateDeb[0], posGateDeb[1], posGateDeb[2], .1f);
	
	// For debug: a brush to paint and mark the ...
	scene::IMesh* mshBrushStamp = smgr->getMesh("D:/media/~~stamps/stamp01/stamp.obj");
	video::SMaterial* matStamp = new video::SMaterial(); // don't forget:: the contstuctor must be called!!!!
	matStamp->setTexture(0, driver->getTexture("d:/media/smoke.bmp")); // 
	matStamp->NormalizeNormals = true;
	// matStamp->MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL; // don't use this for this mesh...
	matStamp->MaterialType = video::EMT_TRANSPARENT_ADD_COLOR; // but this: the material #1 is black
	core::vector3df posInt0;			// intersection zero (the last intersection, not to put in the same place..)

	// Keymaps for cameras:
	SKeyMap keyMap[] = 
	{
		{EKA_MOVE_FORWARD,		KEY_UP		}, 		{EKA_MOVE_FORWARD,		KEY_KEY_W	}, 
		{EKA_MOVE_BACKWARD,		KEY_DOWN	}, 		{EKA_MOVE_BACKWARD,		KEY_KEY_S	}, 
		{EKA_STRAFE_LEFT,		KEY_LEFT	}, 		{EKA_STRAFE_LEFT,		KEY_KEY_A	}, 
		{EKA_STRAFE_RIGHT,		KEY_RIGHT	}, 		{EKA_STRAFE_RIGHT,		KEY_KEY_D	},
		{EKA_JUMP_UP,			KEY_SPACE	}, 		{EKA_JUMP_UP,			KEY_SPACE	},
	};

#ifdef USE_ANAGLYPH_CAMERA_1092
	/*********** Anaglyph Camera  ***********/
	CAnaglyphCamera* camAnaglyph = new CAnaglyphCamera(smgr, (scene::ISceneNode*)0, -1, keyMap, sizeof(keyMap), 10U, 4U, 50.f);
	camAnaglyph->setPosition(core::vector3df(-450.f, 250.f, -450.f));
	camAnaglyph->setTarget(camAnaglyph->getPosition()+core::vector3df(0.f, 0.f, 4000.f));
	camAnaglyph->setFarValue(220000.f);
	// smgr->getRootSceneNode()->addChild(camAnaglyph);
	// std::cout<<"camera anaglyph created\n"<<std::endl;

	camAnaglyph->setActive();				// BUG#89292 // THIS WORKS!
	float fDist = camAnaglyph->getFdx();	// distance between eyes... adjustable at runtime
	float s = 1.f;		// 
	core::position2di pos0 = receiver.GetMouseState().Position;	// center position of the mouse...

	/************************* a virtual panel to display messages: **************************/

	// a virtual panel to display messages:
	scene::IBillboardTextSceneNode* bilCvs = smgr->addBillboardTextSceneNode(
	font, L"0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef", 
	(scene::ISceneNode*)camAnaglyph, 
	core::dimension2df(260.f, 4.f), core::vector3df(0.f, 80.f, -120.f), -1, 0xffffff00, 0xffffff00);
	bilCvs->setMaterialFlag(video::EMF_ZBUFFER, false);
	scene::IBillboardTextSceneNode* bilCvs2 = smgr->addBillboardTextSceneNode(
	font, L"0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef", 
	(scene::ISceneNode*)camAnaglyph, 
	core::dimension2df(260.f, 4.f), core::vector3df(0.f, 75.f, -120.f), -1, 0xffffff00, 0xffffff00);
	bilCvs2->setMaterialFlag(video::EMF_ZBUFFER, false);
#endif

	// Main camera: // main camera, without collision for this project
	scene::ICameraSceneNode* camMain = smgr->addCameraSceneNodeFPS(0, 50.f, 1.f, 0, keyMap, sizeof(keyMap), false, 3.f, false, true);
	camMain->setPosition(core::vector3df(7310.15f, 3045.88f, 5342.35f));
	camMain->setTarget(core::vector3df(+9050.f, 3050.f, +9050.f));
	camMain->setFarValue(220000.f);
	camMain->setName("Main Camera");

	// for debug only: a DEBUG cam
	scene::ICameraSceneNode* camDebug =	smgr->addCameraSceneNodeFPS(0, 50.0f, 1.f, -1,  keyMap, sizeof(keyMap), false, 3.f, false, false);
	camDebug->setPosition(camMain->getPosition());			// set the initial position and target of debug cam just like 
	camDebug->setTarget(camMain->getTarget());				//			the main camera...
	camDebug->setFarValue(camMain->getFarValue());

	camDebug->bindTargetAndRotation(true);
	camDebug->setName("debug cam");
	// smgr->setActiveCamera(camDebug);
	if(met)
	{
		scene::ISceneNodeAnimator* cra = smgr->createCollisionResponseAnimator(
			met, camDebug, core::vector3df(10.f, 45.f, 10.f), core::vector3df(0.f, 0.f, 0.f), core::vector3df(0.f, 30.f, 0.f));
		// met->drop(); // As soon as we're done with the selector, drop it	// not yet, we need it more...
		// camDebug->addAnimator(cra);		// no collision response to the debug camera, just for ray-collision
		cra->drop();  // And likewise, drop the animator when we're done referring to it.
	}
	// for debug only: Add the billboard.
	scene::IBillboardSceneNode * bilDeb = smgr->addBillboardSceneNode();
	bilDeb->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bilDeb->setMaterialTexture(0, driver->getTexture("d:/media/x.bmp"));
	bilDeb->setMaterialFlag(video::EMF_LIGHTING, false);
	bilDeb->setMaterialFlag(video::EMF_ZBUFFER, false);
	bilDeb->setSize(core::dimension2d<f32>(30.0f, 30.0f));
	bilDeb->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it
	bilDeb->setVisible(false);			// initially false...
	
	// FOR DEBUG ONLY: a couple of variables used
	scene::ISceneNode* nodDeb = 0;		// FOR DEBUG ONLY the actually node pointed by the debugging - billboard;
	core::vector3df posTrd;			// FOR DEBUG ONLY		// position of the triad
	u32 nMat = 0;					// FOR DEGUG ONLY		// the highlighted material

	scene::IAnimatedMeshSceneNode* node = 0;
	video::SMaterial matTri;
	matTri.Wireframe = true;

	if(RENDER_ANAGLYPH)
	{
#ifdef USE_ANAGLYPH_CAMERA_1092 
		camAnaglyph->setActive();				// BUG#89292 // THIS WORKS!
#endif
	}
	// add a triad for debug:		trdDebug // nod triad debug from "triad.h"
	ClsTriad* trdDeb = new ClsTriad(device, camDebug);
	trdDeb->setPosition(core::vector3df(32.f, -24.f, 40.f));
	// trdDeb->setVisible(FALSE);				// set invisible!	

	// Initializations:
	scene::ISceneNode* highlightedSceneNode = 0;		// Remember which scene node is highlighted
	scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();

	// std::cout<<"TEST text added"<<std::endl;
	// smgr->setActiveCamera(camAnaglyph);	// BUG#89292 // THIS CRASHES! WARNING!!!
	
	tri->drop(); // not needed anymore, and be sure not to be used again:
	tri = 0;

	// preparing operations for the main loop and variables: 
	device->getCursorControl()->setVisible(false);
	int nFra = 0; // counts frames to display scrolling text in the 
	u32 t0 = device->getTimer()->getTime(); 
	int lastFPS = -1;
	/*************************************** now for the main loop: ***********************************************/
	while(device->run())
	{
		if (device->isWindowActive())
		{
			if(RENDER_ANAGLYPH)
			{
#ifdef USE_ANAGLYPH_CAMERA_1092
				u32 dt = device->getTimer()->getTime()-t0;
				camAnaglyph->OnAnimate(dt);		// TO DO: optimizations necessary!!!
				u32 t0 = device->getTimer()->getTime(); 
				if(receiver.IsKeyDown(irr::KEY_PLUS))	fDist+=0.1f;
				if(receiver.IsKeyDown(irr::KEY_MINUS))	fDist-=0.1f;
				if(receiver.IsKeyDown(irr::KEY_KEY_Q))	std::cout<<"fDist = "<<fDist<<std::endl;
				camAnaglyph->debugAnaglyph(fDist);
#endif
			}
			else
			{
				driver->beginScene(true, true, video::SColor(255, 128, 128, 255));
				smgr->drawAll();
				env->drawAll();				// REMEMBER THIS NEXT TIME YOU ARE DEVELOPING DEBUG IN GAMES!!!!!!!!!!!!!!!!!!!
				driver->endScene();			// We're all done drawing, so end the scene.
				if(receiver.IsKeyDown(irr::KEY_F8))
				{
					smgr->setActiveCamera(camDebug);
					bilDeb->setVisible(true);
				}
				if(receiver.IsKeyDown(irr::KEY_F9))
				{
					smgr->setActiveCamera(camMain);
					bilDeb->setVisible(false);
				}
			}

		}
		
		/*************** BEGIN OF DEBUG SECTION *****************/
		if(DEBUG_SCENE)
		{
			// All intersections in this example are done with a ray cast out from the camera to
			// a distance of 1000.  You can easily modify this to check (e.g.) a bullet
			// trajectory or a sword's position, or create a ray from a mouse click position using
			// ISceneCollisionManager::getRayFromScreenCoordinates()
			core::line3d<f32> ray;
			ray.start = camDebug->getPosition();
			ray.end = ray.start + (camDebug->getTarget() - ray.start).normalize() * 100000.0f;
			// Tracks the current intersection point with the level or a mesh
			core::vector3df intersection;
			// Used to show with triangle has been hit
			core::triangle3df hitTriangle;
			// See the methods of ISceneCollisionManager
			scene::ISceneNode* selectedSceneNode =
				collMan->getSceneNodeAndCollisionPointFromRay(
					ray,
					intersection,		// This will be the position of the collision
					hitTriangle,		// This will be the triangle hit in the collision
					IDFlag_IsPickable,	// This ensures that only nodes that we have set up to be pickable are considered
					0);					// Check the entire scene (this is actually the implicit default)
						// If the ray hit anything, move the billboard to the collision position and draw the triangle that was hit.
			nodDeb = selectedSceneNode;				// FOR DEBUG ONLY
			if(selectedSceneNode)
			{
				bilDeb->setPosition(intersection);
				// We need to reset the transform before doing our own rendering.
				driver->setTransform(video::ETS_WORLD, core::matrix4());
				driver->setMaterial(matTri);
				driver->draw3DTriangle(hitTriangle, video::SColor(0,255,0,0));
				if((selectedSceneNode->getID() & IDFlag_IsHighlightable) == IDFlag_IsHighlightable)
				{
					highlightedSceneNode = selectedSceneNode;
					highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
				}
			}
			if(trdDeb->isVisible()||true)
			{
				core::matrix4 matCamDebug = camDebug->getAbsoluteTransformation();
				core::matrix4 matCamDebugInv;
				matCamDebug.getInverse(matCamDebugInv);
				trdDeb->setRotation(matCamDebugInv.getRotationDegrees());			// 
			}
			if(nodDeb)
			{
				if(receiver.IsKeyDown(irr::KEY_KEY_M)){nMat++; if(nMat>nodDeb->getMaterialCount()) nMat=0;}
				if(receiver.IsKeyDown(irr::KEY_KEY_N)){nMat--; if(nMat<1) nMat=nodDeb->getMaterialCount()-1;}
				if(receiver.IsKeyDown(irr::KEY_KEY_M)||receiver.IsKeyDown(irr::KEY_KEY_N))	// DEBUG ONLY: GET MATERIAL ID			TO BE DELETED!!!
				{
					for(u32 i=0; i<nodDeb->getMaterialCount(); i++)
					{
						if(i==nMat)
						{
							nodDeb->getMaterial(i).EmissiveColor.set(0, 255, 0, 0);
						}
						else
						{
							nodDeb->getMaterial(i).EmissiveColor.set(0, 0, 0, 0);
						}
					}
					Sleep(200);
				}
			}

			// DEBUG 4 MOUSE POS
			if(receiver.GetMouseState().RightButtonDown)
			{
				scene::ISceneNode* nod = smgr->addCubeSceneNode(10.f, 0, -1, bilDeb->getPosition());
				nod->setMaterialFlag(video::EMF_LIGHTING, true);
			}
			if(receiver.GetMouseState().LeftButtonDown)
			{
				if((intersection-posInt0).getLength()>0.1) // not to draw in the same place...
				{
					scene::ISceneNode* nod = smgr->addMeshSceneNode(mshBrushStamp, 0, -1, bilDeb->getPosition());
					nod->setScale(core::vector3df(25.f));
					nod->getMaterial(0) = *matStamp;
					// all this stuff to orient with the surface... NOTE: this can be used for another kind of shading...
					core::vector3df n = hitTriangle.getNormal(); 
					n.normalize();
					core::quaternion q;
					core::vector3df v = core::vector3df(0.f, 1.f, 0.f);
					q.rotationFromTo(v, n);
					core::vector3df veu;
					q.toEuler(veu);
					nod->setRotation(veu*core::RADTODEG);
					// .. till here...
				}
				posInt0 = intersection;
			}

			if(receiver.IsKeyDown(irr::KEY_KEY_P))		// DEBUG ONLY:						TO BE DELETED!!!
			{
				/// /// /// /// ///		PRINTING DEBUG AND DESIGN HELP DATA		/// /// /// /// /// /// /// /// /// /// 
				std::cout<<"Cam("<<camDebug->getPosition().X<<", "<<camDebug->getPosition().Y<<", "<<camDebug->getPosition().Z<<")"<<std::endl;
				if(nodDeb)
				{
					std::cout<<"node in debug: \""<<nodDeb->getName()<<"\""<<std::endl;
					core::vector3df vecRel = core::vector3df(0.f, 0.f, 0.f);
					vecRel = (bilDeb->getPosition() - nodDeb->getPosition())/nodDeb->getScale();
					core::CMatrix4<f32> mat;
					mat.setRotationDegrees(-nodDeb->getRotation());
					// mat.setTranslation(-nodDeb->getPosition());
					// mat.setScale(core::vector3df(1.f/nodDeb->getScale().X, 1.f/nodDeb->getScale().Y, 1.f/nodDeb->getScale().Z));
					mat.transformVect(vecRel);
					std::cout<<"Coloured material = "<<nMat<<std::endl;
					std::cout<<"Bil("<<bilDeb->getPosition().X<<", "<<bilDeb->getPosition().Y<<", "<<bilDeb->getPosition().Z<<")"<<std::endl;
					std::cout<<"BilRel("<<vecRel.X<<", "<<vecRel.Y<<", "<<vecRel.Z<<")"<<std::endl;
					// scene::ISceneNode* nodSphDeb = smgr->addSphereSceneNode(1.f, 16, nodDeb, IDFlag_IsPickable, vecRel); // for debug only...
					std::cout<<"nodDebug002("<<nodDeb->getPosition().X<<"f, "<<nodDeb->getPosition().Y<<"f, "<<nodDeb->getPosition().Z<<"f"<<")\r\n";
					core::vector3df norTri = hitTriangle.getNormal().normalize();
					std::cout<<"hit triangle normal = ("<<norTri.X<<", "<<norTri.Y<<", "<<norTri.Z<<")\r\n";
				}
				else
				{
					std::cout<<"no node in debug"<<std::endl;
				}
				Sleep(700);
			}
			if(receiver.IsKeyDown(irr::KEY_KEY_R))
			{
				scene::ISceneNode* nodSphDeb = smgr->addSphereSceneNode(1.f, 16, nodDeb, IDFlag_IsPickable, intersection); // for debug only...
				nodSphDeb->setMaterialFlag(video::EMF_LIGHTING, true);

			}
			// scene::ISceneNode* nodSphDeb = smgr->addSphereSceneNode(1.f, 16, nodDeb, IDFlag_IsPickable, vecRel); // for debug only...

			if(receiver.IsKeyDown(irr::KEY_PRIOR)) 
			{
				camDebug->setPosition(camDebug->getPosition() + core::vector3df(0.f, 1.5f*300/lastFPS, 0.f));
			}
			if(receiver.IsKeyDown(irr::KEY_NEXT)) 
			{
				camDebug->setPosition(camDebug->getPosition() - core::vector3df(0.f, 1.5f*300/lastFPS, 0.f));
			}
			// Unlight any currently highlighted scene node
			if (highlightedSceneNode)
			{
				highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, true);
				highlightedSceneNode = 0;
			}
		}
	/************************* END OF DEBUG SECTION *********************************************/
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"FARAWAY [";
			str += driver->getName();
			str += L"] FPS:";
			str += fps;
			// device->setWindowCaption(str.c_str());
#ifdef USE_ANAGLYPH_CAMERA_1092
			if(RENDER_ANAGLYPH)
			{
				LPWSTR wszOut = (LPWSTR)GlobalAlloc(GPTR, 0xff);
				cwsprn2(wszOut, str.c_str(), 0x60, wcslen(str.c_str()), 0);
				bilCvs->setText(wszOut);
				GlobalFree(wszOut);
			}
#endif
			lastFPS = fps;
		}
		env->clear();
	}
	delete matStamp;
	delete gat;
	met->drop();
	delete trdDeb;
#ifdef USE_ANAGLYPH_CAMERA_1092
	delete camAnaglyph;
#endif
	device->drop();
	std::cout<<"Hello, World!"<<std::endl;
	return 0;

}

/*************************************************************************************************************************/


