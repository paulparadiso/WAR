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
#include "panningVideoPlayer.h"
#include "defines.h"

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
	int isInsideFlat(int _x, int _y);
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
	
	//void addFp(FrontPlayer *_fp);
	
	PanningVideoPlayer vp;
	PanningVideoPlayer vpFront;
	int getSizeX();
	int getSizeY();
	
	int hoverTime;
	
	void testShape(int _which);
	
	string getThemeString();
	string getUploadString();
	string getLocationString();
	string getFrontTitle();
	void drawFront();
	
	vector<string>themes;
	int textureSet;
	
	int fontsLoaded;
	
	int dateInt;
	
	int numPlays, timeStarted;
	
	ofTexture drawTexture;
	
	string artist, title, date, description, uploader, uploadDate, uploaderLocale;
	
private:
	ofxVec2f drawSize;
	ofxVec2f playSize;
	ofxVec2f restPos;
	ofxVec2f restSize;
	float xAdd, yAdd;
	int stopTime;
	ofTrueTypeFont artFont;
	int oldWidth;
	
	string media;
	
	ofTrueTypeFont artistFont;
	ofTrueTypeFont themeFont;
	ofTrueTypeFont tagsFont;
	ofTrueTypeFont uploadFont;
	
	int alpha;
	int trigger;
};

#endif