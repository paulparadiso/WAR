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
	//cout<<"finished videos"<<endl;
	for(int i = 0; i < 5; i++){
		themes[i] = 0;
	}
	for(int i = 0; i < 6; i++){
		dates[i] = 0;
	}
	this->makeThemes();
	cout<<"finished themes"<<endl;
	this->makeDates();
	cout<<"finished dates"<<endl;
	this->makeVideos();
	isPlaying = 0;
	rightClose.setup("nav/close_button.png");
	leftClose.setup("nav/close_button.png");
	rightClose.id = RIGHT_CLOSE_ID;
	leftClose.id = LEFT_CLOSE_ID;
	cout<<"finished setup"<<endl;
	playingLeft = playingRight = 0;
	hadInside = 0;
	loader.setup();
	rightOn = 0;
	leftOn = 0;
	haveActives[0] = 0;
	haveActives[1] = 0;
	for(int i = 0; i < MAX_VIDEOS; i++){
		videos[i] = loader.videos[i];
	}
	hoverTextBoxes[0].init("fonts/SpartanLTStd-BookClass.otf",12);
	hoverTextBoxes[0].wrapTextX(ofGetWidth()/2 - ((ofGetWidth()/3) - 25));
	hoverTextBoxes[0].setText("This is a large block of text that will appear on the right hand side of the screen when someone hovers over an item on that side of the screen.\n");
	hoverTextBoxes[1].init("fonts/SpartanLTStd-BookClass.otf",12);
	hoverTextBoxes[1].wrapTextX(ofGetWidth()/2 - ((ofGetWidth()/3) - 25));
	hoverTextBoxes[1].setText("This is a large block of text that will appear on the right hand side of the screen when someone hovers over an item on that side of the screen.\n");
	haveMessage[0] = 0;
	haveMessage[1] = 0;
}

void VisibleObjectManager::makeThemes(){
	//The Body Politic
	NavObject *tmpTheme = new NavObject("nav/body_politic.png");
	tmpTheme->isLeft = 1;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(25,THEME_PLUS);
	tmpTheme->id = -5;
	themeObjects.push_back(tmpTheme);
	//Consciousness Raising
	tmpTheme = new NavObject("nav/consciousness.png");
	tmpTheme->isLeft = 1;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(330,THEME_PLUS);
	tmpTheme->id = -6;
	themeObjects.push_back(tmpTheme);
	//Identity
	tmpTheme = new NavObject("nav/identity.png");
	tmpTheme->isLeft = 0;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(726,THEME_PLUS);
	tmpTheme->id = -7;
	themeObjects.push_back(tmpTheme);
	//Media
	tmpTheme = new NavObject("nav/media.png");
	tmpTheme->isLeft = 0;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(926,THEME_PLUS);
	tmpTheme->id = -8;
	themeObjects.push_back(tmpTheme);
	//Activism and Social Protest
	tmpTheme = new NavObject("nav/social_protest.png");
	tmpTheme->isLeft = 0;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(1085,THEME_PLUS);
	tmpTheme->id = -9;
	themeObjects.push_back(tmpTheme);
	
}

void VisibleObjectManager::makeDates(){
	NavObject *tmpDate = new NavObject("nav/1950s.png");
	tmpDate->isLeft = 1;
	tmpDate->state = DEFAULT_DATE_STATE;
	tmpDate->setPos(124,ofGetHeight() - DATE_MINUS);
	tmpDate->id = -10;
	tmpDate->resetState();
	if(tmpDate->isPlaying)
		dates[abs(tmpDate->id) - 10] = 1;
	dateObjects.push_back(tmpDate);
	tmpDate = new NavObject("nav/1960s.png");
	tmpDate->isLeft = 1;
	tmpDate->setPos(331,ofGetHeight() - DATE_MINUS);
	tmpDate->id = -11;
	tmpDate->state = DEFAULT_DATE_STATE;
	tmpDate->resetState();
	if(tmpDate->isPlaying)
		dates[abs(tmpDate->id) - 10] = 1;
	dateObjects.push_back(tmpDate);
	tmpDate = new NavObject("nav/1970s.png");
	tmpDate->isLeft = 1;
	tmpDate->setPos(538,ofGetHeight() - DATE_MINUS);
	tmpDate->id = -12;
	tmpDate->state = DEFAULT_DATE_STATE;
	tmpDate->resetState();
	if(tmpDate->isPlaying)
		dates[abs(tmpDate->id) - 10] = 1;
	dateObjects.push_back(tmpDate);
	tmpDate = new NavObject("nav/1980s.png");
	tmpDate->isLeft = 0;
	tmpDate->setPos(745,ofGetHeight() - DATE_MINUS);
	tmpDate->id = -13;
	tmpDate->state = DEFAULT_DATE_STATE;
	tmpDate->resetState();
	if(tmpDate->isPlaying)
		dates[abs(tmpDate->id) - 10] = 1;
	dateObjects.push_back(tmpDate);
	tmpDate = new NavObject("nav/1990s.png");
	tmpDate->isLeft = 0;
	tmpDate->setPos(952,ofGetHeight() - DATE_MINUS);
	tmpDate->id = -14;
	tmpDate->state = DEFAULT_DATE_STATE;
	tmpDate->resetState();
	if(tmpDate->isPlaying)
		dates[abs(tmpDate->id) - 10] = 1;
	dateObjects.push_back(tmpDate);
	tmpDate = new NavObject("nav/2000s.png");
	tmpDate->isLeft = 0;
	tmpDate->setPos(1159,ofGetHeight() - DATE_MINUS);
	tmpDate->id = -15;
	tmpDate->state = DEFAULT_DATE_STATE;
	tmpDate->resetState();
	if(tmpDate->isPlaying)
		dates[abs(tmpDate->id) - 10] = 1;
	dateObjects.push_back(tmpDate);
}

void VisibleObjectManager::makeVideos(){
	//int numVideos = dir.listDir("video");
	//int theme = 0;
	//for(int i = 0; i < MAX_VIDEOS; i++){
//		VideoObject *tmpVideo = new VideoObject(dir.getPath(i));
//		tmpVideo->resizeByWidth(VIDEO_WIDTH);
//		tmpVideo->theme = theme;
//		theme = (theme + 1) % 5;
//		tmpVideo->id = i;
//		if(i < 9){
//			tmpVideo->isLeft = 1;
//			tmpVideo->setPos(220 * (i / 3) + 700, 210 * (i % 3) + 100);
//		} else {
//			tmpVideo->isLeft = 0;
//			tmpVideo->setPos(220* ((i - 9) / 3) + 20, 210 * (i  % 3) + 110);
//		}
//		this->addObject(tmpVideo);
//	}
	
}

void VisibleObjectManager::addObject(VisibleObject *_vo){
	_vo->resizeByWidth(250.0);
	_vo->id = idAssign++;
	videoObjects.push_back(_vo);
}

void VisibleObjectManager::update(){
	//FrontPlayer must be updated first.
	vector<VisibleObject*>::iterator vi;
	for(vi = videoObjects.begin(); vi < videoObjects.end(); vi++){
		(*vi)->update();
	}
}

void VisibleObjectManager::update(int _x, int _y){
	this->update();
	this->checkInsides(_x,_y);
	loader.update();
	if(!loader.isThreadRunning()) {
		if(loader.haveNewVideo){
			if(!haveActives[0] || !haveActives[1]){
				videos[loader.nextVideo].drawTexture.clear();
				videos[loader.nextVideo] = loader.videos[loader.nextVideo];
				loader.nextVideo = (loader.nextVideo + 1) % MAX_VIDEOS; 
				loader.haveNewVideo = 0;
		
			}
		}
	}
}

void VisibleObjectManager::toggleTheme(int _t){
	themes[_t] = !themes[_t];
}

void VisibleObjectManager::togglePlay(){
	isPlaying = !isPlaying;
}

void VisibleObjectManager::draw(){
	this->drawThemes(0);
	this->drawDates();
	this->drawVideos(0);
}

void VisibleObjectManager::drawPlayer(){
	for(int i = 0; i < MAX_VIDEOS; i++){
		if(videos[i].state == STATE_PLAY){
			haveActives[videos[i].isLeft] = 1;
			loader.haveActives[videos[i].isLeft] = videos[i].id;
			actives[videos[i].isLeft] = &videos[i];
			videos[i].drawFront();
			drawCloseBoxes(videos[i].isLeft,&videos[i]);
		}
	}
}

void VisibleObjectManager::drawCloseBoxes(int _side,VisibleObject *_vo){
	if(!_side){
		rightClose.setPos(_vo->box.x, _vo->box.y);
		rightClose.drawFlat();
	} else {
		ofxVec4f box = _vo->box;
		leftClose.setPos(_vo->box.x + _vo->box.z - 40, _vo->box.y);
		leftClose.drawFlat();
	}
}

void VisibleObjectManager::drawThemes(int _which){
	//cout<<"running draw themes loop"<<endl;
	vector<VisibleObject*>::iterator ti;
	for(ti = themeObjects.begin(); ti < themeObjects.end(); ti++){
		if((*ti)->isLeft == _which){
			(*ti)->draw(lastHoverId,hoverTime);
		}
	}
}

void VisibleObjectManager::drawThemes2D(){
	vector<VisibleObject*>::iterator ti;
	for(ti = themeObjects.begin(); ti < themeObjects.end(); ti++){
		(*ti)->draw(lastHoverId,hoverTime);
	}
}

void VisibleObjectManager::drawDates(){
	vector<VisibleObject*>::iterator ti;
	for(ti = dateObjects.begin(); ti < dateObjects.end(); ti++){
		(*ti)->draw();
	}
}

void VisibleObjectManager::drawDates2D(){
	vector<VisibleObject*>::iterator ti;
	for(ti = dateObjects.begin(); ti < dateObjects.end(); ti++){
		(*ti)->draw(lastHoverId, hoverTime);
	}
}
	
void VisibleObjectManager::drawVideos(int _which){
		for(int i = 0; i < MAX_VIDEOS; i++){
		if(videos[i].isLeft == _which){
			if(themes[videos[i].themesInt] == 1 && dates[videos[i].dateInt] == 1){
				videos[i].draw(lastHoverId,ofGetElapsedTimeMillis() - hoverTime);
			}
		}
	}
}

void VisibleObjectManager::stopVideo(int _side){
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
	
	int changed = 0;
	int leave = 0;
		if(haveActives[0]){
		if(rightClose.isInsideFlat(_x,_y)){
			leave = 1;
			if(rightClose.id == lastHoverId){
				if(ofGetElapsedTimeMillis() - hoverTime > HOVER_CLICK_TIME){
					cout<<"right front calling react."<<endl;
					rightClose.react(1);
					hoverTime = ofGetElapsedTimeMillis();
					lastHoverId = -1;
					actives[0]->react(0);
					loader.numPlays[actives[0]->id]++;
					if(loader.numPlays[actives[0]->id] == MAX_VIDEO_PLAYS){
						actives[0]->shouldPlay = 0;
					}
					//cout << "loader.numPlaye["<<actives[0]->id<<" = "<<loader.numPlays[actives[0]->id]<<endl;
					haveActives[0] = 0;
					loader.haveActives[0] = -1;
				}
			} else {
				lastHoverId = rightClose.id;
				hoverTime = ofGetElapsedTimeMillis();
			}
		}
	}
	if(haveActives[1]){
		if(leftClose.isInsideFlat(_x,_y)){
			leave = 1;
			if(leftClose.id == lastHoverId){
				if(ofGetElapsedTimeMillis() - hoverTime > HOVER_CLICK_TIME){
					cout<<"left front calling react."<<endl;
					leftClose.react(1);
					hoverTime = ofGetElapsedTimeMillis();
					lastHoverId = -1;
					actives[1]->react(0);
					loader.numPlays[actives[1]->id]++;
					if(loader.numPlays[actives[1]->id] == MAX_VIDEO_PLAYS){
						actives[1]->shouldPlay = 0;
					}
					//cout << "loader.numPlaye["<<actives[1]->id<<" = "<<loader.numPlays[actives[1]->id]<<endl;
					loader.haveActives[1] = 0;
					haveActives[1] = 0;
				}
			} else {
				lastHoverId = leftClose.id;
				hoverTime = ofGetElapsedTimeMillis();
			}
		}
	}
	VisibleObject* tmpObject;
	VisibleObject* playObject;
	vector<VisibleObject*>::iterator vi;
	for(vi = themeObjects.begin(); vi < themeObjects.end(); vi++){
		if((*vi)->isInsideFlat(_x,_y)){
			if((*vi)->id == lastHoverId ){
				if(ofGetElapsedTimeMillis() - hoverTime > THEME_HOVER_CLICK_TIME && !hadInside){
					int result = (*vi)->react(1);
					hoverTime = ofGetElapsedTimeMillis();
					lastHoverId = -1;
					hadInside = 1;
				}
			} else {
				lastHoverId = (*vi)->id;
				hoverTime = ofGetElapsedTimeMillis();
				changed = 1;
			}
			if((*vi)->isPlaying){
				themes[abs((*vi)->id) - 5] = 1;
			} else {
				themes[abs((*vi)->id) - 5] = 0;
			}
			leave = 1;
		}
	}
	for(vi = dateObjects.begin(); vi < dateObjects.end(); vi++){
		if((*vi)->isInsideFlat(_x,_y)){
			if((*vi)->id == lastHoverId ){
				if(ofGetElapsedTimeMillis() - hoverTime > THEME_HOVER_CLICK_TIME && !hadInside){
					int result = (*vi)->react(1);
					hoverTime = ofGetElapsedTimeMillis();
					lastHoverId = -1;
					hadInside = 1;
				}
			} else {
				lastHoverId = (*vi)->id;
				hoverTime = ofGetElapsedTimeMillis();
				changed = 1;
			}
			if((*vi)->isPlaying){
				dates[abs((*vi)->id) - 10] = 1;
			} else {
				dates[abs((*vi)->id) - 10] = 0;
			}
			leave = 1;
		}
	}
	if(leave){
		return;
	}
	hadInside = 0;
	int remove =  -1;
	int playing = -1;
	for(int i = 0; i < MAX_VIDEOS; i++){
		if(themes[videos[i].themesInt] == 1){
			if(videos[i].isInside(_x,_y)){
				tmpObject = &videos[i];
				remove = 1;
			}
		}
	}
	if(remove > -1){
		if(tmpObject->id == lastHoverId){
			if(ofGetElapsedTimeMillis() - hoverTime > HOVER_CLICK_TIME){
				cout<<"calling react"<<endl;
				int result = tmpObject->react(1);
				if(result == STATE_PLAY){
					if(haveActives[tmpObject->isLeft]){
						actives[tmpObject->isLeft]->react(0);
						actives[tmpObject->isLeft] = tmpObject;
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

void VisibleObjectManager::updateShapes(int _which){
	vector<VisibleObject*>::iterator vi;
	for(vi = videoObjects.begin(); vi < videoObjects.end(); vi++){
		if((*vi)->isLeft){
			(*vi)->testShape(_which);
		}
	}
}

