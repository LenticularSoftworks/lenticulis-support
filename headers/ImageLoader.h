#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <iostream>
#include <string>

#include <Magick++.h>

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
 * Singleton service for registering images into backend and retrieving 
 * info about registered images. Aslo this class is used to load original
 * images from file.
 *
 */
class ImageLoader {
	public:
		// Configuration constants
		static const int MIPMAP_SIZE = 768;

		// Constants defining return codes (frontend)
		static const int IMAGE_OK = 1;
		static const int IMAGE_NOT_FOUND = 2;
		static const int IMAGE_CORRUPTED = 3;
		static const int IMAGE_DEPTH_UNSUPPORTED = 4;
		static const int IMAGE_FORMAT_UNSUPPORTED = 5;

		// Get singleton instance (frontend/backend)
		static ImageLoader& instance() {
			static ImageLoader instance;
			return instance;
		};

		// Image registering and retrieving (frontend)
		int registerImage(std::string fileName);

		// Register image and instantly fill it's 
		// attributes into variables given as parameter
		int registerImage(	std::string fileName,
					std::string* format,
					int* colorSpace,
					size_t* width,
					size_t* height,
					void* mipmap );

		// ImageBundle info retrieving (frontend)
		// Will return NULL on not registered image
		int getImageId(std::string fileName);
		std::string getImageFileName(int id);
		std::string getImageFormat(int id);
		int getImageColorSpace(int id);
		size_t getImageWidth(int id);
		size_t getImageHeight(int id);
		void* getImageMipmap(int id);

		// Load original image from file (backend)
		Magick::Image* getImage(int id);
		Magick::Image* getImage(std::string fileName);
	private:
		// List of registered images (backend)
		std::list<ImageInfo*> imgList;

		// Create mipmap (backend)
		Magick::Image* createMipmap(Magick::Image* img);
		
		// Private constructor as this is singleon
		ImageLoader() { };

		ImageLoader(ImageLoader const&)		= delete;
		void operator=(ImageLoader const&)	= delete;
};

#endif
