#include "muil_font_firasans_9_ru.hpp"

namespace muil {

// 
//  Font data for Fira Sans 9pt
// 

// Character bitmaps for Fira Sans 9pt
const uint8_t firaSans9ptBitmaps[] = 
{
	// @0 '!' (1 pixels wide)
	0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 

	// @14 '"' (3 pixels wide)
	0x00, 0x00, 0xA0, 0xA0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @28 '#' (6 pixels wide)
	0x00, 0x00, 0x28, 0x28, 0x7C, 0x48, 0x48, 0x48, 0xFC, 0x48, 0x58, 0x00, 0x00, 0x00, 

	// @42 '$' (6 pixels wide)
	0x10, 0x10, 0x38, 0x4C, 0x40, 0x60, 0x38, 0x0C, 0x04, 0x8C, 0x78, 0x10, 0x10, 0x00, 

	// @56 '%' (8 pixels wide)
	0x00, 0x00, 0xE2, 0xA4, 0xA4, 0xA8, 0xF0, 0x17, 0x25, 0x2D, 0x47, 0x00, 0x00, 0x00, 

	// @70 '&' (7 pixels wide)
	0x00, 0x00, 0x70, 0x48, 0xC8, 0x50, 0x62, 0x92, 0x8C, 0x8C, 0x7A, 0x00, 0x00, 0x00, 

	// @84 ''' (1 pixels wide)
	0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @98 '(' (3 pixels wide)
	0x20, 0x20, 0x40, 0x40, 0x40, 0x40, 0xC0, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x00, 

	// @112 ')' (3 pixels wide)
	0x80, 0x40, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x40, 0xC0, 0x00, 

	// @126 '*' (5 pixels wide)
	0x00, 0x20, 0xA8, 0x70, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @140 '+' (4 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0xF0, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @154 ',' (2 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x80, 0x00, 

	// @168 '-' (3 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @182 '.' (1 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 

	// @196 '/' (4 pixels wide)
	0x00, 0x10, 0x10, 0x20, 0x20, 0x20, 0x20, 0x40, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 

	// @210 '0' (5 pixels wide)
	0x00, 0x00, 0x70, 0x98, 0x88, 0x88, 0x88, 0x88, 0x88, 0x98, 0x70, 0x00, 0x00, 0x00, 

	// @224 '1' (4 pixels wide)
	0x00, 0x00, 0x30, 0x70, 0x90, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 

	// @238 '2' (5 pixels wide)
	0x00, 0x00, 0x70, 0x88, 0x08, 0x08, 0x18, 0x10, 0x20, 0x40, 0xF8, 0x00, 0x00, 0x00, 

	// @252 '3' (5 pixels wide)
	0x00, 0x00, 0x70, 0xC8, 0x08, 0x08, 0x30, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x00, 

	// @266 '4' (6 pixels wide)
	0x00, 0x00, 0x00, 0x10, 0x20, 0x20, 0x48, 0x48, 0xFC, 0x08, 0x08, 0x00, 0x00, 0x00, 

	// @280 '5' (5 pixels wide)
	0x00, 0x00, 0x78, 0x40, 0x40, 0x78, 0x08, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x00, 

	// @294 '6' (5 pixels wide)
	0x00, 0x00, 0x70, 0xC0, 0x80, 0xF0, 0x88, 0x88, 0x88, 0x98, 0x70, 0x00, 0x00, 0x00, 

	// @308 '7' (5 pixels wide)
	0x00, 0x00, 0xF8, 0x08, 0x10, 0x10, 0x30, 0x20, 0x20, 0x40, 0x40, 0x00, 0x00, 0x00, 

	// @322 '8' (5 pixels wide)
	0x00, 0x00, 0x70, 0x98, 0x88, 0xD0, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 

	// @336 '9' (5 pixels wide)
	0x00, 0x00, 0x70, 0x90, 0x88, 0x88, 0x98, 0xF8, 0x10, 0x30, 0xC0, 0x00, 0x00, 0x00, 

	// @350 ':' (1 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 

	// @364 ';' (2 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x40, 0x40, 0x40, 0x80, 0x00, 

	// @378 '<' (4 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x10, 0x60, 0x80, 0xC0, 0x30, 0x10, 0x00, 0x00, 0x00, 0x00, 

	// @392 '=' (4 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @406 '>' (4 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x10, 0x30, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 

	// @420 '?' (5 pixels wide)
	0x00, 0x00, 0x70, 0xC8, 0x08, 0x10, 0x20, 0x20, 0x00, 0x20, 0x20, 0x00, 0x00, 0x00, 

	// @434 '@' (10 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x20, 0x80, 0x40, 0x40, 0x8E, 0x40, 0x92, 0x40, 0x92, 0x40, 0x92, 0x40, 0x92, 0x40, 0x9D, 0x80, 0x40, 0x00, 0x60, 0x00, 0x1F, 0x00, 

	// @462 'A' (7 pixels wide)
	0x00, 0x00, 0x10, 0x28, 0x28, 0x28, 0x68, 0x44, 0x7C, 0x44, 0x86, 0x00, 0x00, 0x00, 

	// @476 'B' (6 pixels wide)
	0x00, 0x00, 0xF0, 0x88, 0x88, 0x88, 0xF0, 0x8C, 0x84, 0x8C, 0xF8, 0x00, 0x00, 0x00, 

	// @490 'C' (5 pixels wide)
	0x00, 0x00, 0x78, 0xC8, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC8, 0x78, 0x00, 0x00, 0x00, 

	// @504 'D' (6 pixels wide)
	0x00, 0x00, 0xF0, 0x88, 0x84, 0x84, 0x84, 0x84, 0x84, 0x88, 0xF0, 0x00, 0x00, 0x00, 

	// @518 'E' (5 pixels wide)
	0x00, 0x00, 0xF8, 0x80, 0x80, 0x80, 0xF0, 0x80, 0x80, 0x80, 0xF8, 0x00, 0x00, 0x00, 

	// @532 'F' (5 pixels wide)
	0x00, 0x00, 0xF8, 0x80, 0x80, 0x80, 0xF0, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 

	// @546 'G' (6 pixels wide)
	0x00, 0x00, 0x78, 0xC4, 0x80, 0x80, 0x9C, 0x84, 0x84, 0xC4, 0x78, 0x00, 0x00, 0x00, 

	// @560 'H' (6 pixels wide)
	0x00, 0x00, 0x84, 0x84, 0x84, 0x84, 0xFC, 0x84, 0x84, 0x84, 0x84, 0x00, 0x00, 0x00, 

	// @574 'I' (1 pixels wide)
	0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 

	// @588 'J' (3 pixels wide)
	0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x40, 0x40, 0x80, 0x00, 

	// @602 'K' (6 pixels wide)
	0x00, 0x00, 0x8C, 0x98, 0x90, 0xA0, 0xE0, 0xB0, 0x90, 0x88, 0x8C, 0x00, 0x00, 0x00, 

	// @616 'L' (5 pixels wide)
	0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xF8, 0x00, 0x00, 0x00, 

	// @630 'M' (8 pixels wide)
	0x00, 0x00, 0xC6, 0xC6, 0xA6, 0xAA, 0xAA, 0xAA, 0x9A, 0x92, 0x83, 0x00, 0x00, 0x00, 

	// @644 'N' (6 pixels wide)
	0x00, 0x00, 0xC4, 0xC4, 0xA4, 0xA4, 0xB4, 0x94, 0x9C, 0x8C, 0x8C, 0x00, 0x00, 0x00, 

	// @658 'O' (7 pixels wide)
	0x00, 0x00, 0x78, 0xC4, 0x84, 0x86, 0x86, 0x86, 0x84, 0xC4, 0x78, 0x00, 0x00, 0x00, 

	// @672 'P' (6 pixels wide)
	0x00, 0x00, 0xF0, 0x88, 0x8C, 0x8C, 0x88, 0xF0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 

	// @686 'Q' (7 pixels wide)
	0x00, 0x00, 0x78, 0xC4, 0x84, 0x86, 0x86, 0x86, 0x84, 0xC4, 0x7C, 0x06, 0x00, 0x00, 

	// @700 'R' (6 pixels wide)
	0x00, 0x00, 0xF8, 0x88, 0x8C, 0x88, 0xF0, 0x90, 0x98, 0x88, 0x8C, 0x00, 0x00, 0x00, 

	// @714 'S' (6 pixels wide)
	0x00, 0x00, 0x38, 0x44, 0x40, 0x60, 0x38, 0x04, 0x04, 0x84, 0x78, 0x00, 0x00, 0x00, 

	// @728 'T' (6 pixels wide)
	0x00, 0x00, 0xFC, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 

	// @742 'U' (6 pixels wide)
	0x00, 0x00, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0xCC, 0x78, 0x00, 0x00, 0x00, 

	// @756 'V' (7 pixels wide)
	0x00, 0x00, 0x86, 0x44, 0x44, 0x44, 0x68, 0x28, 0x28, 0x30, 0x10, 0x00, 0x00, 0x00, 

	// @770 'W' (10 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xCC, 0xC0, 0x4C, 0x80, 0x4C, 0x80, 0x4C, 0x80, 0x52, 0x80, 0x52, 0x80, 0x33, 0x00, 0x33, 0x00, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @798 'X' (6 pixels wide)
	0x00, 0x00, 0xC4, 0x48, 0x28, 0x30, 0x30, 0x38, 0x68, 0x4C, 0xC4, 0x00, 0x00, 0x00, 

	// @812 'Y' (7 pixels wide)
	0x00, 0x00, 0xC6, 0x44, 0x48, 0x28, 0x30, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 

	// @826 'Z' (6 pixels wide)
	0x00, 0x00, 0x7C, 0x08, 0x08, 0x10, 0x30, 0x20, 0x60, 0x40, 0xFC, 0x00, 0x00, 0x00, 

	// @840 '[' (2 pixels wide)
	0xC0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0x00, 0x00, 

	// @854 '\' (4 pixels wide)
	0x00, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x00, 0x00, 

	// @868 ']' (2 pixels wide)
	0xC0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xC0, 0x00, 0x00, 

	// @882 '^' (6 pixels wide)
	0x30, 0x38, 0x48, 0xC4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @896 '_' (6 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 

	// @910 '`' (3 pixels wide)
	0x00, 0xC0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @924 'a' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xF0, 0x10, 0x10, 0xF0, 0x90, 0x90, 0xE8, 0x00, 0x00, 0x00, 

	// @938 'b' (5 pixels wide)
	0x00, 0x80, 0x80, 0x80, 0xB0, 0xC8, 0x88, 0x88, 0x88, 0xC8, 0xB0, 0x00, 0x00, 0x00, 

	// @952 'c' (4 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x70, 0x80, 0x80, 0x80, 0x80, 0x80, 0x70, 0x00, 0x00, 0x00, 

	// @966 'd' (5 pixels wide)
	0x00, 0x08, 0x08, 0x08, 0x78, 0x88, 0x88, 0x88, 0x88, 0x88, 0x78, 0x00, 0x00, 0x00, 

	// @980 'e' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x70, 0x98, 0x88, 0xF8, 0x80, 0x88, 0x70, 0x00, 0x00, 0x00, 

	// @994 'f' (5 pixels wide)
	0x00, 0x38, 0x40, 0x40, 0xF0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 

	// @1008 'g' (6 pixels wide)
	0x00, 0x00, 0x00, 0x04, 0x7C, 0x48, 0x4C, 0x48, 0x38, 0x40, 0x78, 0x04, 0x84, 0x78, 

	// @1022 'h' (5 pixels wide)
	0x00, 0x80, 0x80, 0x80, 0xB0, 0xC8, 0x88, 0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 

	// @1036 'i' (1 pixels wide)
	0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 

	// @1050 'j' (2 pixels wide)
	0x00, 0x40, 0x40, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xC0, 0x80, 

	// @1064 'k' (5 pixels wide)
	0x00, 0x80, 0x80, 0x80, 0x98, 0x90, 0xA0, 0xE0, 0xA0, 0x90, 0x98, 0x00, 0x00, 0x00, 

	// @1078 'l' (2 pixels wide)
	0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0x00, 0x00, 0x00, 

	// @1092 'm' (8 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xB7, 0xC9, 0x89, 0x89, 0x89, 0x89, 0x89, 0x00, 0x00, 0x00, 

	// @1106 'n' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xB0, 0xC8, 0x88, 0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 

	// @1120 'o' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 

	// @1134 'p' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xB0, 0xC8, 0x88, 0x88, 0x88, 0x88, 0xF0, 0x80, 0x80, 0x80, 

	// @1148 'q' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x78, 0x88, 0x88, 0x88, 0x88, 0x98, 0x68, 0x08, 0x08, 0x08, 

	// @1162 'r' (4 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xB0, 0xC0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 

	// @1176 's' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x78, 0x48, 0x40, 0x30, 0x08, 0x88, 0x70, 0x00, 0x00, 0x00, 

	// @1190 't' (4 pixels wide)
	0x00, 0x00, 0x40, 0x40, 0xF0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x30, 0x00, 0x00, 0x00, 

	// @1204 'u' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 0x88, 0x98, 0x68, 0x00, 0x00, 0x00, 

	// @1218 'v' (6 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x8C, 0x48, 0x48, 0x48, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 

	// @1232 'w' (8 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x89, 0x59, 0x55, 0x55, 0x56, 0x56, 0x26, 0x00, 0x00, 0x00, 

	// @1246 'x' (6 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xCC, 0x58, 0x30, 0x30, 0x30, 0x48, 0xCC, 0x00, 0x00, 0x00, 

	// @1260 'y' (6 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x84, 0x48, 0x48, 0x48, 0x70, 0x30, 0x30, 0x20, 0x20, 0x40, 

	// @1274 'z' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x78, 0x10, 0x10, 0x20, 0x60, 0x40, 0xF8, 0x00, 0x00, 0x00, 

	// @1288 '{' (3 pixels wide)
	0x20, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x20, 0x00, 

	// @1302 '|' (1 pixels wide)
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 

	// @1316 '}' (3 pixels wide)
	0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x20, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x00, 

	// @1330 '~' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @1344 'Ё' (5 pixels wide)
	0x50, 0x00, 0xF8, 0x80, 0x80, 0x80, 0xF0, 0x80, 0x80, 0x80, 0xF8, 0x00, 0x00, 0x00, 

	// @1358 'А' (7 pixels wide)
	0x00, 0x00, 0x10, 0x28, 0x28, 0x28, 0x68, 0x44, 0x7C, 0x44, 0x86, 0x00, 0x00, 0x00, 

	// @1372 'Б' (6 pixels wide)
	0x00, 0x00, 0xF8, 0x80, 0x80, 0xF0, 0x88, 0x8C, 0x8C, 0x88, 0xF0, 0x00, 0x00, 0x00, 

	// @1386 'В' (6 pixels wide)
	0x00, 0x00, 0xF0, 0x88, 0x88, 0x88, 0xF0, 0x8C, 0x84, 0x8C, 0xF8, 0x00, 0x00, 0x00, 

	// @1400 'Г' (5 pixels wide)
	0x00, 0x00, 0xF8, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 

	// @1414 'Д' (7 pixels wide)
	0x00, 0x00, 0x3C, 0x24, 0x24, 0x24, 0x64, 0x44, 0x44, 0x44, 0xFE, 0x82, 0x82, 0x00, 

	// @1428 'Е' (5 pixels wide)
	0x00, 0x00, 0xF8, 0x80, 0x80, 0x80, 0xF0, 0x80, 0x80, 0x80, 0xF8, 0x00, 0x00, 0x00, 

	// @1442 'Ж' (10 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xC8, 0xC0, 0x48, 0x80, 0x29, 0x00, 0x29, 0x00, 0x3F, 0x00, 0x29, 0x00, 0x69, 0x80, 0x48, 0x80, 0xC8, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @1470 'З' (6 pixels wide)
	0x00, 0x00, 0x78, 0xC8, 0x08, 0x08, 0x30, 0x0C, 0x0C, 0x8C, 0x78, 0x00, 0x00, 0x00, 

	// @1484 'И' (6 pixels wide)
	0x00, 0x00, 0x8C, 0x8C, 0x9C, 0x94, 0x94, 0xA4, 0xA4, 0xC4, 0xC4, 0x00, 0x00, 0x00, 

	// @1498 'Й' (6 pixels wide)
	0x30, 0x00, 0x8C, 0x8C, 0x9C, 0x94, 0x94, 0xA4, 0xA4, 0xC4, 0xC4, 0x00, 0x00, 0x00, 

	// @1512 'К' (6 pixels wide)
	0x00, 0x00, 0x8C, 0x88, 0x90, 0x90, 0xF0, 0x90, 0x98, 0x88, 0x8C, 0x00, 0x00, 0x00, 

	// @1526 'Л' (7 pixels wide)
	0x00, 0x00, 0x3E, 0x22, 0x22, 0x22, 0x62, 0x62, 0x42, 0x42, 0xC2, 0x00, 0x00, 0x00, 

	// @1540 'М' (8 pixels wide)
	0x00, 0x00, 0xC6, 0xC6, 0xA6, 0xAA, 0xAA, 0xAA, 0x9A, 0x92, 0x83, 0x00, 0x00, 0x00, 

	// @1554 'Н' (6 pixels wide)
	0x00, 0x00, 0x84, 0x84, 0x84, 0x84, 0xFC, 0x84, 0x84, 0x84, 0x84, 0x00, 0x00, 0x00, 

	// @1568 'О' (7 pixels wide)
	0x00, 0x00, 0x78, 0xC4, 0x84, 0x86, 0x86, 0x86, 0x84, 0xC4, 0x78, 0x00, 0x00, 0x00, 

	// @1582 'П' (6 pixels wide)
	0x00, 0x00, 0xFC, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x00, 0x00, 0x00, 

	// @1596 'Р' (6 pixels wide)
	0x00, 0x00, 0xF0, 0x88, 0x8C, 0x8C, 0x88, 0xF0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 

	// @1610 'С' (5 pixels wide)
	0x00, 0x00, 0x78, 0xC8, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC8, 0x78, 0x00, 0x00, 0x00, 

	// @1624 'Т' (6 pixels wide)
	0x00, 0x00, 0xFC, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 

	// @1638 'У' (7 pixels wide)
	0x00, 0x00, 0x86, 0x44, 0x44, 0x68, 0x28, 0x38, 0x10, 0x10, 0x60, 0x00, 0x00, 0x00, 

	// @1652 'Ф' (9 pixels wide)
	0x00, 0x00, 0x08, 0x00, 0x3F, 0x00, 0x69, 0x80, 0x48, 0x80, 0x48, 0x80, 0xC8, 0x80, 0x48, 0x80, 0x69, 0x80, 0x3F, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @1680 'Х' (6 pixels wide)
	0x00, 0x00, 0xC4, 0x48, 0x28, 0x30, 0x30, 0x38, 0x68, 0x4C, 0xC4, 0x00, 0x00, 0x00, 

	// @1694 'Ц' (7 pixels wide)
	0x00, 0x00, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0xFE, 0x02, 0x02, 0x00, 

	// @1708 'Ч' (6 pixels wide)
	0x00, 0x00, 0xC4, 0xC4, 0xC4, 0xC4, 0x44, 0x7C, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 

	// @1722 'Ш' (9 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x88, 0x80, 0x88, 0x80, 0x88, 0x80, 0x88, 0x80, 0x88, 0x80, 0x88, 0x80, 0x88, 0x80, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @1750 'Щ' (10 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x88, 0x80, 0x88, 0x80, 0x88, 0x80, 0x88, 0x80, 0x88, 0x80, 0x88, 0x80, 0x88, 0x80, 0xFF, 0xC0, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00, 

	// @1778 'Ъ' (7 pixels wide)
	0x00, 0x00, 0xE0, 0x20, 0x20, 0x3C, 0x22, 0x22, 0x22, 0x22, 0x3C, 0x00, 0x00, 0x00, 

	// @1792 'Ы' (8 pixels wide)
	0x00, 0x00, 0x83, 0x83, 0x83, 0xF3, 0x8B, 0x8B, 0x8B, 0x8B, 0xF3, 0x00, 0x00, 0x00, 

	// @1806 'Ь' (6 pixels wide)
	0x00, 0x00, 0x80, 0x80, 0x80, 0xF0, 0x88, 0x8C, 0x8C, 0x88, 0xF0, 0x00, 0x00, 0x00, 

	// @1820 'Э' (6 pixels wide)
	0x00, 0x00, 0x70, 0x88, 0x0C, 0x04, 0x7C, 0x04, 0x0C, 0x88, 0x70, 0x00, 0x00, 0x00, 

	// @1834 'Ю' (9 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x8F, 0x00, 0x91, 0x00, 0x90, 0x80, 0xB0, 0x80, 0xF0, 0x80, 0xB0, 0x80, 0x90, 0x80, 0x91, 0x00, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	// @1862 'Я' (6 pixels wide)
	0x00, 0x00, 0x7C, 0x44, 0xC4, 0x44, 0x3C, 0x24, 0x64, 0x44, 0xC4, 0x00, 0x00, 0x00, 

	// @1876 'а' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xF0, 0x10, 0x10, 0xF0, 0x90, 0x90, 0xE8, 0x00, 0x00, 0x00, 

	// @1890 'б' (5 pixels wide)
	0x00, 0x18, 0x60, 0x80, 0xF0, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 

	// @1904 'в' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xF0, 0x88, 0x88, 0xF0, 0x88, 0x88, 0xF0, 0x00, 0x00, 0x00, 

	// @1918 'г' (4 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xF0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 

	// @1932 'д' (6 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x3C, 0x6C, 0x4C, 0x4C, 0x4C, 0x4C, 0xFC, 0x84, 0x84, 0x00, 

	// @1946 'е' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x70, 0x98, 0x88, 0xF8, 0x80, 0x88, 0x70, 0x00, 0x00, 0x00, 

	// @1960 'ж' (8 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xC9, 0x4A, 0x2A, 0x3E, 0x2A, 0x4B, 0xC9, 0x00, 0x00, 0x00, 

	// @1974 'з' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x70, 0x88, 0x08, 0x30, 0x08, 0x88, 0x78, 0x00, 0x00, 0x00, 

	// @1988 'и' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x98, 0x98, 0xB8, 0xA8, 0xE8, 0xC8, 0xC8, 0x00, 0x00, 0x00, 

	// @2002 'й' (5 pixels wide)
	0x00, 0x50, 0x70, 0x00, 0x98, 0x98, 0xB8, 0xA8, 0xE8, 0xC8, 0xC8, 0x00, 0x00, 0x00, 

	// @2016 'к' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x98, 0x90, 0xA0, 0xE0, 0xB0, 0x90, 0x88, 0x00, 0x00, 0x00, 

	// @2030 'л' (6 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x3C, 0x24, 0x24, 0x64, 0x44, 0x44, 0xC4, 0x00, 0x00, 0x00, 

	// @2044 'м' (6 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xB4, 0xB4, 0xB4, 0x84, 0x00, 0x00, 0x00, 

	// @2058 'н' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 

	// @2072 'о' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 

	// @2086 'п' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 

	// @2100 'р' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xB0, 0xC8, 0x88, 0x88, 0x88, 0x88, 0xF0, 0x80, 0x80, 0x80, 

	// @2114 'с' (4 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x70, 0x80, 0x80, 0x80, 0x80, 0x80, 0x70, 0x00, 0x00, 0x00, 

	// @2128 'т' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 

	// @2142 'у' (6 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x84, 0x48, 0x48, 0x48, 0x70, 0x30, 0x30, 0x20, 0x20, 0x40, 

	// @2156 'ф' (8 pixels wide)
	0x00, 0x10, 0x10, 0x10, 0x76, 0x99, 0x91, 0x91, 0x91, 0x9B, 0x76, 0x10, 0x10, 0x10, 

	// @2170 'х' (6 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xCC, 0x58, 0x30, 0x30, 0x30, 0x48, 0xCC, 0x00, 0x00, 0x00, 

	// @2184 'ц' (6 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0xFC, 0x04, 0x04, 0x00, 

	// @2198 'ч' (4 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0xF0, 0x10, 0x10, 0x00, 0x00, 0x00, 

	// @2212 'ш' (7 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0xFE, 0x00, 0x00, 0x00, 

	// @2226 'щ' (8 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0xFF, 0x01, 0x01, 0x00, 

	// @2240 'ъ' (6 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xE0, 0x20, 0x3C, 0x24, 0x24, 0x24, 0x3C, 0x00, 0x00, 0x00, 

	// @2254 'ы' (7 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x82, 0x82, 0xF2, 0x9A, 0x9A, 0x9A, 0xF2, 0x00, 0x00, 0x00, 

	// @2268 'ь' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xF0, 0x98, 0x88, 0x98, 0xF0, 0x00, 0x00, 0x00, 

	// @2282 'э' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0xF0, 0x08, 0x08, 0x78, 0x08, 0x08, 0xF0, 0x00, 0x00, 0x00, 

	// @2296 'ю' (8 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x8E, 0x92, 0xB3, 0xF1, 0xB3, 0x92, 0x8E, 0x00, 0x00, 0x00, 

	// @2310 'я' (5 pixels wide)
	0x00, 0x00, 0x00, 0x00, 0x78, 0x48, 0x48, 0x38, 0x28, 0x48, 0xC8, 0x00, 0x00, 0x00, 

	// @2324 'ё' (5 pixels wide)
	0x00, 0xD0, 0x00, 0x00, 0x70, 0x98, 0x88, 0xF8, 0x80, 0x88, 0x70, 0x00, 0x00, 0x00
};

// Character descriptors for Fira Sans 9pt
// { [Char width in bits], [Offset into firaSans9ptCharBitmaps in bytes] }
const FontCharInfo firaSans9ptDescriptors[] =
{
	{L'!', 1, 0}, 		// ! 
	{L'"', 3, 14}, 		// " 
	{L'#', 6, 28}, 		// # 
	{L'$', 6, 42}, 		// $ 
	{L'%', 8, 56}, 		// % 
	{L'&', 7, 70}, 		// & 
	{L'\'', 1, 84}, 		// ' 
	{L'(', 3, 98}, 		// ( 
	{L')', 3, 112}, 		// ) 
	{L'*', 5, 126}, 		// * 
	{L'+', 4, 140}, 		// + 
	{L',', 2, 154}, 		// , 
	{L'-', 3, 168}, 		// - 
	{L'.', 1, 182}, 		// . 
	{L'/', 4, 196}, 		// / 
	{L'0', 5, 210}, 		// 0 
	{L'1', 4, 224}, 		// 1 
	{L'2', 5, 238}, 		// 2 
	{L'3', 5, 252}, 		// 3 
	{L'4', 6, 266}, 		// 4 
	{L'5', 5, 280}, 		// 5 
	{L'6', 5, 294}, 		// 6 
	{L'7', 5, 308}, 		// 7 
	{L'8', 5, 322}, 		// 8 
	{L'9', 5, 336}, 		// 9 
	{L':', 1, 350}, 		// : 
	{L';', 2, 364}, 		// ; 
	{L'<', 4, 378}, 		// < 
	{L'=', 4, 392}, 		// = 
	{L'>', 4, 406}, 		// > 
	{L'?', 5, 420}, 		// ? 
	{L'@', 10, 434}, 		// @ 
	{L'A', 7, 462}, 		// A 
	{L'B', 6, 476}, 		// B 
	{L'C', 5, 490}, 		// C 
	{L'D', 6, 504}, 		// D 
	{L'E', 5, 518}, 		// E 
	{L'F', 5, 532}, 		// F 
	{L'G', 6, 546}, 		// G 
	{L'H', 6, 560}, 		// H 
	{L'I', 1, 574}, 		// I 
	{L'J', 3, 588}, 		// J 
	{L'K', 6, 602}, 		// K 
	{L'L', 5, 616}, 		// L 
	{L'M', 8, 630}, 		// M 
	{L'N', 6, 644}, 		// N 
	{L'O', 7, 658}, 		// O 
	{L'P', 6, 672}, 		// P 
	{L'Q', 7, 686}, 		// Q 
	{L'R', 6, 700}, 		// R 
	{L'S', 6, 714}, 		// S 
	{L'T', 6, 728}, 		// T 
	{L'U', 6, 742}, 		// U 
	{L'V', 7, 756}, 		// V 
	{L'W', 10, 770}, 		// W 
	{L'X', 6, 798}, 		// X 
	{L'Y', 7, 812}, 		// Y 
	{L'Z', 6, 826}, 		// Z 
	{L'[', 2, 840}, 		// [ 
	{L'\\', 4, 854}, 		// \ 
	{L']', 2, 868}, 		// ] 
	{L'^', 6, 882}, 		// ^ 
	{L'_', 6, 896}, 		// _ 
	{L'`', 3, 910}, 		// ` 
	{L'a', 5, 924}, 		// a 
	{L'b', 5, 938}, 		// b 
	{L'c', 4, 952}, 		// c 
	{L'd', 5, 966}, 		// d 
	{L'e', 5, 980}, 		// e 
	{L'f', 5, 994}, 		// f 
	{L'g', 6, 1008}, 		// g 
	{L'h', 5, 1022}, 		// h 
	{L'i', 1, 1036}, 		// i 
	{L'j', 2, 1050}, 		// j 
	{L'k', 5, 1064}, 		// k 
	{L'l', 2, 1078}, 		// l 
	{L'm', 8, 1092}, 		// m 
	{L'n', 5, 1106}, 		// n 
	{L'o', 5, 1120}, 		// o 
	{L'p', 5, 1134}, 		// p 
	{L'q', 5, 1148}, 		// q 
	{L'r', 4, 1162}, 		// r 
	{L's', 5, 1176}, 		// s 
	{L't', 4, 1190}, 		// t 
	{L'u', 5, 1204}, 		// u 
	{L'v', 6, 1218}, 		// v 
	{L'w', 8, 1232}, 		// w 
	{L'x', 6, 1246}, 		// x 
	{L'y', 6, 1260}, 		// y 
	{L'z', 5, 1274}, 		// z 
	{L'{', 3, 1288}, 		// { 
	{L'|', 1, 1302}, 		// | 
	{L'}', 3, 1316}, 		// } 
	{L'~', 5, 1330}, 		// ~ 
	{L'Ё', 5, 1344}, 		// Ё 
	{L'А', 7, 1358}, 		// А 
	{L'Б', 6, 1372}, 		// Б 
	{L'В', 6, 1386}, 		// В 
	{L'Г', 5, 1400}, 		// Г 
	{L'Д', 7, 1414}, 		// Д 
	{L'Е', 5, 1428}, 		// Е 
	{L'Ж', 10, 1442}, 		// Ж 
	{L'З', 6, 1470}, 		// З 
	{L'И', 6, 1484}, 		// И 
	{L'Й', 6, 1498}, 		// Й 
	{L'К', 6, 1512}, 		// К 
	{L'Л', 7, 1526}, 		// Л 
	{L'М', 8, 1540}, 		// М 
	{L'Н', 6, 1554}, 		// Н 
	{L'О', 7, 1568}, 		// О 
	{L'П', 6, 1582}, 		// П 
	{L'Р', 6, 1596}, 		// Р 
	{L'С', 5, 1610}, 		// С 
	{L'Т', 6, 1624}, 		// Т 
	{L'У', 7, 1638}, 		// У 
	{L'Ф', 9, 1652}, 		// Ф 
	{L'Х', 6, 1680}, 		// Х 
	{L'Ц', 7, 1694}, 		// Ц 
	{L'Ч', 6, 1708}, 		// Ч 
	{L'Ш', 9, 1722}, 		// Ш 
	{L'Щ', 10, 1750}, 		// Щ 
	{L'Ъ', 7, 1778}, 		// Ъ 
	{L'Ы', 8, 1792}, 		// Ы 
	{L'Ь', 6, 1806}, 		// Ь 
	{L'Э', 6, 1820}, 		// Э 
	{L'Ю', 9, 1834}, 		// Ю 
	{L'Я', 6, 1862}, 		// Я 
	{L'а', 5, 1876}, 		// а 
	{L'б', 5, 1890}, 		// б 
	{L'в', 5, 1904}, 		// в 
	{L'г', 4, 1918}, 		// г 
	{L'д', 6, 1932}, 		// д 
	{L'е', 5, 1946}, 		// е 
	{L'ж', 8, 1960}, 		// ж 
	{L'з', 5, 1974}, 		// з 
	{L'и', 5, 1988}, 		// и 
	{L'й', 5, 2002}, 		// й 
	{L'к', 5, 2016}, 		// к 
	{L'л', 6, 2030}, 		// л 
	{L'м', 6, 2044}, 		// м 
	{L'н', 5, 2058}, 		// н 
	{L'о', 5, 2072}, 		// о 
	{L'п', 5, 2086}, 		// п 
	{L'р', 5, 2100}, 		// р 
	{L'с', 4, 2114}, 		// с 
	{L'т', 5, 2128}, 		// т 
	{L'у', 6, 2142}, 		// у 
	{L'ф', 8, 2156}, 		// ф 
	{L'х', 6, 2170}, 		// х 
	{L'ц', 6, 2184}, 		// ц 
	{L'ч', 4, 2198}, 		// ч 
	{L'ш', 7, 2212}, 		// ш 
	{L'щ', 8, 2226}, 		// щ 
	{L'ъ', 6, 2240}, 		// ъ 
	{L'ы', 7, 2254}, 		// ы 
	{L'ь', 5, 2268}, 		// ь 
	{L'э', 5, 2282}, 		// э 
	{L'ю', 8, 2296}, 		// ю 
	{L'я', 5, 2310}, 		// я 
	{L'ё', 5, 2324} 		// ё 
};

// Font information for Fira Sans 9pt
const FontInfo firaSans9ptFontInfo =
{
	14, //  Character height
	L'!', //  Start character
	L'ё', //  End character
	1, //  Width, in pixels, of space character
	firaSans9ptDescriptors, //  Character decriptor array
	firaSans9ptBitmaps, //  Character bitmap array
};

} // namespace muil