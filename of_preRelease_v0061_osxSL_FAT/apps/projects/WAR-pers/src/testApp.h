#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxDirList.h"
#include "videoObject.h"
#include "textObject.h"
#include "visibleObjectManager.h"

#define PORT 9000

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
	
		int w,h;
		ofTrueTypeFont msgFont;
		int showMsg;
		int timeOfLastFlash;
		int flashDelay;
	
	private:
		ofxOscReceiver	receiver;
		float			wiiX, wiiY;
		VisibleObjectManager vom;
		ofxDirList dir;
		ofImage flashLightImage;
		//ofVideoPlayer video;
		VideoObject vo;
		ofImage wall;
		ofImage floor;
		ofImage ceiling;
		TextObject to;
	
};

#endif