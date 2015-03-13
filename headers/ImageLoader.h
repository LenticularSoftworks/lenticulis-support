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

	// Configuration constants (frontend/backend)
	extern "C" DLL_PUBLIC const int MIPMAP_SIZE;

	// Constants defining return codes (frontend)
	extern "C" DLL_PUBLIC const int IMAGE_OK;
	extern "C" DLL_PUBLIC const int IMAGE_NOT_FOUND;
	extern "C" DLL_PUBLIC const int IMAGE_CORRUPTED;
	extern "C" DLL_PUBLIC const int IMAGE_DEPTH_UNSUPPORTED;
	extern "C" DLL_PUBLIC const int IMAGE_FORMAT_UNSUPPORTED;

	// Image registering and retrieving (frontend)
	extern "C" DLL_PUBLIC int registerImage(std::string fileName);
	extern "C" DLL_PUBLIC int registerImageP(std::string fileName, std::string* format, int* colorSpace, size_t* width, size_t* height, void* mipmap);

	// ImageInfo attributes retrieving (frontend)
	// Will return NULL on not registered image
	extern "C" DLL_PUBLIC int getImageId(std::string fileName);
	extern "C" DLL_PUBLIC std::string getImageFileName(int id);
	extern "C" DLL_PUBLIC std::string getImageFormat(int id);
	extern "C" DLL_PUBLIC int getImageColorSpace(int id);
	extern "C" DLL_PUBLIC size_t getImageWidth(int id);
	extern "C" DLL_PUBLIC size_t getImageHeight(int id);
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
