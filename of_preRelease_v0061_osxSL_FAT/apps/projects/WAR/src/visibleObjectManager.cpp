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
	lastHoverId = -1;
	currentlyPlaying = -1;
	currentlyBig = -1;
}

void VisibleObjectManager::addObject(VisibleObject *_vo){
	_vo->resizeByWidth(250.0);
	_vo->id = idAssign++;
	objects.push_back(_vo);
}

void VisibleObjectManager::update(){
	vector<VisibleObject*>::iterator vi;
	for(vi = objects.begin(); vi < objects.end(); vi++){
		(*vi)->update();
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

void VisibleObjectManager::resetById(int _id){
	vector<VisibleObject*>::iterator vi;
	for(vi = objects.begin(); vi < objects.end(); vi++){
		if((*vi)->id == _id){
			(*vi)->react(0);
		}
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
			if(ofGetElapsedTimeMillis() - hoverTime > HOVER_CLICK_TIME){
				int result = tmpObject->react(1);
				if(result == 2){
					if(currentlyPlaying > -1 && currentlyPlaying != tmpObject->id){
						this->resetById(currentlyPlaying);
						currentlyPlaying = tmpObject->id;
					} else {
						currentlyPlaying = tmpObject->id;
					}
				}
				if(result == 1){
					if(currentlyBig > -1 && currentlyBig != currentlyPlaying){
						this->resetById(currentlyBig);
						currentlyBig = tmpObject->id;
					} else {
						currentlyBig = tmpObject->id;
					}

				}
				hoverTime = ofGetElapsedTimeMillis();
				lastHoverId = -1;
			}
		} else {
			lastHoverId = tmpObject->id;
			hoverTime = ofGetElapsedTimeMillis();
		}
	}
}



