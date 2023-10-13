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
  int height, width, channels;
  Image(int height, int width, int channels)
      : height(height), width(width), channels(channels) {
    data.resize(height * width * channels);
  }
  byte& operator()(int w, int h, int c) {
    return data[(h * width + w) * channels + c];
  }
  void setPixel(int w, int h, ColorI3 color) {
    this->Image::operator()(w, h, 0) = color.x();
    this->Image::operator()(w, h, 1) = color.y();
    this->Image::operator()(w, h, 2) = color.z();
  }
  void writePNG(const char* path) {
    stbi_write_png(path, width, height, channels, data.data(),
                   width * channels);
  }
};