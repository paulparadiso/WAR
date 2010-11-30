/*
 *  visibleObjectManager.cpp
 *  WAR
 *
 *  Created by Paul Paradiso on 11/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "visibleObjectManager.h"

VisibleObjectManager::VisibleObjectManager(){
	idAssign = 0;
}

void VisibleObjectManager::addObject(VisibleObject *_vo){
	_vo->resizeByWidth(250.0);
	_vo->id = idAssign++;
	objects.push_back(_vo);
}

void VisibleObjectManager::update(){
	vector<VisibleObject*>::iterator vi;
	int count = 0;
	for(vi = objects.begin(); vi < objects.end(); vi++){
		if(count == 0){
			(*vi)->react(2);
		}
		(*vi)->update();
		count++;
	}
}

void VisibleObjectManager::update(int _x, int _y){
	this->update();
	this->checkInsides(_x,_y);
}

void VisibleObjectManager::draw(){
	vector<VisibleObject*>::iterator vi;
	for(vi = objects.begin(); vi < objects.end(); vi++){
		(*vi)->draw();
	}
}

void VisibleObjectManager::randomPositions(){
	vector<VisibleObject*>::iterator vi;
	for(vi = objects.begin(); vi < objects.end(); vi++){
		(*vi)->setPos(ofRandom(0,ofGetWidth()),ofRandom(0, ofGetHeight()));
		(*vi)->adjustPosition();
	}
}

void VisibleObjectManager::checkInsides(int _x, int _y){
	VisibleObject* tmpObject;
	vector<VisibleObject*>::iterator vi;
	int count = 0;
	int remove =  -1;
	for(vi = objects.begin(); vi < objects.end(); vi++){
		if((*vi)->isInside(_x,_y)){
			tmpObject = (*vi);
			remove = count;
		}
		count++;
	}	
	if(remove > -1){
		objects.erase(objects.begin() + remove);
		objects.push_back(tmpObject);
		if(tmpObject->id == lastHoverId){
			hoverTime += ofGetElapsedTimeMillis() - hoverTime; 
			if(hoverTime > HOVER_CLICK_TIME){
				tmpObject->react(1);
			}
		} else {
			lastHoverId = tmpObject->id;
			hoverTime = ofGetElapsedTimeMillis();
		}
	}
}

//void VisibleObjectManager::addObject(VisibleObject _vo){
//	objects.push_back(_vo);
//}


