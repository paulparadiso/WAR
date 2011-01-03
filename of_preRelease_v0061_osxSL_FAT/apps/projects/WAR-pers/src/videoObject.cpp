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
	vp.loadMovie(_path);
	size.set(vp.getWidth(),vp.getHeight());
	drawSize.set(size);
	vp.setPosition(0.2);
	vp.setLoopState(OF_LOOP_NONE);
	//rot = ofRandom(-30.0,30.0);
	rot = 0;
	state = STATE_REST;
	stopTime = 0;
	xAdd = 0.0;
	artFont.loadFont("HelveticaNeue.dfont", 8);
	this->updateShape();
	oldWidth = 0;
	artistFont.loadFont("SpartanLTStd-HeavyClass.otf",14);
	themeFont.loadFont("SpartanLTStd-BookClass.otf",10);
	tagsFont.loadFont("SpartanLTStd-HeavyClass.otf",10);
	uploadFont.loadFont("SpartanLTStd-BookClass.otf",8);
}

void VideoObject::update(){
	if(state == STATE_PLAY){
		vp.idleMovie();
		if(vp.getIsMovieDone()){
			state = STATE_REST;
			this->resetState();
		}
	}
}

void VideoObject::draw(){
	ofEnableAlphaBlending();
	if(this->isLeft){
		if(state != STATE_PLAY){
			ofPushMatrix();
			ofSetColor(255, 255, 255, 255);
			ofTranslate(ofGetWidth()/2, 0, -600);
			ofRotateY(-45.0);
			ofRotateY(90);
			ofTranslate(-ofGetWidth(), 0, 0);
			vp.draw(pos.x,pos.y, drawSize.x, drawSize.y);
			ofPopMatrix();
			//(*ti)->drawShape();
			if(isHovering)
				this->drawShape();
		} else {
			//vp.draw(75, (ofGetHeight()/2) - (playSize.y /2), playSize.x, playSize.y);
			int topX = 25;
			int topY = ofGetHeight() / 2 - playSize.y / 2;
			ofFill();
			ofSetColor(0, 0, 0, 128);
			ofRect(0,topY - 100, ofGetWidth()/3, (ofGetHeight()/2) + 160);
			ofSetColor(255, 255, 255,255);
			vp.draw(topX, topY, playSize.x, playSize.y);
			artistFont.drawString("JANE DOE 1985",topX, topY - 50);
			themeFont.drawString("IDENTITY",topX, topY - 20);
			tagsFont.drawString("TAGS: music, new york, performance",topX, ofGetHeight()/2 + topY + 10);
			uploadFont.drawString("Artwork added on 12.10.2010 at 8:30PM from Brooklyn, NY by lynn",topX, ofGetHeight()/2 + topY + 30);
		}
	} else {
		if(state != STATE_PLAY){
			ofPushMatrix();
			ofSetColor(255, 255, 255, 255);
			ofTranslate(ofGetWidth()/2, 0, -600);
			ofRotateY(-45.0);
			vp.draw(pos.x,pos.y, drawSize.x, drawSize.y);
			ofPopMatrix();
			if(isHovering)
				this->drawShape();
			
		} else {
			int topX = (ofGetWidth() / 3 * 2) + 25;
			int topY = (ofGetHeight()/2) - (playSize.y/2);
			ofFill();
			ofSetColor(0, 0, 0, 128);
			ofRect(ofGetWidth()/3 * 2,topY - 100, ofGetWidth(), (ofGetHeight()/2) + 160);
			ofSetColor(255, 255, 255, 255);
			vp.draw((ofGetWidth()/3 * 2 + 25), topY, playSize.x, playSize.y);
			artistFont.drawString("JANE DOE 1979",topX, topY - 50);
			themeFont.drawString("CONSCIOUSNESS RAISING",topX, topY - 20);
			tagsFont.drawString("TAGS: war, art, new york",topX, ofGetHeight()/2 + topY + 10);
			uploadFont.drawString("Artwork added on 12.10.2010 at 8:30PM from Brooklyn, NY by lynn",topX, ofGetHeight()/2 + topY + 30);\
			//vp.draw(ofGetWidth()/2,0);
		}
	}
	ofDisableAlphaBlending();
}
	
int VideoObject::react(int _lvl){
	if(_lvl){
		if(state == STATE_PLAY)
			return state;
		state++;
		this->resetState();
		return state;
	} else {
		state = STATE_REST;
		this->resetState();
		return state;
	}
}

void VideoObject::stopVideo(){
	state = STATE_REST;
	this->resetState();
}
	
int VideoObject::isInside(int _x, int _y){
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

void VideoObject::updateActualShape(){
	abShape.clear();
	vector<ofxVec2f*>::iterator vi;
	ofxVec3f translation;
	ofxVec3f rotation;
	ofPushMatrix();
	if(isLeft){
		translation.set(-210,0,140);
		rotation.set(0,1,0);
	} else {
		translation.set(ofGetWidth()/2, 0, -600.0);
		rotation.set(0,1,0);
	}
	for(vi = shape.begin(); vi < shape.end(); vi++){
		ofxVec3f *tmp = new ofxVec3f((*vi)->x,(*vi)->y, 0);
		if(isLeft){
			tmp->rotate(45,rotation);
		} else {
			tmp->rotate(-45,rotation);
		}
		*tmp += translation;
		abShape.push_back(tmp);
	}
	ofPopMatrix();
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
				if(this->id == 1){
					cout<<"Have coors of "<<glx<<": "<<gly<<": "<<glz<<endl;
				}
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

void VideoObject::resetState(){
	float tw;
	float twPercent;
	switch(state){
		case STATE_REST:
			vp.setPosition(0.2);
			vp.stop();
			//this->resizeByWidth(225);
			break;
		case STATE_HOVER:
			break;
		case STATE_PLAY:
			oldWidth = vp.width;
			tw = ofGetWidth()/3 - 50;
			twPercent = tw / size.x;
			playSize.set(tw, size.y * twPercent);
			vp.play();
			vp.setPosition(0.0);
			vp.setLoopState(OF_LOOP_NONE);
			if(isLeft){
				vp.setPan(0.0);
			} else {
				vp.setPan(1.0);
			}
			break;
		default:
			break;
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