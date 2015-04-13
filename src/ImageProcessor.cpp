#include <string>

#include "ImageLoader.h"
#include "ImageProcessor.h"

void ImageProcessor::initializeCanvas(unsigned int width, unsigned int height) {
	std::string size_str = std::to_string(width) + std::string("x") + std::to_string(height);
	ImageProcessor::canvas = new Magick::Image(size_str, Magick::Color("white"));
}

int ImageProcessor::loadImage(int id) {
	Magick::Image* img = ImageLoader::getImage(id);

	// Check if image is registered
	if (!img) {
		return ImageProcessor::ProcessReturnCodes::PROCESS_IMAGE_NOT_REGISTERED;
	}

	img->backgroundColor(Magick::Color("transparent"));
	ImageProcessor::curr = img;
	return ImageProcessor::ProcessReturnCodes::PROCESS_OK;;
}

int ImageProcessor::resizeImage(unsigned int width, unsigned int height) {

	// Check if some image is loaded
	if (!ImageProcessor::curr) {
		return ImageProcessor::ProcessReturnCodes::PROCESS_IMAGE_NOT_INITIALIZED;
	}

	// Force resizing
	std::string size_str = std::to_string(width) + std::string("x") + std::to_string(height) + std::string("!");
	ImageProcessor::curr->resize(size_str);
	ImageProcessor::curr->syncPixels();

	return ImageProcessor::ProcessReturnCodes::PROCESS_OK;
}

int ImageProcessor::rotateImage(double deg) {

	// Check if some image is loaded
	if (!ImageProcessor::curr) {
		return ImageProcessor::ProcessReturnCodes::PROCESS_IMAGE_NOT_INITIALIZED;
	}

	ImageProcessor::curr->rotate(deg);
	ImageProcessor::curr->syncPixels();
	return ImageProcessor::ProcessReturnCodes::PROCESS_OK;
}

int ImageProcessor::compositeImage(unsigned int x_pos, unsigned int y_pos) {

	// Check if some image is loaded
	if (!ImageProcessor::curr) {
		return ImageProcessor::ProcessReturnCodes::PROCESS_IMAGE_NOT_INITIALIZED;
	} else if (!ImageProcessor::canvas) {
		return ImageProcessor::ProcessReturnCodes::PROCESS_CANVAS_NOT_INITIALIZED;
	}

	ImageProcessor::canvas->composite(*curr, x_pos, y_pos);
	ImageProcessor::canvas->syncPixels();
	return ImageProcessor::ProcessReturnCodes::PROCESS_OK;
}

void ImageProcessor::finalizeImage() {
	delete ImageProcessor::curr;
}

int ImageProcessor::exportCanvas(char* filename, unsigned char quality) {
	ImageProcessor::canvas->quality(quality);
	ImageProcessor::canvas->write(filename);

	return ImageProcessor::ProcessReturnCodes::PROCESS_OK;
}
