#include "ImageLoader.h"

using namespace ImageLoader;

ImageInfo::ImageInfo(int id, std::string fileName, std::string format, Magick::ColorspaceType colorSpace, size_t width, size_t height, Magick::Image* mipmap) {
        this->id = id;
        this->fileName = fileName;
        this->format = format;
        this->colorSpace = colorSpace;
        this->width = width;
        this->height = height;
        this->mipmap = mipmap;
}

int ImageInfo::getId() {
        return this->id;
}

std::string ImageInfo::getFileName() {
        return this->fileName;
}

std::string ImageInfo::getFormat() {
        return this->format;
}

Magick::ColorspaceType ImageInfo::getColorSpace() {
        return this->colorSpace;
}

size_t ImageInfo::getWidth() {
        return this->width;
}

size_t ImageInfo::getHeight() {
        return this->height;
}

Magick::Image* ImageInfo::getMipmap() {
        return this->mipmap;
}

void ImageInfo::setFileName(std::string fileName) {
        this->fileName = fileName;
}
