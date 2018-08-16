#ifndef MUIL_DISPLAY_HPP_FILE_INCLUDED_
#define MUIL_DISPLAY_HPP_FILE_INCLUDED_

#include <stdint.h>
#include "muil_font.hpp"
#include "muil_text_lib.hpp"

namespace muil {

enum class HorizAlign
{
	Left,
	Center,
	Right,
};

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	Color() : r(0), g(0), b(0) {}
	Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

	static Color black() { return Color(0, 0, 0); }
	static Color white() { return Color(255, 255, 255); }
	static Color blue() { return Color(0, 0, 255); }
	static Color red() { return Color(255, 0, 0); }
	static Color green() { return Color(0, 255, 0); }

	Color light(int value) const;
	Color adjusted(int multiplier, int divider);
	static Color between(const Color &color1, const Color &color2, int16_t value, int16_t max);

	bool operator == (const Color &other) const
	{
		return (r == other.r) && (g == other.g) && (b == other.b);
	}

	bool operator != (const Color &other) const
	{
		return !operator == (other);
	}
};

struct Size
{
	uint16_t width;
	uint16_t height;

	Size() : width(0), height(0) {}
	Size(uint16_t width, uint16_t height) : width(width), height(height) {}
};

struct Point
{
	int16_t x;
	int16_t y;

	Point() : x(0), y(0) {}
	Point(int16_t x, int16_t y) : x(x), y(y) {}

	Point moved(int16_t dx, int16_t dy) const { return Point(x + dx, y + dy); }

	bool equals(const Point &other) const
	{
		return (x == other.x) && (y == other.y);
	}

	bool operator == (const Point &other) const
	{
		return equals(other);
	}

	bool operator != (const Point &other) const
	{
		return !equals(other);
	}
};

struct Rect
{
	int16_t x1;
	int16_t y1;
	int16_t x2;
	int16_t y2;

	Rect() : x1(0), y1(0), x2(0), y2(0) {}
	Rect(const Point &pt, const Size &size) : x1(pt.x), y1(pt.y), x2(pt.x+size.width), y2(pt.y+size.height) {}
	Rect(int16_t x1, int16_t y1, int16_t x2, int16_t y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

	Rect inflated(int value) const;
	int16_t height() const { return y2-y1+1; }
	int16_t width() const { return x2-x1+1; }

	bool contains(const Point pt) const;
	bool intersects(const Rect &other_rect) const;
};

unsigned display_get_width();
unsigned display_get_height();
uint16_t display_get_dpi();

void     display_set_offset(int x, int y);
void     display_set_point(int x, int y, const Color &color);
void     display_fill_rect(int x1, int y1, int x2, int y2, const Color &color);
void     display_fill_rect(const Rect &rect, const Color &color);

void     display_draw_rect(const Rect &rect, int16_t width, const Color &color);
void     display_draw_vertical_gradient(const Rect &rect, const Color &color1, const Color &color2);
void     display_draw_horizontal_gradient(const Rect &rect, const Color &color1, const Color &color2);

void     display_paint_character(int x0, int y0, const uint8_t *data, uint8_t width, uint8_t height, const Color &color, const Color *bg_color);

void     display_print_any(int x, int y, const ICharactersProvider & provider, const FontInfo &font, const Color &color, const Color *bg_color);
void     display_print_any_in_rect(const Rect &rect, HorizAlign align, const ICharactersProvider & provider, const FontInfo &font, const Color &color, const Color *bg_color);

void     display_print_string(int x, int y, const wchar_t *text, const FontInfo &font, const Color &color, const Color *bg_color);
void     display_print_string(int x, int y, const char *utf8_text, const FontInfo &font, const Color &color, const Color *bg_color);

void     display_print_string_in_rect(const Rect &rect, HorizAlign align, const wchar_t *text, const FontInfo &font, const Color &color, const Color *bg_color);
void     display_print_string_in_rect(const Rect &rect, HorizAlign align, const char *utf8_text, const FontInfo &font, const Color &color, const Color *bg_color);

void     display_print_integer(int x, int y, int value, int pt_pos, const FontInfo &font, const Color &color, const Color *bg_color);
void     display_print_integer_in_rect(const Rect &rect, HorizAlign align, int value, int pt_pos, const FontInfo &font, const Color &color, const Color *bg_color);

void     display_print_integer16(int x, int y, uint32_t value, const FontInfo &font, const Color &color, const Color *bg_color);
void     display_print_integer16_in_rect(const Rect &rect, HorizAlign align, uint32_t value, const FontInfo &font, const Color &color, const Color *bg_color);

void     display_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color &color);

Size     display_calc_text_size(const FontInfo &font, const ICharactersProvider & provider);
Size     display_get_text_size(const FontInfo &font, const wchar_t *text);
uint16_t display_get_font_height(const FontInfo &font);

void     default_display_paint_character(int x0, int y0, const uint8_t *data, uint8_t width, uint8_t height, const Color &color, const Color *bg_color);
void     default_display_fill_rect(const Rect &rect, const Color &color);

} // namespace muil

#endif
