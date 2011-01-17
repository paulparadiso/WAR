/*
 *  visibleObject.h
 *  WAR
 *
 *  Created by Paul Paradiso on 11/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

/*
 This abstract class is inheried by the various media player
 objects that may or may not be built for WAR.  
*/

#ifndef _VISIBLE_OBJECT_H_
#define _VISIBLE_OBJECT_H_

#include "ofxVectorMath.h"

#define MARGIN 100
#define STATE_REST 0
#define STATE_HOVER 1
#define STATE_PLAY 2

class VisibleObject{
public:
	virtual int isInside(int _x, int _y) = 0;
	virtual int isInsideFlat(int _x, int _y) = 0;
	virtual void draw() = 0;
	virtual void draw(int _id, int _time) = 0;
	virtual void update() = 0;
	virtual int react(int _lvl) = 0;
	virtual void setPos(float _x, float _y) = 0;
	virtual void setPosVel(float _xV, float _yV) = 0;
	virtual void setPosAcc(float _xA, float _yA) = 0;
	virtual void setRot(float _r) = 0;
	virtual void setRotVel(float _rV) = 0;
	virtual void setRotAcc(float _rA) = 0;
	virtual void resizeByHeight(float _h) = 0;
	virtual void resizeByWidth(float _w) = 0;
	virtual void adjustPosition() = 0;
	
	virtual void updateShape() = 0;
	virtual void drawShape() = 0;
	virtual void stopVideo() = 0;
	virtual void resetState() = 0;
	virtual void updateActualShape() = 0;
	virtual void drawFront() = 0;
	
	VisibleObject(){
	};
	
	ofxVec2f pos;
	ofxVec3f abPos;
	
	vector<ofxVec2f*>shape;
	vector<ofxVec3f*>abShape;
	
	ofxVec2f posVel;
	ofxVec2f posAcc;
	float rot;
	float rotVel;
	float rotAcc;
	ofxVec2f size;
	
	int id;
	
	int state;
	int isLeft;
	int theme;
	
	int isHovering;
	
	int isPlaying;
	
	int themesInt;
	
	int trans_x;
	int trans_z;
	
	int shouldPlay;
	
	virtual void testShape(int _which) = 0;
	
	ofxVec4f box;

};

#endif
