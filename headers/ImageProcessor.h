#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <Magick++.h>

#include "Defs.h"

namespace ImageProcessor {

	// Currently processed canvas
	Magick::Image* canvas;

	// Cunrrently loaded image
	Magick::Image* curr;

	// Enumerator of image procession return codes
	enum ProcessReturnCodes {
		PROCESS_OK = 0,
		PROCESS_IMAGE_NOT_REGISTERED = 1,
		PROCESS_IMAGE_NOT_INITIALIZED = 2,
		PROCESS_CANVAS_NOT_INITIALIZED = 3,
	};

	// Initialize canvas
	extern "C" DLL_PUBLIC void initializeCanvas(unsigned int width, unsigned int height);

	// Load new image to process
	extern "C" DLL_PUBLIC int loadImage(int id);

	// Resize currently loaded image
	extern "C" DLL_PUBLIC int resizeImage(unsigned int width, unsigned int height);

	// Rotate currently loaded image counter-clockwise
	extern "C" DLL_PUBLIC int rotateImage(double deg);

	// Composite currently loaded image onto canvas
	extern "C" DLL_PUBLIC int compositeImage(unsigned int x_pos, unsigned int y_pos);

	// Free currently loaded image
	extern "C" DLL_PUBLIC void finalizeImage();

	// Finalize image changes and save canvas onto disk
	extern "C" DLL_PUBLIC int exportCanvas(char* filename, unsigned char quality);
}

#endif
