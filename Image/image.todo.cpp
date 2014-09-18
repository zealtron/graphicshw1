#include "image.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include <time.h>
// allows the use of min() and max() functions
#include <algorithm>
using namespace std;


Pixel::Pixel(const Pixel32& p)
{
    r = (float)p.r/255.0f;
    g = (float)p.g/255.0f;
    b = (float)p.b/255.0f;
}

Pixel32::Pixel32(const Pixel& p)
{
    r = (unsigned char)max(0.0f,min(255.0f,p.r*255.0f));
    g = (unsigned char)max(0.0f,min(255.0f,p.g*255.0f));
    b = (unsigned char)max(0.0f,min(255.0f,p.b*255.0f));
}

int Image32::AddRandomNoise(const float& noise,Image32& outputImage) const
{
	if ( noise < 0 || noise > 1) {
            return 0;
        }
        int count = outputImage.width()*outputImage.height();
        double counter = count * noise;
        while(counter > 0) {
            int x = rand()%outputImage.width();
            int y = rand()%outputImage.height();
            Pixel32& p = outputImage.pixel(x,y);
            p.r = rand()%255;
            p.b = rand()%255;
            p.g = rand()%255;
            counter--;
        }
	return 1;
}

int Image32::Brighten(const float& brightness,Image32& outputImage) const
{
        if(brightness < 0) {
            return 0;
        }
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++) {
                Pixel32& p = outputImage.pixel(x,y);
                p.r = (unsigned char)max(0,min(255,(int)(p.r*brightness)));
                p.g = (unsigned char)max(0,min(255,(int)(p.g*brightness)));
                p.b = (unsigned char)max(0,min(255,(int)(p.b*brightness)));
            }
        }
        return 1;
}

int Image32::Luminance(Image32& outputImage) const
{
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++) {
                Pixel32& p = outputImage.pixel(x,y);
                int l = p.r*0.3 + p.g*0.59 + p.b*0.11;
                p.r = l;
                p.g = l;
                p.b = l;
            }
        }
	return 1;
}

int Image32::Contrast(const float& contrast,Image32& outputImage) const
{
        int l = 0;
        int total = 0;
        for(int a = 0; a < outputImage.width(); a++) { //x
            for(int b = 0; b < outputImage.height(); b++) { //y
                Pixel32& p = outputImage.pixel(a,b);
                l += p.r*0.3 + p.g*0.59 + p.b*0.11;
                total++;
            }
        }
        l = l / total;
        //cout << l << " ";
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++) {
                Pixel32& p = outputImage.pixel(x,y);
                p.r = (unsigned char)max(0,min(255,(int)((p.r-l)*contrast + (255-l))));
                p.g = (unsigned char)max(0,min(255,(int)((p.g-l)*contrast + (255-l))));
                p.b = (unsigned char)max(0,min(255,(int)((p.b-l)*contrast + (255-l))));
            }
        }
	return 1;
}

int Image32::Saturate(const float& saturation,Image32& outputImage) const
{
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++){
                Pixel32& p = outputImage.pixel(x,y);
                float l = p.r*0.3 + p.g*0.59 + p.b*0.11;
                int r = (p.r-l)*saturation;
                int g = (p.g-l)*saturation;
                int b = (p.b-l)*saturation;
                p.r = (unsigned char)max(0,min(255,(int)(l+r)));
                p.g = (unsigned char)max(0,min(255,(int)(l+g)));
                p.b = (unsigned char)max(0,min(255,(int)(l+b)));
            }
        }
	return 1;
}

int Image32::Quantize(const int& bits,Image32& outputImage) const
{
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++) {
                Pixel32& p = outputImage.pixel(x,y);
                int r = floor(p.r/(256/pow(2,bits)));
                int g = floor(p.g/(256/pow(2,bits)));
                int b = floor(p.b/(256/pow(2,bits)));
                //cout << r << " " << g << " " << b << " ";
                p.r = (unsigned char)r*(256/pow(2,bits));
                p.g = (unsigned char)g*(256/pow(2,bits));
                p.b = (unsigned char)b*(256/pow(2,bits));
            }
        }
	return 1;
}

int Image32::RandomDither(const int& bits,Image32& outputImage) const
{
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++) {
                Pixel32& p = outputImage.pixel(x,y);
                /*int R = p.r;
                int G = p.g;
                int B = p.b;
                double r = floor((min(255,(int)(p.r + rand()%(R/2)))/(256/pow(2,bits))));
                double g = floor((min(255,(int)(p.g + rand()%(G/2)))/(256/pow(2,bits))));
                double b = floor((min(255,(int)(p.b + rand()%(B/2)))/(256/pow(2,bits))));
                p.r = (unsigned char)max(0,min(255,(int)(r*(256/pow(2,bits)))));
                p.g = (unsigned char)max(0,min(255,(int)(g*(256/pow(2,bits)))));
                p.b = (unsigned char)max(0,min(255,(int)(b*(256/pow(2,bits)))));
                int r = p.r;
                if(r != 0) r = rand()%r;
                int g = p.g;
                if(g != 0) g = rand()%g;
                int b = p.b;
                if(b != 0) b = rand()%b;*/
                p.r = floor((p.r + rand()%(int)(128/pow(2,bits)))*(pow(2,bits)/256))*(256/pow(2,bits));
                p.g = floor((p.g + rand()%(int)(128/pow(2,bits)))*(pow(2,bits)/256))*(256/pow(2,bits));
                p.b = floor((p.b + rand()%(int)(128/pow(2,bits)))*(pow(2,bits)/256))*(256/pow(2,bits));
            }
        }
	return 1;
}
int Image32::OrderedDither2X2(const int& bits,Image32& outputImage) const
{
        if(bits < 0) return 0;
        int d [2][2];
        d[0][0] = 1;
        d[0][1] = 3;
        d[1][0] = 4;
        d[1][1] = 2;
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++) {
                Pixel32& p = outputImage.pixel(x,y);
                int i = x%2;
                int j = y%2;
                float cr = (p.r/255)*(pow(2,bits-1));
                float cg = (p.g/255)*(pow(2,bits-1));
                float cb = (p.b/255)*(pow(2,bits-1));
                float er = cr - floor(cr);
                if(er > (d[i][j] / 5)) {
                    p.r = ceil(cr*(255/pow(2,bits-1)));
                } else {
                    p.r = floor(cr*(255/pow(2,bits-1)));
                }
                float eg = cg - floor(cg);
                if(eg > (d[i][j] / 5)) {
                    p.g = ceil(cg*(255/pow(2,bits-1)));
                } else {
                    p.g = floor(cg*(255/pow(2,bits-1)));
                }
                float eb = cb - floor(eb);
                if(eb > (d[i][j] / 5)) {
                    p.b = ceil(cb*(255/pow(2,bits-1)));
                } else { 
                    p.b = floor(cb*(255/pow(2,bits-1)));
                }
            }
        }         
	return 1;
}

int Image32::FloydSteinbergDither(const int& bits,Image32& outputImage) const
{
	return 0;
}

int Image32::Blur3X3(Image32& outputImage) const
{
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++) {
                Pixel32& p = outputImage.pixel(x,y);
                /*p.r *= 4/16;
                p.g *= 4/16;
                p.b *= 4/16;*/
                //cout << p.r << " " << p.g << " " << p.b << " ";
                int r = p.r * 4/16;
                int g = p.r * 4/16;
                int b = p.r * 4/16;
                if( x - 1 >= 0 && y - 1 >= 0) { //check upper left
                    Pixel32& pul = outputImage.pixel(x-1,y-1);
                    r += pul.r * 1/16;
                    g += pul.g * 1/16;
                    b += pul.b * 1/16;
                }
                if( x - 1 >= 0 && y + 1 < outputImage.height()) { //check lower left
                    Pixel32& pll = outputImage.pixel(x-1,y+1);
                    r += pll.r * 1/16;
                    g += pll.g * 1/16;
                    b += pll.b * 1/16;
                }
                if( x + 1 < outputImage.width() && y - 1 >= 0) { //check upper right
                    Pixel32& pur = outputImage.pixel(x+1,y-1);
                    r += pur.r * 1/16;
                    g += pur.g * 1/16;
                    b += pur.b * 1/16;
                }
                if( x + 1 < outputImage.width() && y + 1 < outputImage.height()) { //check lower right
                    Pixel32& plr = outputImage.pixel(x+1,y+1);
                    r += plr.r * 1/16;
                    g += plr.g * 1/16;
                    b += plr.b * 1/16;
                }
                if( x - 1 >= 0) { //check left
                    Pixel32& pl = outputImage.pixel(x-1,y);
                    r += pl.r * 2/16;
                    g += pl.g * 2/16;
                    b += pl.b * 2/16;
                }
                if( y - 1 >= 0) { //check up
                    Pixel32& pu = outputImage.pixel(x,y-1);
                    r *= pu.r * 2/16;
                    g *= pu.g * 2/16;
                    b *= pu.b * 2/16;
                }
                if( x + 1 < outputImage.width()) { //check right
                    Pixel32& pr = outputImage.pixel(x+1,y);
                    r += pr.r * 2/16;
                    g += pr.g * 2/16;
                    b += pr.b * 2/16;
                }
                if( y + 1 < outputImage.height()) { //check down
                    Pixel32& pd = outputImage.pixel(x,y+1);
                    r += pd.r * 2/16;
                    g += pd.g * 2/16;
                    b += pd.b * 2/16;
                }
                p.r = r;
                p.g = g;
                p.b = b;
            }   
        }
	return 1;
}

int Image32::EdgeDetect3X3(Image32& outputImage) const
{
	return 0;
}
int Image32::ScaleNearest(const float& scaleFactor,Image32& outputImage) const
{
	return 0;
}

int Image32::ScaleBilinear(const float& scaleFactor,Image32& outputImage) const
{
	return 0;
}

int Image32::ScaleGaussian(const float& scaleFactor,Image32& outputImage) const
{
	return 0;
}

int Image32::RotateNearest(const float& angle,Image32& outputImage) const
{
	return 0;
}

int Image32::RotateBilinear(const float& angle,Image32& outputImage) const
{
	return 0;
}
	
int Image32::RotateGaussian(const float& angle,Image32& outputImage) const
{
	return 0;
}


int Image32::SetAlpha(const Image32& matte)
{
	return 0;
}

int Image32::Composite(const Image32& overlay,Image32& outputImage) const
{
	return 0;
}

int Image32::CrossDissolve(const Image32& source,const Image32& destination,const float& blendWeight,Image32& ouputImage)
{
	return 0;
}
int Image32::Warp(const OrientedLineSegmentPairs& olsp,Image32& outputImage) const
{
	return 0;
}

int Image32::FunFilter(Image32& outputImage) const
{
	return 0;
}

int Image32::Crop(const int& x1,const int& y1,const int& x2,const int& y2,Image32& outputImage) const
{
        int width = outputImage.width();
        int height = outputImage.height();
        if(x1 > width || x2 > width || y1 > height || y2 > height) {
            cout << "Out of range" << "\n";
            return 0;
        }
        Pixel32 ar [width][height];
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++) {
                ar[x][y] = outputImage.pixel(x,y);
            }
        }
        outputImage.setSize(abs(x1-x2),abs(y1-y2));
        int countx = 0;
        int county = 0;
        for(int w = x1; w < x2; w++) {
            for(int h = y1; h < y2; h++) {
                Pixel32& p = outputImage.pixel(countx,county);
                p = ar[w][h];
                county++;
            }
            county = 0;
            countx++;
        }
	return 1;
}

Pixel32 Image32::NearestSample(const float& x,const float& y) const
{
	return Pixel32();
}

Pixel32 Image32::BilinearSample(const float& x,const float& y) const
{
	return Pixel32();
}

Pixel32 Image32::GaussianSample(const float& x,const float& y,const float& variance,const float& radius) const
{
	return Pixel32();
}
