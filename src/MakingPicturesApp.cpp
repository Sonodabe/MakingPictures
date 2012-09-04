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
void gradientBackground(uint8_t* data, int r1, int g1, int b1, int r2, int g2, int b2);
void drawRect(int posX, int posY, int width, int height, uint8_t* data);
void drawLine(int xOne, int yOne, int xTwo, int yTwo, uint8_t* data);
void drawPixel(int index, uint8_t* data);

//Color variables
int red = 0;
int green = 0;
int blue = 0;

double count_ = 0;
double speed = .05;

void MakingPicturesApp::setup()
{
    my_surface_ = new Surface(appWidth, appHeight, false);
    uint8_t* data = (*my_surface_).getData();
    clearBackground(data);
}

void MakingPicturesApp::mouseDown( MouseEvent event )
{
    int posX, posY;
    posX = event.getX();
    posY = event.getY();
    
    if(abs(appWidth-50-posX)<=20 && abs(50-posY)<=20)
        speed = .3-speed;
    
    
    
    
}

void MakingPicturesApp::update()
{
    int vertRectX = appWidth/2+300*sin(count_);
    int horRectY = appHeight/2+50*sin(.5*(count_+3.14/2));
    int outerCircleRad = 60+20*sin(count_+3*3.14/2);
    int innerCircleRad = 180+20*sin(count_+3*3.14/2);
    
    
    uint8_t* data_array = (*my_surface_).getData();
    gradientBackground(data_array, 80, 0, 120, 0, 0, 0);
    fill(0, 150, 150);
    drawRect(vertRectX, appHeight/2, 80, 400, data_array);
    fill(255, 0, 0);
    drawCircle2(appWidth/2-200, appHeight/2, 200, data_array, outerCircleRad);
    fill(0, 0, 255);
    drawCircle2(appWidth/2+200, appHeight/2, 200, data_array, outerCircleRad);
    fill(200, 0, 200);
    drawRect(appWidth/2, horRectY, 550, 150, data_array);
    fill(0);
    drawLine(appWidth/2, horRectY, appWidth/2-275, horRectY+75, data_array);
    drawLine(appWidth/2, horRectY, appWidth/2+275, horRectY+75, data_array);

    fill(0, 255, 0);
    drawCircle(appWidth/2, appHeight/2, innerCircleRad, data_array, 105);
    fill(0);
    drawLine(appWidth/2, horRectY, appWidth/2+275, horRectY-75, data_array);
    drawLine(appWidth/2, horRectY, appWidth/2-275, horRectY-75, data_array);

    fill(255, 0, 0);
    drawCircle2(appWidth-50, 50, 20, data_array, 20);
    
    if(speed < .3-speed){
    fill(0, 255, 255);
    } else{
        fill(0, 255, 0);
    }
    drawCircle2(appWidth-50, 50, 20, data_array, 5);

    
    count_+=speed;
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
        data[i] = 20;
    }
}

void gradientBackground(uint8_t* data, int r1, int g1, int b1, int r2, int g2, int b2){
    int index;
    double percentAcross;

    for(int tempY = 0; tempY<appHeight; tempY++){
        for(int tempX = 0; tempX<appWidth; tempX++){
            index = getIndex(tempX, tempY);
            if(index >= 0 && index < appWidth*appHeight*3){
                percentAcross = (double)tempY/appHeight;
                data[index+0] = percentAcross*r2+(1-percentAcross)*r1;
                data[index+1] = percentAcross*g2+(1-percentAcross)*g1;
                data[index+2] = percentAcross*b2+(1-percentAcross)*b1;
            }
        }
    }
}

void drawPixel(int index, uint8_t* data){
    if(index>=0 && index <appHeight*appWidth*3){
        data[index] = red;
        data[index+1] = green;
        data[index+2] = blue;
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
        drawPixel(index, data);   
        index = getIndex(i, posY-deltaY);
        drawPixel(index, data);

    }

    int deltaX;
    for(int i = max(0, posY-radius); i<min(appHeight, posY+radius); i++){
        deltaX = round(sqrt(radius*radius-(i-posY)*(i-posY)));
        index = getIndex(posX+deltaX, i);
        drawPixel(index, data);     
        index = getIndex(posX-deltaX, i);
        drawPixel(index, data);

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
        
        drawPixel(index, data);
        //Smaller numbers allow for more precise graphing... but
        //larger numbers allow for cooler patterns.
        angle+=.01;
    }
    drawCircle2(posX, posY, radius-1, data, repeats-1);
}

/**
 *  Draws and fills a rectangle with a specified center point, 
 *  width and height.
 *
 *  @param posX the x coordinate for the center point
 *  @param posY the y coordinate for the center point
 *  @param width The width of the rectangle
 *  @param height The height of the rectangle
 *  @param data The pointer to the array that the rect will be written
 **/
void drawRect(int posX, int posY, int width, int height, uint8_t* data){
    int index;
    for(int tempY = -height/2; tempY<=height/2; tempY++){
        for(int tempX = -width/2; tempX<=width/2; tempX++){
            index = getIndex(tempX+posX, tempY+posY);
            drawPixel(index, data);
        }
    }
}

/**
 *  Draws a line given the two endpoints of the line; uses:
 *             sin(angle) = deltaY /deltaX
 *  and then just uses the different x values to determine how much
 *  the deltaY is.
 *
 *  @param xOne the x coordinate of one of the end points
 *  @param yOne the y coordinate of one of the end points
 *  @param xTwo the x coordinate of the other end point
 *  @param yTwo the y coordinate of the other end point
 *  @param data the pointer of the array on which the line will
 *  be written
 **/
void drawLine(int xOne, int yOne, int xTwo, int yTwo, uint8_t* data){
    int index;
    
    if(xOne == xTwo){
        int bigY, smallY;
        if(yOne > yTwo){
            bigY = yOne;
            smallY = yTwo;
        }else{
            smallY = yOne;
            bigY = yTwo;
        }
        
        for(int i = smallY; i<=bigY; i++){
            index = getIndex(xOne, i);
            drawPixel(index, data);
        }
    }else{
            double angle;
            int tempY;
            int bigX, smallX;
            int bigY, smallY;
            //BigY smallY referring to the y coordinate corresponding to the 
            //larger/smaller x coordinate
            if(xOne >= xTwo){
                bigX = xOne;
                bigY = yOne;
                smallX = xTwo;
                smallY = yTwo;
            }else{
                bigX = xTwo;
                bigY = yTwo;
                smallX = xOne;
                smallY = yOne;
            }
            
            angle = atan((double)(bigY-smallY)/(bigX-smallX));

            for(int i = smallX; i<=bigX; i++){
                tempY = round((i-smallX)*tan(angle));
                index = getIndex(i, tempY+smallY);
                drawPixel(index, data);
        }
    }
}


void MakingPicturesApp::draw()
{
    gl::draw(*my_surface_);
}

CINDER_APP_BASIC( MakingPicturesApp, RendererGl )
