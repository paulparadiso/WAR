#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	w = ofGetWidth();
	h = ofGetHeight();
	msgFont.loadFont("Helvetica.dfont",64);
	timeOfLastFlash = 0;
	showMsg = 1;
	flashDelay = 100;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	string msg = "WAR";
	int sW = msgFont.stringWidth(msg);
	int sH = msgFont.stringHeight(msg);
	ofSetColor(255, 0, 0);
	if(showMsg){
		msgFont.drawString(msg,w/2 - sW/2, h/2);
		if(ofGetElapsedTimeMillis() - timeOfLastFlash > flashDelay){
			showMsg = 0;
			timeOfLastFlash = ofGetElapsedTimeMillis();
		}
	} else {
		if(ofGetElapsedTimeMillis() - timeOfLastFlash > flashDelay){
			showMsg = 1;
			timeOfLastFlash = ofGetElapsedTimeMillis();
		}
	}
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

