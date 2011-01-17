#include "testApp.h"

void VectorDirection(ofxVec3f *a, ofxVec3f *b, ofxVec3f *vect);

//--------------------------------------------------------------
void testApp::setup(){
	ofSetCircleResolution(50);
	//ofSetFrameRate(60);
	ofSetWindowTitle("!WAR");
	ofSetWindowShape(1366, 768);
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofHideCursor();
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	w = 0;
	h = 0;	// set in update()
	
	msgFont.loadFont("fonts/Helvetica.dfont",64);
	timeOfLastFlash = 0;
	showMsg = 1;
	flashDelay = 100;	
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup( PORT );
	
	ofImage floorImage;
	ofImage ceilingImage;
	ofImage woodtestImage;
	
	lightImage.loadImage("room/flashlight-mask-invert-rings_big.png");
	lightMask.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	
	rightWallImage.loadImage("room/bg_right.png");
	rightWall.allocate(1366,768,GL_RGB);
	rightWall.loadData(rightWallImage.getPixels(),1366,768,GL_RGB);
	rightFBO.allocate(rightWallImage.getWidth(), rightWallImage.getHeight());
	//cout<<"dims = "<<rightWall.getWidth()<<", "<<rightWall.getHeight()<<endl;
	leftWallImage.loadImage("room/bg_left.png");
	leftWall.allocate(1366,768,GL_RGB);
	leftFBO.allocate(leftWallImage.getWidth(),leftWallImage.getHeight());
	//leftWall.loadData(leftWallImage.getPixels(),1366,768,GL_RGB);
	floorImage.loadImage("room/parquet13.jpg");
	floor.allocate(floorImage.getWidth(),floorImage.getHeight(),GL_RGB);
	floor.loadData(floorImage.getPixels(),floorImage.getWidth(),floorImage.getHeight(),GL_RGB);
	ceilingImage.loadImage("room/floor_c.png");
	//ceiling.allocate(1029,1221,GL_RGB);
	ceiling.allocate(1366,1366,GL_RGB);
	ceiling.loadData(ceilingImage.getPixels(),1366,1366,GL_RGB);
	
	woodtestImage.loadImage("room/woodtest.jpg");
	woodtest.allocate(512,512,GL_RGB);
	woodtest.loadData(woodtestImage.getPixels(), 512, 512, GL_RGB);
	flashLightImage.loadImage("flashlight-mask-invert-rings.png");
	
	ofSetVerticalSync(true);
	//glEnable(GL_DEPTH_TEST);
	frameToggle = 0;
	lightFBO.allocate(ofGetWidth(),ofGetHeight(),GL_RGBA);

}

//--------------------------------------------------------------
void testApp::update(){
	// check for waiting messages	
	while( receiver.hasWaitingMessages() )
	{
		cout<<"Have messages."<<endl;
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
	vom.update(wiiX,wiiY);
	//makeMask(wiiX,wiiY,ofGetWidth()/2, ofGetHeight()/2);
}

float testApp::scaleMouse(){
	if(wiiX < ofGetWidth()/2){
		return -(float)wiiX;
	} else {
		return -((float)ofGetWidth() - wiiX);
	}
}

void testApp::makeNormals(ofxVec3f a,ofxVec3f b,ofxVec3f c,ofxVec3f *_d){
	float vec1X = b.x-a.x;
	float vec1Y = b.y-a.y;
	float vec1Z = b.z-a.z;
	
	float vec2X = c.x-b.x;
	float vec2Y = c.y-b.y;
	float vec2Z = c.z-b.z;
	
	float crossX = vec1Y*vec2Z - vec1Z*vec2Y;
	float crossY = vec1Z*vec2X - vec1X*vec2Z;
	float crossZ = vec1X*vec2Y - vec1Y*vec2X;
	
	float dist = sqrt(crossX*crossX + crossY*crossY + crossZ*crossZ);
	_d->set(crossX/dist, crossY/dist, crossZ/dist);
}

//--------------------------------------------------------------
void testApp::draw(){
	rightFBO.begin();
	rightWallImage.draw(0,0);
	vom.drawVideos(0);
	rightFBO.end();
	leftFBO.begin();
	leftWallImage.draw(0,0);
	vom.drawVideos(1);
	leftFBO.end();
	glEnable(GL_BLEND);
	glColorMask(false, false, false, true);
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
	glColor4f(1.0,1.0,1.0,0.90f);
	glClear(GL_COLOR_BUFFER_BIT);
	//lightMask.draw(0,0);
	lightImage.draw(wiiX - lightImage.getWidth()/2, wiiY - lightImage.getHeight()/2);
	//glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
	//glColor4f(1.0,1.0,1.0,0.9f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//lightImage.draw(ofGetWidth()/2 - lightImage.getWidth()/2, ofGetHeight()/2 - lightImage.getHeight()/2);
	//lightFBO.draw(0,0);
	glColorMask(true,true,true,true);
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
	glColor4f(1.0,1.0,1.0,1.0f);
	glEnable(GL_DEPTH_TEST);
	ofSetColor(255, 255, 255);
	this->draw3D();
	glDisable(GL_DEPTH_TEST);
	ofDisableAlphaBlending();
	ofEnableAlphaBlending();
	vom.drawThemes2D();
	vom.drawDates2D();
	vom.drawPlayer();
	//if(vom.haveMessage[0]){
//		vom.hoverTextBoxes[0].draw(ofGetWidth()/2, ofGetHeight()/2);
//		vom.haveMessage[0] = 0;
//	}
//	if(vom.haveMessage[1]){
//		vom.hoverTextBoxes[1].draw(ofGetWidth()/2 - vom.hoverTextBoxes[1].getWidth(), ofGetHeight()/2);
//		vom.haveMessage[1] = 0;
//	}
	ofDisableAlphaBlending();
	//ofSetColor(255, 255, 255);
}

void testApp::makeMask(int _x1, int _y1, int _x2, int _y2){
	unsigned char *screen = (unsigned char*)malloc(ofGetWidth() * ofGetHeight() * 4);
	unsigned char *image = (unsigned char*)lightImage.getPixels();
	int offsetX1 = lightImage.getWidth()/2 - _x1;
	int offsetY1 = lightImage.getHeight()/2 - _y1;
	int offsetX2 = lightImage.getWidth()/2 - _x2;
	int offsetY2 = lightImage.getHeight()/2 - _y2;
	for(int y = 0; y < ofGetHeight(); y++){
		for(int x = 0; x < ofGetWidth(); x++){
			int lightPixel1 = ((offsetX1 + x) + (offsetY1 + y) * lightImage.getWidth()) * 4; 
			int lightPixel2 = ((offsetX2 + x) + (offsetY2 + y) * lightImage.getWidth()) * 4;
			if(image[lightPixel1] == 255 && image[lightPixel2] == 255){
				continue;
			}
			int screenPixel= (x + y * ofGetWidth()) * 4;
			screen[screenPixel] = (image[lightPixel1] + image[lightPixel2]) / 2;
			screen[screenPixel + 1] = (image[lightPixel1 + 1] + image[lightPixel2 + 1]) / 2;
			screen[screenPixel + 2] = (image[lightPixel1 + 2] + image[lightPixel2 + 2]) / 2;
			screen[screenPixel + 3] = (image[lightPixel1 + 3] + image[lightPixel2 + 3]) / 2;
			//screen[screenPixel] = ((int)image[lightPixel1] + (int)image[lightPixel2]) / 2;
		}
	}
	lightMask.loadData((unsigned char*)screen, ofGetWidth(), ofGetHeight(), GL_RGBA);
}

void testApp::draw3D(){
	ofSetColor(255, 255, 255);
	ofxVec3f rightns;
	ofxVec3f leftns;
	makeNormals(ofxVec3f(ofGetWidth()/2, ofGetHeight(), -600),
				ofxVec3f(ofGetWidth()/2, 0, -600),
				ofxVec3f(ofGetWidth() + ofGetWidth()/2, 0,ofGetWidth() - 600),
				&rightns);
	
	makeNormals(ofxVec3f(-ofGetWidth()/2,0,ofGetWidth() -600),
				ofxVec3f(-ofGetWidth()/2,ofGetHeight(),ofGetWidth() -600),
				ofxVec3f(0, 0,- 600),
				&leftns);
	
	//float rightN[3] = {rightns.x,rightns.y,rightns.z};
	float rightN[3] = {-1.0,0.3,1.0};
	//float leftN[3] = {leftns.x,leftns.y,leftns.z};
	float leftN[3] = {1.0,0.3,1.0};
	ofxVec3f rightEdge(rightWall.getWidth(),0,-600);
	rightEdge.rotate(-30, ofxVec3f(0,1,0));
	//cout<<"Right x and z = "<<rightEdge.x<<", "<<rightEdge.z<<endl;
	rightFBO.draw3D(ofGetWidth()/2,0,-600,rightEdge.x, rightWall.getHeight(),rightEdge.z,rightN);
	leftFBO.draw3D(ofGetWidth()/2 - rightEdge.x + 117,0,rightEdge.z,leftWall.getWidth(),leftWall.getHeight(),-600,leftN);
	float topN[3] = {0.2,0.2,0.2};
	float bottomN[3] = {0.2,0.2,0.2};
	float topF[3] = {ofGetWidth()/2,0,-600};
	float topR[3] = {ofGetWidth()/2 + rightEdge.x,0,rightEdge.z};
	float topB[3] = {ofGetWidth()/2,0,1200};
	float topL[3] = {ofGetWidth()/2 - rightEdge.x + 117,0,rightEdge.z};
	ceiling.draw3DTop((float*)&topF,
					  (float*)&topR,
					  (float*)&topB,
					  (float*)&topL, ceiling.getWidth(),ceiling.getHeight(),1.0,1.0);
	topF[1] = rightWall.getHeight();
	topR[1] = rightWall.getHeight();
	topB[1] = rightWall.getHeight();
	topL[1] = rightWall.getHeight();
	floor.draw3DTop((float*)&topF,
					(float*)&topR,
					(float*)&topB,
					(float*)&topL, floor.getWidth(),floor.getHeight(),1.0,1.0);
	
	ofFill();
	ofSetColor(255, 255, 255);
}

void testApp::loadGLTexture(){
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, woodtestImage.getPixels());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void testApp::drawCubeRoom(){
//	glClear(GL_COLOR_BUFFER_BIT);
//	glLoadIdentity();
//	loadGLTexture();
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(quad[0].x,quad[0].y,quad[0].z);
	glTexCoord2f(1.0, 0.0); glVertex3f(quad[1].x,quad[1].y,quad[1].z);
	glTexCoord2f(1.0, 1.0); glVertex3f(quad[2].x,quad[2].y,quad[2].z);
	glTexCoord2f(0.0, 1.0); glVertex3f(quad[3].x,quad[3].y,quad[3].z);
	glEnd();
}

void VectorDirection(ofxVec3f *a, ofxVec3f *b, ofxVec3f *vect)
{
    vect->x = a->x - b->x;
    vect->y = a->y - b->y;
    vect->z = a->z - b->z;
	
	float len = (sqrt(pow(vect->x, 2) + pow(vect->y, 2) + pow(vect->z, 2)));
    if(len == 0) len = 1.0f;

	vect->set(vect->x /= len,vect->y /= len,vect->z /= len);
}


void testApp::drawRoom(){
	ofPushMatrix();
	ofSetColor(255, 255, 255);
	ofTranslate(ofGetWidth()/2, 0, -600);
	ofRotateY(-45.0);
	rightWall.draw(0,0,ofGetWidth(),ofGetHeight());
	ofTranslate(-ofGetWidth(), 0, 0);
	ofRotateY(180);
	ofRotateY(90.0);
	ofTranslate(-ofGetWidth(), 0, 0);
	leftWall.draw(0,0,ofGetWidth(),ofGetHeight());
	ofTranslate(ofGetWidth(), 0, 0);
	ofRotateY(-90.0);
	ofTranslate(0, ofGetHeight(), 0);
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
	ofRotateX(-90);
	ofTranslate(0, -ofGetHeight(), ceiling.getHeight());
	ofRotateX(-90);
	ceiling.draw(0,0);
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
		//vom.stopVideo(0);
	if(key == 'l')
		//vom.stopVideo(1);
	if(key == 't')
		vom.updateShapes(0);
	if(key == 'g')
		vom.updateShapes(1);
	if(key == 'y')
		vom.updateShapes(2);
	if(key == 'h')
		vom.updateShapes(3);
	if(key == OF_KEY_UP){
		if(ambience < 255) {
			ambience++;
		}
	}
	if(key == OF_KEY_DOWN){
		if(ambience > 0){
			ambience--;
		}
	}
	
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

