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
	vp.setLoopState(false);
	rot = ofRandom(-30.0,30.0);
	state = STATE_REST;
	stopTime = 0;
	xAdd = 0.0;
	artFont.loadFont("HelveticaNeue.dfont", 8);
	this->updateShape();
}

void VideoObject::update(){
	if(ofGetElapsedTimeMillis() < stopTime && drawSize.x >= restSize.x){
		this->resizeByWidth(drawSize.x + (xAdd * (ofGetElapsedTimeMillis() - (stopTime - GROW_TIME))));
	}
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
	glPushMatrix();
	glTranslatef(pos.x,pos.y,0);
	glRotatef(rot,0,0,1);
	//vp.draw(pos.x, pos.y, drawSize.x, drawSize.y);
	if (state > STATE_REST) {
		ofSetColor(0, 0, 0, 200);
		ofRect(0.0, 0.0, drawSize.x + 175, drawSize.y);
		ofSetColor(0,255,0,255);
		artFont.drawString(" ARTIST: JANE DOE",drawSize.x + 10, 10);
		artFont.drawString("ARTWORK: UNTITLED #4",drawSize.x + 10, 30);
		artFont.drawString("   YEAR: 1974",drawSize.x + 10, 50);
		ofSetColor(255, 255, 255, 255);
	} else {
		ofSetColor(255, 255, 255, 255);
	}
	vp.draw(0,0, drawSize.x, drawSize.y);
	glPopMatrix();
	ofDisableAlphaBlending();
	//this->drawShape();
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
	
int VideoObject::isInside(int _x, int _y){
	int inside = 0;
	vector<ofxVec2f*>::iterator vi, vj;
	for(vi = shape.begin(), vj = shape.end() - 1; vi < shape.end(); vj = vi++){
		if(((*vi)->y  <= _y && _y < (*vj)->y || (*vj)->y <= _y && _y < (*vi)->y) &&
		   _x < ((*vj)->x - (*vi)->x) * (_y - (*vi)->y) / ((*vj)->y - (*vi)->y) + (*vi)->x){
			inside = !inside;
		}
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

void VideoObject::resetState(){
	switch(state){
		case STATE_REST:
			vp.setPosition(0.2);
			vp.stop();
			stopTime = ofGetElapsedTimeMillis() + GROW_TIME;
			pos = restPos;
			xAdd = ((restSize.x - drawSize.x) / GROW_TIME);
			cout<<"resizing by "<<xAdd<<endl;
			//drawSize = restSize;
			break;
		case STATE_HOVER:
			restPos = pos;
			restSize = drawSize;
			this->resizeByPercent(1.0);
			break;
		case STATE_PLAY:
			this->resizeByPercent(1.0);
			vp.setPosition(0.0);
			vp.play();
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
	float wPercent = _w / drawSize.x;
	drawSize.set(_w, drawSize.y * wPercent);
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