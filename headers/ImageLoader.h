#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <iostream>
#include <string>
#include <list>
#include <Magick++.h>

#include "Defs.h"

// Quantum depth ImageMagick is compiled with
#define QUANTUM_DEPTH 8

namespace ImageLoader {

	/*
	 * Crate to store information about registered image
	 *
	 */
	class ImageInfo {
		public:
			// Getters
			int getId();
			char* getFileName();
			char* getFormat();
			int getColorSpace();
			unsigned int getWidth();
			unsigned int getHeight();
			unsigned int* getMipmap();

			// Setters
			void setFileName(char* fileName);

			// Constructor
			ImageInfo(	int id,
					char* fileName,
					char* format,
					int colorSpace,
					unsigned int width,
					unsigned int height,
					unsigned int* mipmap );

			~ImageInfo();
		private:
			int id;					// Image identifier
			char* fileName;				// Image file name
			char* format;				// Image format in string
			int colorSpace;  			// Image color space, eg. "CMYK"
			unsigned int width;			// Original image width
			unsigned int height;			// Original image height
			unsigned int* mipmap;			// Generated mipmap for image
	};

	/*
 	 * ImageInfo registering and retrieving attributtes about registered images
 	 *
 	 */

	// Mipmap configuration constant
	extern const int MIPMAP_SIZE;

	// Enumerator of error image registration return codes
	enum RegisterErrorCode {
		IMAGE_NOT_FOUND = -1,
		IMAGE_CORRUPTED = -2,
		IMAGE_DEPTH_UNSUPPORTED = -3,
		IMAGE_FORMAT_UNSUPPORTED = -4,
		IMAGE_UNKNOWN_ERROR = -5
	};

	// Return currently configured size of mipmap
	extern "C" DLL_PUBLIC int getMipmapSize();

	// Initialize ImageMagick
	extern "C" DLL_PUBLIC void initializeMagick();

	// Get PSD image layer info such as layer count and layer labels
	extern "C" DLL_PUBLIC int getLayerInfo(char* filename, char* labels);

	// Image registering and retrieving
	// On successful image registration returns positive value representing registered image's id
	// On failed image registration returns negative value representing error code
	extern "C" DLL_PUBLIC int registerImage(char* fileName);
	extern "C" DLL_PUBLIC int registerImageP(char* fileName, char* format, int* colorSpace, unsigned int* width, unsigned int* height, unsigned int** mipmap);

	// ImageInfo attributes retrieving
	// Will return NULL on not registered image
	extern "C" DLL_PUBLIC int getImageId(char* fileName);
	extern "C" DLL_PUBLIC char* getImageFileName(int id);
	extern "C" DLL_PUBLIC char* getImageFormat(int id);
	extern "C" DLL_PUBLIC int getImageColorSpace(int id);
	extern "C" DLL_PUBLIC unsigned int getImageWidth(int id);
	extern "C" DLL_PUBLIC unsigned int getImageHeight(int id);
	extern "C" DLL_PUBLIC unsigned int* getImageMipmap(int id);

	// Destroy all registered images
	extern "C" DLL_PUBLIC void clearRegisteredImages();

	extern "C" DLL_PUBLIC void destroyImage(int id);

	// List of registered images
	static std::list<ImageInfo*> imgList;

	// Returns ImageInfo containing id gives as parameter
	ImageInfo* findImageInfo(int id);

	// Load original image from file
	Magick::Image* getImage(int id);
	Magick::Image* getImage(std::string fileName);

	// Create mipmap
	unsigned int* createMipmap(Magick::Image img);
};

#endif
