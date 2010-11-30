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
	vp.loadMovie(_path);
	size.set(vp.getWidth(),vp.getHeight());
	drawSize.set(size);
	vp.setPosition(0.2);
	vp.setLoopState(true);
	rot = ofRandom(-45.0,45.0);
	this->updateShape();
}

void VideoObject::update(){
	vp.idleMovie();
}

void VideoObject::draw(){
	glPushMatrix();
	glTranslatef(pos.x,pos.y,0);
	glRotatef(rot,0,0,1);
	//vp.draw(pos.x, pos.y, drawSize.x, drawSize.y);
	ofSetColor(255, 255, 255);
	vp.draw(0,0, drawSize.x, drawSize.y);
	glPopMatrix();
	this->drawShape();
}
	
void VideoObject::react(int _lvl){
	if(_lvl == 2 && isPlaying == 0){
		vp.play();
	}
}
	
int VideoObject::isInside(int _x, int _y){
	int inside = 0;
	vector<ofxVec2f*>::iterator vi, vj;
	for(vi = shape.begin(), vj = shape.end() - 1; vi < shape.end(); vj = vi++){
		if(((*vi)->y  <= _y && _y < (*vj)->y || (*vj)->y <= _y && _y < (*vi)->y) &&
		   _x < ((*vj)->x - (*vi)->x) * (_y - (*vi)->y) / ((*vj)->y - (*vi)->y) + (*vi)->x){
			inside = !inside;
		}
		//if((((*vi)->y > _y) || ((*vj)->y > _y)) &&
//		   (_y < ((*vj)->x - (*vi)->x) * (_y - (*vi)->y) / ((*vj)->y - (*vi)->y) + (*vi)->x)){
//			inside  = !inside;
//		}
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
}

void VideoObject::drawShape(){
	vector<ofxVec2f*>::iterator vi, vj;
	ofSetColor(255, 0, 0);
	for(vi = shape.begin(), vj = shape.end() - 1; vi < shape.end(); vj = vi++){
		//ofEllipse((*vi)->x, (*vi)->y, 10, 10);
		ofLine((*vj)->x, (*vj)->y, (*vi)->x, (*vi)->y);
	}
}

void VideoObject::resizeByHeight(float _h){
	float hPercent = _h  / size.y;
	drawSize.set(size.x * hPercent, _h);
}
void VideoObject::resizeByWidth(float _w){
	float wPercent = _w / size.x;
	drawSize.set(_w, size.y * wPercent);
}
