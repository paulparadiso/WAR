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
	this->makeThemes();
	cout<<"finished themes"<<endl;
	this->makeDates();
	cout<<"finished dated"<<endl;
	this->makeVideos();
	cout<<"finished videos"<<endl;
	for(int i = 0; i < 5; i++){
		themes[i] = 0;
	}
	isPlaying = 0;
	rightClose.setup("nav/close_button.png");
	leftClose.setup("nav/close_button.png");
	rightClose.id = RIGHT_CLOSE_ID;
	leftClose.id = LEFT_CLOSE_ID;
	cout<<"finished setup"<<endl;
	playingLeft = playingRight = 0;
}

void VisibleObjectManager::makeThemes(){
	//The Body Politic
	NavObject *tmpTheme = new NavObject("nav/body_politic.png");
	tmpTheme->isLeft = 1;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(506,THEME_PLUS);
	tmpTheme->id = -5;
	themeObjects.push_back(tmpTheme);
	//Consciousness Raising
	tmpTheme = new NavObject("nav/consciousness.png");
	tmpTheme->isLeft = 1;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(910,THEME_PLUS);
	tmpTheme->id = -6;
	themeObjects.push_back(tmpTheme);
	//Identity
	tmpTheme = new NavObject("nav/identity.png");
	tmpTheme->isLeft = 0;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(50,THEME_PLUS);
	tmpTheme->id = -7;
	themeObjects.push_back(tmpTheme);
	//Media
	tmpTheme = new NavObject("nav/media.png");
	tmpTheme->isLeft = 0;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(215,THEME_PLUS);
	tmpTheme->id = -8;
	themeObjects.push_back(tmpTheme);
	//Activism and Social Protest
	tmpTheme = new NavObject("nav/activism.png");
	tmpTheme->isLeft = 0;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(350,THEME_PLUS);
	tmpTheme->id = -9;
	themeObjects.push_back(tmpTheme);
	
}

void VisibleObjectManager::makeDates(){
	NavObject *tmpDate = new NavObject("1950s.png");
	tmpDate->isLeft = 1;
	tmpDate->state = STATE_REST;
	tmpDate->setPos(306,ofGetHeight() - DATE_MINUS);
	dateObjects.push_back(tmpDate);
	tmpDate = new NavObject("1960s.png");
	tmpDate->isLeft = 1;
	tmpDate->state = STATE_REST;
	tmpDate->setPos(516,ofGetHeight() - DATE_MINUS);
	dateObjects.push_back(tmpDate);
	tmpDate = new NavObject("1970s.png");
	tmpDate->isLeft = 1;
	tmpDate->state = STATE_REST;
	tmpDate->setPos(736,ofGetHeight() - DATE_MINUS);
	dateObjects.push_back(tmpDate);
	tmpDate = new NavObject("1980s.png");
	tmpDate->isLeft = 0;
	tmpDate->state = STATE_REST;
	tmpDate->setPos(156,ofGetHeight() - DATE_MINUS);
	dateObjects.push_back(tmpDate);
	tmpDate = new NavObject("1990s.png");
	tmpDate->isLeft = 0;
	tmpDate->state = STATE_REST;
	tmpDate->setPos(396,ofGetHeight() - DATE_MINUS);
	dateObjects.push_back(tmpDate);
	tmpDate = new NavObject("2000s.png");
	tmpDate->isLeft = 0;
	tmpDate->state = STATE_REST;
	tmpDate->setPos(606,ofGetHeight() - DATE_MINUS);
	dateObjects.push_back(tmpDate);
}

void VisibleObjectManager::makeVideos(){
	int numVideos = dir.listDir("video");
	int theme = 0;
	for(int i = 0; i < 18; i++){
		VideoObject *tmpVideo = new VideoObject(dir.getPath(i));
		if(i == 5){
			tester.setup(dir.getPath(i));
			tester.resizeByWidth(600);
			tester.setPos((ofGetWidth()/2) - (tester.getSizeX() / 2), (ofGetHeight() / 2) - (tester.getSizeY() /2));
		}
		tmpVideo->resizeByWidth(VIDEO_WIDTH);
		tmpVideo->theme = theme;
		theme = (theme + 1) % 5;
		tmpVideo->id = i;
		if(i < 9){
			tmpVideo->isLeft = 1;
			tmpVideo->setPos(256 * (i / 3) + 556, 190 * (i % 3) + 100);
		} else {
			tmpVideo->isLeft = 0;
			tmpVideo->setPos(256 * ((i - 9) / 3) + 20, 190 * (i  % 3) + 100);
		}
		this->addObject(tmpVideo);
	}
	
}

void VisibleObjectManager::addObject(VisibleObject *_vo){
	_vo->resizeByWidth(250.0);
	_vo->addFp(&fp);
	_vo->id = idAssign++;
	videoObjects.push_back(_vo);
}

void VisibleObjectManager::update(){
	//FrontPlayer must be updated first.
	fp.update();
	vector<VisibleObject*>::iterator vi;
	for(vi = videoObjects.begin(); vi < videoObjects.end(); vi++){
		(*vi)->update();
	}
}

void VisibleObjectManager::update(int _x, int _y){
	this->update();
	this->checkInsides(_x,_y);
}

void VisibleObjectManager::toggleTheme(int _t){
	themes[_t] = !themes[_t];
}

void VisibleObjectManager::togglePlay(){
	isPlaying = !isPlaying;
	if(isPlaying){
		tester.vp.play();
		tester.vp.setPosition(0.0);
	} else {
		tester.vp.stop();
	}	
}

void VisibleObjectManager::draw(){
	this->drawThemes();
	this->drawDates();
	this->drawVideos();
	fp.draw();
	if(fp.haveRight)
		this->drawCloseBoxes(0);
	if(fp.haveLeft)
		this->drawCloseBoxes(1);
}

void VisibleObjectManager::drawCloseBoxes(int _side){
	if(!_side){
		ofxVec4f box = fp.getBoxSize(0);
		rightClose.setPos(box.x, box.y);
		rightClose.drawFlat();
	} else {
		ofxVec4f box = fp.getBoxSize(1);
		leftClose.setPos(box.z - 40, box.y);
		leftClose.drawFlat();
	}
}

void VisibleObjectManager::drawThemes(){
	//cout<<"running draw themes loop"<<endl;
	vector<VisibleObject*>::iterator ti;
	for(ti = themeObjects.begin(); ti < themeObjects.end(); ti++){
		(*ti)->draw();
		//cout<<"drawing theme"<<endl;
	}
}

void VisibleObjectManager::drawDates(){
	vector<VisibleObject*>::iterator ti;
	for(ti = dateObjects.begin(); ti < dateObjects.end(); ti++){
		(*ti)->draw();
	}
}
	
void VisibleObjectManager::drawVideos(){
	int count = 0;
	vector<VisibleObject*>::iterator ti;
	for(ti = videoObjects.begin(); ti < videoObjects.end(); ti++){
		if(themes[(*ti)->theme] == 1){
			(*ti)->draw();
		}
		if((*ti)->isPlaying)
			//cout<<count<<" is playing"<<endl;
		count++;
	}	
}

void VisibleObjectManager::stopVideo(int _side){
	fp.stop(_side);
}

void VisibleObjectManager::randomPositions(){
	vector<VisibleObject*>::iterator vi;
	for(vi = videoObjects.begin(); vi < videoObjects.end(); vi++){
		(*vi)->setPos(ofRandom(0,ofGetWidth()),ofRandom(0, ofGetHeight()));
		(*vi)->adjustPosition();
	}
}

void VisibleObjectManager::resetById(int _id){
	vector<VisibleObject*>::iterator vi;
	for(vi = videoObjects.begin(); vi < videoObjects.end(); vi++){
		if((*vi)->id == _id){
			(*vi)->react(0);
		}
	}
}

void VisibleObjectManager::checkInsides(int _x, int _y){
	int leave = 0;
	if(fp.haveRight){
		if(fp.isInside(_x,_y)){
			ofxVec4f box = fp.getBoxSize(0);
			rightClose.setPos(box.x, box.y);
			if(rightClose.isInsideFlat(_x,_y)){
				if(rightClose.id == lastHoverId){
					if(ofGetElapsedTimeMillis() - hoverTime > HOVER_CLICK_TIME){
						rightClose.react(1);
						hoverTime = ofGetElapsedTimeMillis();
						lastHoverId = -1;
					}
				} else {
					lastHoverId = rightClose.id;
					hoverTime = ofGetElapsedTimeMillis();
				}
			} 
			if(rightClose.isPlaying){
				fp.stop(0);
				rightClose.react(0);
			}
			leave = 1;
		}
	}
	if(fp.haveLeft){
		if(fp.isInside(_x,_y)){
			ofxVec4f box = fp.getBoxSize(1);
			leftClose.setPos(box.z - 40, box.y);
			if(leftClose.isInsideFlat(_x,_y)){
				if(leftClose.id == lastHoverId){
					if(ofGetElapsedTimeMillis() - hoverTime > HOVER_CLICK_TIME){
						int result = leftClose.react(1);
						hoverTime = ofGetElapsedTimeMillis();
						lastHoverId = -1;
					}
				} else {
					lastHoverId = leftClose.id;
					hoverTime = ofGetElapsedTimeMillis();
				}
			}
			if(leftClose.isPlaying){
				fp.stop(1);
				leftClose.react(0);
			}
			leave = 1;
		}
	}
	if(leave){
		return;
	}
	VisibleObject* tmpObject;
	VisibleObject* playObject;
	vector<VisibleObject*>::iterator vi;
	int count = 0;
	int remove =  -1;
	int playing = -1;
	for(vi = videoObjects.begin(); vi < videoObjects.end(); vi++){
		if(themes[(*vi)->theme]==1){
			if((*vi)->isInside(_x,_y)){
				tmpObject = (*vi);
				remove = count;
			}
		}
		count++;
	}
	if(remove > -1){
		if(tmpObject->id == lastHoverId){
			if(ofGetElapsedTimeMillis() - hoverTime > HOVER_CLICK_TIME){
				int result = tmpObject->react(1);
				hoverTime = ofGetElapsedTimeMillis();
				lastHoverId = -1;
			}
		} else {
			lastHoverId = tmpObject->id;
			hoverTime = ofGetElapsedTimeMillis();
		}
	}
	for(vi = themeObjects.begin(); vi < themeObjects.end(); vi++){
		if((*vi)->isInside(_x,_y)){
			if((*vi)->id == lastHoverId){
				if(ofGetElapsedTimeMillis() - hoverTime > HOVER_CLICK_TIME){
					int result = (*vi)->react(1);
					hoverTime = ofGetElapsedTimeMillis();
					lastHoverId = -1;
				}
			} else {
				lastHoverId = (*vi)->id;
				hoverTime = ofGetElapsedTimeMillis();
			}
			if((*vi)->isPlaying){
				themes[abs((*vi)->id) - 5] = 1;
			} else {
				themes[abs((*vi)->id) - 5] = 0;
			}
		}
	}
}

void VisibleObjectManager::updateShapes(int _which){
	vector<VisibleObject*>::iterator vi;
	for(vi = videoObjects.begin(); vi < videoObjects.end(); vi++){
		if((*vi)->isLeft){
			(*vi)->testShape(_which);
		}
	}
}

