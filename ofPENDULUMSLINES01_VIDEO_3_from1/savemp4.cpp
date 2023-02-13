#include "ofApp.h"
#include "ofConstants.h"
#include "savemp4.h"
// FFMPEG
int* buffer = new int[VIDEOW*VIDEOH];
FILE* ffmpeg;

//--------------------------------------------------------------
void setupmp4()
{
	#ifdef SAVEFFMPEG
	char wbuf[80];
	srand(time(NULL));
	sprintf(wbuf,"output%d.mp4",int(10000*(rand()/float(RAND_MAX))));
	std::rename("output.mp4", wbuf);

	// FFMPEG
	// start ffmpeg telling it to expect raw rgba 720p-60hz frames
	// -i - tells it to read frames from stdin
	//const char* cmd = "ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s 1280x720 -hwaccel auto -i - "
	//	                 "-threads 1  -x264opts opencl -preset ultrafast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";

	//const char* cmd = string("ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s 1280x720 -hwaccel auto -i - "
	//	                 "-threads 1  -x264opts opencl -preset ultrafast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";		                 
	char cmd[180];
	//sprintf(cmd,"ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s %dx%d -hwaccel auto -i - -threads 1  -x264opts opencl -preset superfast -y -pix_fmt yuv420p -crf 18 output.mp4",VIDEOW,VIDEOH);
	sprintf(cmd,"ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s %dx%d -hwaccel auto -i - -threads 4 -preset slow -y -crf 17 -vf vflip output.mp4",VIDEOW,VIDEOH);//-x264opts opencl
                 //"-threads 1  -x264opts opencl -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";
	//const char* cmd = "ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s 1280x720 -i - "
      //           "-threads 1 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";
	// open pipe to ffmpeg's stdin in binary write mode
		// FFMPEG
	#ifdef __linux__ 
	ffmpeg = popen(cmd, "w");		// LINUX
		#elif _WIN32
	ffmpeg = _popen(cmd, "wb");		// WINDOWS
	#else
	#endif
	//ffmpeg = popen(cmd, "w");		// LINUX
	#endif
}

void savemp4frame()
{
	//FFMPEG
	#ifdef SAVEFFMPEG
	static int frame = 0;
	//if(!(frame++ % 32))//*TSCAL))
	{
		glReadPixels(0, 0, VIDEOW, VIDEOH, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		fwrite(buffer, sizeof(int)*VIDEOW*VIDEOH, 1, ffmpeg);	}
	#endif
}

void quitmp4()
{
	#ifdef SAVEFFMPEG
	// FFMPEG
	#ifdef __linux__ 
   	pclose(ffmpeg);
   	#elif _WIN32
   	_pclose(ffmpeg);
   	#else
   	#endif
	#endif
}