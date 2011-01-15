/*
 *  videoLoader.cpp
 *  WAR
 *
 *  Created by Paul Paradiso on 1/14/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "videoLoader.h"

VideoLoader::VideoLoader(){
	string str,strTotal;
	ifstream in;
	in.open(ofToDataPath("cache/video_cache.json", true).c_str());
	while(in){
		strTotal += str;
		getline(in, str);
	}
	in.close();
	localResult = parseJSON(strTotal);
	lastMovieLoadedTime = 0;
	nextMovie = 0;
	isDoingMovies = 1;
	haveLoadedVideo = 0;
	//cout<<strTotal<<endl;
}

void VideoLoader::setup(vector<VisibleObject*>*_vos){
	videoObjects = _vos;
}

void VideoLoader::update(){
	checkVideos();
}

void VideoLoader::checkVideos(){
	if(isDoingMovies){
		if(ofGetElapsedTimeMillis() - lastMovieLoadedTime > MOVIE_LOAD_TIME){
			loadMovie();
			lastMovieLoadedTime = ofGetElapsedTimeMillis();
		}
	}
}

void VideoLoader::loadMovie(){
	cout<<"Load Movie Called."<<endl;
	Value v = root[nextMovie];
	cout<<"adding movie "<<v["path"].asString()<<endl;
	VideoObject *tmpVideo = new VideoObject("cache/" + v["path"].asString());
	tmpVideo->isLeft = 0;
	tmpVideo->theme = ofRandom(0, 5);
	tmpVideo->id = nextMovie;
	tmpVideo->setPos(ofRandom(50, 600),ofRandom(100, ofGetHeight() - 200));
	videoObjects->push_back(tmpVideo);
	nextMovie++;
	if(nextMovie == numElements)
		isDoingMovies = 0;
}

string VideoLoader::parseJSON(string _str){
	string result;
	result = "......loading JSON.......\n";
	bool parsingSuccess = reader.parse(_str,root);
	if(!parsingSuccess){
		cout<<"Failed to parse JSON\n"<< reader.getFormatedErrorMessages();
		return result;
	}
	Value text = root[0];
	numElements = (int)text.size();
	cout<<"First path = "<<text["url"].asString()<<"of "<<numElements<<" objects"<<endl;
	return result;
}

