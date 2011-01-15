/*
 *  textObject.h
 *  WAR
 *
 *  Created by Paul Paradiso on 12/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NAV_OBJECT_H_
#define _NAV_OBJECT_H_

#include "visibleObject.h"
#include "ofMain.h"
#include "ofxVectorMath.h"
#include <vector>
#include "frontPlayer.h"

#define GROW_TIME 500.0
#define FONT "SpartanLTStd-HeavyClass.otf"

class NavObject : public VisibleObject{

	public:
		NavObject();
		NavObject(string _text);
		void setup(string _text);
		int isInside(int _x, int _y);
		void draw();
		void draw(int _id, int _time);
		void update();
		int react(int _lvl);
		void setPos(float _x, float _y);
		void setPosVel(float _xV, float _yV);
		void setPosAcc(float _xA, float _yA);
		void setRot(float _r);
		void setRotVel(float _rV);
		void setRotAcc(float _rA);
		void resizeByHeight(float _h);
		void resizeByWidth(float _w);
		void adjustPosition();
	
		void setFontSize(int _s);
	
		void updateShape();
		void updateActualShape();
		void drawShape();
		void resetState();
		void stopVideo();
	    void drawFront();
	
		void addFp(FrontPlayer *_fp);
	
		int isInsideFlat(int _x, int _y);
	
		void drawFlat();
	
		void testShape(int _which);
	
		FrontPlayer *fp;
	
	private:
		ofxVec2f drawSize;
		ofTrueTypeFont font;
		ofImage navImage;
		ofImage navImageGlow;
		ofxVec3f trans;
		string text;
};

#endif
