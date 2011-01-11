#ifndef _ALPHA_VIDEO_PLAYER
#define _ALPHA_VIDEO_PLAYER

#include "ofMain.h"
#include "ofTexture.h"

#ifdef OF_VIDEO_PLAYER_FOBS
	#include "Decoder.h"
#else 
	#include "ofQtUtils.h"
#endif

#define OF_LOOP_NONE					0x01
#define OF_LOOP_PALINDROME				0x02
#define OF_LOOP_NORMAL					0x03

//---------------------------------------------

class AlphaVideoPlayer{
	
	public:
	
		AlphaVideoPlayer ();
		~AlphaVideoPlayer();
				
		bool 				loadMovie(string name);
		void 				closeMovie();
		
		void 				idleMovie();				// rename to updateMovie?
		void 				play();				
		void 				stop();
		
		int 				width, height;
		float  				speed;
		bool 				bLoaded;	
				
		bool 				isFrameNew();
		unsigned char * 	getPixels();
		float 				getPosition();
		float 				getSpeed();
		float 				getDuration();	
		
		void 				setPosition(float pct);
		void 				setVolume(int volume);
		void 				setLoopState(int state);
		void   				setSpeed(float speed);
		
		void 				setUseTexture(bool bUse);
		void 				draw(float x, float y, float w, float h);	
		void 				draw(float x, float y);	
		
		void 				setPaused(bool bPause);

		//--------------------------------------
		#ifdef OF_VIDEO_PLAYER_FOBS

		//--------------------------------------
			omnividea::fobs::Decoder	* fobsDecoder;
			int					iTotalFrames;
			int					loopMode;
			float               timeLastIdle;
			float               diffTime;
			float               positionPct;
			int                 lastFrameIndex;  // as we play, look for changed frames
			float               durationMillis;
		//--------------------------------------
		#else    // quicktime
		//--------------------------------------
			MovieController  	thePlayer;
			GWorldPtr 			offscreenGWorld;
			Movie 			 	moviePtr;
			unsigned char * 	offscreenGWorldPixels;	// 32 bit: argb (qt k32ARGBPixelFormat)	
		//--------------------------------------
		#endif
		//--------------------------------------
			
		unsigned char * 	pixels;					// 32 bit: rgba
		bool 				bHavePixelsChanged;
		ofTexture 			tex;					// a ptr to the texture we are utilizing
		bool 				bUseTexture;			// are we using a texture	
		bool				allocated;				// so we know to free pixels or not

	protected:
	
		
		void 				start();
		void 				createImgMemAndGWorld();
		bool 				bStarted;
		bool 				bPlaying;
		bool 				bPaused;
		bool 				bIsFrameNew;			// if we are new


};

//-------------------------- helpful for rgba->rgb conversion
		typedef struct{
			unsigned char r; 
			unsigned char g; 
			unsigned char b;
		} pixels24;

		typedef struct{
			unsigned char r; 
			unsigned char g; 
			unsigned char b;
			unsigned char a;
		} pix32;

		void 				alphaConvertPixels(unsigned char * gWorldPixels, unsigned char * rgbaPixels, int w, int h);


#endif






