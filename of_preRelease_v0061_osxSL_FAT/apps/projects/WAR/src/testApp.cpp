#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetCircleResolution(50);
	ofSetFrameRate(60);
	ofSetWindowTitle("!WAR");
	//ofSetWindowShape(1920, 1200);
	ofSetWindowShape(1024, 768);
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	//ofSetFullscreen(true);
	
	//vom.addObject(new VideoObject("stVbdXdDSlE.flv"));
	int numVideos = dir.listDir("video");
	for(int i = 0; i < numVideos; i++){
		vom.addObject(new VideoObject(dir.getPath(i)));
	}
	
	vom.randomPositions();
	
	w = 0;
	h = 0;	// set in update()
	
	msgFont.loadFont("Helvetica.dfont",64);
	timeOfLastFlash = 0;
	showMsg = 1;
	flashDelay = 100;
	
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup( PORT );
	
	//video.loadMovie("video/BQZUXcMVYYc.mp4");
//	video.play();

	flashLightImage.loadImage("flashlight-mask-invert-rings.png");
}

//--------------------------------------------------------------
void testApp::update(){
	// check for waiting messages
	vom.update(wiiX,wiiY);
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
	
	//glEnable(GL_BLEND);
//	glColorMask(false,false,false,true);
//	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
//	flashLightImage.draw(wiiX - 250, wiiY - 250);
//	glColorMask(true,true,true,true);
//	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
//	glColor4f(1.0,1.0,1.0,1.0);
	//video.draw(50,50,ofGetWidth(), ofGetHeight());
	vom.draw();
	ofEllipse(wiiX, wiiY, 10, 10);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	wiiX = x;
	wiiY = y;
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

