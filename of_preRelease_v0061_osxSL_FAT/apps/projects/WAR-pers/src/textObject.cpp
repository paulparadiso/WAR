/*
 *  textObject.cpp
 *  WAR
 *
 *  Created by Paul Paradiso on 12/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "textObject.h"

TextObject::TextObject(): VisibleObject(){
}

TextObject::TextObject(string _text): VisibleObject(){
	this->setup(_text);
}

void TextObject::setup(string _text){
	font.loadFont(FONT,16);
	text = _text;
	trans.set(ofGetWidth()/2,0,-600);
	rot = 0;
	isHovering = 0;
	this->updateShape();
}

int TextObject::isInside(int _x, int _y){
	isHovering = 0;
	int inside = 0;
	vector<ofxVec3f*>::iterator vi, vj;
	for(vi = abShape.begin(), vj = abShape.end() - 1; vi < abShape.end(); vj = vi++){
		if(((*vi)->y  <= _y && _y < (*vj)->y || (*vj)->y <= _y && _y < (*vi)->y) &&
		   _x < ((*vj)->x - (*vi)->x) * (_y - (*vi)->y) / ((*vj)->y - (*vi)->y) + (*vi)->x){
			inside = !inside;
		}
	}
	if(inside){
		isHovering = 1;
		cout << "BANG!" <<endl;
	}
	return inside;
}

void TextObject::draw(){
	if(this->isLeft){
		ofEnableAlphaBlending();
		ofPushMatrix();
		ofSetColor(255, 255, 255, 225);
		ofTranslate(ofGetWidth()/2, 0, -600);
		ofRotateY(-45.0);
		ofRotateY(90);
		ofTranslate(-ofGetWidth(), 0, 0);
		//ofRotateY(-180);
		font.drawString(text,pos.x,pos.y);
		ofPopMatrix();
		ofDisableAlphaBlending();
	} else {
		ofEnableAlphaBlending();
		ofPushMatrix();
		ofSetColor(255, 255, 255, 225);
		ofTranslate(ofGetWidth()/2, 0, -600);
		ofRotateY(-45.0);
		font.drawString(text,pos.x,pos.y);
		ofPopMatrix();
		ofDisableAlphaBlending();
	}
	if(isHovering)
		this->drawShape();
}

void TextObject::setFontSize(int _s){
	font.loadFont(FONT,_s);
	this->updateShape();
}

void TextObject::update(){
}

int TextObject::react(int _lvl){
}

void TextObject::setPos(float _x, float _y){
	pos.set(_x,_y);
	this->updateShape();
}

void TextObject::setPosVel(float _xV, float _yV){
}

void TextObject::setPosAcc(float _xA, float _yA){
}

void TextObject::setRot(float _r){
}

void TextObject::setRotVel(float _rV){
}

void TextObject::setRotAcc(float _rA){
}

void TextObject::resizeByHeight(float _h){
}

void TextObject::resizeByWidth(float _w){
}

void TextObject::adjustPosition(){
}

void TextObject::updateShape(){
	shape.clear();
	ofRectangle r = font.getStringBoundingBox(text,pos.x,pos.y);
	//calculate bottom left corner
	ofxVec2f tl = ofxVec2f(pos.x + cos(ofDegToRad(90.0 + rot)) * (r.y-r.height), pos.y + sin(ofDegToRad(90.0 + rot)) * (r.y-r.height));
	//fill shape vector with rotated values.  The order is intentional.
	shape.push_back(new ofxVec2f(tl.x,tl.y));
	shape.push_back(new ofxVec2f(tl.x + cos(ofDegToRad(rot)) * (r.x + r.width), tl.y + sin(ofDegToRad(rot)) * (r.x+r.width)));
	shape.push_back(new ofxVec2f(r.x + cos(ofDegToRad(rot)) * (r.x+r.width), r.y + sin(ofDegToRad(rot)) * (r.x+r.width)));
	shape.push_back(new ofxVec2f(r.x,r.y));
	this->updateActualShape();
}

void TextObject::updateActualShape(){
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

void TextObject::drawShape(){
	if(!isHovering)
		return;
	vector<ofxVec3f*>::iterator vi, vj;
	ofSetColor(0, 255, 0);
	for(vi = abShape.begin(), vj = abShape.end() - 1; vi < abShape.end(); vj = vi++){
		//ofEllipse((*vi)->x, (*vi)->y, 10, 10);
		cout<<"drawing line at:  "<<(*vj)->x<<": "<<(*vj)->y<<": "<<(*vj)->z<<endl;
		//ofLine((*vj)->x, (*vj)->y, (*vi)->x, (*vi)->y);
		glBegin(GL_LINES);
		glVertex3f((*vj)->x,(*vj)->y,(*vj)->z);
		glVertex3f((*vi)->x,(*vi)->y,(*vi)->z);
		glEnd();
		
	}
}

void TextObject::resetState(){
}