/*
 Brandon Sonoda
 CSE 274
 Homework One: Making Pictures
 */

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

//Screen dimensions
static const int appWidth = 800;
static const int appHeight = 600;
static const int textureSize = 1024;

//Created Methods
int getIndex(int posX, int posY);
void drawCircle(int posX, int posY, int radius, uint8_t* data);
void drawCircle(int posX, int posY, int radius, uint8_t* data, int repeats);
void drawCircle2(int posX, int posY, int radius, uint8_t* data, int repeats);
void fill(int r, int g, int b);
void fill(int greyScale);
void clearBackground(uint8_t* data);

//Color variables
int red = 0;
int green = 0;
int blue = 0;

void MakingPicturesApp::setup()
{
    my_surface_ = new Surface(appWidth, appHeight, false);
    uint8_t* data = (*my_surface_).getData();
    clearBackground(data);
}

void MakingPicturesApp::mouseDown( MouseEvent event )
{
}

void MakingPicturesApp::update()
{
    uint8_t* data_array = (*my_surface_).getData();
    clearBackground(data_array);
    
    fill(255, 0, 0);
    drawCircle2(appWidth/2-200, 250, 200, data_array, 80);
    fill(0, 0, 255);
    drawCircle2(appWidth/2+200, 250, 200, data_array, 80);
    fill(0, 255, 0);
    drawCircle(appWidth/2, 250, 200, data_array, 80);
}

void MakingPicturesApp::prepareSettings(Settings* settings){
	(*settings).setWindowSize(appWidth,appHeight);
	(*settings).setResizable(false);
}

/**
 *  Changes the red, green and blue variables... very similar to the fill method
 *  in processing.
 *
 *  @param r The new red value (0-255)
 *  @param g The new green value (0-255)
 *  @param b The new blue value (0-255)
 **/
void fill(int r, int g, int b){
    red = r;
    green = g;
    blue = b;
}

/**
 *  This method takes one variable, and changes all three of the color components
 *  to the same color, making it greyscale
 *
 *  @param greyscale The new level of greyscale (0-255)
 **/
void fill(int greyScale){
    red = greyScale;
    green = greyScale;
    blue = greyScale;
}

/**
 *  Given a set of coordinates, this method returns their location in the data_array
 *  
 *  @param posX The X coordinate of the desired point
 *  @param posY The Y coordinate of the desired point
 *
 *  @return The index of the input point
 **/
int getIndex(int posX, int posY){
    return 3*(posY*appWidth+posX);
}


/**
 *  Changes all of the values of the data array to 0
 *  
 *  @param data The array that is going to be converted to zero
 **/
void clearBackground(uint8_t* data){
    for(int i = 0; i<appWidth*appHeight*3; i++){
        data[i] = 0;
    }
}

/**
 *  Draws a circle using a formula based off of:
 *                x^2+y+2 = r^2
 *  This was found to have many when drawing though.
 *  This is a recursive method, which allows for completely filled
 *  circles along with rings
 *
 *  @param posX The x coordinate of this circle's center
 *  @param posY The y coordinate of this circle's center
 *  @param radius The radius of this circle
 *  @param data The pointer of the array which the circles will be written
 *  @param repeats The amount of times that the circle will be repeated... This 
 *  starts out with the most outward circle, and then moves inward.
 **/
void drawCircle(int posX, int posY, int radius, uint8_t* data, int repeats){
    if(repeats<=0)
        return;
    int deltaY;
    int index;

    // Draws the graph for both the x as the independent variable and
    // y as the independent variable, this makes the picture symmetrical,
    // but also makes the method less efficient.
    
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


/**
 *  Draws a circle using the sin() function, and it's periodicness
 *  This is a recursive method, which allows for completely filled
 *  circles along with rings
 *
 *  @param posX The x coordinate of this circle's center
 *  @param posY The y coordinate of this circle's center
 *  @param radius The radius of this circle
 *  @param data The pointer of the array which the circles will be written
 *  @param repeats The amount of times that the circle will be repeated... This 
 *  starts out with the most outward circle, and then moves inward.
 **/
void drawCircle2(int posX, int posY, int radius, uint8_t* data, int repeats){
    if(repeats<=0)
        return;
    int tempX, tempY;
    double angle = 0;
    
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
        //Smaller numbers allow for more precise graphing... but
        //larger numbers allow for cooler patterns.
        angle+=.01;
    }
    drawCircle2(posX, posY, radius-1, data, repeats-1);
}


void MakingPicturesApp::draw()
{
    gl::draw(*my_surface_);
}

CINDER_APP_BASIC( MakingPicturesApp, RendererGl )
