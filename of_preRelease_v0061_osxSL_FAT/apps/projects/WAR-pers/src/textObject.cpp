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
	rot = -45;
}

int TextObject::isInside(int _x, int _y){
	
}

void TextObject::draw(){
	font.drawString(text,pos.x,pos.y);
}

void TextObject::setFontSize(int _s){
	font.loadFont(FONT,_s);
}

void TextObject::update(){
}

int TextObject::react(int _lvl){
}

void TextObject::setPos(float _x, float _y){
	pos.set(_x,_y);
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

