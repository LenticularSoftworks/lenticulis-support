#include <cstring>

#include "ImageLoader.h"

const int ImageLoader::MIPMAP_SIZE = 768;

const int IMAGE_OK = 1;
const int IMAGE_NOT_FOUND = 2;
const int IMAGE_CORRUPTED = 3;
const int IMAGE_DEPTH_UNSUPPORTED = 4;
const int IMAGE_FORMAT_UNSUPPORTED = 5;

// ImageLoader implementation
int ImageLoader::registerImage(char* fileName) {
	// TODO
	return 0;
}

int registerImageP(char* fileName, char** format, int* colorSpace, size_t* width, size_t* height, void* mipmap ) {
	// TODO
	return 0;
}


int ImageLoader::getImageId(char* fileName) {
	// TODO
	return 0;
}

const char* ImageLoader::getImageFileName(int id) {
	// TODO
	return std::string("").c_str();
}

const char* ImageLoader::getImageFormat(int id) {
	// TODO
	return std::string("").c_str();
}

int ImageLoader::getImageColorSpace(int id) {
	// TODO
	return 0;
}

unsigned int ImageLoader::getImageWidth(int id) {
	// TODO
	return 0;
}

unsigned int ImageLoader::getImageHeight(int id) {
	// TODO
	return 0;
}

void* ImageLoader::getImageMipmap(int id) {
	// TODO
	return nullptr;
}

Magick::Image* ImageLoader::getImage(int id) {
	// TODO
	return nullptr;
}

Magick::Image* ImageLoader::getImage(std::string fileName) {
	// TODO
	return nullptr;
}

Magick::Image* ImageLoader::createMipmap(Magick::Image* img_ptr) {
	size_t col = 0, row = 0;
	int x_size = MIPMAP_SIZE, y_size = MIPMAP_SIZE;

	// Construct initial string, '3/2' constant is safe here as we have fixed mipmap size
	std::string size_str = std::to_string((int)(x_size * 3/2)) + std::string("x") + std::to_string(y_size);

	Magick::Image* mmap_ptr = new Magick::Image(size_str, Magick::Color("white"));
	while (x_size > 0 && y_size > 0) {
		// Construct new Geometry string
		size_str = std::to_string(x_size) + std::string("x") + std::to_string(y_size);

		// Resize current image and composite it to mipmap
		img_ptr->scale(size_str);
		mmap_ptr->composite(*img_ptr, col, row);	

		// Update col only on first loop, 
		// update row on all loops except first
		col = x_size == MIPMAP_SIZE ? img_ptr->columns() : col;
		row = y_size == MIPMAP_SIZE ? 0 : row + img_ptr->rows();
	
		x_size /= 2, y_size /= 2;					
	}

	return mmap_ptr;
}
