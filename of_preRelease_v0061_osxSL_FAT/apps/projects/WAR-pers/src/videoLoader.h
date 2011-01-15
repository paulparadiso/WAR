/*
 *  videoLoader.h
 *  WAR
 *
 *  Created by Paul Paradiso on 1/14/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _VIDEO_LOADER_H_
#define _VIDEO_LOADER_H_

#include "ofMain.h"
#include "ofxThread.h"
#include "json.h"
#include "visibleObject.h"
#include "videoObject.h"
#include <fstream>
#include <iostream>
#include <string>
#include "defines.h"

using namespace Json;

class VideoLoader{
public:
	
	VideoLoader();
	string parseJSON(string _str);
	
	void setup(vector<VisibleObject*>*_vos);
	void update();
	
	void checkVideos();
	void loadMovie();
	
	Reader reader;
	Value root;
	string localResult;
	vector<VisibleObject*>*videoObjects;
	
	int lastMovieLoadedTime;	
	int nextMovie;
	int isDoingMovies;
	int numElements;
	int haveLoadedVideo;
};

#endif
