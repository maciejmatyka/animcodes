#pragma once
#include "ofMain.h"

#define MAXPOINTS 1000000
const int W = 1920;//280;
const int H = 1080;//720;
const int VIDEOW = W;//1280;
const int VIDEOH = H;//1280;
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
  
  ofPoint pts[MAXPOINTS];
  int nPts = 0;  
  ofPolyline polyline;
  ofImage img;
		
};
