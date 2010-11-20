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
}

void VisibleObjectManager::addObject(VisibleObject *_vo){
	_vo->resizeByWidth(250.0);
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
	}
}

//void VisibleObjectManager::addObject(VisibleObject _vo){
//	objects.push_back(_vo);
//}


