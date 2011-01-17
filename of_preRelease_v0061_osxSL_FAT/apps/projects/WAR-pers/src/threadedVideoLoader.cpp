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
	lastVideoLoadedTime = 0;
	nextVideo = 0;
	nextNewVideo = 0;
	isDoingVideos = 1;
	isThreaded = IS_THREADED;
	haveNewVideo = 0;
	side = 0;
	enoughVideos = MAX_VIDEOS;
	posRightIndex = 0;
	posLeftIndex = 0;
	//cout<<strTotal<<endl;
	
	posRight[0].set(20,20);
	posRight[1].set(50,270);
	posRight[2].set(220,520);
	posRight[3].set(375,100);
	posRight[4].set(540,400);
	
	posLeft[0].set(ofGetWidth() - 320,20);
	posLeft[1].set(ofGetWidth() - 350,290);
	posLeft[2].set(ofGetWidth() - 520,535);
	posLeft[3].set(ofGetWidth() - 675,100);
	posLeft[4].set(ofGetWidth() - 625,300);
	
	haveActives[0] = -1;
	haveActives[1] = -1;
	
	themes[0] = "Conschiousness Raising";
	themes[1] = "The Body Politic";
	themes[2] = "Media";
	themes[3] = "Social Protest";
	themes[4] = "Indentity";
	
	for(int i = 0; i < MAX_VIDEOS; i++){
		loadMovie();
		numPlays[i] = 0;
		nextVideo = (nextVideo + 1) % MAX_VIDEOS;
	}
	
}

void ThreadedVideoLoader::setup(vector<VisibleObject*>*_vos){
	videoObjects = _vos;
}

void ThreadedVideoLoader::setup(){
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

int ThreadedVideoLoader::makeDateInt(string _date){
	int dateInt = -1;
	cout<<"checking date - "<<_date<<endl;
	int found = 0;
	string year = "95";
	found = _date.find(year);
	if(found > -1)
		dateInt = 0;
	year = "96";
	found = _date.find(year);
	if(found > -1)
	   dateInt = 1;
	year = "97";
	found = _date.find(year);
	if(found > -1)
	   dateInt = 2;
	year = "98";
	found = _date.find(year);
	if(found > -1)
	   dateInt = 3;
	year = "99";
	found = _date.find(year);
	if(found > -1){
		dateInt = 4;
		cout<<"gotcha 90s"<<endl;
	}
	year = "20";
	found = _date.find(year);
	if(found > -1){
		dateInt = 5;
		cout<<"gotcha 2000s"<<endl;
	}
	if(dateInt == -1)
	   dateInt = DEFAULT_DATE;
	cout<<"dateInt = "<<dateInt<<endl;
	return dateInt;
}

string ThreadedVideoLoader::stripSlashes(string _input){
	int found = 0;
	while(found = _input.find('\\') > -1){
		_input.erase(found,1);
	}
	return _input;						
}

void ThreadedVideoLoader::checkVideos(){
	if(isDoingVideos){
		if(ofGetElapsedTimeMillis() - lastVideoLoadedTime > MOVIE_LOAD_TIME){
			lastVideoLoadedTime = ofGetElapsedTimeMillis();
			if(isThreaded){
				if(haveNewVideo == 0){
					if(!haveActives[side])
						startThread(true,false);
				}
			} else {
				loadMovie();
			}
		}
	} else {
		for(int i = 0; i < MAX_VIDEOS; i++){
			if(numPlays[i] >= MAX_VIDEO_PLAYS){
				cout << "Num plays met."<<endl;
				if(haveNewVideo == 0 && haveActives[videos[i].isLeft] != videos[i].id){
					nextVideo = videos[i].id;
					side = videos[i].isLeft;
					if(side){
						posLeftIndex = videos[i].positionIndex;
						cout<<"setting pos index to " << posLeftIndex<<endl;
					} else {
						posRightIndex = videos[i].positionIndex;
						cout<<"setting pos index to " << posRightIndex<<endl;
					}
					startThread(true, false);
				}
			} else {
			}
		}
	}
}

//string artist, title, date, description, uploader, uploadDate, uploaderLocale;

void ThreadedVideoLoader::loadMovie(){
	Value v = root[nextNewVideo];
	nextNewVideo = (nextNewVideo + 1) % numElements;
	cout<<"adding movie "<<v["path"].asString()<<endl;
	videos[nextVideo].vp.setUseTexture(false);
	videos[nextVideo].textureSet = 0;
	videos[nextVideo].vp.loadMovie("cache/" + v["path"].asString());
	videos[nextVideo].isLeft = side;
	if(!side){
		videos[nextVideo].setPos(posRight[posRightIndex].x,posRight[posRightIndex].y);
		cout<<"Pos right set to " <<posRightIndex<<endl;
		videos[nextVideo].positionIndex = posRightIndex;
		posRightIndex = posRightIndex + 1 % 5;
	} else {
		videos[nextVideo].setPos(posLeft[posLeftIndex].x,posLeft[posLeftIndex].y);
		cout<<"Pos right set to " <<posLeftIndex<<endl;
		videos[nextVideo].positionIndex = posLeftIndex;
		posLeftIndex = posLeftIndex + 1 % 5;
	}
	Value t = v["themes"];
	videos[nextVideo].themesInt = ofToInt(t[0].asString()) - 3;
	for(int i = 0; i < t.size(); i++){
		//tmpVideo->themes.push_back(themes[ofToInt(t[i].asString())]);
	}
	videos[nextVideo].artist = stripSlashes(v["name"].asString());
	videos[nextVideo].title = stripSlashes(v["title"].asString());
	videos[nextVideo].date = v["work_date"].asString();
	videos[nextVideo].dateInt = makeDateInt(v["work_date"].asString());
	videos[nextVideo].description = stripSlashes(v["description"].asString());
	videos[nextVideo].uploadDate = v["upload_date"].asString();
	videos[nextVideo].uploaderLocale = v["geo_city"].asString() + ", " + v["geo_region"].asString() + ", " + v["get_country_code"].asString();
	videos[nextVideo].theme = ofRandom(0, 5);
	videos[nextVideo].id = nextVideo;
	videos[nextVideo].state = STATE_REST;
	videos[nextVideo].fontsLoaded = 0;
	videos[nextVideo].shouldPlay = 1;
	numPlays[nextVideo] = 0;
	//videoObjects->push_back(tmpVideo);
	haveNewVideo = 1;
	side = !side;
	//nextVideo++;
	if(nextVideo == enoughVideos - 1)
		isDoingVideos = 0;
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

