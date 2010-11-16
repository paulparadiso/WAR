#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetCircleResolution(50);
	ofSetFrameRate(60);
	ofSetWindowTitle("!WAR");
	ofSetWindowShape(1920, 1200);
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofSetFullscreen(true);
	
	w = 0;
	h = 0;	// set in update()
	
	msgFont.loadFont("Helvetica.dfont",64);
	timeOfLastFlash = 0;
	showMsg = 1;
	flashDelay = 100;
	
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup( PORT );
	
	// video
	bool result = video.loadMovie("videos/phantom.flv");
	cout << "loadMovie returned " << result << "\n";
	video.play();
	
	// flashlight
	flashlightImage.loadImage("flashlight-mask-invert-rings.png");
}

//--------------------------------------------------------------
void testApp::update(){
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		if(w == 0 || h == 0){
			w = ofGetWidth();
			h = ofGetHeight();			
		}
		// get the next message
		ofxOscMessage m;
		float x,y;
		receiver.getNextMessage( &m );
		
		if ( m.getAddress() == "/wii/2/ir/0" )
		{
			x = m.getArgAsFloat( 0 );
			wiiX = x * w;
			cout << "x: " << wiiX << " y: " << wiiY << "\n";
		}
		else if ( m.getAddress() == "/wii/2/ir/1" )
		{
			y = 1 - m.getArgAsFloat( 0 );
			wiiY = y * h;
			cout << "x: " << wiiX << " y: " << wiiY << "\n";
		}
		else
		{
			cout << "unrecognized message: " << m.getAddress() << "\n";
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
//	ofSetColor(255, 130, 0);
//	float radius = 50;
//	ofFill();
//	ofCircle(wiiX, wiiY, radius);
	
	glEnable(GL_BLEND);
	glColorMask(false, false, false, true);
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
	//glColor4f(0.5,0.5,1,0.5f);
	//ofCircle (wiiX,wiiY,200);
	flashlightImage.draw(wiiX-250, wiiY-250);
	
	glColorMask(true,true,true,true);
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
	//ofSetColor(0xff0000);
	glColor4f(1.0,1.0,1.0,1.0f);
//	for (int i = 0; i < 400; i++){
//		ofLine(ofRandom(0,300), ofRandom(0,300), ofRandom(0,300), ofRandom(0,300));
//	}
	// 478 x 359
	video.draw(50,50, 478*2, 359*2);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

