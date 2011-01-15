/*
 *  threadedVideoLoader.h
 *  WAR
 *
 *  Created by Paul Paradiso on 1/13/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _THREADED_VIDEO_LOADER_H_
#define _THREADED_VIDEO_LOADER_H_

#include "ofMain.h"
#include "ofxThread.h"
#include "json.h"
#include "visibleObject.h"
#include "videoObject.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include "defines.h"

using namespace Json;

class ThreadedVideoLoader : public ofxThread{
public:
	
	ThreadedVideoLoader();
	string parseJSON(string _str);
	
	void setup(vector<VisibleObject*>*_vos);
	void threadedFunction();
	
	void checkVideos();
	void loadMovie();
	void update();
	
	Reader reader;
	Value root;
	string localResult;
	vector<VisibleObject*>*videoObjects;
	
	int lastMovieLoadedTime;	
	int nextMovie;
	int isDoingMovies;
	int numElements;
	int enoughVideos;
	int movieToLoad;
	
	int isThreaded;
	int side;
	
	ofxVec2f posLeft[5];
	int posRightIndex;
	ofxVec2f posRight[5];
	int posLeftIndex;
	
	string themes[5];
};

#endif
