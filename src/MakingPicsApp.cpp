#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "cinder/CinderMath.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MakingPicsApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings *settings);

private:
	Surface* mySurface_;
	int frame_number_;
	boost::posix_time::ptime app_start_time_;
	int times_clicked_;


	static const int kAppWidth=600;
	static const int kAppHeight=600;
	static const int kTextureSize=1024; //Must be the next power of 2 bigger or equal to app dimensions
	static const int scale = 144;

	void lines(uint8_t* pixels, int x_1, int x_2, int y_1, int y_2, Color8u c);

};

void MakingPicsApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( kAppWidth, kAppHeight );
    settings->setFrameRate( 60.0f );
}



void MakingPicsApp::lines(uint8_t* pixels, int x_1, int x_2, int y_1, int y_2, Color8u c)
    {
       

       int x1=x_1;
       int y1=y_1;

       int x2=x_2;
       int y2=y_2;

       if(x_1>x_2)
	  {
	     x1=x_2;
	     y1=y_2;

	     x2=x_1;
	     y2=y_1;
	  }

       int dx=abs(x2-x1);
       int dy=abs(y2-y1);
       int inc_dec=((y2>=y1)?1:-1);

       if(dx>dy)
	  {
	     int two_dy=(2*dy);
	     int two_dy_dx=(2*(dy-dx));
	     int p=((2*dy)-dx);

	     int x=x1;
	     int y=y1;

		 pixels[3*(x+y*kTextureSize)] = c.r;                       // Draw the current pixel
		 pixels[3*(x+y*kTextureSize)+1] = c.g; 
		 pixels[3*(x+y*kTextureSize)+2] = c.b;

	     while(x<x2)
		{
		   x++;

		   if(p<0)
		      p+=two_dy;

		   else
		      {
			 y+=inc_dec;
			 p+=two_dy_dx;
		      }

		   pixels[3*(x+y*kTextureSize)] = c.r;                       // Draw the current pixel
		   pixels[3*(x+y*kTextureSize)+1] = c.g; 
		   pixels[3*(x+y*kTextureSize)+2] = c.b;
		}
	  }

       else
	  {
	     int two_dx=(2*dx);
	     int two_dx_dy=(2*(dx-dy));
	     int p=((2*dx)-dy);

	     int x=x1;
	     int y=y1;

		 pixels[3*(x+y*kTextureSize)] = c.r;                       // Draw the current pixel
		 pixels[3*(x+y*kTextureSize)+1] = c.g; 
		 pixels[3*(x+y*kTextureSize)+2] = c.b; 

	     while(y!=y2)
		{
		   y+=inc_dec;

		   if(p<0)
		      p+=two_dx;

		   else
		      {
			 x++;
			 p+=two_dx_dy;
		      }

		   pixels[3*(x+y*kTextureSize)] = c.r;                       // Draw the current pixel
		   pixels[3*(x+y*kTextureSize)+1] = c.g; 
		   pixels[3*(x+y*kTextureSize)+2] = c.b; 
		}
	  }
	}


void MakingPicsApp::setup()
{
	frame_number_=0;
	//This is the setup that everyone needs to do
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
}

void MakingPicsApp::mouseDown( MouseEvent event )
{
}

void MakingPicsApp::update()
{
	//Get our array of pixel information
	uint8_t* dataArray = (*mySurface_).getData();

	
	for (int y = 0; y < (kAppHeight); y++) {
		for (int x = 0; x < (kAppWidth); x++) {
		dataArray[3*(y * kTextureSize + x)] = 0;
		dataArray[3*(y * kTextureSize + x) + 1] = 0;
		dataArray[3*(y * kTextureSize + x) + 2] = 0;
	}
	}
	
	// Draw Rainbow
	Color8u c;
	int row_start = 0;
	int row_max = 12;
	int total_lines = row_max * 6;
for (int h = 0; h <= 5; h++) {
	row_start = row_max * h;
	switch (h) {
		case 0:
		c = Color8u(255, 0, 0);
		break;
		case 1:
		c = Color8u(255, 165, 0);
		break;
		case 2:
		c = Color8u(255, 255, 0);
		break;
		case 3:
		c = Color8u(0, 255, 0);
		break;
		case 4:
		c = Color8u(0, 0, 255);
		break;
		case 5:
		c = Color8u(138, 43, 226);
		break;
}
	
	for (int i = 0; i < row_max; i++) {
		lines(dataArray, (int)(kAppWidth/2 + (36 * tan(3.141592654/3)) + (double)((row_start + i )/1.732050808)-10), (int)(kAppWidth/2 + 2 * scale), row_start + i + (kAppHeight/2) - total_lines/2, (int) (.8 * (row_start + i) + (kAppHeight/2) + total_lines/2), c);
		//lines(dataArray, (int)(kAppWidth/2 + (36 * tan(3.141592654/3)) + (double)((row_start + i )/1.732050808)-10), (int)(kAppWidth/2 + 2 * scale), row_start + i + (kAppHeight/2) - total_lines/2, (int) (.9) * (row_start + i + (kAppHeight/2) + total_lines/2), c);
	}
}
	//Draw Triangle
	c = Color8u(255, 255, 255);
	for (int i = 0; i < 5; i++) {
		c = Color8u(255 - 20 * i, 255 -  20* i, 255 - 20 * i);
	lines(dataArray, kAppWidth/2 - scale - i, kAppWidth/2 - i, kAppHeight/2 + scale + i, kAppHeight/2 - scale - i, c);
	lines(dataArray, kAppWidth/2 - i, kAppWidth/2 + scale - i, kAppHeight/2 - scale - i, kAppHeight/2 + scale + i, c);
	lines(dataArray, kAppWidth/2 - scale - i, kAppWidth/2 + scale + i, kAppHeight/2 + scale + i, kAppHeight/2 + scale + i, c);
	}

	// Draw Ray
	for (int i = 0; i < 5; i++) {
	lines(dataArray, kAppWidth/2 - 2 * scale, kAppWidth/2 - (int)(.5 * scale) - i, kAppHeight/2 + (int)(scale * .4) + i, kAppHeight/2 + i, Color8u(255, 255, 255));
	}

	// Draw Burst
	int offset = 0;
	int burst_size = 25;
	int i = burst_size;

	for (int x = 0; x <= burst_size; x++){
		i--;
		c = Color8u(255 - (10 * x), 255 - (10*x), 255 - (10 * x));
		for (int y = i; y <= burst_size; y++){
			offset = (3*((kAppWidth/2 - y + burst_size) * kTextureSize + (x + kAppWidth/2 - (int)(.5 * scale))));
			dataArray[offset] = c.r;
			dataArray[offset + 1] = c.g;
			dataArray[offset + 2] = c.b;
	}
		for (int y = i; y <= burst_size; y++){
			offset = (3*((y + kAppWidth/2 - burst_size) * kTextureSize + (x + kAppWidth/2 - (int)(.5 * scale))));
			dataArray[offset] = c.r;
			dataArray[offset + 1] = c.g;
			dataArray[offset + 2] = c.b;
		}

		/*
	for (int y = 0; y < burst_height; y++){
		i--;
		j++;
		c = Color8u(255 - (10 * y), 255 - (10*y), 255 - (10 * y));
		for (int x = i; x < burst_height; x++){
			offset = (3*((y + kAppWidth/2 - burst_height) * kTextureSize + (x + kAppWidth/2 - (int)(.5 * scale))));
			dataArray[offset] = c.r;
			dataArray[offset + 1] = c.g;
			dataArray[offset + 2] = c.b;
	}
		for (int x = j; x < burst_height; x++){
			offset = (3*((y + kAppWidth/2) * kTextureSize + (x + kAppWidth/2 - (int)(.5 * scale))));
			dataArray[offset] = c.r;
			dataArray[offset + 1] = c.g;
			dataArray[offset + 2] = c.b;
	}
	}
	*/
	}

	if(frame_number_ == 0){
		//writeImage("brinkmwj.png",*mySurface_);
		//We do this here, instead of setup, because we don't want to count the writeImage time in our estimate
		app_start_time_ = boost::posix_time::microsec_clock::local_time();
	}

	frame_number_++;

	//For debugging: Print the actual frames per second
	boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration msdiff = now - app_start_time_;
    console() << (1000.0*frame_number_) / msdiff.total_milliseconds() << std::endl;
}

void MakingPicsApp::draw()
{
	gl::draw(*mySurface_); 
}

CINDER_APP_BASIC( MakingPicsApp, RendererGl )
