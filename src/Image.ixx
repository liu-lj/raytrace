export module Image;

import <vector>;
import Color;

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

export {
  using byte = unsigned char;

  struct Image {
    std::vector<byte> data;
    int height, width, channels;

    Image(int height, int width, int channels)
        : height(height), width(width), channels(channels) {
      data.resize(height * width * channels);
    }

    inline byte& operator()(int h, int w, int c) {
      return data[(h * width + w) * channels + c];
    }

    inline void setPixel(int h, int w, ColorI3 color) {
      this->Image::operator()(h, w, 0) = color.x();
      this->Image::operator()(h, w, 1) = color.y();
      this->Image::operator()(h, w, 2) = color.z();
    }

    inline void writePNG(const char* path) {
      stbi_write_png(path, width, height, channels, data.data(),
                     width * channels);
    }
  };
}