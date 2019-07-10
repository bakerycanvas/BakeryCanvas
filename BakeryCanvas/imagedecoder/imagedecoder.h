#ifndef BAKERYCANVAS_IMAGEDECODER_H
#define BAKERYCANVAS_IMAGEDECODER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class ImageDecoder {
    public:
    static ImageDecoder* create(const std::string& filename);

    explicit ImageDecoder(const std::string& filename);
    ~ImageDecoder();

    int getWidth();
    int getHeight();
    int getPitch();
    unsigned char* getPixelData();

    private:
    bool isValid = false;
    SDL_Surface* surface = nullptr;
};

#endif  // BAKERYCANVAS_IMAGEDECODER_H
