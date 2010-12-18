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
	artistFont.loadFont("SpartanLTStd-HeavyClass.otf",16);
	themeFont.loadFont("SpartanLTStd-BookClass.otf",12);
	tagsFont.loadFont("SpartanLTStd-HeavyClass.otf",12);
	uploadFont.loadFont("SpartanLTStd-BookClass.otf",10);
	isPlaying = 0;
	cout<<"finished setup"<<endl;
}

void VisibleObjectManager::makeThemes(){
	TextObject *tmpTheme = new TextObject("THE BODY POLITIC");
	tmpTheme->isLeft = 1;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(306,THEME_PLUS);
	themeObjects.push_back(tmpTheme);
	tmpTheme = new TextObject("MEDIA");
	tmpTheme->isLeft = 1;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(610,THEME_PLUS);
	themeObjects.push_back(tmpTheme);
	tmpTheme = new TextObject("IDENTITY");
	tmpTheme->isLeft = 1;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(800,THEME_PLUS);
	themeObjects.push_back(tmpTheme);
	tmpTheme = new TextObject("CONSCIOUSNESS RAISING");
	tmpTheme->isLeft = 0;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(60,THEME_PLUS);
	themeObjects.push_back(tmpTheme);
	tmpTheme = new TextObject("ACTIVISM & SOCIAL PROTEST");
	tmpTheme->isLeft = 0;
	tmpTheme->state = STATE_REST;
	tmpTheme->setPos(390,THEME_PLUS);
	themeObjects.push_back(tmpTheme);
	
}

void VisibleObjectManager::makeDates(){
	TextObject *tmpDate = new TextObject("1950");
	tmpDate->isLeft = 1;
	tmpDate->state = STATE_REST;
	tmpDate->setPos(306,ofGetHeight() - DATE_MINUS);
	tmpDate->setFontSize(DATE_SIZE);
	dateObjects.push_back(tmpDate);
	tmpDate = new TextObject("1960");
	tmpDate->isLeft = 1;
	tmpDate->state = STATE_REST;
	tmpDate->setPos(516,ofGetHeight() - DATE_MINUS);
	tmpDate->setFontSize(DATE_SIZE);
	dateObjects.push_back(tmpDate);
	tmpDate = new TextObject("1970");
	tmpDate->isLeft = 1;
	tmpDate->state = STATE_REST;
	tmpDate->setPos(736,ofGetHeight() - DATE_MINUS);
	tmpDate->setFontSize(DATE_SIZE);
	dateObjects.push_back(tmpDate);
	tmpDate = new TextObject("1980");
	tmpDate->isLeft = 0;
	tmpDate->state = STATE_REST;
	tmpDate->setPos(156,ofGetHeight() - DATE_MINUS);
	tmpDate->setFontSize(DATE_SIZE);
	dateObjects.push_back(tmpDate);
	tmpDate = new TextObject("1990");
	tmpDate->isLeft = 0;
	tmpDate->state = STATE_REST;
	tmpDate->setPos(396,ofGetHeight() - DATE_MINUS);
	tmpDate->setFontSize(DATE_SIZE);
	dateObjects.push_back(tmpDate);
	tmpDate = new TextObject("2000");
	tmpDate->isLeft = 0;
	tmpDate->state = STATE_REST;
	tmpDate->setPos(606,ofGetHeight() - DATE_MINUS);
	tmpDate->setFontSize(DATE_SIZE);
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
			tmpVideo->setPos(256 * (i / 3) + 256, 190 * (i % 3) + 100);
		} else {
			tmpVideo->isLeft = 0;
			tmpVideo->setPos(256 * ((i - 9) / 3) + 20, 190 * (i  % 3) + 100);
		}
		this->addObject(tmpVideo);
	}
	
}

void VisibleObjectManager::addObject(VisibleObject *_vo){
	_vo->resizeByWidth(250.0);
	_vo->id = idAssign++;
	videoObjects.push_back(_vo);
}

void VisibleObjectManager::update(){
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
	if(isPlaying){
		ofEnableAlphaBlending();
		int topX = ofGetWidth()/2 - tester.getSizeX()/2;
		int topY = ofGetHeight()/2 -  tester.getSizeY()/2;
		ofFill();
		ofSetColor(0, 0, 0, 127);
		ofRect(0, 0, ofGetWidth(), ofGetHeight());
		ofSetColor(255, 255, 255,255);
		tester.draw();
		artistFont.drawString("LAURIE ANDERSON 1979",topX, topY - 50);
		themeFont.drawString("CONSCIOUSNESS RAISING",topX, topY - 20);
		tagsFont.drawString("TAGS: anderson, music, new york",topX, ofGetHeight()/2 + topY + 90);
		uploadFont.drawString("Artwork added on 12.10.2010 at 8:30PM from Brooklyn, NY by lynn",topX, ofGetHeight()/2 + topY + 100);
		ofDisableAlphaBlending();
	}
	//vector<VisibleObject*>::iterator vi;
//	int haveDrawing = 0; 
//	VisibleObject* playing;
//	for(vi = videoObjects.begin(); vi < videoObjects.end(); vi++){
//		if((*vi)->id != currentlyPlaying){
//			(*vi)->draw();
//		} else {
//			haveDrawing = 1;
//			playing = (*vi);
//		}
//	}
//	if(haveDrawing)
//		playing->draw();
}

void VisibleObjectManager::drawThemes(){
	vector<VisibleObject*>::iterator ti;
	for(ti = themeObjects.begin(); ti < themeObjects.end(); ti++){
		if((*ti)->isLeft){
			ofEnableAlphaBlending();
			ofPushMatrix();
			ofSetColor(255, 255, 255, 225);
			ofTranslate(ofGetWidth()/2, 0, -600);
			ofRotateY(-45.0);
			ofRotateY(90);
			ofTranslate(-ofGetWidth(), 0, 0);
			//ofRotateY(-180);
			(*ti)->draw();
			ofPopMatrix();
			ofDisableAlphaBlending();
		} else {
			ofEnableAlphaBlending();
			ofPushMatrix();
			ofSetColor(255, 255, 255, 225);
			ofTranslate(ofGetWidth()/2, 0, -600);
			ofRotateY(-45.0);
			(*ti)->draw();
			ofPopMatrix();
			ofDisableAlphaBlending();
		}
	}
}

void VisibleObjectManager::drawDates(){
	vector<VisibleObject*>::iterator ti;
	for(ti = dateObjects.begin(); ti < dateObjects.end(); ti++){
		if((*ti)->isLeft){
			ofEnableAlphaBlending();
			ofPushMatrix();
			ofSetColor(255, 255, 255, 225);
			ofTranslate(ofGetWidth()/2, 0, -600);
			ofRotateY(-45.0);
			ofRotateY(90);
			ofTranslate(-ofGetWidth(), 0, 0);
			//ofRotateY(-180);
			(*ti)->draw();
			ofPopMatrix();
			ofDisableAlphaBlending();
		} else {
			ofEnableAlphaBlending();
			ofPushMatrix();
			ofSetColor(255, 255, 255, 225);
			ofTranslate(ofGetWidth()/2, 0, -600);
			ofRotateY(-45.0);
			(*ti)->draw();
			ofPopMatrix();
			ofDisableAlphaBlending();
		}
	}
}
	
void VisibleObjectManager::drawVideos(){
	vector<VisibleObject*>::iterator ti;
	for(ti = videoObjects.begin(); ti < videoObjects.end(); ti++){
		if(themes[(*ti)->theme] == 1){
			if((*ti)->isLeft){
				ofEnableAlphaBlending();
				ofPushMatrix();
				ofSetColor(255, 255, 255, 255);
				ofTranslate(ofGetWidth()/2, 0, -600);
				ofRotateY(-45.0);
				ofRotateY(90);
				ofTranslate(-ofGetWidth(), 0, 0);
				//ofRotateY(-180);
				(*ti)->draw();
				ofPopMatrix();
				ofDisableAlphaBlending();
			} else {
				ofEnableAlphaBlending();
				ofPushMatrix();
				ofSetColor(255, 255, 255, 255);
				ofTranslate(ofGetWidth()/2, 0, -600);
				ofRotateY(-45.0);
				(*ti)->draw();
				ofPopMatrix();
				ofDisableAlphaBlending();
			}
		}
	}
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
	VisibleObject* tmpObject;
	VisibleObject* playObject;
	vector<VisibleObject*>::iterator vi;
	int count = 0;
	int remove =  -1;
	int playing = -1;
	for(vi = videoObjects.begin(); vi < videoObjects.end(); vi++){
		if((*vi)->isInside(_x,_y)){
			tmpObject = (*vi);
			remove = count;
		}
		//if((*vi)->id == currentlyPlaying){
//			playObject = (*vi);
//			playing = count;
//		}
		count++;
	}
	//if(playing > -1){
//		videoObjects.push_back(playObject);
//		videoObjects.erase(videoObjects.begin() + playing);
//	}
	if(remove > -1){
		videoObjects.erase(videoObjects.begin() + remove);
		videoObjects.push_back(tmpObject);
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



