/*
 *  videoObject.h
 *  WAR
 *
 *  Created by Paul Paradiso on 11/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VIDEO_OBJECT_H_
#define _VIDEO_OBJECT_H_

#include "visibleObject.h"
#include "ofMain.h"
#include "ofxVectorMath.h"
#include <vector>

#define GROW_TIME 500.0

class PanningVideoPlayer : public ofVideoPlayer{
public:
	void setPan(float _pan){
		SetMovieAudioBalance(moviePtr, _pan, 0);
	}
};

class VideoObject : public VisibleObject{
public:
	
	VideoObject();
	VideoObject(string _path);
	void setup(string _path);
	void draw();
	void update();
	int react(int _lvl);
	int isInside(int _x, int _y);
	void setPos(float _x, float _y);
	void setPosVel(float _xV, float _yV);
	void setPosAcc(float _xA, float _yA);
	void setRot(float _r);
	void setRotVel(float _rV);
	void setRotAcc(float _rA);
	void resizeByHeight(float _h);
	void resizeByWidth(float _w);
	void resizeByPercent(float _p);
	void adjustPosition();
	PanningVideoPlayer vp;
	int getSizeX();
	int getSizeY();
	void updateShape();
	void updateActualShape();
	void drawShape();
	void resetState();
	
private:
	ofxVec2f drawSize;
	ofxVec2f playSize;
	int isPlaying;	
	ofxVec2f restPos;
	ofxVec2f restSize;
	float xAdd, yAdd;
	int stopTime;
	ofTrueTypeFont artFont;
	int oldWidth;
	
	ofTrueTypeFont artistFont;
	ofTrueTypeFont themeFont;
	ofTrueTypeFont tagsFont;
	ofTrueTypeFont uploadFont;
};

#endif