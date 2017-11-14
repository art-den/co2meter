#pragma once

#include <stdint.h>

namespace muil {

class ICharactersConsumer
{
public:
	virtual void consume(wchar_t character) = 0;
};


class ICharactersProvider
{
public:
	virtual void provide(ICharactersConsumer &consumer) const = 0;
};


class Utf16CharactersProvider : public ICharactersProvider
{
public:
	Utf16CharactersProvider();
	explicit Utf16CharactersProvider(const wchar_t *text);
	void provide(ICharactersConsumer &consumer) const override;

	void set_text(const wchar_t * text);

private:
	const wchar_t * text_;
};


class Utf8CharactersProvider : public ICharactersProvider
{
public:
	Utf8CharactersProvider();
	explicit Utf8CharactersProvider(const char *text);
	void provide(ICharactersConsumer &consumer) const override;

	void set_text(const char * text);

private:
	const char * text_;
	unsigned len_;

	unsigned calc_len(const char *text);
	wchar_t get_char_and_move_to_next(const char* &ptr) const;
};


class IntegerCharactersProvider : public ICharactersProvider
{
public:
	IntegerCharactersProvider(int value) : value_(value) {}

	int get_value() const { return value_; }
	void set_value(int value) { value_ = value; }

protected:
	int value_;
};

class Integer10CharactersProvider : public IntegerCharactersProvider
{
public:
	Integer10CharactersProvider(int value, int pt_pos, bool show_sign = false);
	void provide(ICharactersConsumer &consumer) const override;

private:
	const int pt_pos_;
	const bool show_sign_;
};


class Integer16CharactersProvider : public IntegerCharactersProvider
{
public:
	explicit Integer16CharactersProvider(unsigned value);
	void provide(ICharactersConsumer &consumer) const override;
};

class HourAndMiniteCharactersProvider : public ICharactersProvider
{
public:
	HourAndMiniteCharactersProvider(uint8_t hour, uint8_t minute) :
		hour_(hour), minute_(minute)
	{}

	void provide(ICharactersConsumer &consumer) const override;

private:
	const uint8_t hour_;
	const uint8_t minute_;
};

void print_text(ICharactersConsumer &consumer, const char *utf8_text);
void print_text(ICharactersConsumer &consumer, const wchar_t *text);
void print_int10(ICharactersConsumer &consumer, int value, int pt_pos, bool show_sign);
void print_int10(ICharactersConsumer &consumer, int value);

} // namespace muil
