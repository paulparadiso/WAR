/*
 *  textObject.cpp
 *  WAR
 *
 *  Created by Paul Paradiso on 12/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "navObject.h"

NavObject::NavObject(): VisibleObject(){
}

NavObject::NavObject(string _text): VisibleObject(){
	this->setup(_text);
}

void NavObject::setup(string _text){
	//font.loadFont(FONT,16);
//	text = _text;
	trans.set(ofGetWidth()/2,0,-600);
	rot = 0;
	isHovering = 0;
	navImage.loadImage(_text);
	string end = "_glow.png";
	vector<string>splits = ofSplitString(_text, ".");
	navImageGlow.loadImage(splits[0] + end);
	size.set(navImage.getWidth(), navImage.getHeight());
	drawSize.set(size);
	this->updateShape();
}

int NavObject::isInside(int _x, int _y){
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

void NavObject::draw(){
	ofEnableAlphaBlending();
	ofPushMatrix();
	if(this->isLeft){
		ofTranslate(ofGetWidth()/2, 0, -600);
		ofRotateY(-45.0);
		ofRotateY(90);
		ofTranslate(-ofGetWidth(), 0, 0);
	} else {
		ofTranslate(ofGetWidth()/2, 0, -600);
		ofRotateY(-45.0);
	}
	if(isHovering){
		if(isPlaying){
			ofSetColor(255, 255, 255, 255);
			navImageGlow.draw(pos.x,pos.y);
		} else {
			ofSetColor(255, 255, 255, 255);
			navImage.draw(pos.x,pos.y);
		}
	} else {
		if(isPlaying){
			ofSetColor(255, 255, 255, 225);
			navImageGlow.draw(pos.x,pos.y);
		} else {
			ofSetColor(255, 255, 255, 200);
			navImage.draw(pos.x,pos.y);
		}
	}
	ofPopMatrix();
	ofDisableAlphaBlending();
}

void NavObject::stopVideo(){
}

void NavObject::setFontSize(int _s){
	font.loadFont(FONT,_s);
	this->updateShape();
}

void NavObject::update(){
}

int NavObject::react(int _lvl){
	if(_lvl){
		if(state == STATE_PLAY){
			state = STATE_REST;
		} else {
			state = STATE_PLAY;
		}
		this->resetState();
		return state;
	} else {
		state = STATE_REST;
		this->resetState();
		return state;
	}
}

void NavObject::setPos(float _x, float _y){
	pos.set(_x,_y);
	this->updateShape();
}

void NavObject::setPosVel(float _xV, float _yV){
}

void NavObject::setPosAcc(float _xA, float _yA){
}

void NavObject::setRot(float _r){
}

void NavObject::setRotVel(float _rV){
}

void NavObject::setRotAcc(float _rA){
}

void NavObject::resizeByHeight(float _h){
}

void NavObject::resizeByWidth(float _w){
}

void NavObject::adjustPosition(){
}

void NavObject::updateShape(){
	//shape.clear();
//	ofRectangle r = font.getStringBoundingBox(text,pos.x,pos.y);
//	//calculate bottom left corner
//	ofxVec2f tl = ofxVec2f(pos.x + cos(ofDegToRad(90.0 + rot)) * (r.y-r.height), pos.y + sin(ofDegToRad(90.0 + rot)) * (r.y-r.height));
//	//fill shape vector with rotated values.  The order is intentional.
//	shape.push_back(new ofxVec2f(tl.x,tl.y));
//	shape.push_back(new ofxVec2f(tl.x + cos(ofDegToRad(rot)) * (r.x + r.width), tl.y + sin(ofDegToRad(rot)) * (r.x+r.width)));
//	shape.push_back(new ofxVec2f(r.x + cos(ofDegToRad(rot)) * (r.x+r.width), r.y + sin(ofDegToRad(rot)) * (r.x+r.width)));
//	shape.push_back(new ofxVec2f(r.x,r.y));
//	this->updateActualShape();
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

void NavObject::updateActualShape(){
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

void NavObject::drawShape(){
	//if(!isHovering)
//		return;
//	vector<ofxVec3f*>::iterator vi, vj;
//	ofSetColor(0, 255, 0);
//	for(vi = abShape.begin(), vj = abShape.end() - 1; vi < abShape.end(); vj = vi++){
//		//ofEllipse((*vi)->x, (*vi)->y, 10, 10);
//		cout<<"drawing line at:  "<<(*vj)->x<<": "<<(*vj)->y<<": "<<(*vj)->z<<endl;
//		//ofLine((*vj)->x, (*vj)->y, (*vi)->x, (*vi)->y);
//		glBegin(GL_LINES);
//		glVertex3f((*vj)->x,(*vj)->y,(*vj)->z);
//		glVertex3f((*vi)->x,(*vi)->y,(*vi)->z);
//		glEnd();
//		
//	}
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
		cout<<"drawing line at:  "<<(*vj)->x<<": "<<(*vj)->y<<": "<<(*vj)->z<<endl;
		//ofLine((*vj)->x, (*vj)->y, (*vi)->x, (*vi)->y);
		glBegin(GL_LINES);
		//glVertex3f((*vj)->x,(*vj)->y,(*vj)->z);
		//glVertex3f((*vi)->x,(*vi)->y,(*vi)->z);
		glVertex3d(glx2,gly2,glz2);
		glVertex3d(glx,gly,glz);
		glEnd();
		
	}
	
}

void NavObject::resetState(){
	if(state == STATE_PLAY){
		isPlaying = 1;
	}
	if(state == STATE_REST){
		isPlaying = 0;
	}
}