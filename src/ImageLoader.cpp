#include <cstring>
#include <algorithm>

#include "ImageLoader.h"

// Size of largest mipmap element edge
const int ImageLoader::MIPMAP_SIZE = 768;

// Export MIPMAP_SIZE constant via function
int ImageLoader::getMipmapSize() {
	return ImageLoader::MIPMAP_SIZE;
}

// Initialize ImageMagick
void ImageLoader::initializeMagick() {
	Magick::InitializeMagick(nullptr);

#ifdef _M_AMD64
	Magick::ResourceLimits::map(4294967296);
	Magick::ResourceLimits::memory(4294967296);
#endif
}

// Retrieve PSD format layer information such as number of layers
// and labels for each layer in single string delimited by ';' char
int ImageLoader::getLayerInfo(char* filename, char* labels) {
	Magick::Image img;
	std::string tmp_name;
	std::string tmp_labels = "";

	// TODO - Extract layer data from file more effectively as this way loads whole file for each layer
	int i = 0;
	while (true) {
		try {
			tmp_name = std::string(filename) + std::string("[") +
					std::to_string(i + 1) + std::string("]");
			img.read(tmp_name), i++;

			if ( !i || img.label().length() ) {
				tmp_labels += std::string(img.label()) + std::string(";");
			} else {
				return 1;
			}

			// As we support layers only on PSD files, return 1 on other formats
			// explicitly, otherwise this method become infinite loop
			if (std::strcmp(img.magick().c_str(), "PSD")) {
				return 1;
			}

		} catch ( Magick::Error &error ) {
			break;
		}
	}

	std::strcpy(labels, tmp_labels.c_str());
	return i;
}

// Image registration function, registers image to backend
// and returns it's id, if image registration fails, returns 
// negative value representing specific error code
int ImageLoader::registerImage(char* fileName) {
	ImageLoader::ImageInfo* img_info;
	Magick::Image img;

	try {
		img.read(fileName);

		if ( img.depth() != QUANTUM_DEPTH ) {
			std::cerr << "Error: Unsupported quantum depth" << std::endl;
			return ImageLoader::RegisterErrorCode::IMAGE_DEPTH_UNSUPPORTED;
		}

	} catch ( Magick::ErrorBlob &error ) {
		std::cerr << "Error: " << error.what() << std::endl;
		return ImageLoader::RegisterErrorCode::IMAGE_NOT_FOUND;
	} catch ( Magick::ErrorDelegate &error ) {
		std::cerr << "Error: " << error.what() << std::endl;
		return ImageLoader::RegisterErrorCode::IMAGE_FORMAT_UNSUPPORTED;
	} catch ( Magick::ErrorCorruptImage &error ) {
		std::cerr << "Error: " << error.what() << std::endl;
		return ImageLoader::RegisterErrorCode::IMAGE_CORRUPTED;
	} catch ( Magick::ErrorCoder &error ) {
		// Literally no reaction to this exception as this mostly means TIFF warnings and errors but image
		// is still loaded correctly, so we can ignore them
	} catch ( Magick::Error &error ) {
		std::cerr << "Error: " << error.what() << std::endl;
		return ImageLoader::RegisterErrorCode::IMAGE_UNKNOWN_ERROR;
	}

	// Get ImageInfo attributes
	int id = (int) ImageLoader::imgList.size() + 1;		// We want to index from 1, not zero
	char* format = new char[img.magick().length() + 1];
	std::strcpy(format, img.magick().c_str());
	int colorSpace = (int)(img.colorSpace());
	unsigned int width = (unsigned int)(img.columns());
	unsigned int height = (unsigned int)(img.rows());
	unsigned int* mipmap = ImageLoader::createMipmap(img);

	// Create and register ImageInfo crate
	img_info = new ImageLoader::ImageInfo(id, fileName, format, colorSpace, width, height, mipmap);
	ImageLoader::imgList.push_front(img_info);

	// Return id of succesfully registered image
	return id;
}

// Calls registerImage(), return it's return code and fills variables 
// given as parameter with newly registered image attrivutes
int ImageLoader::registerImageP(char* fileName, char* format, int* colorSpace, unsigned int* width, unsigned int* height, unsigned int** mipmap) {
	int id = ImageLoader::registerImage(fileName);				// Register Image

	// On image registration error, return it's error code
	if ( id < 0 ) {
		return id;
	}

	// Get ImageInfo object
	ImageLoader::ImageInfo* img_info = ImageLoader::findImageInfo(id);

	// Fill attributes given given as parameters with image attributes
	sprintf(format, "%s", img_info->getFormat());
	*colorSpace = img_info->getColorSpace();
	*width = img_info->getWidth();
	*height = img_info->getHeight();
	*mipmap = img_info->getMipmap();
	
	return id;
}

// Return id of registered image with filename given as parameter, returns 0 on
// not registered image
int ImageLoader::getImageId(char* fileName) {
	for ( ImageLoader::ImageInfo* img_info : ImageLoader::imgList ) {
		if ( strcmp(img_info->getFileName(), fileName) ) {
			return img_info->getId();
		}
	}

	return 0;
}

// Return filename of registered image with id given as parameter
char* ImageLoader::getImageFileName(int id) {
	ImageInfo* img_info = ImageLoader::findImageInfo(id);
	return  img_info ? img_info->getFileName() : nullptr;
}

// Return format of registered image with id given as parameter
char* ImageLoader::getImageFormat(int id) {
	ImageInfo* img_info = ImageLoader::findImageInfo(id);
	return img_info ? img_info->getFormat() : nullptr;
}

// Return colorspace of registered image with id given as parameter
int ImageLoader::getImageColorSpace(int id) {
	ImageInfo* img_info = ImageLoader::findImageInfo(id);
	return img_info ? img_info->getColorSpace() : -1;
}

// Return width of registered image with id given as parameter
unsigned int ImageLoader::getImageWidth(int id) {
	ImageInfo* img_info = ImageLoader::findImageInfo(id);
	return img_info ? img_info->getWidth() : 0;
}

// Return height of registered image with id given as parameter
unsigned int ImageLoader::getImageHeight(int id) {
	ImageInfo* img_info = ImageLoader::findImageInfo(id);
	return img_info ? img_info->getHeight() : 0;
}

// Return mipmap of registered image with id given as parameter
unsigned int* ImageLoader::getImageMipmap(int id) {
	ImageInfo* img_info = ImageLoader::findImageInfo(id);
	return img_info ? img_info->getMipmap() : nullptr;
}

// Destroys all registered images
void ImageLoader::clearRegisteredImages() {
	for ( ImageLoader::ImageInfo* img_info : ImageLoader::imgList ) {
		delete img_info;
	}

	ImageLoader::imgList.clear();
}

// Return ImageInfo object of registered image with id given as parameter
ImageLoader::ImageInfo* ImageLoader::findImageInfo(int id) {
	for ( ImageLoader::ImageInfo* img_info : ImageLoader::imgList ) {
		if ( (img_info->getId()) == id ) {
			return img_info;
		}
	}

	return nullptr;
}

// Return original Magick::Image object of registered image with id 
// given as parameter
Magick::Image* ImageLoader::getImage(int id) {
	Magick::Image* img_p = new Magick::Image();
	ImageInfo* img_info = ImageLoader::findImageInfo(id);

	if (!img_info) {
		delete img_p;
		return nullptr;
	}

	try {
		img_p->read(img_info->getFileName());
	} catch ( Magick::ErrorCoder &error ) {
		// Literally no reaction to this exception as this mostly means TIFF warnings and errors but image
		// is still loaded correctly, so we can ignore them
	}

	return img_p;
}

// Return original Magick::Image of registered image with filename given as prameter
Magick::Image* ImageLoader::getImage(std::string fileName) {
	int id = ImageLoader::getImageId((char*)(fileName.c_str()));
	return ImageLoader::getImage(id);
}

// Return pure pixel data of mipmap based on Magick::Image object given as parameter
unsigned int* ImageLoader::createMipmap(Magick::Image img) {
	size_t x_pos = 0, y_pos = 0;
	int x_size = MIPMAP_SIZE, y_size = MIPMAP_SIZE;
	size_t mmap_s_size = MIPMAP_SIZE * MIPMAP_SIZE * 3/2 * sizeof(unsigned int);
	unsigned int* mmap_data = new unsigned int[mmap_s_size];

	// Construct initial string, '3/2' constant is safe here as we have fixed mipmap size
	std::string size_str = std::to_string((int)(x_size * 3/2)) + std::string("x") + std::to_string(y_size);

	Magick::Image mmap(size_str, Magick::Color("white"));
	while (x_size > 0 && y_size > 0) {
		// Construct new Geometry string
		size_str = std::to_string(x_size) + std::string("x") + std::to_string(y_size);

		// Resize current image and composite it to mipmap
		img.scale(size_str);
		mmap.composite(img, x_pos, y_pos);

		// Update col only on first loop, 
		// update row on all loops except first
		x_pos = x_size == MIPMAP_SIZE ? img.columns() : x_pos;
		y_pos = y_size == MIPMAP_SIZE ? 0 : y_pos + img.rows();
	
		x_size /= 2, y_size /= 2;
	}

	// Propagate all changes to mipmap
	mmap.syncPixels();
	memcpy(mmap_data, mmap.getPixels(0, 0, MIPMAP_SIZE * 3/2, MIPMAP_SIZE), mmap_s_size);

	// Return pure pixel data	
	return mmap_data;
}
