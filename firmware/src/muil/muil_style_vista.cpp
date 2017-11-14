#include <stddef.h>
#include "muil_style_vista.hpp"

namespace muil {

// normal button

static const uint8_t vista_normal_btn_lt[] = {
	0xff, 0x90, 0x77,
	0x88, 0x91, 0xe8,
	0x75, 0xea, 0xfa
};

static const uint8_t vista_normal_btn_t[] = {
	0x70,
	0xfc,
	0xf2
};

static const uint8_t vista_normal_btn_rt[] = {
	0x77, 0x90, 0xff,
	0xe8, 0x91, 0x88,
	0xfa, 0xea, 0x75
};


static const uint8_t vista_normal_btn_l[] = {
	0x70, 0xfb, 0xf1,
	0x70, 0xfb, 0xef,
	0x70, 0xfa, 0xed,
	0x70, 0xfa, 0xec,
	0x70, 0xf9, 0xe9,
	0x70, 0xf5, 0xdb,
	0x70, 0xf5, 0xd8,
	0x70, 0xf5, 0xd7,
	0x70, 0xf4, 0xd4,
	0x70, 0xf3, 0xd1
};

static const uint8_t vista_normal_btn_c[] = {
	0xf2,
	0xf0,
	0xed,
	0xed,
	0xeb,
	0xdb,
	0xd8,
	0xd8,
	0xd4,
	0xd1
};

static const uint8_t vista_normal_btn_r[] = {
	0xf1, 0xfb, 0x70,
	0xef, 0xfb, 0x70,
	0xed, 0xfa, 0x70,
	0xec, 0xfa, 0x70,
	0xe9, 0xf9, 0x70,
	0xdb, 0xf5, 0x70,
	0xd8, 0xf5, 0x70,
	0xd7, 0xf5, 0x70,
	0xd4, 0xf4, 0x70,
	0xd1, 0xf3, 0x70
};

static const uint8_t vista_normal_btn_lb[] = {
	0x75, 0xe2, 0xec,
	0x88, 0x8f, 0xe4,
	0xff, 0x86, 0x75
};

static const uint8_t vista_normal_btn_b[] = {
	0xcf,
	0xf3,
	0x70
};

static const uint8_t vista_normal_btn_rb[] = {
	0xec, 0xe2, 0x75,
	0xe4, 0x8f, 0x88,
	0x75, 0x86, 0xff
};

// pressed button

static const uint8_t vista_pressed_btn_lt[] = {
	0xff, 0x54, 0x58,
	0x51, 0x61, 0x99,
	0x57, 0x99, 0xc8
};

static const uint8_t vista_pressed_btn_t[] = {
	0x59,
	0xad,
	0xf1
};

static const uint8_t vista_pressed_btn_rt[] = {
	0x58, 0x54, 0xff,
	0xa7, 0x61, 0x51,
	0xc8, 0xa7, 0x57
};

static const uint8_t vista_pressed_btn_l[] = {
	0x59, 0xbb, 0xeb,
	0x59, 0xbe, 0xec,
	0x59, 0xbb, 0xe8,
	0x59, 0xb9, 0xe6,
	0x59, 0xb5, 0xe1,
	0x59, 0xa3, 0xc9,
	0x59, 0x9c, 0xc1,
	0x59, 0x9a, 0xbe,
	0x59, 0x93, 0xb6,
	0x59, 0x8c, 0xae
};

static const uint8_t vista_pressed_btn_c[] = {
	0xf1,
	0xed,
	0xe8,
	0xe8,
	0xe2,
	0xc7,
	0xbf,
	0xbf,
	0xb6,
	0xad
};

static const uint8_t vista_pressed_btn_r[] = {
	0xea, 0xbb, 0x59,
	0xec, 0xbe, 0x59,
	0xe8, 0xbb, 0x59,
	0xe6, 0xb9, 0x59,
	0xe1, 0xb5, 0x59,
	0xc9, 0xa3, 0x59,
	0xc1, 0x9c, 0x59,
	0xbe, 0x9a, 0x59,
	0xb6, 0x93, 0x59,
	0xae, 0x8c, 0x59
};

static const uint8_t vista_pressed_btn_lb[] = {
	0x59, 0x88, 0xa9,
	0x68, 0x5b, 0x9f,
	0xff, 0x6e, 0x5b
};

static const uint8_t vista_pressed_btn_b[] = {
	0xa9,
	0xa6,
	0x59
};

static const uint8_t vista_pressed_btn_rb[] = {
	0xa9, 0x88, 0x59,
	0x9f, 0x5b, 0x68,
	0x5b, 0x6e, 0xff
};

void vista_draw_button(const Rect &rect, Color color, ButtonStyle style, bool composite)
{
	if (composite) return;
	switch (style)
	{
	case ButtonStyle::Normal:
			paint_bitmapped_widget(
				rect, color,
				vista_normal_btn_lt, vista_normal_btn_t, vista_normal_btn_rt,
				vista_normal_btn_l,  vista_normal_btn_c, vista_normal_btn_r,
				vista_normal_btn_lb, vista_normal_btn_b, vista_normal_btn_rb,
				3, 1, 3,
				3, 10, 3
			);
			break;

	case ButtonStyle::Pressed:
			paint_bitmapped_widget(
				rect, color,
				vista_pressed_btn_lt, vista_pressed_btn_t, vista_pressed_btn_rt,
				vista_pressed_btn_l,  vista_pressed_btn_c, vista_pressed_btn_r,
				vista_pressed_btn_lb, vista_pressed_btn_b, vista_pressed_btn_rb,
				3, 1, 3,
				3, 10, 3
			);
			break;
	}
}

void vista_draw_choice_rect(const Rect &rect, Color color, ButtonStyle style)
{
	vista_draw_button(rect, color, style, false);
}

// normal checkbox

static const uint8_t vista_normal_chk_lt[] = {
	0x8e, 0x8e, 0x8e,
	0x8e, 0xf4, 0xf4,
	0x8e, 0xf4, 0xb2
};

static const uint8_t vista_normal_chk_t[] = {
	0x8e, 0x8e, 0x8e,
	0xf4, 0xf4, 0xf4,
	0xb2, 0xb2, 0xb3
};

static const uint8_t vista_normal_chk_rt[] = {
	0x8e, 0x8e, 0x8e,
	0xf4, 0xf4, 0x8e,
	0xbd, 0xf4, 0x8e
};

static const uint8_t vista_normal_chk_l[] = {
	0x8e, 0xf4, 0xb2,
	0x8e, 0xf4, 0xb4,
	0x8e, 0xf4, 0xbc
};

static const uint8_t vista_normal_chk_c[] = {
	0xce, 0xd0, 0xd8,
	0xd5, 0xde, 0xe8,
	0xe3, 0xee, 0xf3
};

static const uint8_t vista_normal_chk_r[] = {
	0xc4, 0xf4, 0x8e,
	0xd0, 0xf4, 0x8e,
	0xdf, 0xf4, 0x8e
};

static const uint8_t vista_normal_chk_lb[] = {
	0x8e, 0xf4, 0xca,
	0x8e, 0xf4, 0xf4,
	0x8e, 0x8e, 0x8e
};

static const uint8_t vista_normal_chk_b[] = {
	0xd8, 0xe3, 0xeb,
	0xf4, 0xf4, 0xf4,
	0x8e, 0x8e, 0x8e
};

static const uint8_t vista_normal_chk_rb[] = {
	0xe9, 0xf4, 0x8e,
	0xf4, 0xf4, 0x8e,
	0x8e, 0x8e, 0x8e
};

// pressed checkbox

static const uint8_t vista_pressed_chk_lt[] = {
	0x59, 0x59, 0x59,
	0x59, 0xde, 0xde,
	0x59, 0xde, 0xa8
};

static const uint8_t vista_pressed_chk_t[] = {
	0x59, 0x59, 0x59,
	0xde, 0xde, 0xde,
	0xa8, 0xa8, 0xa9
};

static const uint8_t vista_pressed_chk_rt[] = {
	0x59, 0x59, 0x59,
	0xde, 0xde, 0x59,
	0xae, 0xde, 0x59
};

static const uint8_t vista_pressed_chk_l[] = {
	0x59, 0xde, 0xa8,
	0x59, 0xde, 0xa9,
	0x59, 0xde, 0xae
};

static const uint8_t vista_pressed_chk_c[] = {
	0xcc, 0xcd, 0xd2,
	0xd1, 0xd6, 0xdd,
	0xd9, 0xe4, 0xec
};

static const uint8_t vista_pressed_chk_r[] = {
	0xb2, 0xde, 0x59,
	0xbf, 0xde, 0x59,
	0xd2, 0xde, 0x59
};

static const uint8_t vista_pressed_chk_lb[] = {
	0x59, 0xde, 0xb7,
	0x59, 0xde, 0xde,
	0x59, 0x59, 0x59
};

static const uint8_t vista_pressed_chk_b[] = {
	0xc9, 0xd9, 0xe2,
	0xde, 0xde, 0xde,
	0x59, 0x59, 0x59
};

static const uint8_t vista_pressed_chk_rb[] = {
	0xe0, 0xde, 0x59,
	0xde, 0xde, 0x59,
	0x59, 0x59, 0x59
};

void vista_draw_checkbox_rect(const Rect &rect, Color color, ButtonStyle style)
{
	switch (style)
	{
	case ButtonStyle::Normal:
			paint_bitmapped_widget(
				rect, color,
				vista_normal_chk_lt, vista_normal_chk_t, vista_normal_chk_rt,
				vista_normal_chk_l,  vista_normal_chk_c, vista_normal_chk_r,
				vista_normal_chk_lb, vista_normal_chk_b, vista_normal_chk_rb,
				3, 3, 3,
				3, 3, 3
			);
			break;

	case ButtonStyle::Pressed:
			paint_bitmapped_widget(
				rect, color,
				vista_pressed_chk_lt, vista_pressed_chk_t, vista_pressed_chk_rt,
				vista_pressed_chk_l,  vista_pressed_chk_c, vista_pressed_chk_r,
				vista_pressed_chk_lb, vista_pressed_chk_b, vista_pressed_chk_rb,
				3, 3, 3,
				3, 3, 3
			);
			break;
	}
}

// normal indented widget

static const uint8_t vista_normal_indented_lt[] = {
	0xd6, 0xbd,
	0xe3, 0xeb
};

static const uint8_t vista_normal_indented_t[] = {
	0xad,
	0xfe
};

static const uint8_t vista_normal_indented_rt[] = {
	0xbd, 0xd6,
	0xeb, 0xde
};

static const uint8_t vista_normal_indented_l[] = {
	0xe3, 0xfe
};

static const uint8_t vista_normal_indented_c[] = {
	0xfe
};

static const uint8_t vista_normal_indented_r[] = {
	0xff, 0xde
};

static const uint8_t vista_normal_indented_lb[] = {
	0xe3, 0xeb,
	0xeb, 0xe7
};

static const uint8_t vista_normal_indented_b[] = {
	0xfe,
	0xe7
};

static const uint8_t vista_normal_indented_rb[] = {
	0xeb, 0xde,
	0xe7, 0xeb
};

void vista_draw_indented_ctrl_rect(const Rect &rect, Color color, ButtonStyle style)
{
	paint_bitmapped_widget(
		rect, color,
		vista_normal_indented_lt, vista_normal_indented_t, vista_normal_indented_rt,
		vista_normal_indented_l,  vista_normal_indented_c, vista_normal_indented_r,
		vista_normal_indented_lb, vista_normal_indented_b, vista_normal_indented_rb,
		2, 1, 2,
		2, 1, 2
	);

}

int vista_get_indented_ctrl_border()
{
	return 2;
}


} // namespace muil
