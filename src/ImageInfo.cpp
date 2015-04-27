#include <string.h>

#include "ImageLoader.h"

using namespace ImageLoader;

// Simple crate to store image id, filename, format, colorSpace (Magick::ColorSpace), width, height and mipmap
// Except mipmap, all sttributes representing attributes of original image
ImageInfo::ImageInfo(int id, char* fileName, char* format, int colorSpace, unsigned int width, unsigned int height, unsigned int* mipmap) {
	this->id = id;
	this->fileName = new char[strlen(fileName)];
	strcpy(this->fileName, fileName);
	this->format = format;
	this->colorSpace = colorSpace;
	this->width = width;
	this->height = height;
	this->mipmap = mipmap;
}

ImageInfo::~ImageInfo() {
	delete[] format;
	delete[] mipmap;
}

int ImageInfo::getId() {
	return this->id;
}

char* ImageInfo::getFileName() {
	return this->fileName;
}

char* ImageInfo::getFormat() {
	return this->format;
}

int ImageInfo::getColorSpace() {
	return this->colorSpace;
}

unsigned int ImageInfo::getWidth() {
	return this->width;
}

unsigned int ImageInfo::getHeight() {
	return this->height;
}

unsigned int* ImageInfo::getMipmap() {
	return this->mipmap;
}

void ImageInfo::setFileName(char* fileName) {
	this->fileName = fileName;
}
