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
}

void VideoObject::update(){
	vp.idleMovie();
}

void VideoObject::draw(){
	vp.draw(pos.x, pos.y, drawSize.x, drawSize.y);
}
	
void VideoObject::react(int _lvl){
	if(_lvl == 2 && isPlaying == 0){
		vp.play();
	}
}
	
int VideoObject::isInside(int _x, int _y){
	if((_x > pos.x && _x < pos.x + drawSize.x) && 
	   (_y > pos.y && _y < pos.y + drawSize.y)){
		return 1;
		cout<<"Inside"<<endl;
	}
	return 0;
}

void VideoObject::setPos(float _x, float _y){
	pos.set(_x,_y);
}

void VideoObject::setPosVel(float _xV, float _yV){
	posVel.set(_xV,_yV);
}

void VideoObject::setPosAcc(float _xA, float _yA){
	posAcc.set(_xA,_yA);
}

void VideoObject::setRot(float _r){
	rot = _r;
}

void VideoObject::setRotVel(float _rV){
	rotVel = _rV;
}

void VideoObject::setRotAcc(float _rA){
	rotAcc = _rA;
}

void VideoObject::resizeByHeight(float _h){
	float hPercent = _h  / size.y;
	drawSize.set(size.x * hPercent, _h);
}
void VideoObject::resizeByWidth(float _w){
	float wPercent = _w / size.x;
	drawSize.set(_w, size.y * wPercent);
}
