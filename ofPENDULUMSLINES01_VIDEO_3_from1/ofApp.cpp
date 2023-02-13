#include "ofApp.h"
#include "savemp4.h"

#define M_PI 3.14159265358979323846264338327950288419716939937510
#define N 1
double px0 = W/2, py0 = H/4;
double r1 = 220;
double r2 = 220;
double m1 = 6.0;
double m2 = 4.0;
double a1[N];
double a2[N];
double a1v[N];
double a2v[N];
double dt = 0.4;
double g=1.0;

const int NT =10060;
int nt=0;
vector<vector<double>> xt;
vector<vector<double>> yt;
//vector<double> xt(NT);
//vector<double> yt(NT);

ofVec3f palette(float t, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d )
{
    return ofVec3f(	a.x + b.x * cos( 6.28318*(c.x*t+d.x)),  
					a.y + b.y * cos( 6.28318*(c.y*t+d.y)),  
					a.z + b.z * cos( 6.28318*(c.z*t+d.z)));
}
//--------------------------------------------------------------
void ofApp::setup()
{
	xt.resize(N);
	yt.resize(N);
	for(int i=0; i<N; i++)
	{
		xt[i].resize(NT);
		yt[i].resize(NT);
	}

	for(int i=0; i<N; i++)
	{
		a1[i] = 0.5*M_PI;
		a2[i] = 0.79*M_PI + 0.0001*(i/float(N)-0.5);
		a1v[i] = 0;
		a2v[i] = 0;
	}

	setupmp4();
	ofColor myColor;
    myColor.set( 0, 0, 0 );
	ofClear( myColor );
}

//--------------------------------------------------------------
void ofApp::update()
{
	for(int i=0; i<N; i++)
	{
		// Double Pendulum
		//
		a1[i] = a1[i] + a1v[i] * dt;
		a2[i] = a2[i] + a2v[i] * dt;
		//https://www.myphysicslab.com/pendulum/double-pendulum-en.html
		double dv1= 
			-g*(2*m1+m2)*sin(a1[i])-m2*g*sin(a1[i]-2*a2[i])-2*sin(a1[i]-a2[i])*m2*(a2v[i]*a2v[i]*r2+a1v[i]*a1v[i]*r1*cos(a1[i]-a2[i]));
		dv1/= r1*(2*m1+m2-m2*cos(2*a1[i]-2*a2[i]));

		double dv2=
			2*sin(a1[i]-a2[i])*(a1v[i]*a1v[i]*r1*(m1+m2)+g*(m1+m2)*cos(a1[i])+a2v[i]*a2v[i]*r2*m2*cos(a1[i]-a2[i]));
		dv2/= r2*(2*m1+m2-m2*cos(2*a1[i]-2*a2[i]));


/*		if(a2v[i]>0) dv2+=(-a2v[i]*a2v[i]*0.4);
			else
			dv2+=(a2v[i]*a2v[i]*0.4);
		if(a1v[i]>0) dv1+=(-a1v[i]*a1v[i]*0.4);
			else
			dv1+=(a1v[i]*a1v[i]*0.4);
			*/
		a1v[i]=a1v[i]+(dv1 )*dt;
		a2v[i]=a2v[i]+(dv2 )*dt;
		//a1v[i]*=0.9996;
		//a2v[i]*=0.9996;
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofColor myColor;
    myColor.set( 0, 0, 0 );
	//static int bylo=0;
	//if(bylo<5)
	//{
	//	bylo++;
	ofClear( myColor );
	
	
	//ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofDisableAlphaBlending();
	
	//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	for(int i=0; i<N; i++)
	{
		ofSetLineWidth(10.0);
		glEnable( GL_LINE_SMOOTH );
glEnable( GL_POLYGON_SMOOTH );
glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
		//for(int j=nt-1; j>0; j--)
		for(int j=0; j<=nt-1; j++)
		{
					ofVec3f col = palette(i/(float(N))+0.5*j/float(nt-1), 
								ofVec3f(0.5, 0.5, 0.5), 
								ofVec3f(0.5, 0.5, 0.5), 
								ofVec3f(1.0, 1.0, 1.0), ofVec3f(0.0, 0.1, 0.2));

			float f = 2.3*j/float(nt-1);
			//ofSetColor(f*255*col.x,f*255*col.y,f*255*col.z,2*f*255);
			ofSetColor(f*255*col.x,f*255*col.y,f*255*col.z,f*255);
			//ofDrawCircle(xt[i][j],yt[i][j],1.0);
			if(xt[i][j]!=0 && xt[i][j-1]!=0)
			{
				ofDrawLine(xt[i][j],yt[i][j],xt[i][j-1],yt[i][j-1]);
				ofSetColor(f*255*col.x,f*255*col.y,f*255*col.z,0.05*f*255);
				ofDrawLine(xt[i][j]+5,yt[i][j],xt[i][j-1]+5,yt[i][j-1]);
				ofDrawLine(xt[i][j]-5,yt[i][j],xt[i][j-1]-5,yt[i][j-1]);
				ofDrawLine(xt[i][j],yt[i][j]+5,xt[i][j-1],yt[i][j-1]+5);
				ofDrawLine(xt[i][j],yt[i][j]-5,xt[i][j-1],yt[i][j-1]-5);
			}
		}

		

		ofSetLineWidth(10.0);
		//ofSetLineWidth(5.0);
		// draw pendulum
		double x1 = px0+r1*sin(a1[i]);
		double y1 = py0+r1*cos(a1[i]);
		double x2 = x1+r2*sin(a2[i]);
		double y2 = y1+r2*cos(a2[i]);
		
		//int r=255*(i/float(N));
		ofVec3f col = palette(0.85*i/float(N), 
								ofVec3f(0.5, 0.5, 0.5), 
								ofVec3f(0.5, 0.5, 0.5), 
								ofVec3f(1.0, 1.0, 1.0), ofVec3f(0.0, 0.1, 0.2));

		//ofSetColor(255*col.x,255*col.y,255*col.z,5);
		ofSetColor(355*col.x,355*col.y,355*col.z,255);
		//ofSetColor(255,255,255,255);

		ofDrawLine(px0, py0, x1, y1);
		ofDrawLine(x1, y1, x2, y2);

		//ofDrawCircle(px0,py0,2.4);
		//ofDrawCircle(x1,y1,2.4);
		//ofDrawCircle(x2,y2,2.4);
		
		// tail
		xt[i][nt] = x2;
		yt[i][nt] = y2;
		
		if(nt<NT-1)
			nt++;
		else for(int j=0; j<nt; j++)
		{
			xt[i][j]=xt[i][j+1];
			yt[i][j]=yt[i][j+1];
		}

	}
	//ofImage img;
	//img.grabScreen(0,0,W,H);
	//char workbuf[90];
	//static int frame=0;
	//frame++;
	//sprintf(workbuf,"img%08d.jpg",frame);
	//img.save(workbuf);
	savemp4frame();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
  /*if (nPts < MAXPOINTS) 
  {
    pts[nPts].x = x;
    pts[nPts].y = y;
    nPts++;
  }
  
  polyline.addVertex(ofPoint(x, y));*/
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
  //polyline.addVertex(ofPoint(x, y));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
//  nPts = 0;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
