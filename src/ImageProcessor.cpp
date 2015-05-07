#include <string>

#include "ImageLoader.h"
#include "ImageProcessor.h"

// Initialize new image with specified width and height
void ImageProcessor::initializeCanvas(unsigned int width, unsigned int height) {
	std::string size_str = std::to_string(width) + std::string("x") + std::to_string(height);
	ImageProcessor::canvas = new Magick::Image(size_str, Magick::Color("white"));
}

// Load image for processing, image must be registered before via ImageLoader class
int ImageProcessor::loadImage(int id) {
	Magick::Image* img = ImageLoader::getImage(id);

	// Check if image is registered
	if (!img) {
		return ImageProcessor::ProcessReturnCodes::PROCESS_IMAGE_NOT_REGISTERED;
	}

	// Background needs to be transparent as of rotation(bounding box will be shown otherwise)
	img->backgroundColor(Magick::Color("transparent"));
	ImageProcessor::curr = img;

	return ImageProcessor::ProcessReturnCodes::PROCESS_OK;;
}

// Resize currently loaded image
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

// Rotate currently loaded image
int ImageProcessor::rotateImage(double deg) {

	// Check if some image is loaded
	if (!ImageProcessor::curr) {
		return ImageProcessor::ProcessReturnCodes::PROCESS_IMAGE_NOT_INITIALIZED;
	}

	ImageProcessor::curr->rotate(deg);
	ImageProcessor::curr->syncPixels();

	return ImageProcessor::ProcessReturnCodes::PROCESS_OK;
}

// Place currently loaded image onto canvas, position given as parameters are
// position of image center, 'OverCompositeOp' operator is used as
// composite operator
int ImageProcessor::compositeImage(int x_pos, int y_pos) {

	// Check if some image is loaded
	if (!ImageProcessor::curr) {
		return ImageProcessor::ProcessReturnCodes::PROCESS_IMAGE_NOT_INITIALIZED;
	} else if (!ImageProcessor::canvas) {
		return ImageProcessor::ProcessReturnCodes::PROCESS_CANVAS_NOT_INITIALIZED;
	}

	// Recalculate position as parameters are position of center
	x_pos -= ( ImageProcessor::curr->columns() / 2 );
	y_pos -= ( ImageProcessor::curr->rows() / 2 );

	ImageProcessor::canvas->composite(*curr, x_pos, y_pos, Magick::CompositeOperator::OverCompositeOp);
	ImageProcessor::canvas->syncPixels();

	return ImageProcessor::ProcessReturnCodes::PROCESS_OK;
}

// Delete currently loaded image from memory
void ImageProcessor::finalizeImage() {
	delete ImageProcessor::curr;
}

// Save current canvas into file and delete it from memory
int ImageProcessor::exportCanvas(char* filename, unsigned char quality) {
	ImageProcessor::canvas->quality(quality);
	ImageProcessor::canvas->write(filename);

	delete ImageProcessor::canvas;

	return ImageProcessor::ProcessReturnCodes::PROCESS_OK;
}
