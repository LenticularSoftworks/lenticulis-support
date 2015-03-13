#include "ImageLoader.h"

// ImageBundle implementation
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

// ImageLoader implementation
int ImageLoader::registerImage(std::string fileName) {
	// TODO
}

int registerImage(std::string fileName, std::string* format, int* colorSpace, size_t* width, size_t* height, void* mipmap ) {
	// TODO
}


int ImageLoader::getImageId(std::string fileName) {
	// TODO
}

std::string ImageLoader::getImageFileName(int id) {
	// TODO
}

std::string ImageLoader::getImageFormat(int id) {
	// TODO
}

int ImageLoader::getImageColorSpace(int id) {
	// TODO
}

size_t ImageLoader::getImageWidth(int id) {
	// TODO
}

size_t ImageLoader::getImageHeight(int id) {
	// TODO
}

void* ImageLoader::getImageMipmap(int id) {
	// TODO
}

Magick::Image* ImageLoader::getImage(int id) {
	// TODO
}

Magick::Image* ImageLoader::getImage(std::string fileName) {
	// TODO
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

int main() {
	return EXIT_SUCCESS;
}
