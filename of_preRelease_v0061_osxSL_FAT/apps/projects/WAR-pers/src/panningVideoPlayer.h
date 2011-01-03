/*
 *  panningVideoPlayer.h
 *  WAR
 *
 *  Created by Paul Paradiso on 1/3/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _PANNING_VIDEO_PLAYER_H_
#define _PANNING_VIDEO_PLAYER_H_

#include "ofMain.h"

class PanningVideoPlayer : public ofVideoPlayer{
public:
	void setPan(float _pan){
		SetMovieAudioBalance(moviePtr, _pan, 0);
	}
};

#endif