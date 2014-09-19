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
        int seed = time(NULL);
        srand(seed);
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
                int r = floor(p.r*(pow(2,bits)/256));
                int g = floor(p.g*(pow(2,bits)/256));
                int b = floor(p.b*(pow(2,bits)/256));
                //cout << r << " " << g << " " << b << " ";
                p.r = (unsigned char)r*(255/(pow(2,bits)-1));//*(256/pow(2,bits));
                p.g = (unsigned char)g*(255/(pow(2,bits)-1));//*(256/pow(2,bits));
                p.b = (unsigned char)b*(255/(pow(2,bits)-1));//*(256/pow(2,bits));
            }
        }
	return 1;
}

int Image32::RandomDither(const int& bits,Image32& outputImage) const
{
        int seed = time(NULL);
        srand(seed);
        if(bits < 0) return 0;
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
                int noise = rand()%(int)(255/(pow(2,bits)-1));
                /*if(noise < p.r && noise < p.g && noise < p.b) {
                    p.r = 255;
                    p.g = 255;
                    p.b = 255;
                } else {*/
                p.r = (unsigned char)max(0,min(255,(int)(floor((p.r + noise)*(pow(2,bits)/256))*(255/pow(2,bits)-1))));
                p.g = (unsigned char)max(0,min(255,(int)(floor((p.g + noise)*(pow(2,bits)/256))*(255/pow(2,bits)-1))));
                p.b = (unsigned char)max(0,min(255,(int)(floor((p.b + noise)*(pow(2,bits)/256))*(255/pow(2,bits)-1))));
            }
        }
	return 1;
}
int Image32::OrderedDither2X2(const int& bits,Image32& outputImage) const
{
        if(bits < 0) return 0;
        float d [2][2];
        d[0][0] = 1.0f;
        d[0][1] = 3.0f;
        d[1][0] = 4.0f;
        d[1][1] = 2.0f;
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++) {
                Pixel32& p = outputImage.pixel(x,y);
                int i = x%2;
                int j = y%2;
                int r = p.r;
                int g = p.g;
                int b = p.b;
                float cr = r*((pow(2,bits)-1)/255);
                float cg = g*((pow(2,bits)-1)/255);
                float cb = b*((pow(2,bits)-1)/255);
                float er = cr - floor(cr);
                //cout << cr << " " << cg << " " << cb << " ";
                if(er > (d[i][j] / 5)) {
                    p.r = (unsigned char)max(0,min(255,(int)(ceil(cr)*(255/(pow(2,bits)-1)))));
                } else {
                    p.r = (unsigned char)max(0,min(255,(int)(floor(cr)*(255/(pow(2,bits)-1)))));
                }
                float eg = cg - floor(cg);
                if(eg > (d[i][j] / 5)) {
                    p.g = (unsigned char)max(0,min(255,(int)(ceil(cg)*(255/(pow(2,bits)-1)))));
                } else {
                    p.g = (unsigned char)max(0,min(255,(int)(floor(cg)*(255/(pow(2,bits)-1)))));
                }
                float eb = cb - floor(cb);
                if(eb > (d[i][j] / 5)) {
                    p.b = (unsigned char)max(0,min(255,(int)(ceil(cb)*(255/(pow(2,bits)-1)))));
                } else { 
                    p.b = (unsigned char)max(0,min(255,(int)(floor(cb)*(255/(pow(2,bits)-1)))));
               }
            }
        }         
	return 1;
}

int Image32::FloydSteinbergDither(const int& bits,Image32& outputImage) const
{
        if(bits < 0) return 0;
        /*float fsr[(int)outputImage.width()][(int)outputImage.height]; //red error
        fsr[0][0] = 0f;
        float fsg[(int)outputImage.width()][(int)outputImage.height]; //green error
        fsg[0][0] = 0f;
        float fsb[(int)outputImage.width()][(int)outputImage.height]; // blue error
        fsb[0][0] = 0f;*/
        /*Pixel32 fs[outputImage.width()][outputImage.height()];
        for(int a = 0; a < outputImage.width(); a++) { //initializing buffer with image
            for(int b = 0; b < outputImage.height(); b++) {
                fs[a][b] = outputImage.pixel(a,b);
            }
        }*/
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++) {
                Pixel32& p = outputImage.pixel(x,y);
                float r = floor(p.r*(pow(2,bits)/256))*(255/(pow(2,bits)-1));
                float er = p.r - r;
                float g = floor(p.g*(pow(2,bits)/256))*(255/(pow(2,bits)-1));
                float eg = p.g - g;
                float b = floor(p.b*(pow(2,bits)/256))*(255/(pow(2,bits)-1));
                float eb = p.b - b;
                if(y + 1 < outputImage.height()) { // checks right; alpha 7/16
                    Pixel32& pr = outputImage.pixel(x,y+1);
                    pr.r = (unsigned char)max(0,min(255,(int)(pr.r + ((7*er)/16))));
                    pr.g = (unsigned char)max(0,min(255,(int)(pr.g + ((7*eg)/16))));
                    pr.b = (unsigned char)max(0,min(255,(int)(pr.b + ((7*eb)/16))));
                    /*fsr[x][y+1] += (7*er)/16;
                    fsg[x][y+1] += (7*eg)/16;
                    fsb[x][y+1] += (7*eb)/16;
                    fs[x][y+1].r += (7*er)/16;
                    fs[x][y+1].g += (7*eg)/16;
                    fs[x][y+1].b += (7*eb)/16;*/
                }
                if(x + 1 < outputImage.width() && y - 1 >= 0) { // check lower left; beta 3/16
                    Pixel32& pll = outputImage.pixel(x+1,y-1);
                    pll.r = (unsigned char)max(0,min(255,(int)(pll.r + ((3*er)/16))));
                    pll.g = (unsigned char)max(0,min(255,(int)(pll.g + ((3*eg)/16))));
                    pll.b = (unsigned char)max(0,min(255,(int)(pll.b + ((3*eb)/16))));
                    /*fs[x+1][y-1].r += (3*er)/16;
                    fs[x+1][y-1].g += (3*eg)/16;
                    fs[x+1][y-1].b += (3*eb)/16;*/  
                }
                if(x+1 < outputImage.width()) { //checks down; gamma 5/16
                    Pixel32& pd = outputImage.pixel(x+1,y);
                    pd.r = (unsigned char)max(0,min(255,(int)(pd.r + ((5*er)/16))));
                    pd.g = (unsigned char)max(0,min(255,(int)(pd.g + ((5*eg)/16))));
                    pd.b = (unsigned char)max(0,min(255,(int)(pd.b + ((5*eb)/16))));
                    /*fs[x+1][y].r += (5*er)/16;
                    fs[x+1][y].g += (5*eg)/16;
                    fs[x+1][y].b += (5*eb)/16;*/
                }
                if(x+1 < outputImage.width() && y+1 < outputImage.height()) { //checks lower right; delta 1/16
                    Pixel32& plr = outputImage.pixel(x+1,y+1);
                    plr.r = (unsigned char)max(0,min(255,(int)(plr.r + (er/16))));
                    plr.g = (unsigned char)max(0,min(255,(int)(plr.g + (eg/16))));
                    plr.b = (unsigned char)max(0,min(255,(int)(plr.b + (eb/16))));
                    /*fs[x+1][y+1].r += (1*er)/16;
                    fs[x+1][y+1].g += (1*eg)/16;
                    fs[x+1][y+1].b += (1*eb)/16;*/
                }
            }
        }
       /* for(int w = 0; w < outputImage.width(); w++) { //copying buffer....
            for(int h = 0; h < outputImage.height(); h++) {
                Pixel32& pix = outputImage.pixel(w,h);
                pix = fs[w][h];
            }
        }*/ 
	return 1;
}

int Image32::Blur3X3(Image32& outputImage) const
{
        Pixel32 pblur[outputImage.width()][outputImage.height()];
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++) {
                Pixel32& p = outputImage.pixel(x,y);
                /*p.r *= 4/16;
                p.g *= 4/16;
                p.b *= 4/16;*/
                //cout << p.r << " " << p.g << " " << p.b << " ";
                float r = p.r * 4/16;
                float g = p.r * 4/16;
                float b = p.r * 4/16;
                int mask = 4;
                //cout << mask << " ";
                if( x - 1 >= 0 && y - 1 >= 0) { //check upper left
                    Pixel32& pul = outputImage.pixel(x-1,y-1);
                    r += pul.r * 1/16;
                    g += pul.g * 1/16;
                    b += pul.b * 1/16;
                    mask += 1;
                }
                if( x - 1 >= 0 && y + 1 < outputImage.height()) { //check lower left
                    Pixel32& pll = outputImage.pixel(x-1,y+1);
                    r += pll.r * 1/16;
                    g += pll.g * 1/16;
                    b += pll.b * 1/16;
                    mask += 1;
                }
                if( x + 1 < outputImage.width() && y - 1 >= 0) { //check upper right
                    Pixel32& pur = outputImage.pixel(x+1,y-1);
                    r += pur.r * 1/16;
                    g += pur.g * 1/16;
                    b += pur.b * 1/16;
                    mask += 1;
                }
                if( x + 1 < outputImage.width() && y + 1 < outputImage.height()) { //check lower right
                    Pixel32& plr = outputImage.pixel(x+1,y+1);
                    r += plr.r * 1/16;
                    g += plr.g * 1/16;
                    b += plr.b * 1/16;
                    mask += 1;
                }
                if( x - 1 >= 0) { //check left
                    Pixel32& pl = outputImage.pixel(x-1,y);
                    r += pl.r * 2/16;
                    g += pl.g * 2/16;
                    b += pl.b * 2/16;
                    mask += 2;
                }
                if( y - 1 >= 0) { //check up
                    Pixel32& pu = outputImage.pixel(x,y-1);
                    r += pu.r * 2/16;
                    g += pu.g * 2/16;
                    b += pu.b * 2/16;
                    mask += 2;
                }
                if( x + 1 < outputImage.width()) { //check right
                    Pixel32& pr = outputImage.pixel(x+1,y);
                    r += pr.r * 2/16;
                    g += pr.g * 2/16;
                    b += pr.b * 2/16;
                    mask += 2;
                }
                if( y + 1 < outputImage.height()) { //check down
                    Pixel32& pd = outputImage.pixel(x,y+1);
                    r += pd.r * 2/16;
                    g += pd.g * 2/16;
                    b += pd.b * 2/16;
                    mask += 2;
                }
                //cout << mask << " ";
                float maskr = mask/16;
                pblur[x][y].r = r/maskr;
                pblur[x][y].g = g/maskr;
                pblur[x][y].b = b/maskr;
            }   
        }
        for(int w = 0; w < outputImage.width(); w++) {
            for(int h = 0; h < outputImage.height(); h++) {
                Pixel32& pix = outputImage.pixel(w,h);
                pix = pblur[w][h];
            }
        }
        //outputImage = blur;
	return 1;
}

int Image32::EdgeDetect3X3(Image32& outputImage) const
{
        Pixel32 edgy[outputImage.width()][outputImage.height()];
        for(int x = 0; x < outputImage.width(); x++) {
            for(int y = 0; y < outputImage.height(); y++) {
                Pixel32& p = outputImage.pixel(x,y);
                /*p.r *= 4/16;
                p.g *= 4/16;
                p.b *= 4/16;*/
                //cout << p.r << " " << p.g << " " << p.b << " ";
                int r = p.r * 8;
                int g = p.g * 8;
                int b = p.b * 8;
                if( x - 1 >= 0 && y - 1 >= 0) { //check upper left
                    Pixel32& pul = outputImage.pixel(x-1,y-1);
                    r += (pul.r * -1);
                    g += (pul.g * -1);
                    b += (pul.b * -1);
                }
                if( x - 1 >= 0 && y + 1 < outputImage.height()) { //check lower left
                    Pixel32& pll = outputImage.pixel(x-1,y+1);
                    r += (pll.r * -1);
                    g += (pll.g * -1);
                    b += (pll.b * -1);
                }
                if( x + 1 < outputImage.width() && y - 1 >= 0) { //check upper right
                    Pixel32& pur = outputImage.pixel(x+1,y-1);
                    r += (pur.r * -1);
                    g += (pur.g * -1);
                    b += (pur.b * -1);
                }
                if( x + 1 < outputImage.width() && y + 1 < outputImage.height()) { //check lower right
                    Pixel32& plr = outputImage.pixel(x+1,y+1);
                    r += (plr.r * -1);
                    g += (plr.g * -1);
                    b += (plr.b * -1);
                }
                if( x - 1 >= 0) { //check left
                    Pixel32& pl = outputImage.pixel(x-1,y);
                    r += (pl.r * -1);
                    g += (pl.g * -1);
                    b += (pl.b * -1);
                }
                if( y - 1 >= 0) { //check up
                    Pixel32& pu = outputImage.pixel(x,y-1);
                    r += (pu.r * -1);
                    g += (pu.g * -1);
                    b += (pu.b * -1);
                }
                if( x + 1 < outputImage.width()) { //check right
                    Pixel32& pr = outputImage.pixel(x+1,y);
                    r += (pr.r * -1);
                    g += (pr.g * -1);
                    b += (pr.b * -1);
                }
                if( y + 1 < outputImage.height()) { //check down
                    Pixel32& pd = outputImage.pixel(x,y+1);
                    r += (pd.r * -1);
                    g += (pd.g * -1);
                    b += (pd.b * -1);
                }
                //cout << r << " " << g << " " << b << " ";
                edgy[x][y].r = (unsigned char)max(0,min(255,r));
                edgy[x][y].g = (unsigned char)max(0,min(255,g));
                edgy[x][y].b = (unsigned char)max(0,min(255,b));
            }   
        }
        for(int w = 0; w < outputImage.width(); w++) {
            for(int h = 0; h < outputImage.height(); h++) {
                Pixel32& pix = outputImage.pixel(w,h);
                pix = edgy[w][h];
            }
        }
	return 1;
}
int Image32::ScaleNearest(const float& scaleFactor,Image32& outputImage) const
{
        int w = (int)(outputImage.width()*scaleFactor);
        int h = (int)(outputImage.height()*scaleFactor);
        //Image32 img = outputImage;
        //img.setSize(w,h);
        Pixel32 img[w][h];
        for(int x = 0; x < w; x++) {
            for(int y = 0; y < h; y++) {
                float u = x / scaleFactor;
                float v = y / scaleFactor;
                Pixel32 p = outputImage.NearestSample(u,v);
                img[x][y] = p;
            }
        }
        outputImage.setSize(w,h);
        for(int a = 0; a < w; a++) { //copying
            for(int b = 0; b < h; b++) {
                outputImage.pixel(a,b) = img[a][b];
            }
        }   
	return 1;
}

int Image32::ScaleBilinear(const float& scaleFactor,Image32& outputImage) const
{
        int w = (int)(outputImage.width()*scaleFactor);
        int h = (int)(outputImage.height()*scaleFactor);
        //Image32 img = outputImage;
        //img.setSize(w,h);
        Pixel32 img[w][h];
        for(int x = 0; x < w; x++) {
            for(int y = 0; y < h; y++) {
                float u = x / scaleFactor;
                float v = y / scaleFactor;
                Pixel32 p = outputImage.BilinearSample(u,v);
                img[x][y] = p;
            }
        }
        outputImage.setSize(w,h);
        for(int a = 0; a < w; a++) { //copying
            for(int b = 0; b < h; b++) {
                outputImage.pixel(a,b) = img[a][b];
            }
        } 
	return 1;
}

int Image32::ScaleGaussian(const float& scaleFactor,Image32& outputImage) const
{
	return 0;
}

int Image32::RotateNearest(const float& angle,Image32& outputImage) const
{
        double hyp = sqrt(pow(outputImage.width(),2) + pow(outputImage.height(),2));
        //double arcsin = asin(outputImage.height()/hyp) * (180/ PI);
        int w = ceil(hyp);
        int h = ceil(hyp);
        //cout << arcsin << " ";
        //int w = (int)(outputImage.width()*sin(-angle));
        //int h = (int)(outputImage.height()*cos(-angle));
        //cout << w << " " << h << " ";i
        float shift = hyp*(sin(angle*(PI/180))*(angle/90));///(ceil(angle/45));
        float factor = hyp*(cos(angle*(PI/180))*(angle/90));///(ceil(angle/45));
        Pixel32 img[w][h];
        for(int x = 0; x < w; x++) {
            for(int y = 0; y < h; y++) {
                float u = x*cos(-(angle*(PI/180))) - y*sin(-(angle*(PI/180))) - factor;
                float v = x*sin(-(angle*(PI/180))) + y*cos(-(angle*(PI/180))) + shift;
                img[x][y] = NearestSample(u,v);
            }
        }     
        outputImage.setSize(w,h);
        for(int i = 0; i < w; i++) {
            for(int j = 0; j < h; j++) {
                Pixel32& p = outputImage.pixel(i,j);
                p = img[i][j];
            }
        }
	return 1;
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
        //cout << x << " " << y << " ";
        if( x >= 0 && x < this->width()-0.5 && y >= 0 && y < this->height()-0.5) {
            int u = floor(x + 0.5);
            int v = floor(y + 0.5);
            Pixel32 p = this->pixel(u,v);
	    return p;
        } else {
            return Pixel32();
        }
}

Pixel32 Image32::BilinearSample(const float& x,const float& y) const
{
        int x1 = floor(x);
        int x2 = x1 + 1;
        int y1 = floor(y);
        int y2 = y1 + 1;
        float dx = x - x1;
        float dy = y - y1;
        Pixel32 s1;// = this->pixel(x1,y1); //src 1
        if(x1 >= 0 && x2 >= 0) {   
            s1 = this->pixel(x1,y1);
            s1.r = (unsigned char)max(0,min(255,(int)(s1.r*(1-dx))));
            s1.g = (unsigned char)max(0,min(255,(int)(s1.g*(1-dx))));
            s1.b = (unsigned char)max(0,min(255,(int)(s1.b*(1-dx))));
        }
        Pixel32 s2; //src 2
        if(x2 < this->width() && x2 >= 0) {
            s2 = this->pixel(x2,y1);
            s2.r = (unsigned char)max(0,min(255,(int)(s2.r*dx)));
            s2.g = (unsigned char)max(0,min(255,(int)(s2.g*dx)));
            s2.b = (unsigned char)max(0,min(255,(int)(s2.b*dx)));
        }
        Pixel32 s3; //src 3i
        if(y2 < this->height() && y2 >= 0) {
            s3 = this->pixel(x1,y2);
            s3.r = (unsigned char)max(0,min(255,(int)(s3.r*(1-dx))));
            s3.g = (unsigned char)max(0,min(255,(int)(s3.g*(1-dx))));
            s3.b = (unsigned char)max(0,min(255,(int)(s3.b*(1-dx))));  
        }
        Pixel32 s4; //src4
        if(x2 < this->width() && y2 < this->height() && x2 >= 0 && y2 >= 0) {
            s4 = this->pixel(x2,y2);
            s4.r = (unsigned char)max(0,min(255,(int)(s4.r*dx)));
            s4.g = (unsigned char)max(0,min(255,(int)(s4.g*dx)));
            s4.b = (unsigned char)max(0,min(255,(int)(s4.b*dx))); 
        }
        Pixel32* a = new Pixel32();
        Pixel32* b = new Pixel32();
        
        a->r = (unsigned char)max(0,min(255,s1.r + s2.r));
        a->g = (unsigned char)max(0,min(255,s1.g + s2.g));
        a->b = (unsigned char)max(0,min(255,s1.b + s2.b));
        b->r = (unsigned char)max(0,min(255,s3.r + s4.r));
        b->g = (unsigned char)max(0,min(255,s3.g + s4.g));
        b->b = (unsigned char)max(0,min(255,s3.b + s4.b)); 
        Pixel32 dst;
        dst.r = (unsigned char)max(0,min(255,(int)(a->r*(1-dy) + b->r*dy)));
        dst.g = (unsigned char)max(0,min(255,(int)(a->g*(1-dy) + b->g*dy)));
        dst.b = (unsigned char)max(0,min(255,(int)(a->b*(1-dy) + b->b*dy))); 
	return dst;
}

Pixel32 Image32::GaussianSample(const float& x,const float& y,const float& variance,const float& radius) const
{
	return Pixel32();
}
