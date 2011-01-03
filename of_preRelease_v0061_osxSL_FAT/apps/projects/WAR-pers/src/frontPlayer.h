/*
 *  frontPlayer.h
 *  WAR
 *
 *  Created by Paul Paradiso on 1/3/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _FRONT_PLAYER_H_
#define _FRONT_PLAYER_H_

#include "panningVideoPlayer.h"
#include "ofMain.h"
#include "ofxVectorMath.h"

class FrontPlayer{

public:
	FrontPlayer();
	void setup();
	void update();
	void draw();
	void stop(int _player);
	int loadMovie(string _movie, int _player);
	
	PanningVideoPlayer *vpRight;
	PanningVideoPlayer *vpLeft;
	
	int haveRight, haveLeft;
	
	ofxVec2f *playSize;
	ofxVec4f getBoxSize(int _side);
private:

	ofTrueTypeFont artistFont;
	ofTrueTypeFont themeFont;
	ofTrueTypeFont tagsFont;
	ofTrueTypeFont uploadFont;
	
	ofxVec4f leftBox, rightBox;
};

#endif

