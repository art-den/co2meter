#include "muil_text_lib.hpp"

namespace muil {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Utf16CharactersProvider::Utf16CharactersProvider()
	: text_(nullptr)
{}

Utf16CharactersProvider::Utf16CharactersProvider(const wchar_t *text)
	: text_(text)
{}


void Utf16CharactersProvider::provide(ICharactersConsumer &consumer) const
{
	for (auto ptr = text_; *ptr; ptr++) consumer.consume(*ptr);
}

void Utf16CharactersProvider::set_text(const wchar_t * text)
{
	text_ = text;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Utf8CharactersProvider::Utf8CharactersProvider() :
	text_(nullptr),
	len_ (0)
{}

Utf8CharactersProvider::Utf8CharactersProvider(const char *text) :
	text_(text          ),
	len_ (calc_len(text))
{}

void Utf8CharactersProvider::provide(ICharactersConsumer &consumer) const
{
	if (text_ == nullptr) return;
	const char* ptr = text_;
	while (*ptr)
	{
		wchar_t chr = get_char_and_move_to_next(ptr);
		if (chr == 0) break;
		consumer.consume(chr);
	}
}

void Utf8CharactersProvider::set_text(const char * text)
{
	text_ = text;
	len_  = calc_len(text);
}

unsigned Utf8CharactersProvider::calc_len(const char *text)
{
	if (text_ == nullptr) return 0;
	unsigned result = 0;
	const char* ptr = text_;
	while (*ptr)
	{
		wchar_t chr = get_char_and_move_to_next(ptr);
		if (chr == 0) break;
		result++;
	}
	return result;
}

wchar_t Utf8CharactersProvider::get_char_and_move_to_next(const char* &ptr) const
{
	auto get_other_bits = [&ptr](char start_bits, unsigned count) -> unsigned
	{
		unsigned result = (unsigned char)start_bits;
		for (unsigned i = 0; i < count; i++)
		{
			result <<= 6;
			if ((*ptr & 0b11000000) != 0b10000000) return 0;
			result |= *ptr & 0b00111111;
			ptr++;
		}
		return result;
	};

	if ((*ptr & 0b10000000) == 0)
		return *ptr++;

	else if ((*ptr & 0b11100000) == 0b11000000)
		return get_other_bits(*ptr++ & 0b00011111, 1);

	else if ((*ptr & 0b11110000) == 0b11100000)
		return get_other_bits(*ptr++ & 0b00001111, 2);

	else if ((*ptr & 0b11111000) == 0b11110000)
		return get_other_bits(*ptr++ & 0b00000111, 3);

	else if ((*ptr & 0b11111100) == 0b11111000)
		return get_other_bits(*ptr++ & 0b00000011, 4);

	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Integer10CharactersProvider::Integer10CharactersProvider(int value, int pt_pos, bool show_sign)
	: IntegerCharactersProvider(value),
	  pt_pos_(pt_pos),
	  show_sign_(show_sign)
{}


void Integer10CharactersProvider::provide(ICharactersConsumer &consumer) const
{
	int value = value_;
	int pt_pos = pt_pos_;

	if (value < 0)
	{
		value = -value;
		consumer.consume('-');
	}

	if (show_sign_ && (value >= 0))
		consumer.consume('+');

	int comp_value = 1000000000;

	if (pt_pos != -1) pt_pos = 9 - pt_pos;

	bool begin = true;
	do
	{
		wchar_t chr = '0';
		while (value >= comp_value)
		{
			value -= comp_value;
			chr++;
		}
		if ((chr != '0') || (pt_pos == 0)) begin = false;
		if (!begin || (comp_value == 1))
			consumer.consume(chr);
		comp_value /= 10;
		if ((pt_pos != -1) && (pt_pos-- == 0))
			consumer.consume('.');
	}
	while (comp_value != 0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Integer16CharactersProvider::Integer16CharactersProvider(unsigned value)
	: IntegerCharactersProvider(value)
{}


void Integer16CharactersProvider::provide(ICharactersConsumer &consumer) const
{
	unsigned value = value_;
	bool begin = true;
	for (unsigned i = 0; i < 8; i++)
	{
		unsigned val = (value & 0xF0000000) >> 28;
		wchar_t chr = (val < 10) ? (val + '0') : (val - 10 + 'A');
		if ((val != 0) || (i == 7)) begin = false;
		if (!begin) consumer.consume(chr);
		value <<= 4;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HourAndMiniteCharactersProvider::provide(ICharactersConsumer &consumer) const
{
	print_int10(consumer, hour_);
	consumer.consume(':');
	if (minute_ < 10) consumer.consume('0');
	print_int10(consumer, minute_);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void print_text(ICharactersConsumer &consumer, const char *utf8_text)
{
	Utf8CharactersProvider provider(utf8_text);
	provider.provide(consumer);
}

void print_text(ICharactersConsumer &consumer, const wchar_t *text)
{
	Utf16CharactersProvider provider(text);
	provider.provide(consumer);
}

void print_int10(ICharactersConsumer &consumer, int value, int pt_pos, bool show_sign)
{
	Integer10CharactersProvider provider(value, pt_pos, show_sign);
	provider.provide(consumer);
}

void print_int10(ICharactersConsumer &consumer, int value)
{
	print_int10(consumer, value, -1, false);
}

} // namespace muil