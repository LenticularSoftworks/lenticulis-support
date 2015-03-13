#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <iostream>
#include <string>
#include <Magick++.h>

#include "Defs.h"

namespace ImageLoader {

	/*
	 * Crate to store information about registered image
	 *
	 */
	class ImageInfo {
		public:
			// Getters (backend)
			int getId();
			std::string getFileName();
			std::string getFormat();
			Magick::ColorspaceType getColorSpace();
			size_t getWidth();
			size_t getHeight();
			Magick::Image* getMipmap();

			// Setters (backend)
			void setFileName(std::string fileName);

			// Constructor (backend)
			ImageInfo(	int id, 
					std::string fileName,
					std::string format,
					Magick::ColorspaceType colorSpace,
					size_t width,
					size_t height,
					Magick::Image* mipmap );
		private:
			int id;					// Image identifier (backend)
			std::string fileName;			// Image file name (backend)
			std::string format;			// Image format in string (backend)
			Magick::ColorspaceType colorSpace;  	// Image color space, eg. "CMYK" (backend)
			size_t width;				// Original image width (backend)
			size_t height;				// Original image height (backend)
			Magick::Image* mipmap;			// Generated mipmap for image (backend)
	};

	/*
 	 * ImageInfo registering and retrieving attributtes about registered images
 	 *
 	 */

	// Mipmap configuration constant
	extern const int MIPMAP_SIZE;

	// Enumerator of image registration return codes
	enum RegisterCode {
		IMAGE_OK = 0,
		IMAGE_NOT_FOUND,
		IMAGE_CORRUPTED,
		IMAGE_DEPTH_UNSUPPORTED,
		IMAGE_FORMAT_UNSUPPORTED 
	};

	// Return currently configured size of mipmap
	extern "C" DLL_PUBLIC int getMipmapSize();

	// Image registering and retrieving (frontend)
	extern "C" DLL_PUBLIC int registerImage(char* fileName);
	extern "C" DLL_PUBLIC int registerImageP(char* fileName, char** format, int* colorSpace, unsigned int* width, unsigned int* height, void* mipmap);

	// ImageInfo attributes retrieving (frontend)
	// Will return NULL on not registered image
	extern "C" DLL_PUBLIC int getImageId(char* fileName);
	extern "C" DLL_PUBLIC const char* getImageFileName(int id);
	extern "C" DLL_PUBLIC const char* getImageFormat(int id);
	extern "C" DLL_PUBLIC int getImageColorSpace(int id);
	extern "C" DLL_PUBLIC unsigned int getImageWidth(int id);
	extern "C" DLL_PUBLIC unsigned int getImageHeight(int id);
	extern "C" DLL_PUBLIC void* getImageMipmap(int id);

	// List of registered images
	static std::list<ImageInfo*> imgList;

	// Load original image from file (backend)
	Magick::Image* getImage(int id);
	Magick::Image* getImage(std::string fileName);

	// Create mipmap (backend)
	Magick::Image* createMipmap(Magick::Image* img);		
};

#endif
