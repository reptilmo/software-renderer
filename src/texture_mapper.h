// texture_mapper.h
#pragma once

#include "color.h"
#include "display.h"
#include "texture.h"
#include "triangle.h"

void draw_textured_triangle(Display* display, const Triangle* triangle, const Texture* texture);
