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
#include "frontPlayer.h"
#include "panningVideoPlayer.h"

#define GROW_TIME 500.0
#define DEFAULT_ALPHA 225
#define HOVER_CLICK_TIME 2000

class VideoObject : public VisibleObject{
public:
	
	VideoObject();
	VideoObject(string _path);
	void setup(string _path);
	void draw();
	void draw(int _id, int _time);
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
	void updateShape();
	void updateActualShape();
	void drawShape();
	void resetState();
	void stopVideo();
	
	void addFp(FrontPlayer *_fp);
	
	PanningVideoPlayer vp;
	int getSizeX();
	int getSizeY();
	
	int hoverTime;
	
	void testShape(int _which);
	
private:
	ofxVec2f drawSize;
	ofxVec2f playSize;
	ofxVec2f restPos;
	ofxVec2f restSize;
	float xAdd, yAdd;
	int stopTime;
	ofTrueTypeFont artFont;
	int oldWidth;
	
	FrontPlayer *fp;
	
	string media;
	
	ofTrueTypeFont artistFont;
	ofTrueTypeFont themeFont;
	ofTrueTypeFont tagsFont;
	ofTrueTypeFont uploadFont;
	
	int alpha;
	int trigger;
};

#endif