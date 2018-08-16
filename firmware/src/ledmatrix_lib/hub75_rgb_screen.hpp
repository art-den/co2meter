#pragma once 

#include <stdint.h>

namespace ledmatrix {
	
/* class Hub75RGBScreenBase */
	
template <
	unsigned HorizUnits,
	unsigned VertUnits,
	unsigned UnitWidth,
	unsigned UnitHeight,
	typename PixelType
>
class Hub75RGBScreenBase
{
public:
	static constexpr unsigned horiz_units = HorizUnits;
	static constexpr unsigned vert_units = VertUnits;
	static constexpr unsigned unit_width = UnitWidth;
	static constexpr unsigned unit_height = UnitHeight;
	static constexpr unsigned width = HorizUnits * UnitWidth;
	static constexpr unsigned height = VertUnits * UnitHeight;
	static constexpr unsigned row_buffer_len = 2 * width + 4;
	static constexpr unsigned unit_scan_height = UnitHeight/2;
	
	template <typename GetRowMaskFun>
	static uint16_t get_row_off_value(
		unsigned            cur_row,
		unsigned            oe_pin_index,
		const GetRowMaskFun &get_row_mask_fun)
	{
		const uint16_t oe_mask = 1 << oe_pin_index;
		const uint16_t row_value = get_row_mask_fun(cur_row);
		
		return oe_mask|row_value;
	}
	
protected:
	using Row = PixelType[width];

	volatile Row rows_[height] = {};

	template <unsigned RedMask, unsigned GreenMask, unsigned BlueMask, typename GetRowMaskFun>
	void fill_data_for_row_into_buffer_internal(
		volatile uint16_t   *dst_begin,
		volatile uint16_t   *dst_end,
		unsigned            cur_row,
		unsigned            row_offset,
		unsigned            bam_stage,
		unsigned            R1PinIndex,
		unsigned            R2PinIndex,
		unsigned            G1PinIndex,
		unsigned            G2PinIndex,
		unsigned            B1PinIndex,
		unsigned            B2PinIndex,
		unsigned            OEPinIndex,
		unsigned            LatPinIndex,
		unsigned            ClkPinIndex,
		const GetRowMaskFun &get_row_mask_fun) const
	{
		typedef uint_fast16_t ResType;
		typedef uint_fast16_t SrcType;

		const ResType ClkMask = 1 << ClkPinIndex;
		const ResType OEMask = 1 << OEPinIndex;
		const ResType LatMask = 1 << LatPinIndex;

		auto* row1 = &rows_[row_offset + UnitHeight - 1 - cur_row][width - 1];
		auto* row2 = &rows_[row_offset + UnitHeight - 1 - (cur_row + UnitHeight / 2)][width - 1];

		const SrcType R1Mask = 1 << R1PinIndex;
		const SrcType G1Mask = 1 << G1PinIndex;
		const SrcType B1Mask = 1 << B1PinIndex;
		const SrcType R2Mask = 1 << R2PinIndex;
		const SrcType G2Mask = 1 << G2PinIndex;
		const SrcType B2Mask = 1 << B2PinIndex;

		const SrcType red_color_mask   = RedMask >> bam_stage;
		const SrcType green_color_mask = GreenMask >> bam_stage;
		const SrcType blue_color_mask  = BlueMask >> bam_stage;
		
		ResType row_value = get_row_mask_fun(cur_row);

		for (unsigned col = 0; col < width; col++)
		{
			ResType value = row_value|OEMask; // OE <- 1
			
			const SrcType v1 = *row1;
			if (v1 & red_color_mask) value |= R1Mask;
			if (v1 & green_color_mask) value |= G1Mask;
			if (v1 & blue_color_mask) value |= B1Mask;
			
			const SrcType v2 = *row2;
			if (v2 & red_color_mask) value |= R2Mask;
			if (v2 & green_color_mask) value |= G2Mask;
			if (v2 & blue_color_mask) value |= B2Mask;

			*dst_begin++ = value;
			*dst_begin++ = value | ClkMask;
			
			row1--;
			row2--;
		}

		*dst_begin++ = row_value|OEMask; // Row
		*dst_begin++ = row_value|LatMask|OEMask; // LAT <- 1
		*dst_begin++ = row_value|OEMask; // LAT <- 0
		*dst_begin++ = row_value;  // OE <- 0

		while (dst_begin != dst_end) {}
	}
};

/* class Hub75RGBScreen555 */

template <
	unsigned HorizUnits,
	unsigned VertUnits,
	unsigned UnitWidth,
	unsigned UnitHeight
>
class Hub75RGBScreen555 : public Hub75RGBScreenBase<HorizUnits, VertUnits, UnitWidth, UnitHeight, uint16_t>
{
public:
	static constexpr unsigned color_depth = 5;
	
	using Base = Hub75RGBScreenBase<HorizUnits, VertUnits, UnitWidth, UnitHeight, uint16_t>;
	
	using Base::horiz_units;
	using Base::vert_units;
	using Base::unit_width;
	using Base::unit_height;
	using Base::width;
	using Base::height;
	using Base::row_buffer_len;
	using Base::unit_scan_height;

	void set_pixel(unsigned x, unsigned y, uint8_t r, uint8_t g, uint8_t b)
	{
		auto &v = Base::rows_[y][x];
		v = 
			(uint16_t(r & 0b11111000) << 7) | 
			(uint16_t(g & 0b11111000) << 2) | 
			(uint16_t(b & 0b11111000) >> 3);
	}

	template <typename GetRowMaskFun>
	void fill_data_for_row_into_buffer(
		volatile uint16_t   *dst_begin,
		volatile uint16_t   *dst_end,
		unsigned            cur_row,
		unsigned            row_offset,
		unsigned            bam_stage,
		unsigned            R1PinIndex,
		unsigned            R2PinIndex,
		unsigned            G1PinIndex,
		unsigned            G2PinIndex,
		unsigned            B1PinIndex,
		unsigned            B2PinIndex,
		unsigned            OEPinIndex,
		unsigned            LatPinIndex,
		unsigned            ClkPinIndex,
		const GetRowMaskFun &get_row_mask_fun) const
	{
		Base::template fill_data_for_row_into_buffer_internal<
			0b100000000000000,
			0b000001000000000,
			0b000000000010000>
		(
			dst_begin,
			dst_end,
			cur_row,
			row_offset,
			bam_stage,
			R1PinIndex,
			R2PinIndex,
			G1PinIndex,
			G2PinIndex,
			B1PinIndex,
			B2PinIndex,
			OEPinIndex,
			LatPinIndex,
			ClkPinIndex,
			get_row_mask_fun
		);
	}
};

/* class Hub75RGBScreen222 */

template <
	unsigned HorizUnits,
	unsigned VertUnits,
	unsigned UnitWidth,
	unsigned UnitHeight
>
class Hub75RGBScreen222 : public Hub75RGBScreenBase<HorizUnits, VertUnits, UnitWidth, UnitHeight, uint8_t>
{
public:
	static constexpr unsigned color_depth = 2;
	
	using Base = Hub75RGBScreenBase<HorizUnits, VertUnits, UnitWidth, UnitHeight, uint8_t>;
	
	using Base::horiz_units;
	using Base::vert_units;
	using Base::unit_width;
	using Base::unit_height;
	using Base::width;
	using Base::height;
	using Base::row_buffer_len;
	using Base::unit_scan_height;

	void set_pixel(unsigned x, unsigned y, uint8_t r, uint8_t g, uint8_t b)
	{
		auto &v = Base::rows_[y][x];
		v = 
			((r & 0b11000000) >> 2) | 
			((g & 0b11000000) >> 4) | 
			((b & 0b11000000) >> 6);
	}

	template <typename GetRowMaskFun>
	void fill_data_for_row_into_buffer(
		volatile uint16_t   *dst_begin,
		volatile uint16_t   *dst_end,
		unsigned            cur_row,
		unsigned            row_offset,
		unsigned            bam_stage,
		unsigned            R1PinIndex,
		unsigned            R2PinIndex,
		unsigned            G1PinIndex,
		unsigned            G2PinIndex,
		unsigned            B1PinIndex,
		unsigned            B2PinIndex,
		unsigned            OEPinIndex,
		unsigned            LatPinIndex,
		unsigned            ClkPinIndex,
		const GetRowMaskFun &get_row_mask_fun) const
	{
		Base::template fill_data_for_row_into_buffer_internal<
			0b100000,
			0b001000,
			0b000010>
		(
			dst_begin,
			dst_end,
			cur_row,
			row_offset,
			bam_stage,
			R1PinIndex,
			R2PinIndex,
			G1PinIndex,
			G2PinIndex,
			B1PinIndex,
			B2PinIndex,
			OEPinIndex,
			LatPinIndex,
			ClkPinIndex,
			get_row_mask_fun
		);
	}
};

enum class Color16
{
	None            = 0b0001,
	Black           = 0b0000,
	Red             = 0b1000,
	Green           = 0b0100,
	Blue            = 0b0010,
	BrightBit       = 0x0001,
	BrightRed       = Red|BrightBit,
	BrightGreen     = Green|BrightBit,
	BrightBlue      = Blue|BrightBit,
	Yellow          = Red|Green,
	BrightYellow    = Yellow|BrightBit,
	Turquoise       = Green|Blue,
	BrightTurquoise = Turquoise|BrightBit,
	Purple          = Red|Blue,
	BrightPurple    = Purple|BrightBit,
	Grey            = Red|Green|Blue,
	White           = Grey|BrightBit
};

template <
	unsigned HorizUnits,
	unsigned VertUnits,
	unsigned UnitWidth,
	unsigned UnitHeight
>
class Hub75RGB16Colors
{
public:
	static constexpr unsigned horiz_units = HorizUnits;
	static constexpr unsigned vert_units = VertUnits;
	static constexpr unsigned unit_width = UnitWidth;
	static constexpr unsigned unit_height = UnitHeight;
	static constexpr unsigned width = HorizUnits * UnitWidth;
	static constexpr unsigned height = VertUnits * UnitHeight;
	static constexpr unsigned row_buffer_len = 2 * width + 4;
	static constexpr unsigned unit_scan_height = UnitHeight/2;
	static constexpr unsigned color_depth = 3;

	void set_pixel(unsigned x, unsigned y, Color16 color)
	{
		put_pixel_unsafe(x, y, color);
	}

	Color16 get_pixel(unsigned x, unsigned y) const
	{
		auto &byte = data_[y][x >> 1];
		if (x & 1)
			return (Color16)(byte & 0x0F);
		else
			return (Color16)((byte >> 4) & 0xF);
	}
	
	template <typename GetRowMaskFun>
	void fill_data_for_row_into_buffer(
		volatile uint16_t   *buf_begin,
		volatile uint16_t   *buf_end,
		unsigned            cur_row,
		unsigned            row_offset,
		unsigned            bam_stage,
		unsigned            R1PinIndex,
		unsigned            R2PinIndex,
		unsigned            G1PinIndex,
		unsigned            G2PinIndex,
		unsigned            B1PinIndex,
		unsigned            B2PinIndex,
		unsigned            OEPinIndex,
		unsigned            LatPinIndex,
		unsigned            ClkPinIndex,
		const GetRowMaskFun &get_row_mask_fun) const
	{
		bool only_bright = bam_stage <= 1;
		
		const uint_fast16_t ClkMask = 1 << ClkPinIndex;
		const uint_fast16_t OEMask = 1 << OEPinIndex;
		const uint_fast16_t LatMask = 1 << LatPinIndex;
		
		auto* row1 = get_ptr(width-1, row_offset + UnitHeight-1-cur_row);
		auto* row2 = get_ptr(width-1, row_offset + UnitHeight-1-(cur_row + UnitHeight/2));
		
		const uint_fast16_t R1Mask = 1 << R1PinIndex;
		const uint_fast16_t G1Mask = 1 << G1PinIndex;
		const uint_fast16_t B1Mask = 1 << B1PinIndex;
		const uint_fast16_t R2Mask = 1 << R2PinIndex;
		const uint_fast16_t G2Mask = 1 << G2PinIndex;
		const uint_fast16_t B2Mask = 1 << B2PinIndex;
		
		const uint_fast8_t r_test = (uint_fast8_t)Color16::Red | (only_bright ? (uint_fast8_t)Color16::BrightBit : 0);
		const uint_fast8_t g_test = (uint_fast8_t)Color16::Green | (only_bright ? (uint_fast8_t)Color16::BrightBit : 0);
		const uint_fast8_t b_test = (uint_fast8_t)Color16::Blue | (only_bright ? (uint_fast8_t)Color16::BrightBit : 0);
		
		uint_fast16_t row_value = get_row_mask_fun(cur_row);

		for (unsigned col = 0; col < width/2; col++)
		{
			uint_fast8_t v1 = *row1--;
			uint_fast8_t v2 = *row2--;

			for (unsigned part = 0; part < 2; part++)
			{
				uint_fast16_t value = row_value|OEMask;

				if ((v1 & r_test) == r_test) value |= R1Mask;
				if ((v1 & g_test) == g_test) value |= G1Mask;
				if ((v1 & b_test) == b_test) value |= B1Mask;

				if ((v2 & r_test) == r_test) value |= R2Mask;
				if ((v2 & g_test) == g_test) value |= G2Mask;
				if ((v2 & b_test) == b_test) value |= B2Mask;
		
				*buf_begin++ = value;
				*buf_begin++ = value|ClkMask;

				v1 >>= 4;
				v2 >>= 4;
			}
		}

		*buf_begin++ = row_value|OEMask;
		*buf_begin++ = row_value|OEMask|LatMask; // LAT <- 1
		*buf_begin++ = row_value|OEMask; // LAT <- 0
		*buf_begin++ = row_value;  // OE <- 0
	}
	
	template <typename GetRowMaskFun>
	static uint16_t get_row_off_value(
		unsigned            cur_row,
		unsigned            oe_pin_index,
		const GetRowMaskFun &get_row_mask_fun)
	{
		const uint16_t oe_mask = 1 << oe_pin_index;
		const uint16_t row_value = get_row_mask_fun(cur_row);
		
		return oe_mask|row_value;
	}

private:
	volatile uint8_t data_[height][width/2] = {};
	
	volatile const uint8_t* get_ptr(int x, int y) const
	{
		return &data_[y][x >> 1];
	}

	void put_pixel_unsafe(int x, int y, Color16 color)
	{
		auto &byte = data_[y][x >> 1];
		if (x & 1)
			byte = (byte & 0xF0) | (uint8_t)color;
		else
			byte = (byte & 0x0F) | ((uint8_t)color << 4);
	}
};



} // namespace ledmatrix