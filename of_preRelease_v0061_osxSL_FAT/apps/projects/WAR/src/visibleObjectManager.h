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
#include "ofMain.h"
#include <vector>

#define HOVER_CLICK_TIME 1000

using namespace std;

class VisibleObjectManager{
public:
	void update();
	void update(int _x, int _y);
	void draw();
	VisibleObjectManager();
	void addObject(VisibleObject *_vo);
	void checkInsides(int _x, int _y);
	void randomPositions();
	//void addObject(VisibleObject _vo);
private:
	vector<VisibleObject*>objects;
	int counting;
	int hoverTime;
	int idAssign;
	int currHoverId;
	int lastHoverId;
};

#endif