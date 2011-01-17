/*
 *  videoObject.cpp
 *  WAR
 *
 *  Created by Paul Paradiso on 11/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "videoObject.h"

VideoObject::VideoObject():VisibleObject(){
}

VideoObject::VideoObject(string _path):VisibleObject(){
	this->setup(_path);
}

void VideoObject::setup(string _path){
	cout<<"Loading Movie at "<<_path<<endl;
	vp.setUseTexture(false);
	vp.loadMovie(_path);
	size.set(vp.getWidth(),vp.getHeight());
	//cout<<vp.getDuration()<<endl;
	drawSize.set(size);
	cout<<"size = "<<drawSize.x<<endl;
	//rot = ofRandom(-30.0,30.0);
	rot = 0;
	state = STATE_REST;
	stopTime = 0;
	xAdd = 0.0;
	//artFont.loadFont("fonts/HelveticaNeue.dfont", 8);
	this->updateShape();
	oldWidth = 0;
	media = _path;
	fontsLoaded = 0;
	//These numbers were determined by the testShape() function.  
	//There is likely a far better way of handling this. 
	trans_x = -279;
	trans_z = 370;
	timeStarted = ofGetElapsedTimeMillis();
	numPlays = 0;
	textureSet = 0;
	scaledX = 0;
}

string VideoObject::getLocationString(){
	return uploaderLocale;
}

string VideoObject::getUploadString(){
	string out = "Uploaded from: " + uploaderLocale + " on: " + uploadDate;
	return out;
}

string VideoObject::getThemeString(){
	string out = "";
	vector<string>::iterator si;
	for(si = themes.begin(); si < themes.end(); si++){
		out += (*si) + ", ";
	}
	return out;
}

string VideoObject::getTitleString(){
	return title;
}

string VideoObject::getArtistString(){
	string out = "";
	out = artist + " - " + date;
	return out;
}
void VideoObject::update(){
	//if((isLeft && !fp->haveLeft) && state == STATE_PLAY){
//		//cout<<"UPDATE reset"<<endl;
//		state = STATE_REST;
//		resetState();
//	}
//	if((!isLeft && !fp->haveRight) && state == STATE_PLAY){
//		//cout<<"UPDATE reset"<<endl;
//		state = STATE_REST;
//		resetState();
//	}
}

void VideoObject::draw(int _id, int _time){
	if(isLeft){
		scaledX = drawSize.x * 1/ofGetWidth() * pos.x;
	} else {
		scaledX = drawSize.x * 1/ofGetWidth() * (ofGetWidth() -  pos.x);
	}
	if(id == _id){
		alpha = DEFAULT_ALPHA + (((255 - DEFAULT_ALPHA) / HOVER_CLICK_TIME) * _time);
		trigger = ((int)(float)scaledX + 25)* ((float)_time / (float)HOVER_CLICK_TIME);
		//cout<<"Trigger = "<<trigger<<endl;
	} else {
		alpha = DEFAULT_ALPHA;
		trigger = 0;
	}
	this->draw();
}

void VideoObject::draw(){
	//ofEnableAlphaBlending();
	//ofEnableSmoothing();
	size.set(vp.getWidth(),vp.getHeight());
	if(state == STATE_PLAY  || shouldPlay == 0){
		return;
	}
	if(!fontsLoaded){
		artistFont.loadFont("fonts/SpartanLTStd-HeavyClass.otf",14);
		themeFont.loadFont("fonts/SpartanLTStd-BookClass.otf",10);
		//tagsFont.loadFont("fonts/SpartanLTStd-HeavyClass.otf",10);
		uploadFont.loadFont("fonts/SpartanLTStd-BookClass.otf",8);
		fontsLoaded = 1;
	}
	if(!textureSet){
		//vp.tex.allocate(vp.width, vp.height, GL_RGB);
		//vp.tex.loadData(vp.pixels, vp.width, vp.height, GL_RGB);
		drawTexture.allocate(vp.getWidth(),vp.getHeight(),GL_RGB);
		drawTexture.loadData(vp.getPixels(),vp.getWidth(),vp.getHeight(),GL_RGB);
		textureSet = 1;
		if(vp.bLoaded){
			vp.play();
			vp.setPosition(0.2);
			vp.setLoopState(OF_LOOP_NONE);
			vp.stop();
		}
	}
	//ofTexture tex;
	//tex.allocate(vp.getWidth(),vp.getHeight(),GL_RGB);
	//tex.loadData(vp.getPixels(), vp.getWidth(), vp.getHeight(), GL_RGB);
	if(this->isLeft){
		if(state != STATE_PLAY){
			//ofPushMatrix();
			ofSetColor(255, 255, 255, 255);
			resizeByWidth(300);
			if(isHovering){
				//this->drawShape();
				ofFill();
				ofSetColor(128, 128, 128,200);
				ofRect(pos.x - 10,pos.y -10 ,scaledX + 20,drawSize.y + 20);
				ofSetColor(255, 255, 255);
				//uploadFont.drawString(artist + " - " + date, pos.x, pos.y + drawSize.y);
				if(state == STATE_HOVER){	
					ofSetColor(0, 255, 0);
					ofRect(pos.x - 10,pos.y + drawSize.y + 10 ,trigger,10);
				}
				ofNoFill();
			}
			ofSetColor(255, 255, 255,alpha);
			//vp.draw(pos.x,pos.y, drawSize.x, drawSize.y);
			drawTexture.draw(pos.x,pos.y, scaledX, drawSize.y);
			ofSetColor(255,255,255,255);
			//ofPopMatrix();
		} 
	} else {
		if(state != STATE_PLAY){
			//ofPushMatrix();
			ofSetColor(255, 255, 255, 255);
			resizeByWidth(300);
			if(isHovering){
				//this->drawShape();
				ofFill();
				ofSetColor(128, 128, 128,200);
				ofRect(pos.x - 10,pos.y -10 ,scaledX + 20,drawSize.y + 20);
				ofSetColor(255, 255, 255);
				//uploadFont.drawString(artist + " - " + date, pos.x, pos.y + drawSize.y);
				if(state == STATE_HOVER){
					ofSetColor(0, 255, 0);
					ofRect(pos.x - 10,pos.y + drawSize.y + 10 ,trigger,10);
				}
				ofNoFill();
			}
			ofSetColor(255, 255, 255,alpha);
			//vp.draw(pos.x,pos.y, drawSize.x, drawSize.y);
			drawTexture.draw(pos.x,pos.y, scaledX, drawSize.y);
			ofSetColor(255,255,255,255);
		}
	}
}

void VideoObject::drawFront(){
	//ofEnableAlphaBlending();

	if(!isLeft){
		//ofTexture texR;
		//texR.allocate(vp.getWidth(), vp.getHeight(), GL_RGB);
		drawTexture.loadData(vp.getPixels(), vp.getWidth(), vp.getHeight(), GL_RGB);
		int topX = (ofGetWidth() / 3 * 2) + 25;
		int topY = (ofGetHeight()/2) - (playSize.y/2);
		ofFill();
		ofSetColor(0, 0, 0, 128);
		box.set(ofGetWidth()/3 * 2 - 50,topY - 80, playSize.x + 50, ofGetHeight() - topY - 120);
		ofRect(box.x, box.y, box.z, box.w);
		ofSetColor(255, 255, 255, 255);
		//vpRight->draw((ofGetWidth()/3 * 2 + 25), topY, playSize->x, playSize->y);
		drawTexture.draw((ofGetWidth()/3 * 2) - 25, topY, playSize.x, playSize.y);
		artistFont.drawString(getArtistString(),topX - 50, topY - 30);
		themeFont.drawString(getTitleString(),topX - 50, topY - 10);
		//tagsFont.drawString("TAGS: war, art, new york",topX, ofGetHeight() - topY + 20);
		uploadFont.drawString(getUploadString(),topX - 50, ofGetHeight() - topY + 15);
	} 
	if(isLeft){
		//ofTexture texR;
		//texR.allocate(vp.getWidth(), vp.getHeight(), GL_RGB);
		drawTexture.loadData(vp.getPixels(), vp.getWidth(), vp.getHeight(), GL_RGB);		
		int topX = 25;
		int topY = (ofGetHeight()/2) - (playSize.y/2);
		ofFill();
		ofSetColor(0, 0, 0, 128);
		box.set(topX,topY - 80, ofGetWidth()/3, ofGetHeight() - topY - 120);
		ofRect(box.x, box.y, box.z, box.w);
		ofSetColor(255, 255, 255,255);
		//vpLeft->draw(topX, topY, playSize->x, playSize->y);
		drawTexture.draw(topX*2, topY, playSize.x, playSize.y);
		artistFont.drawString(getArtistString(),topX + 25, topY - 30);
		themeFont.drawString(getTitleString(),topX + 25, topY - 10);
		//tagsFont.drawString("TAGS: music, new york, performance",topX, ofGetHeight() - topY + 20);
		uploadFont.drawString(getUploadString(),topX + 25, ofGetHeight() - topY + 15);
	}
	//ofDisableAlphaBlending();
}


int VideoObject::react(int _lvl){
	cout<<"reacting"<<endl;
	if(_lvl){
		if(state == STATE_PLAY)
			return state;
		state++;
		this->resetState();
		return state;
	} else {
		if(state == STATE_HOVER){
			state = STATE_REST;
			//cout<<"REACT reset"<<endl;
			this->resetState();
			return state;
		}
		state = STATE_REST;
		this->resetState();
	}
}

void VideoObject::resetState(){
	float tw;
	float twPercent;
	switch(state){
		case STATE_REST:
			vp.setPosition(0.2);
			vp.stop();
			drawTexture.loadData(vp.getPixels(), vp.getWidth(), vp.getHeight(), GL_RGB);
			//this->resizeByWidth(225);
			break;
		case STATE_HOVER:
			break;
		case STATE_PLAY:
			oldWidth = vp.width;
			tw = ofGetWidth()/3 - 50;
			twPercent = tw / size.x;
			playSize.set(tw, size.y * twPercent);
			isPlaying = 1;
			numPlays++;
			if(isLeft){
				vp.setPan(-1.0);
			} else {
				vp.setPan(1.0);
			}
			vp.play();
			vp.setPosition(0.0);
			vp.setLoopState(0);
			break;
		default:
			break;
	}
}

void VideoObject::stopVideo(){
	//state = STATE_REST;
	//this->resetState();
}
	
int VideoObject::isInside(int _x, int _y){
	if(state == STATE_PLAY){
		return 0;
	}
	GLdouble glx = 0;
	GLdouble gly = 0;
	GLdouble glz = 0;
	
	GLdouble glx2 = 0;
	GLdouble gly2 = 0;
	GLdouble glz2 = 0;	
	
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);	
		
	isHovering = 0;
	int inside = 0;
	vector<ofxVec3f*>::iterator vi, vj;
	for(vi = abShape.begin(), vj = abShape.end() - 1; vi < abShape.end(); vj = vi++){
		gluProject((*vi)->x, (*vi)->y, (*vi)->z,
				   mvmatrix, projmatrix, viewport,
				   &glx, &gly, &glz);
		gluProject((*vj)->x, (*vj)->y, (*vj)->z,
				   mvmatrix, projmatrix, viewport,
				   &glx2, &gly2, &glz2);
		gly = ofGetHeight() - gly;
		gly2 = ofGetHeight() - gly2;
		if((gly  <= _y && _y < gly2 || gly2 <= _y && _y < gly) &&
		   _x < (glx2 - glx) * (_y - gly) / (gly2 - gly) + glx){
			inside = !inside;
		}
		
	}
	if(inside){
		isHovering = 1;
	}
	return inside;
}

int VideoObject::isInsideFlat(int _x, int _y){
}

void VideoObject::setPos(float _x, float _y){
	pos.set(_x,_y);
	this->updateShape();
}

void VideoObject::setPosVel(float _xV, float _yV){
	posVel.set(_xV,_yV);
}

void VideoObject::setPosAcc(float _xA, float _yA){
	posAcc.set(_xA,_yA);
}

void VideoObject::setRot(float _r){
	rot = _r;
	this->updateShape();
}

void VideoObject::setRotVel(float _rV){
	rotVel = _rV;
}

void VideoObject::setRotAcc(float _rA){
	rotAcc = _rA;
}

void VideoObject::updateShape(){
	shape.clear();
	//calculate bottom left corner
	ofxVec2f bl = ofxVec2f(pos.x + cos(ofDegToRad(90.0 + rot)) * drawSize.y, pos.y + sin(ofDegToRad(90.0 + rot)) * drawSize.y);
	//fill shape vector with rotated values.  The order is intentional.
	shape.push_back(new ofxVec2f(pos.x,pos.y));
	shape.push_back(new ofxVec2f(pos.x + cos(ofDegToRad(rot)) * drawSize.x, pos.y + sin(ofDegToRad(rot)) * drawSize.x));
	shape.push_back(new ofxVec2f(bl.x + cos(ofDegToRad(rot)) * drawSize.x, bl.y + sin(ofDegToRad(rot)) * drawSize.x));
	shape.push_back(new ofxVec2f(bl.x,bl.y));
	this->updateActualShape();
}

void VideoObject::testShape(int _which){
	if(_which == 0)
		trans_x++;
	if(_which == 1)
		trans_x--;
	if(_which == 2)
		trans_z++;
	if(_which == 3)
		trans_z--;
	this->updateShape();
}

void VideoObject::updateActualShape(){
	abShape.clear();
	vector<ofxVec2f*>::iterator vi;
	ofxVec3f translation;
	//ofxVec3f translationBack;
	ofxVec3f rotation;
	//ofPushMatrix();
	//cout<<"trans_x = "<<trans_x<<", trans_z = "<<trans_z<<endl;
	if(isLeft){
		//translation.set(trans_x,0,trans_z);
		ofxVec3f rightEdge(ofGetWidth(),0,-300);
		rightEdge.rotate(-30, ofxVec3f(0,1,0));
		translation.set(ofGetWidth()/2 - rightEdge.x + 225, 0, rightEdge.z);
		rotation.set(0,1,0);
	} else {
		translation.set(ofGetWidth()/2, 0, -300.0);
		//translationBack.set(0,0,-600);
		rotation.set(0,1,0);
	}
	for(vi = shape.begin(); vi < shape.end(); vi++){
		ofxVec3f *tmp = new ofxVec3f((*vi)->x,(*vi)->y, 0);
		if(isLeft){
			tmp->rotate(30,rotation);
		} else {
			tmp->rotate(-30,rotation);
		}
		*tmp += translation;
		abShape.push_back(tmp);
	}
	//ofPopMatrix();
}

void VideoObject::drawShape(){
	GLdouble glx = 0;
	GLdouble gly = 0;
	GLdouble glz = 0;
	GLdouble glx2 = 0;
	GLdouble gly2 = 0;
	GLdouble glz2 = 0;
		
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
		
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
	
	if(!isHovering)
		return;
	vector<ofxVec3f*>::iterator vi, vj;
	ofSetColor(255, 0, 0);
	for(vi = abShape.begin(), vj = abShape.end() - 1; vi < abShape.end(); vj = vi++){
		//ofEllipse((*vi)->x, (*vi)->y, 10, 10);
		gluProject((*vi)->x, (*vi)->y, (*vi)->z,
						   mvmatrix, projmatrix, viewport,
						   &glx, &gly, &glz);
		gluProject((*vj)->x, (*vj)->y, (*vj)->z,
				   mvmatrix, projmatrix, viewport,
				   &glx2, &gly2, &glz2);
		gly = ofGetHeight() - gly;
		gly2 = ofGetHeight() - gly2;
		//cout<<"drawing line at:  "<<(*vj)->x<<": "<<(*vj)->y<<": "<<(*vj)->z<<endl;
		//ofLine((*vj)->x, (*vj)->y, (*vi)->x, (*vi)->y);
		glBegin(GL_LINES);
		//glVertex3f((*vj)->x,(*vj)->y,(*vj)->z);
		//glVertex3f((*vi)->x,(*vi)->y,(*vi)->z);
		glVertex3d(glx2,gly2,glz2);
		glVertex3d(glx,gly,glz);
		glEnd();
	}
}

void VideoObject::resizeByPercent(float _p){
	if(_p == 0.0)
		return;
	if(_p > 0.0){
		stopTime = ofGetElapsedTimeMillis() + GROW_TIME;
		xAdd = ((drawSize.x * _p) / 100.0) / GROW_TIME;
	} else {
		this->resizeByWidth(drawSize.x - (drawSize.x / _p));
	}
}

void VideoObject::resizeByHeight(float _h){
	float hPercent = _h  / size.y;
	drawSize.set(size.x * hPercent, _h);
}
void VideoObject::resizeByWidth(float _w){
	float wPercent = _w / size.x;
	drawSize.set(_w, size.y * wPercent);
	this->updateShape();
}

int VideoObject::getSizeX(){
	return (int)drawSize.x;
}

int VideoObject::getSizeY(){
	return (int)drawSize.y;
}

void VideoObject::adjustPosition(){
	while(pos.x < MARGIN){
		pos.x += 10;
	}
	while(pos.x > ofGetWidth() - (MARGIN + drawSize.x)){
		pos.x -= 100;
	}
	while(pos.y < MARGIN){
		pos.y += MARGIN;
	}
	while(pos.y > ofGetHeight() - (MARGIN + drawSize.y)){
		pos.y -= 10;
	}
	this->updateShape();
}