#include "CImg.h"
using namespace cimg_library;

int main() {
	CImg<unsigned char> img(256,256,1,3); //define 256 by 256 color image with 8 bits per color
	img.fill(0); //set pixel values to 0 (black)
	unsigned char RGB[] = {0,0,0}; //define color array
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			img.draw_point(i,j,RGB);
			RGB[0]++;	//increment red by 1
		}
		RGB[2]++; //increment blue by 1
	}
	img.display("My first CImg code"); //display image in display window
	return 0;
}