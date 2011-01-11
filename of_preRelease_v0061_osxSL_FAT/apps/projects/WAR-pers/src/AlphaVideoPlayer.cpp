#include "AlphaVideoPlayer.h"

//--------------------------------------------------------------
#ifdef  OF_VIDEO_PLAYER_QUICKTIME
//--------------------------------------------------------------

bool  alphaCreateMovieFromPath(char * path, Movie &movie);
bool 	alphaCreateMovieFromPath(char * path, Movie &movie){
	
	Boolean 	isdir			= false;
	OSErr 		result 			= 0;
	FSSpec 		theFSSpec;
   
	short 		actualResId 	= DoTheRightThing;
	

	#ifdef TARGET_WIN32
		result = NativePathNameToFSSpec (path, &theFSSpec, 0);
		if (result != noErr) { 
			printf("NativePathNameToFSSpec failed %d\n", result); 
			printf("ERROR LOADINGmovie \n"); 
			return false; 
		}
	
	#endif
	
	#ifdef TARGET_OSX
		FSRef 		fsref;
		result = FSPathMakeRef((const UInt8*)path, &fsref, &isdir);
		if (result) { printf("FSPathMakeRef failed %d\n", result); printf("ERROR LOADINGmovie \n"); return false; }
		result = FSGetCatalogInfo(&fsref, kFSCatInfoNone, NULL, NULL, &theFSSpec, NULL);
		if (result) { printf("FSGetCatalogInfo failed %d\n", result); printf("ERROR LOADINGmovie \n"); return false; }
	#endif
	

	short movieResFile;
	result = OpenMovieFile (&theFSSpec, &movieResFile, fsRdPerm);
	if (result == noErr) { 
		
		short   movieResID = 0;
		result = NewMovieFromFile(&movie, movieResFile, &movieResID, (unsigned char *) 0, newMovieActive, (Boolean *) 0);
		
		if (result == noErr){
			CloseMovieFile (movieResFile);
		} else {
			printf("NewMovieFromFile failed %d\n", result); 
			return false;
		}
	} else {
		printf("OpenMovieFile failed %d\n", result); 
		return false;
	} 	

	return true;

}

//----------------------------------------
void alphaConvertPixels(unsigned char * gWorldPixels, unsigned char * rgbaPixels, int w, int h){
	
	// ok for macs?
	// ok for intel macs?
	
	int * gWorldPtr 			= (int *) gWorldPixels;
	pix32 * rgbaPtr 			= (pix32 *) rgbaPixels;
	int totalPixelCount 	= w * h;
	unsigned char * gWorldStart;
	
	//	putting in the boolean, so we can work on 
	//	0,0 in top right...
	//	bool bFlipVertically 	= true;
	
	bool bFlipVertically 	= false;
	
	// -------------------------------------------
	// we flip vertically because the 0,0 position in OF 
	// is the bottom left (not top left, like processing)
	// since the 0,0 of a picture is top left
	// if we upload and drawf the data as is
	// it will be upside-down....
	// -------------------------------------------
	
	if (!bFlipVertically){
		//----- argb->rgb
		for (int i = 0; i < h; i++){
			pix32 * rgbaPtr 			= (pix32 *) rgbaPixels + ((i) * w);
			for (int j = 0; j < w; j++){
				gWorldStart = (unsigned char *)gWorldPtr;	
				memcpy (rgbaPtr, gWorldStart+1, sizeof(pix32));
				rgbaPtr++;
				gWorldPtr++;
			}
		}
	} else {
		//----- flip while argb->rgb
		for (int i = 0; i < h; i++){
			pix32 * rgbaPtr 			= (pix32 *) rgbaPixels + ((h-i-1) * w);
			for (int j = 0; j < w; j++){
				gWorldStart = (unsigned char *)gWorldPtr;	
				memcpy (rgbaPtr, gWorldStart+1, sizeof(pix32));
				rgbaPtr++;
				gWorldPtr++;
			}
		}
	}
}

//--------------------------------------------------------------
OSErr 	AlphaDrawCompleteProc(Movie theMovie, long refCon);
OSErr 	AlphaDrawCompleteProc(Movie theMovie, long refCon){

	AlphaVideoPlayer * ofvp = (AlphaVideoPlayer *)refCon;
	alphaConvertPixels(ofvp->offscreenGWorldPixels, ofvp->pixels, ofvp->width, ofvp->height);
	ofvp->bHavePixelsChanged = true;
	if (ofvp->bUseTexture == true){
		ofvp->tex.loadData(ofvp->pixels, ofvp->width, ofvp->height, GL_RGBA);
	}
	
	return noErr;
}


//--------------------------------------------------------------
#endif
//--------------------------------------------------------------


//---------------------------------------------------------------------------
AlphaVideoPlayer::AlphaVideoPlayer (){

	bLoaded 					= false;
	width 						= 0;
	height						= 0;
	speed 						= 1;
	bUseTexture					= true;
	bStarted					= false;
	pixels						= NULL;
	//--------------------------------------------------------------
    #ifndef  TARGET_LINUX  // !linux = quicktime...
    //--------------------------------------------------------------
    	moviePtr	 				= NULL;
    	allocated 					= false;
	//--------------------------------------------------------------
    #else 
    //--------------------------------------------------------------
		fobsDecoder					= NULL;
	//--------------------------------------------------------------
	#endif
	//--------------------------------------------------------------
	
}

//---------------------------------------------------------------------------
unsigned char * AlphaVideoPlayer::getPixels(){
	return pixels;
}

//---------------------------------------------------------------------------
bool AlphaVideoPlayer::isFrameNew(){
	
	return bIsFrameNew;

}

//---------------------------------------------------------------------------
void AlphaVideoPlayer::idleMovie(){

    if (bLoaded == true){

		//--------------------------------------------------------------
		#ifndef  TARGET_LINUX  // !linux = quicktime...
		//--------------------------------------------------------------

			MoviesTask(moviePtr,0);
			
		//--------------------------------------------------------------
		#else // linux.
		//--------------------------------------------------------------

			diffTime = (float)ofGetElapsedTimeMillis() - (float)(timeLastIdle);
			timeLastIdle = (float)ofGetElapsedTimeMillis();
            float pctDone =  diffTime / durationMillis;   // given this much time, how much of the movie did we do?

            if (bPaused != true) positionPct += (pctDone * speed); //speed;

			//------------------------------------- now, let's do different things if we are looping or not:
			if (loopMode == OF_LOOP_NONE){
			  if (positionPct > 1) positionPct = 1;
			  if (positionPct < 0) positionPct = 0;
			} else if (loopMode == OF_LOOP_PALINDROME) {
			  if (positionPct > 1) {
			      float diff = positionPct - 1.0f;
			      positionPct = 1 - diff;
			      speed *= -1;
			  }
			  if (positionPct < 0){
			      float diff = -positionPct;
			      positionPct = 0 + diff;
			      speed *= -1;
			  }
			} else {
			    while (positionPct < 0) positionPct += 1;
			    while (positionPct > 1) positionPct -= 1;
			}

			fobsDecoder->setFrame((int)(positionPct * iTotalFrames));

			int curFrameIndex =  fobsDecoder->getFrameIndex();

			if (curFrameIndex != lastFrameIndex){
			    unsigned char *rgba = fobsDecoder->getRGBA();
			    memcpy(pixels, rgba, width*height*4);
			    tex.loadData(pixels, width, height, GL_RGBA);
			}


			lastFrameIndex = curFrameIndex;


		//--------------------------------------------------------------
		#endif
		//--------------------------------------------------------------


		// ---------------------------------------------------
		// 		on all platforms, 
		// 		do "new"ness ever time we idle...
		// 		before "isFrameNew" was clearning, 
		// 		people had issues with that...
		// 		and it was badly named so now, newness happens 
		// 		per-idle not per isNew call
		// ---------------------------------------------------
			
		bIsFrameNew = bHavePixelsChanged;
		if (bHavePixelsChanged == true) {
			bHavePixelsChanged = false;
		}
	}

}

//---------------------------------------------------------------------------
void AlphaVideoPlayer::closeMovie(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	if (bLoaded == true){

	    DisposeMovie (moviePtr);
	    #ifdef TARGET_WIN32
			DisposeMovieDrawingCompleteUPP (AlphaDrawCompleteProc);
	    #endif
			moviePtr = NULL;
    }

    //--------------------------------------
	#else
	//--------------------------------------

	 fobsDecoder->close();

	//--------------------------------------
	#endif
    //--------------------------------------

    bLoaded = false;

}

//---------------------------------------------------------------------------
AlphaVideoPlayer::~AlphaVideoPlayer(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
		closeMovie();
		if(allocated)	delete(pixels);
		if(allocated)	delete(offscreenGWorldPixels);
		if ((offscreenGWorld)) DisposeGWorld((offscreenGWorld));

	 //--------------------------------------
	#else
	//--------------------------------------
		
		// [CHECK] anything else necessary for FOBS ? please check 
		if (fobsDecoder != NULL){
			delete fobsDecoder;
		}
		
		if (pixels != NULL){
			delete pixels;
		}
		
	//--------------------------------------
	#endif
	//--------------------------------------
	
	tex.clear();

}


//--------------------------------------
#ifdef OF_VIDEO_PLAYER_QUICKTIME
//--------------------------------------

void AlphaVideoPlayer::createImgMemAndGWorld(){
	
	Rect movieRect;
	movieRect.top 			= 0;
	movieRect.left 			= 0;
	movieRect.bottom 		= height;
	movieRect.right 		= width;
	offscreenGWorldPixels 	= new unsigned char[4 * width * height + 32];
	pixels					= new unsigned char[width*height*4];
	QTNewGWorldFromPtr (&(offscreenGWorld), k32ARGBPixelFormat, &(movieRect), NULL, NULL, 0, (offscreenGWorldPixels), 4 * width);
	LockPixels(GetGWorldPixMap(offscreenGWorld));
	SetGWorld (offscreenGWorld, NULL);
	SetMovieGWorld (moviePtr, offscreenGWorld, nil);
	//------------------------------------ texture stuff:
	if (bUseTexture){
		// create the texture, set the pixels to black and
		// upload them to the texture (so at least we see nothing black the callback)
		tex.allocate(width,height,GL_RGBA);
		memset(pixels, 0, width*height*4);
		tex.loadData(pixels, width, height, GL_RGBA);
		allocated = true;
	}
	
}

//--------------------------------------
#endif
//--------------------------------------




//---------------------------------------------------------------------------
bool AlphaVideoPlayer::loadMovie(string name){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
		
		initializeQuicktime();			// init quicktime
		closeMovie();					// if we have a movie open, close it
		bLoaded 				= false;	// try to load now
		name 					= ofToDataPath(name);
		alphaCreateMovieFromPath((char *)name.c_str(), moviePtr);
		
		bool bDoWeAlreadyHaveAGworld = false;
		if (width != 0 && height != 0){
			bDoWeAlreadyHaveAGworld = true;
		}
		Rect 				movieRect;
		GetMovieBox(moviePtr, &(movieRect));

		if (bDoWeAlreadyHaveAGworld){
			// is the gworld the same size, then lets *not* de-allocate and reallocate:
			if (width == movieRect.right &&
				height == movieRect.bottom){
				SetMovieGWorld (moviePtr, offscreenGWorld, nil);
			} else {
				width 	= movieRect.right;
				height 	= movieRect.bottom;
				delete(pixels);
				delete(offscreenGWorldPixels);
				if ((offscreenGWorld)) DisposeGWorld((offscreenGWorld));
				createImgMemAndGWorld();
			}
		} else {
			width	= movieRect.right;
			height 	= movieRect.bottom;
			createImgMemAndGWorld();
		}

		if (moviePtr == NULL){
			return false;
		}

		//----------------- callback method
	  MovieDrawingCompleteUPP myDrawCompleteProc;
	  myDrawCompleteProc = NewMovieDrawingCompleteUPP (AlphaDrawCompleteProc);
		SetMovieDrawingCompleteProc (moviePtr, movieDrawingCallWhenChanged,  myDrawCompleteProc, (long)this);

		// ------------- get some pixels in there ------
		GoToBeginningOfMovie(moviePtr);
		SetMovieActiveSegment(moviePtr, -1,-1);
		MoviesTask(moviePtr,0);
		
		alphaConvertPixels(offscreenGWorldPixels, pixels, width, height);
		
		if (bUseTexture == true){
			tex.loadData(pixels, width, height, GL_RGBA);
		}
		
		bStarted 				= false;
		bLoaded 				= true;
		bPlaying 				= false;
		bHavePixelsChanged 		= false;
		speed 					= 1;

		return true;

	//--------------------------------------
	#else
	//--------------------------------------

		bLoaded      		= false;
		bPaused 			= false;
		speed 				= 1.0f;
		bHavePixelsChanged 	= false;
		name 					= ofToDataPath(name);
		fobsDecoder 		= new omnividea::fobs::Decoder(name.c_str());
		omnividea::fobs::ReturnCode error = fobsDecoder->open();

		width 					= fobsDecoder->getWidth();
		height 					= fobsDecoder->getHeight();
		pixels					= new unsigned char[width*height*4];

		if (!fobsDecoder->isVideoPresent()){
			return false;
		}

		bLoaded = true;

		if (bUseTexture){
			// create the texture, set the pixels to black and
			// upload them to the texture (so at least we see nothing black the callback)
			tex.allocate(width,height,GL_RGBA);
			memset(pixels, 0, width*height*4);
			tex.loadData(pixels, width, height, GL_RGBA);
		}


		error = fobsDecoder->setFrame(0);

		if(error == omnividea::fobs::NoFrameError) {
			error = omnividea::fobs::OkCode;
			printf("NoFrameError\n");
		}

		if(omnividea::fobs::isOk(error)){
			// get some pixels in:
			unsigned char *rgba = fobsDecoder->getRGBA();
			if(rgba == NULL) error = omnividea::fobs::GenericError;
			if(isOk(error))
			{
				memcpy(pixels, rgba, width*height*4);
				tex.loadData(pixels, width, height, GL_RGBA);
			}
		}


		iTotalFrames		= (int)(fobsDecoder->getFrameRate()*fobsDecoder->getDurationSeconds());
		positionPct 		= 0;
		timeLastIdle 		= ofGetElapsedTimef();
        durationMillis      = fobsDecoder->getDurationSeconds() * 1000.0f;


		return true;


	//--------------------------------------
	#endif
	//--------------------------------------

}



//--------------------------------------------------------
void AlphaVideoPlayer::start(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	if (bLoaded == true && bStarted == false){
		SetMovieActive(moviePtr, true);

		//------------------ set the movie rate to default
		//------------------ and preroll, so the first frames come correct

		TimeValue timeNow 	= 	GetMovieTime(moviePtr, 0);
		Fixed playRate 		=	GetMoviePreferredRate(moviePtr); 		//Not being used!

		PrerollMovie(moviePtr, timeNow, X2Fix(speed));
		SetMovieRate(moviePtr,  X2Fix(speed));
		setLoopState(OF_LOOP_NORMAL);

		// get some pixels in there right away:
		MoviesTask(moviePtr,0);
		
		alphaConvertPixels(offscreenGWorldPixels, pixels, width, height);
		bHavePixelsChanged = true;
		if (bUseTexture == true){
			tex.loadData(pixels, width, height, GL_RGBA);
		}

		bStarted = true;
		bPlaying = true;
	}

	//--------------------------------------
	#endif
	//--------------------------------------

}

//--------------------------------------------------------
void AlphaVideoPlayer::play(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	if (!bStarted){
	 	start();
	}else {
		// ------------ lower level "startMovie"
		// ------------ use desired speed & time (-1,1,etc) to Preroll...
		bPlaying = true;
		TimeValue timeNow;
	   	timeNow = GetMovieTime(moviePtr, nil);
		PrerollMovie(moviePtr, timeNow, X2Fix(speed));
		SetMovieRate(moviePtr,  X2Fix(speed));
		MoviesTask(moviePtr, 0);
	}

	//--------------------------------------
	#endif
	//--------------------------------------

}

//--------------------------------------------------------
void AlphaVideoPlayer::stop(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	StopMovie (moviePtr);
	bPlaying = false;
	SetMovieActive (moviePtr, false);

	//--------------------------------------
	#endif
	//--------------------------------------

}

//--------------------------------------------------------
void AlphaVideoPlayer::setVolume(int volume){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	SetMovieVolume(moviePtr, volume);

	//--------------------------------------
	#endif
	//--------------------------------------

	// no volume currently for FOBS players :(

}

//--------------------------------------------------------
void AlphaVideoPlayer::setLoopState(int state){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		TimeBase myTimeBase;
		long myFlags = 0L;
		myTimeBase = GetMovieTimeBase(moviePtr);
		myFlags = GetTimeBaseFlags(myTimeBase);
		switch (state) {
			case OF_LOOP_NORMAL:
				myFlags |= loopTimeBase;
				myFlags &= ~palindromeLoopTimeBase;
				SetMoviePlayHints(moviePtr, hintsLoop, hintsLoop);
				SetMoviePlayHints(moviePtr, 0L, hintsPalindrome);
				break;
			case OF_LOOP_PALINDROME:
				myFlags |= loopTimeBase;
				myFlags |= palindromeLoopTimeBase;
				SetMoviePlayHints(moviePtr, hintsLoop, hintsLoop);
				SetMoviePlayHints(moviePtr, hintsPalindrome, hintsPalindrome);
				break;
			case OF_LOOP_NONE:
				default:
				myFlags &= ~loopTimeBase;
				myFlags &= ~palindromeLoopTimeBase;
				SetMoviePlayHints(moviePtr, 0L, hintsLoop |
				hintsPalindrome);
				break;
		}
		SetTimeBaseFlags(myTimeBase, myFlags);

	//--------------------------------------
	#else
	//--------------------------------------

		loopMode = state;

	//--------------------------------------
	#endif
	//--------------------------------------

}


//---------------------------------------------------------------------------
void AlphaVideoPlayer::setPosition(float pct){

 	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

	 	TimeRecord tr;
	 	tr.base 		= GetMovieTimeBase(moviePtr);
		long total 		= GetMovieDuration(moviePtr );
		long newPos 	= (long)((float)total * pct);
		SetMovieTimeValue(moviePtr, newPos);

	//--------------------------------------
	#else
	//--------------------------------------

        pct = CLAMP(pct, 0,1);
        positionPct = pct;  // check between 0 and 1;

	//--------------------------------------
	#endif
	//--------------------------------------

}

//---------------------------------------------------------------------------
float AlphaVideoPlayer::getDuration(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		return GetMovieDuration (moviePtr) / (double) GetMovieTimeScale (moviePtr);

	//--------------------------------------
	#else
	//--------------------------------------

		return fobsDecoder->getDurationSeconds();

	//--------------------------------------
	#endif
	//--------------------------------------

}

//---------------------------------------------------------------------------
float AlphaVideoPlayer::getPosition(){

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		long total 		= GetMovieDuration(moviePtr);
		long current 	= GetMovieTime(moviePtr, nil);
		float pct 		= ((float)current/(float)total);
		return pct;

	//--------------------------------------
	#else
	//--------------------------------------

		return (float) fobsDecoder->getFrameTime() / getDuration();

	//--------------------------------------
	#endif
	//--------------------------------------


}

//---------------------------------------------------------------------------
void AlphaVideoPlayer::setSpeed(float _speed){

	speed 				= _speed;

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------
	if (bPlaying == true){
		//setMovieRate actually plays, so let's call it only when we are playing
		SetMovieRate(moviePtr, X2Fix(speed));
	}
	//--------------------------------------
	#endif
	//--------------------------------------
}

//---------------------------------------------------------------------------
float AlphaVideoPlayer::getSpeed(){
	return speed;
}

//---------------------------------------------------------------------------
void AlphaVideoPlayer::setPaused(bool _bPause){

	bPaused = _bPause;

	//--------------------------------------
	#ifdef OF_VIDEO_PLAYER_QUICKTIME
	//--------------------------------------

		// there might be a more "quicktime-ish" way (or smarter way)
		// to do this for now, to pause, just set the movie's speed to zero,
		// on un-pause, set the movie's speed to "speed"
		// (and hope that speed != 0...)
		if (bPlaying == true){
			if (bPaused == true) 	SetMovieRate(moviePtr, X2Fix(0));
			else 					SetMovieRate(moviePtr, X2Fix(speed));
		}

	//--------------------------------------
	#endif
	//--------------------------------------

}

//------------------------------------
void AlphaVideoPlayer::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//------------------------------------
void AlphaVideoPlayer::draw(float _x, float _y, float _w, float _h){
	if (bUseTexture){
		tex.draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void AlphaVideoPlayer::draw(float _x, float _y){
	draw(_x, _y, width, height);
}



