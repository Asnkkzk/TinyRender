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
constexpr TGAColor blue    = {255,   0,   0, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

std::tuple<int,int> project(vec3 v) { // First of all, (x,y) is an orthogonal projection of the vector (x,y,z).
    return { (v.x + 1.) *  width/2,   // Second, since the input models are scaled to have fit in the [-1,1]^3 world coordinates,
             (v.y + 1.) * height/2 }; // we want to shift the vector (x,y) and then scale it to span the entire screen.
}

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
double sign(int ax,int ay,int bx,int by,int cx,int cy){
    return .5*((bx-ax)*(cy-by)-(cx-bx)*(by-ay));
}
void TriangleRasterization(int ax,int ay,int bx,int by,int cx,int cy,TGAImage &framebuffer,TGAColor color){
    //line(ax,ay,bx,by,framebuffer,red);
    //line(ax,ay,cx,cy,framebuffer,green);
    //line(bx,by,cx,cy,framebuffer,blue);
    int xmin=min(ax,min(bx,cx));
    int xmax=max(ax,max(bx,cx));
    int ymin=min(ay,min(by,cy));
    int ymax=max(ay,max(by,cy));
    double signA=sign(ax,ay, bx, by, cx, cy);
    cout<<signA<<endl;
    if(signA<0) return;

    for(int i=xmin;i<=xmax;i++){
        for(int j=ymin;j<=ymax;j++){
             double alpha=sign(i,j,bx,by,cx,cy)/signA;
             double beta=sign(i,j,cx,cy,ax,ay)/signA;
             double gamma=sign(i,j,ax,ay,bx,by)/signA;
            if(alpha<0||beta<0||gamma<0) continue;//任意一个小于0，代表此像素点在三角形外，不去渲染.
            //红绿蓝的渐变色三角形填充
            // unsigned char b=static_cast<unsigned char>(alpha*blue[0]+beta*green[0]+gamma*red[0]);
            // unsigned char g=static_cast<unsigned char>(alpha*blue[1]+beta*green[1]+gamma*red[1]);
            // unsigned char r=static_cast<unsigned char>(alpha*blue[2]+beta*green[2]+gamma*red[2]);    
            
            // framebuffer.set(i,j,{b,g,r,255});

            framebuffer.set(i,j,color);
        }
    }

}


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
        return 1;
    }
    Model model(argv[1]);
    TGAImage framebuffer(width, height, TGAImage::RGB);
    
    // int ax=0,ay=0,cx=50,cy=280,bx=260,by=70;
    // TriangleRasterization(ax,ay,bx,by,cx,cy,framebuffer);
    for (int i=0; i<model.nfaces(); i++) { // iterate through all triangles
        auto [ax, ay] = project(model.vert(i, 0));
        auto [bx, by] = project(model.vert(i, 1));
        auto [cx, cy] = project(model.vert(i, 2));
        TGAColor rnd;
        for (int c=0; c<3; c++) rnd[c] = std::rand()%255;
        TriangleRasterization(ax, ay, bx, by, cx, cy, framebuffer, rnd);
    }

    framebuffer.write_tga_file("framebuffer1.tga");
    return 0;
}