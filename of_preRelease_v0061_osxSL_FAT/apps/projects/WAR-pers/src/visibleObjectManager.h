/*
 *  visibleObjectManager.h
 *  WAR
 *
 *  Created by Paul Paradiso on 11/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VISIBLE_OBJECT_MANAGER_H_
#define _VISIBLE_OBJECT_MANAGER_H_

#include "visibleObject.h"
#include "navObject.h"
#include "videoObject.h"
#include "ofxDirList.h"
#include "ofMain.h"
#include <vector>

#define HOVER_CLICK_TIME 1000
#define DATE_SIZE 28
#define DATE_MINUS 50
#define THEME_PLUS 50
#define VIDEO_WIDTH 90
#define RIGHT_CLOSE_ID -20
#define LEFT_CLOSE_ID -21

using namespace std;

class VisibleObjectManager{
public:
	void update();
	void update(int _x, int _y);
	void draw();
	void drawThemes();
	void drawDates();
	void drawVideos();
	VisibleObjectManager();
	void addObject(VisibleObject *_vo);
	void checkInsides(int _x, int _y);
	void randomPositions();
	void resetById(int _id);
	void toggleTheme(int _t);
	void togglePlay();
	void stopVideo(int _side);
	void drawCloseBoxes(int _side);
	
	FrontPlayer fp;
	void updateShapes(int _which);
	//void addObject(VisibleObject _vo);
private:
	vector<VisibleObject*>videoObjects;
	vector<VisibleObject*>themeObjects;
	vector<VisibleObject*>dateObjects;
	int themes[5];
	int counting;
	int hoverTime;
	int idAssign;
	int currHoverId;
	int lastHoverId;
	int lastReactId;
	int currentlyPlaying;
	int currentlyBig;
	ofxDirList dir;
	int isPlaying;
	VideoObject tester;
	
	void makeThemes();
	void makeDates();
	void makeVideos();
	
	int playingLeft, playingRight;
	
	NavObject rightClose, leftClose;
	
	
};

#endif