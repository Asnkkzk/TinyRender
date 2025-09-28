#include <cmath>
#include <tuple>
#include "geometry.h"
#include "model.h"
#include "tgaimage.h"
using namespace std;

constexpr int width  = 300;
constexpr int height = 300;

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    bool steep = std::abs(ax-bx) < std::abs(ay-by);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax>bx) { // make it left−to−right
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int y = ay;
    int ierror = 0;
    for (int x=ax; x<=bx; x++) {
        if (steep) // if transposed, de−transpose
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
        ierror += 2 * std::abs(by-ay);
        if (ierror > bx - ax) {
            y += by > ay ? 1 : -1;
            ierror -= 2 * (bx-ax);
        }
    }
}
int sign(int ax,int ay,int bx,int by,int cx,int cy){
    int ans=(bx-ax)*(cy-by)-(cx-bx)*(by-ay);
    if(ans>0) return 1;
    else return -1;
}
void TriangleRasterization(int ax,int ay,int bx,int by,int cx,int cy,TGAImage &framebuffer){
    line(ax,ay,bx,by,framebuffer,red);
    line(ax,ay,cx,cy,framebuffer,green);
    line(bx,by,cx,cy,framebuffer,blue);
    int xmin=min(ax,min(bx,cx));
    int xmax=max(ax,max(bx,cx));
    int ymin=min(ay,min(by,cy));
    int ymax=max(ay,max(by,cy));
    int signA=sign(ax,ay, bx, by, cx, cy);
    cout<<signA<<endl;
    TGAColor rnd;

    for(int i=xmin;i<=xmax;i++){
        for(int j=ymin;j<=ymax;j++){
             int alpha=sign(i,j,bx,by,cx,cy)/signA;
             int beta=sign(i,j,cx,cy,ax,ay)/signA;
             int gamma=sign(i,j,ax,ay,bx,by)/signA;
            if(alpha+beta+gamma!=3) continue;
            for (int c=0; c<3; c++) rnd[c] = std::rand()%255;
            framebuffer.set(i,j,rnd);
        }
    }

}


int main(int argc, char** argv) {

    TGAImage framebuffer(width, height, TGAImage::RGB);
    
    int ax=0,ay=0,bx=50,by=280,cx=260,cy=70;
    TriangleRasterization(ax,ay,bx,by,cx,cy,framebuffer);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}