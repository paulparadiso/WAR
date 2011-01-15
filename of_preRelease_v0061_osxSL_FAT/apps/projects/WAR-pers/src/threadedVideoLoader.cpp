/*
 *  threadedVideoLoader.cpp
 *  WAR
 *
 *  Created by Paul Paradiso on 1/13/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "threadedVideoLoader.h"

ThreadedVideoLoader::ThreadedVideoLoader(){
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
	isThreaded = 1;
	side = 0;
	enoughVideos = MAX_VIDEOS;
	posRightIndex = 0;
	posLeftIndex = 0;
	//cout<<strTotal<<endl;
	
	posRight[0].set(20,20);
	posRight[1].set(50,250);
	posRight[2].set(220,280);
	posRight[3].set(375,100);
	posRight[4].set(490,600);
	
	posLeft[0].set(1000,500);
	posLeft[1].set(1100,250);
	posLeft[2].set(800,400);
	posLeft[3].set(900,20);
	posLeft[4].set(1150,500);
	
	themes[0] = "Conschiousness Raising";
	themes[1] = "The Body Politic";
	themes[2] = "Media";
	themes[3] = "Social Protest";
	themes[4] = "Indentity";
	
}

void ThreadedVideoLoader::setup(vector<VisibleObject*>*_vos){
	videoObjects = _vos;
}

void ThreadedVideoLoader::threadedFunction(){
	if(lock()){
		loadMovie();
		unlock();
		ofSleepMillis(1 * 1000);
	}
	stopThread();
}

void ThreadedVideoLoader::update(){
	checkVideos();
}

void ThreadedVideoLoader::checkVideos(){
	if(isDoingMovies){
		if(ofGetElapsedTimeMillis() - lastMovieLoadedTime > MOVIE_LOAD_TIME){
			lastMovieLoadedTime = ofGetElapsedTimeMillis();
			if(isThreaded){
				startThread(true,false);
			} else {
				loadMovie();
			}
		}
	}
}

//string artist, title, date, description, uploader, uploadDate, uploaderLocale;

void ThreadedVideoLoader::loadMovie(){
	Value v = root[nextMovie];
	cout<<"adding movie "<<v["path"].asString()<<endl;
	VideoObject *tmpVideo = new VideoObject("cache/" + v["path"].asString());
	tmpVideo->isLeft = side;
	if(!side){
		tmpVideo->setPos(posRight[posRightIndex].x,posRight[posRightIndex].y);
		posRightIndex = posRightIndex + 1 % 5;
	} else {
		tmpVideo->setPos(posLeft[posLeftIndex].x,posLeft[posLeftIndex].y);
		posLeftIndex = posLeftIndex + 1 % 5;
	}
	side = !side;
	Value t = v["themes"];
	tmpVideo->themesInt = ofToInt(t[0].asString()) - 3;
	for(int i = 0; i < t.size(); i++){
		//tmpVideo->themes.push_back(themes[ofToInt(t[i].asString())]);
	}
	tmpVideo->artist = v["name"].asString();
	tmpVideo->title = v["title"].asString();
	tmpVideo->date = v["work_date"].asString();
	tmpVideo->description = v["description"].asString();
	tmpVideo->uploadDate = v["upload_date"].asString();
	tmpVideo->uploaderLocale = v["geo_city"].asString() + ", " + v["geo_region"].asString() + ", " + v["get_country_code"].asString();
	tmpVideo->theme = ofRandom(0, 5);
	tmpVideo->id = nextMovie;
	tmpVideo->state = STATE_REST;
	videoObjects->push_back(tmpVideo);
	nextMovie++;
	if(nextMovie == enoughVideos)
		isDoingMovies = 0;
}

string ThreadedVideoLoader::parseJSON(string _str){
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

