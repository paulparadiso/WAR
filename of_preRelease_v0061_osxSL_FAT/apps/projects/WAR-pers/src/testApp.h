#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxDirList.h"
#include "videoObject.h"
#include "ofxFBOTexture.h"
#include "ofx3DUtils.h"
#include "navObject.h"
#include "visibleObjectManager.h"
#include "defines.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void drawRoom();
		void drawCubeRoom();
		void draw3D();
	
		int w,h;
		ofTrueTypeFont msgFont;
		int showMsg;
		int timeOfLastFlash;
		int flashDelay;
	
		//ofxLight light1; //this will be a directional light
//		ofxLight light2; //this one a spot light
//		ofxLight light3; //and this one a point light
//		ofxLight ambientLight;
	
		void loadGLTexture();
	
		float centerX, centerY, centerZ;
		float rotX, rotY;
	
		bool bSmoothLight;
	
		float scaleMouse();	
		void makeNormals(ofxVec3f a,ofxVec3f b,ofxVec3f c,ofxVec3f *_d);
	
	
	private:
	
		ofImage rightWallImage;
		ofImage leftWallImage;
	
		ofxOscReceiver	receiver;
		float			wiiX, wiiY;
		VisibleObjectManager vom;
		ofxDirList dir;
		ofImage flashLightImage;
		ofVideoPlayer video;
		VideoObject vo;
		ofTexture rightWall;
		ofTexture leftWall;
		ofTexture floor;
		ofTexture ceiling;
		ofTexture light;
		ofImage lightImage;
		NavObject to;
	
		ofTexture woodtest;
		ofImage woodtestImage;
	
		GLuint texture[1];
	
		ofxVec3f quad[4];
	
		//ofxCamera camera;
	
		ofxFBOTexture rightFBO, leftFBO, lightFBO;
	
		ofxVec3f target;
		ofxVec3f source;
		ofxVec3f direction;
		ofxVec3f camPos;
	
		int frameToggle;
	
		int ambience;
	
};

#endif
