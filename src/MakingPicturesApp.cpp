/**
 Brandon Sonoda
 CSE 274
 Homework One: Making Pictures
 **/

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MakingPicturesApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void prepareSettings(Settings* settings);
    
private:
    Surface* my_surface_;
};

static const int appWidth = 800;
static const int appHeight = 600;
static const int textureSize = 1024;

int getIndex(int posX, int posY);
void drawCircle(int posX, int posY, int radius, uint8_t* data);
void drawCircle(int posX, int posY, int radius, uint8_t* data, int repeats);
void fill(int r, int g, int b);
void fll(int greyScale);
int red = 0;
int green = 0;
int blue = 0;


void MakingPicturesApp::setup()
{
    my_surface_ = new Surface(appWidth, appHeight, false);
    uint8_t* data = (*my_surface_).getData();
    for(int i = 0; i<appWidth*appHeight*3; i++){
            data[i] = 255;
    }
}

void MakingPicturesApp::mouseDown( MouseEvent event )
{
}

void MakingPicturesApp::update()
{
    uint8_t* data_array = (*my_surface_).getData();
    fill(255, 0, 0);
    drawCircle(appWidth/2-200, 250, 200, data_array);//, 60);
    fill(0, 0, 255);
    drawCircle(appWidth/2+150, 250, 200, data_array);//, 60);
    fill(0, 255, 0);
    drawCircle(appWidth/2, 250, 200, data_array);//, 60);

}

void fill(int r, int g, int b){
    red = r;
    green = g;
    blue = b;
}

void fill(int greyScale){
    red = greyScale;
    green = greyScale;
    blue = greyScale;
}

int getIndex(int posX, int posY){
    return 3*(posY*appWidth+posX);
}
void MakingPicturesApp::prepareSettings(Settings* settings){
	(*settings).setWindowSize(appWidth,appHeight);
	(*settings).setResizable(false);
}

void drawCircle(int posX, int posY, int radius, uint8_t* data){
    int tempX, tempY;
    double angle;
    
    int index;
    
    while(angle < 6.28){
        tempX = posX+radius*sin(angle+3.14/2);
        tempY = posY+radius*sin(angle);
        index = getIndex(tempX, tempY);
        
        if(index >= 0 && index < 3*appWidth*appHeight){
            data[index] = red;
            data[index+1] = green;
            data[index+2] = blue;
        }
        
        angle+=.001;
    }
}

void drawCircle(int posX, int posY, int radius, uint8_t* data, int repeats){
    if(repeats<=0)
        return;
    int deltaY;
    int index;
    for(int i = max(0, posX-radius); i<min(appWidth, posX+radius); i++){
        deltaY = round(sqrt(radius*radius-(i-posX)*(i-posX)));
        index = getIndex(i, posY+deltaY);
        if(index>=0 && index <appHeight*appWidth*3){
            data[index] = red;
            data[index+1] = green;
            data[index+2] = blue;
        }       
        index = getIndex(i, posY-deltaY);
        if(index>=0 && index <appHeight*appWidth*3){
            data[index] = red;
            data[index+1] = green;
            data[index+2] = blue;
        }
    }
    int deltaX;
    for(int i = max(0, posY-radius); i<min(appHeight, posY+radius); i++){
        deltaX = round(sqrt(radius*radius-(i-posY)*(i-posY)));
        index = getIndex(posX+deltaX, i);
        if(index>=0 && index <appHeight*appWidth*3){
            data[index] = red;
            data[index+1] = green;
            data[index+2] = blue;
        }       
        index = getIndex(posX-deltaX, i);
        if(index>=0 && index <appHeight*appWidth*3){
            data[index] = red;
            data[index+1] = green;
            data[index+2] = blue;
        }
    }
    drawCircle(posX, posY, radius-1, data, repeats-1);
}

void MakingPicturesApp::draw()
{
    gl::draw(*my_surface_);
}

CINDER_APP_BASIC( MakingPicturesApp, RendererGl )
