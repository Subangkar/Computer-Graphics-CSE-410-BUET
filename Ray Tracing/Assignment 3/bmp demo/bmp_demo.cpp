#include "bitmap_image.hpp"

int main()
{
    bitmap_image image(500,500);
    for (int x = 0; x < 200; x++) {
        for (int y = 0; y < 200; y++) {
            image.set_pixel(x, y, 255, 0,0);
        }
    }
    image.save_image("out.bmp");
    return 0;
}
