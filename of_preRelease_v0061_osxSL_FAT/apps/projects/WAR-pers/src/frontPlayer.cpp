/*
 *  frontPlayer.cpp
 *  WAR
 *
 *  Created by Paul Paradiso on 1/3/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "frontPlayer.h"


FrontPlayer::FrontPlayer(){
	this->setup();
}

void FrontPlayer::setup(){
	artistFont.loadFont("fonts/SpartanLTStd-HeavyClass.otf",14);
	themeFont.loadFont("fonts/SpartanLTStd-BookClass.otf",10);
	tagsFont.loadFont("fonts/SpartanLTStd-HeavyClass.otf",10);
	uploadFont.loadFont("fonts/SpartanLTStd-BookClass.otf",8);
}

void FrontPlayer::update(){
	if(haveRight){
		vpRight->idleMovie();
		if(vpRight->getIsMovieDone() || vpRight->getSpeed() == 0){
			cout<<"Movie Finished"<<endl;
			haveRight = 0;
		}
	}
	if(haveLeft){
		vpLeft->idleMovie();
		if(vpLeft->getIsMovieDone() || vpLeft->getSpeed() == 0){
			haveLeft = 0;
		}
	}
}

void FrontPlayer::draw(){
	ofEnableAlphaBlending();
	if(haveRight){
		int topX = (ofGetWidth() / 3 * 2) + 25;
		int topY = (ofGetHeight()/2) - (playSize->y/2);
		ofFill();
		ofSetColor(0, 0, 0, 128);
		rightBox.set(ofGetWidth()/3 * 2,topY - 100, ofGetWidth(), (ofGetHeight()/2) + 160);
		ofRect(rightBox.x, rightBox.y, rightBox.z, rightBox.w);
		ofSetColor(255, 255, 255, 255);
		vpRight->draw((ofGetWidth()/3 * 2 + 25), topY, playSize->x, playSize->y);
		artistFont.drawString("JANE DOE 1979",topX, topY - 50);
		themeFont.drawString("CONSCIOUSNESS RAISING",topX, topY - 20);
		tagsFont.drawString("TAGS: war, art, new york",topX, ofGetHeight()/2 + topY + 10);
		uploadFont.drawString("Artwork added on 12.10.2010 at 8:30PM from Brooklyn, NY by lynn",topX, ofGetHeight()/2 + topY + 30);
	} 
	if(haveLeft){
		int topX = 25;
		int topY = ofGetHeight() / 2 - playSize->y / 2;
		ofFill();
		ofSetColor(0, 0, 0, 128);
		leftBox.set(0,topY - 100, ofGetWidth()/3, (ofGetHeight()/2) + 160);
		ofRect(leftBox.x, leftBox.y, leftBox.z, leftBox.w);
		ofSetColor(255, 255, 255,255);
		vpLeft->draw(topX, topY, playSize->x, playSize->y);
		artistFont.drawString("JANE DOE 1985",topX, topY - 50);
		themeFont.drawString("IDENTITY",topX, topY - 20);
		tagsFont.drawString("TAGS: music, new york, performance",topX, ofGetHeight()/2 + topY + 10);
		uploadFont.drawString("Artwork added on 12.10.2010 at 8:30PM from Brooklyn, NY by lynn",topX, ofGetHeight()/2 + topY + 30);
	}
	ofDisableAlphaBlending();
}

void FrontPlayer::stop(int _player){
	if(_player == 0 || _player == 2){
		vpRight->setPosition(0.2);
		vpRight->stop();
		haveRight = 0;
	}
	if(_player == 1 || _player == 2){
		vpLeft->setPosition(0.2);
		vpLeft->stop();
		haveLeft = 0;
	}
}

ofxVec4f FrontPlayer::getBoxSize(int _side){
	if(!_side){
		return rightBox;
	} else {
		return leftBox;
	}
}

int FrontPlayer::loadMovie(string _movie, int _player){
}