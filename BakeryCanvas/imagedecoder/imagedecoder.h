#ifndef BAKERYCANVAS_IMAGEDECODER_H
#define BAKERYCANVAS_IMAGEDECODER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class ImageDecoder {
    public:
    static ImageDecoder* create(const char* filename);

    explicit ImageDecoder(SDL_Surface* surface);
    ~ImageDecoder();

    int getWidth();
    int getHeight();
    int getPitch();
    unsigned char* getPixelData();

    private:
    SDL_Surface* surface = nullptr;
};

#endif  // BAKERYCANVAS_IMAGEDECODER_H
