#include <iterator>

#include "co2unit.hpp"
#include "hardware.hpp"

constexpr unsigned UsartBaudRate = 9600;

static const volatile uint8_t start_cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
static volatile unsigned  send_ptr = 0;
static const volatile uint8_t *send_data = nullptr;
static volatile unsigned send_len = 0;

static volatile uint8_t receive_buffer[9];
static volatile unsigned receive_ptr = 0;
static volatile unsigned receive_len = 0;

static volatile unsigned result = 0;
static volatile unsigned measures_count = 0;

void co2unit_init_hardware()
{
	CO2UsartTxPin::conf_alt_push_pull();
	CO2UsartRxPin::conf_in();

	CO2Usart::clock_on();
	CO2Usart::reset();
	CO2Usart::enable();

	CO2Usart::set_stop_bits(StopBits::_1);

	CO2Usart::set_baud_rate(SysClockFreq / UsartBaudRate);

	CO2Usart::enable_transmitter();
	CO2Usart::enable_receiver();

	CO2Usart::enable_tc_interrupt();
	CO2Usart::enable_rxne_interrupt();
	
	NVIC_SetPriority(CO2Usart::IRQn, 4);
	NVIC_EnableIRQ(CO2Usart::IRQn);
}

void co2unit_init()
{
	receive_len = 0;
	measures_count = 0;
	result = 0;
}

void co2unit_start_measure()
{
	send_ptr = 0;
	send_data = start_cmd;
	send_len = std::size(start_cmd)-1; // так как мы отправим один байт вне USART2_IRQHandler
	CO2Usart::send_data(*send_data);
}

unsigned co2unit_get_result()
{
	return result;
}

unsigned co2unit_get_measures_count()
{
	return measures_count;
}

static void start_receive_data()
{
	receive_ptr = 0;
	receive_len = std::size(receive_buffer);
}

static void process_received_data()
{
	uint8_t crc = 0;
	for (unsigned i = 1; i < std::size(receive_buffer)-1; i++) 
		crc += receive_buffer[i];

	crc = ~crc;
	crc++;

	if (receive_buffer[std::size(receive_buffer)-1] != crc) return;

	result = ((unsigned)receive_buffer[2] << 8) + receive_buffer[3];

	measures_count++;
}

extern "C" void CO2Usart_IRQHandler()
{
	if (CO2Usart::get_tc_flag())
	{
		if (send_len != 0)
		{
			send_len--;
			send_data++;
			CO2Usart::send_data(*send_data);
		}
		else
		{
			start_receive_data();
		}

		CO2Usart::clear_tc_flag();
	}

	if (CO2Usart::get_rxne_flag())
	{
		if (receive_len != 0)
		{
			uint8_t data = CO2Usart::get_data();

			if ((receive_ptr == 0) && (data != 0xFF)) // Проверяем, что это начало команды
			{
				receive_len = 0;
				CO2Usart::clear_rxne_flag();
				return;
			}

			receive_buffer[receive_ptr] = data;
			receive_ptr++;

			receive_len--;

			if (receive_len == 0) process_received_data();
		}

		CO2Usart::clear_rxne_flag();
	}
}