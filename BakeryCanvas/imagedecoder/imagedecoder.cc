#include "imagedecoder.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "env.h"

ImageDecoder::ImageDecoder(SDL_Surface* surface) {
    this->surface = surface;
}

ImageDecoder::~ImageDecoder() {
    SDL_FreeSurface(this->surface);
}

ImageDecoder* ImageDecoder::create(const char* filename) {
    auto surface = IMG_Load(BKEnvironment::assets(filename).c_str());
    auto decoder = new ImageDecoder(surface);
    return decoder;
}

int ImageDecoder::getWidth() {
    return this->surface->w;
}

int ImageDecoder::getHeight() {
    return this->surface->h;
}

int ImageDecoder::getPitch() {
    return this->surface->pitch;
}

unsigned char* ImageDecoder::getPixelData() {
    return (unsigned char*)this->surface->pixels;
}
