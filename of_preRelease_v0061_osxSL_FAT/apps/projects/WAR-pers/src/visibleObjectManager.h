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
#include "videoLoader.h"
#include "threadedVideoLoader.h"
#include "defines.h"

using namespace std;

class VisibleObjectManager{
public:
	VisibleObjectManager();
	void update();
	void update(int _x, int _y);
	void draw();
	void drawThemes(int _which);
	void drawThemes2D();
	void drawDates2D();
	void drawDates();
	void drawVideos(int _which);
	void addObject(VisibleObject *_vo);
	void checkInsides(int _x, int _y);
	void randomPositions();
	void resetById(int _id);
	void toggleTheme(int _t);
	void togglePlay();
	void stopVideo(int _side);
	void drawCloseBoxes(int _side, VisibleObject *_vo);
	
	ThreadedVideoLoader loader;
	
	//FrontPlayer fp;
	void updateShapes(int _which);
	
	void drawPlayer();
	//void addObject(VisibleObject _vo);
	vector<VisibleObject*>videoObjects;
private:
	vector<VisibleObject*>themeObjects;
	vector<VisibleObject*>dateObjects;
	int themes[5];
	int dates[6];
	int counting;
	int hoverTime;
	int idAssign;
	int currHoverId;
	int lastHoverId;
	int lastReactId;
	int hadInside;
	int currentlyPlaying;
	int currentlyBig;
	ofxDirList dir;
	int isPlaying;
	
	void makeThemes();
	void makeDates();
	void makeVideos();
	
	VisibleObject* actives[2];
	int haveActives[2];
	
	int playingLeft, playingRight;
	
	NavObject rightClose, leftClose;
	int rightOn, leftOn;
	
};

#endif