#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetCircleResolution(50);
	ofSetFrameRate(60);
	ofSetWindowTitle("!WAR");
//	ofSetWindowShape(1360, 768);
	ofSetWindowShape(1366, 768);
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	//ofSetFullscreen(true);
	ofHideCursor();
	
	w = 0;
	h = 0;	// set in update()
	
	msgFont.loadFont("fonts/Helvetica.dfont",64);
	timeOfLastFlash = 0;
	showMsg = 1;
	flashDelay = 100;
	
	//video.loadMovie("video/N4puCtdG3Mk.flv");
	//video.play();
	
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup( PORT );
	
	rightWall.loadImage("room/right1366.png");
	leftWall.loadImage("room/left1366.png");
	floor.loadImage("room/oldwood2.jpg");
	ceiling.loadImage("room/ceiling.gif");
	
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
	
	this->drawRoom();
	vom.draw();
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(255, 255, 255, 128);
	ofEllipse(wiiX, wiiY, 200, 200);
	ofNoFill();
	ofDisableAlphaBlending();
	ofSetColor(255, 255, 255);
}

void testApp::drawRoom(){
	ofPushMatrix();
	ofNoFill();
	ofSetColor(255, 255, 255);
	ofTranslate(ofGetWidth()/2, 0, -600);
	ofRotateY(-45.0);
	ofRotateX(90);
	ceiling.draw(0,0);
	ofRect(0, 0, ofGetWidth() * 2, ofGetHeight() * 2);
	ofPopMatrix();
	ofPushMatrix();
	//ofSetColor(255, 255, 255);
	ofTranslate(ofGetWidth()/2, 0, -600);
	ofRotateY(-45.0);
	//ofRect(0,0,ofGetWidth(),ofGetHeight());
	rightWall.draw(0,0);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	ofRotateY(90.0);
	ofTranslate(-ofGetWidth(), 0, 0);
	ofRotateY(180);
	leftWall.draw(0,0,-ofGetWidth(),ofGetHeight());
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	ofPopMatrix();
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight(), -600);
	ofRotateY(-45.0);
	ofRotateX(90);
	floor.draw(0,0);
	floor.draw(189,0);
	floor.draw(0,128);
	floor.draw(189,128);
	floor.draw(378,0);
	floor.draw(567,0);
	floor.draw(0,256);
	floor.draw(378,128);
	floor.draw(189,384);
	floor.draw(0,384);
	floor.draw(0,512);
	floor.draw(0,768);
	
	//floor.draw(189,128);
	ofRect(0, 0, ofGetWidth() * 2, ofGetHeight() * 2);
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == '1')
		vom.toggleTheme(0);
	if(key == '2')
		vom.toggleTheme(1);
	if(key == '3')
		vom.toggleTheme(2);
	if(key == '4')
		vom.toggleTheme(3);
	if(key == '5')
		vom.toggleTheme(4);
	if(key == 'f')
		ofToggleFullscreen();
	if(key == 'p')
		vom.togglePlay();
	if(key == 'r')
		vom.stopVideo(0);
	if(key == 'l')
		vom.stopVideo(1);
	if(key == 't')
		vom.updateShapes(0);
	if(key == 'g')
		vom.updateShapes(1);
	if(key == 'y')
		vom.updateShapes(2);
	if(key == 'h')
		vom.updateShapes(3);
	
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

