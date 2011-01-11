#include "testApp.h"

void VectorDirection(ofxVec3f *a, ofxVec3f *b, ofxVec3f *vect);

//--------------------------------------------------------------
void testApp::setup(){
	ofSetCircleResolution(50);
	ofSetFrameRate(60);
	ofSetWindowTitle("!WAR");
//	ofSetWindowShape(1360, 768);
	ofSetWindowShape(1366, 768);
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	//ofDisableArbTex();
	//ofSetFullscreen(true);
	ofHideCursor();
	//glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
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
	
	ofImage floorImage;
	ofImage ceilingImage;
	ofImage woodtestImage;
	
	//lightImage.loadImage("room/flashlight-mask-invert-rings.png");
	//light.allocate(512,512,GL_RGBA);
	//light.loadData(lightImage.getPixels(),512,512,GL_RGBA);
	
	rightWallImage.loadImage("room/bg_right.png");
	rightWall.allocate(1366,768,GL_RGB);
	rightWall.loadData(rightWallImage.getPixels(),1366,768,GL_RGB);
	rightFBO.allocate(rightWallImage.getWidth(), rightWallImage.getHeight());
	//cout<<"dims = "<<rightWall.getWidth()<<", "<<rightWall.getHeight()<<endl;
	leftWallImage.loadImage("room/bg_left.png");
	leftWall.allocate(1366,768,GL_RGB);
	leftFBO.allocate(leftWallImage.getWidth(),leftWallImage.getHeight());
	//leftWall.loadData(leftWallImage.getPixels(),1366,768,GL_RGB);
	floorImage.loadImage("room/carpet.jpg");
	floor.allocate(1366,1366,GL_RGB);
	floor.loadData(floorImage.getPixels(),1366,1366,GL_RGB);
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
	centerX = ofGetWidth()/2;
	centerY = ofGetHeight()/2;
	centerZ = 0;
	
	rotX = 0;
	rotY = 0;
	bSmoothLight = true;
	
	//loadGLTexture();
	
	//reflexions!!
	ofxMaterialSpecular(240, 240, 240); //how much specular light will be reflect by the surface
	ofxMaterialShininess(120); //how concentrated the reflexion will be (between 0 and 128
	
	//each light will emit white reflexions
	light1.specular(255, 255, 255);
	light2.specular(255, 255, 255);
	light3.specular(255, 255, 255);
	ambientLight.specular(255, 255, 255);
	
	camPos.set(ofGetWidth()/2,ofGetHeight()/2,10000);
	//camPos.rotate(-45,ofxVec3f(0,1,0));
	
	camera.setOrigin(OF_ORIGIN_ZERO);
	camera.position(camPos.x, camPos.y, 10000);
	//camera.eye(ofGetWidth()/2,ofGetHeight()/2,100);
	//camera.orbitAround(ofxVec3f(ofGetWidth()/2, ofGetHeight(),0),ofxVec3f(1,0,0), 22.5);
	
	ambience = DEFAULT_LIGHT_LEVEL;

}

//--------------------------------------------------------------
void testApp::update(){
	// check for waiting messages

	//glTexParameteri(rightFBO.texData.textureTarget, GL_GENERATE_MIPMAP, GL_TRUE);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, ofGetWidth(), ofGetHeight(), GL_RGB, GL_UNSIGNED_BYTE, <#const void *data#>)
	//glTexParameteri(leftFBO.texData.textureTarget, GL_GENERATE_MIPMAP, GL_TRUE);
	
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
			//cout << "x: " << wiiX << " y: " << wiiY << "\n";
		}
		else if ( m.getAddress() == "/wii/2/ir/1" )
		{
			y = 1 - m.getArgAsFloat( 0 );
			wiiY = y * h;
			//cout << "x: " << wiiX << " y: " << wiiY << "\n";
		}
		else
		{
			//cout << "unrecognized message: " << m.getAddress() << "\n";
		}
		
	}
	rotX += 1;
	rotY += 2;
	
	//light1
	float L1DirectionX = 1;
	float L1DirectionY = 0;
	float L1DirectionZ = 0;
	
	//light1.directionalLight(255, 255, 255, L1DirectionX, L1DirectionY, L1DirectionZ);
	
	//light2
	float L2ConeAngle = 5;
	float L2Concentration = 120;
	float L2PosX = 0;
	float L2PosY = ofGetHeight();
	float L2PosZ = 100;
	float L2DirectionX = 0;
	float L2DirectionY = 0;
	float L2DirectionZ = -1;
	
	if(wiiX > ofGetWidth()/2){
		target.set(ofGetWidth()/4 * 3,wiiY,-400);
	} else {
		target.set(ofGetWidth()/4,wiiY,-400);
	}
	//cout<<"target z = "<<target.z<<endl;
	source.set(wiiX,wiiY,scaleMouse());
	direction.set(0,0,-1.0);
	
	VectorDirection(&target, &source, &direction);
	//cout<<"direction = "<<direction.x<<", "<<direction.y<<", "<<direction.z<<endl;
	
	light2.spotLight(255, 255, 255, 
					 source.x, source.y, source.z, 
					 direction.x, direction.y, direction.z,
					 10,
					 100);
	
	//light2.directionalLight(255, 255, 255, direction.x, direction.y, direction.z);
	light2.globalAmbientLight(ambience, ambience, ambience);
	
	//target.set(ofGetWidth()/2,ofGetHeight()/2,-600);
	
	//VectorDirection((float*)&target, (float*)&source, (float*)&direction);
	
	//light1.directionalLight(255, 255, 255, direction.x, direction.y, direction.z);
	
	//light3
	float L3PosX = 900;
	float L3PosY = 0;
	float L3PosZ = 100;
	light3.pointLight(255, 255, 255, L3PosX, L3PosY, L3PosZ);
	//light2.pointLight(255,255,255,L3PosX, L3PosY, L3PosZ);
	//camera.lerpPosition(0,0,500,0.1);
	camPos.set(ofGetWidth()/2,ofGetHeight()/2,10000);
	//camPos.rotate(-45,ofxVec3f(0,1,0));
	
	camera.setOrigin(OF_ORIGIN);
	camera.position(camPos.x, camPos.y, 4000);
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
	glEnable(GL_DEPTH_TEST);
	camera.place();
	ofEnableAlphaBlending();
	ofxLightsOn(); //turn lights on
	ofSetColor(255, 255, 255);
	//ofxSphere(ofGetWidth()/2, ofGetHeight()/2, -600, 100);
	rightFBO.begin();
	rightWallImage.draw(0,0);
	vom.drawVideos(0);
	vom.drawThemes(0);
	rightFBO.end();
	leftFBO.begin();
	leftWallImage.draw(0,0);
	vom.drawVideos(1);
	vom.drawThemes(1);
	leftFBO.end();
	this->draw3D();
	glDisable(GL_DEPTH_TEST);
	ofxLightsOff(); //turn lights off to draw text
	vom.drawPlayer();
	ofFill();
	ofSetColor(255, 255, 255, 128);
	ofEllipse(wiiX, wiiY, 200, 200);
	ofNoFill();
	ofSetColor(255, 255, 255);
	ofDisableAlphaBlending();
}

void testApp::draw3D(){
	ofSetColor(255, 0, 0);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	ofSetColor(255, 255, 255);
	//ofTranslate(ofGetWidth()/2, 0, -600);
	
	//ofRotateX(10);	
	//ofRotateY(-45);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	//glBegin(GL_QUADS);
//	glNormal3f(0.0,0.0,-1.0);
//	glEnd();
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
	//cout<<"right normals "<<rightns.x<<", "<<rightns.y<<", "<<rightns.z<<endl;
	//float leftN[3] = {leftns.x,leftns.y,leftns.z};
	float leftN[3] = {1.0,0.3,1.0};
	//cout<<"left normals "<<leftns.x<<", "<<leftns.y<<", "<<leftns.z<<endl;
	//rightFBO.draw3D(0,0,0,rightWall.getWidth(), ofGetHeight(),0,rightN);
	rightFBO.draw3D(ofGetWidth()/2,0,-800,rightWall.getWidth(), ofGetHeight(),rightWall.getWidth() - 800,rightN);
	//rightWallImage.draw(0,0);
	//leftFBO.draw3D(0,0,ofGetWidth(),0,leftWall.getHeight(),0,leftN);
	leftFBO.draw3D(-ofGetWidth()/2,0,ofGetWidth() -800,ofGetWidth(),leftWall.getHeight(),-800,leftN);
	float topN[3] = {0.2,0.2,0.2};
	float bottomN[3] = {0.2,0.2,0.2};
	ceiling.draw3DTop(ofGetWidth()/2,0,-1000,ceiling.getHeight()-1000,0,0,topN);
	floor.draw3DBottom(ofGetWidth()/2,0,-1000,ceiling.getHeight()-1000,0,0,bottomN);
	ofFill();
	ofSetColor(255, 255, 255);
}

void testApp::loadGLTexture(){
	//ofTexture aTex;
	//aTex.allocate(512, 512, GL_RGB);
	//aTex.loadData(woodtestImage.getPixels(), 512, 512, GL_RGB);
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
	
	//cout<<"b "<<vect->z<<endl;
    
    float len = (sqrt(pow(vect->x, 2) + pow(vect->y, 2) + pow(vect->z, 2)));
    if(len == 0) len = 1.0f;
    
    //vect[0] /= len ;
    //vect[1] /= len ;
    //vect[2] /= len ;
	//cout<<"a "<<vect->z<<endl;
	vect->set(vect->x /= len,vect->y /= len,vect->z /= len);
}


void testApp::drawRoom(){
	ofPushMatrix();
	ofSetColor(255, 255, 255);
	ofTranslate(ofGetWidth()/2, 0, -600);
	ofRotateY(-45.0);
	//ofRotateY(180);
	//ofTranslate(ofGetWidth(), 0, 0);
	rightWall.draw(0,0,ofGetWidth(),ofGetHeight());
	ofTranslate(-ofGetWidth(), 0, 0);
	ofRotateY(180);
	ofRotateY(90.0);
	ofTranslate(-ofGetWidth(), 0, 0);
	leftWall.draw(0,0,ofGetWidth(),ofGetHeight());
	//ceiling.draw(0,0);
	ofTranslate(ofGetWidth(), 0, 0);
	ofRotateY(-90.0);
	ofTranslate(0, ofGetHeight(), 0);
	ofRotateX(90);
	//leftWall.draw(0,0,ofGetWidth(),ofGetHeight());
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
	//ofTranslate(0, -ofGetHeight(), 0);
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

