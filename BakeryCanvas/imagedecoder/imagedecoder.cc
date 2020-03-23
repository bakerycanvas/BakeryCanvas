#include "imagedecoder.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "env.h"
#include "logger.h"

ImageDecoder::ImageDecoder(const std::string& filename) {
    auto fullFilename = BKEnvironment::assets(filename);
    if (BKEnvironment::exists(fullFilename)) {
        auto surface = IMG_Load(fullFilename.c_str());
        this->surface = surface;
        this->isValid = true;
    } else {
        Logger::error(("[ImageDecoder] Cannot open file " + fullFilename).c_str());
    }
}

ImageDecoder::~ImageDecoder() {
    SDL_FreeSurface(this->surface);
}

ImageDecoder* ImageDecoder::create(const std::string& filename) {
    return new ImageDecoder(filename);
}

int ImageDecoder::getWidth() {
    if (this->isValid) {
        return this->surface->w;
    }
    return 0;
}

int ImageDecoder::getHeight() {
    if (this->isValid) {
        return this->surface->h;
    }
    return 0;
}

int ImageDecoder::getPitch() {
    if (this->isValid) {
        return this->surface->pitch;
    }
    return 0;
}

unsigned char* ImageDecoder::getPixelData() {
    if (this->isValid) {
        return (unsigned char*)this->surface->pixels;
    }
    return nullptr;
}
