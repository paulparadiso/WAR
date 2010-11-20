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

class VideoObject : public VisibleObject{
public:
	
	VideoObject();
	VideoObject(string _path);
	void draw();
	void update();
	void react(int _lvl);
	int isInside(int _x, int _y);
	void setPos(float _x, float _y);
	void setPosVel(float _xV, float _yV);
	void setPosAcc(float _xA, float _yA);
	void setRot(float _r);
	void setRotVel(float _rV);
	void setRotAcc(float _rA);
	void resizeByHeight(float _h);
	void resizeByWidth(float _w);
	
private:
	ofVideoPlayer vp;
	int isPlaying;	
	ofxVec2f drawSize;
};

#endif