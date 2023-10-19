#pragma once

#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include "Color.hpp"

using byte = unsigned char;

struct Image {
  std::vector<byte> data;
  size_t height, width, channels;

  Image(size_t height, size_t width, size_t channels)
      : height(height), width(width), channels(channels) {
    data.resize(height * width * channels);
  }

  inline byte& operator()(size_t h, size_t w, size_t c) {
    return data[(h * width + w) * channels + c];
  }

  inline void setPixel(size_t h, size_t w, ColorI3 color) {
    this->Image::operator()(h, w, 0) = color.x();
    this->Image::operator()(h, w, 1) = color.y();
    this->Image::operator()(h, w, 2) = color.z();
  }

  inline void setPixel(size_t h, size_t w, ColorF3 color) {
    setPixel(h, w, ToColorI3(color));
  }

  inline void linearToGamma() {
    for (size_t i = 0; i < data.size(); i++) 
      data[i] = std::pow(data[i] / 255.0f, 1 / 2.2f) * 255;
  }

  inline void writePNG(const char* path) {
    stbi_write_png(path, width, height, channels, data.data(),
                   width * channels);
  }
};