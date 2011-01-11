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
	//cout<<vp.getDuration()<<endl;
	drawSize.set(size);
	
	cout<<"size = "<<drawSize.x<<endl;
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
	media = _path;
	artistFont.loadFont("SpartanLTStd-HeavyClass.otf",14);
	themeFont.loadFont("SpartanLTStd-BookClass.otf",10);
	tagsFont.loadFont("SpartanLTStd-HeavyClass.otf",10);
	uploadFont.loadFont("SpartanLTStd-BookClass.otf",8);
	//These numbers were determined by the testShape() function.  
	//There is likely a far better way of handling this. 
	trans_x = -279;
	trans_z = 370;
}

void VideoObject::addFp(FrontPlayer *_fp){
	fp = _fp;
}

void VideoObject::update(){
	if((isLeft && !fp->haveLeft) && state == STATE_PLAY){
		//cout<<"UPDATE reset"<<endl;
		state = STATE_REST;
		resetState();
	}
	if((!isLeft && !fp->haveRight) && state == STATE_PLAY){
		//cout<<"UPDATE reset"<<endl;
		state = STATE_REST;
		resetState();
	}
}

void VideoObject::draw(int _id, int _time){
	if(id == _id){
		alpha = DEFAULT_ALPHA + (((255 - DEFAULT_ALPHA) / HOVER_CLICK_TIME) * _time);
		trigger = (int)(float)drawSize.x * ((float)_time / (float)HOVER_CLICK_TIME);
		cout<<"Trigger = "<<trigger<<endl;
	} else {
		alpha = DEFAULT_ALPHA;
		trigger = 0;
	}
	this->draw();
}

void VideoObject::draw(){
	//ofEnableAlphaBlending();
	//ofEnableSmoothing();
	if(this->isLeft){
		if(state != STATE_PLAY){
			ofPushMatrix();
			ofSetColor(255, 255, 255, 255);
			//ofTranslate(ofGetWidth()/2, 0, -600);
			//ofRotateY(-45.0);
			//ofRotateY(90);
			//ofTranslate(ofGetWidth() * 2, 0, 0);
			resizeByWidth(200);
			if(isHovering){
				//this->drawShape();
				ofFill();
				ofSetColor(0, 0, 0,200);
				ofRect(pos.x - 10,pos.y -10 ,drawSize.x + 20,drawSize.y + 60);
				uploadFont.drawString("Jane Doe - 1971", pos.x, pos.y + drawSize.y + 20);
				if(state == STATE_HOVER){
					ofDisableAlphaBlending();
					ofSetColor(0, 255, 0);
					ofRect(pos.x ,pos.y + drawSize.y + 10 ,trigger,10);
					ofEnableAlphaBlending();
				}
				ofNoFill();
			}
			ofSetColor(255, 255, 255,alpha);
			vp.draw(pos.x,pos.y, drawSize.x
					, drawSize.y);
			ofSetColor(255,255,255,255);
			ofPopMatrix();
			//(*ti)->drawShape();
		} 
	} else {
		if(state != STATE_PLAY){
			//ofPushMatrix();
			ofSetColor(255, 255, 255, 255);
			//ofTranslate(ofGetWidth()/2, 0, -600);
			//ofRotateY(-45.0);
			//ofTranslate(ofGetWidth()*2, ofGetHeight()*2, 0);
			resizeByWidth(200);
			if(isHovering){
				//this->drawShape();
				ofFill();
				ofSetColor(0, 0, 0,200);
				ofRect(pos.x - 10,pos.y -10 ,drawSize.x + 20,drawSize.y + 60);
				uploadFont.drawString("Jane Doe - 1971", pos.x, pos.y + drawSize.y + 20);
				if(state == STATE_HOVER){
					ofDisableAlphaBlending();
					ofSetColor(0, 255, 0);
					ofRect(pos.x ,pos.y + drawSize.y + 10 ,trigger,10);
					ofEnableAlphaBlending();
				}
				ofNoFill();
			}
			ofSetColor(255, 255, 255,alpha);
			vp.draw(pos.x,pos.y, drawSize.x, drawSize.y);
			ofSetColor(255,255,255,255);
		}
	}
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
		//cout<<"REACT reset"<<endl;
		this->resetState();
		return state;
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
			if(isLeft){
				if(!fp->haveLeft){
					fp->vpLeft = &vp;
					fp->playSize = &playSize;
					fp->haveNewLeft = 1;
				}
			} else {
				if(!fp->haveRight){
					fp->vpRight = &vp;
					fp->playSize = &playSize;
					fp->haveNewRight = 1;
				}
			}
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
	ofPushMatrix();
	//cout<<"trans_x = "<<trans_x<<", trans_z = "<<trans_z<<endl;
	if(isLeft){
		translation.set(trans_x,0,trans_z);
		rotation.set(0,1,0);
	} else {
		translation.set(ofGetWidth()/2, 0, -800.0);
		//translationBack.set(0,0,-600);
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