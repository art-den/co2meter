#pragma once

namespace ledmatrix {

template <typename Screen>
struct UnitBamData
{
	volatile uint16_t data1[Screen::row_buffer_len * Screen::color_depth] = {};
	volatile uint16_t data2[Screen::row_buffer_len * Screen::color_depth] = {};
	volatile const uint16_t* bam_data_send_ptr = nullptr;
	volatile uint16_t* bam_data_to_fill = nullptr;
};

template <
	typename R1Pin,
	typename R2Pin,
	typename G1Pin,
	typename G2Pin,
	typename B1Pin,
	typename B2Pin,
	typename APin,
	typename BPin,
	typename CPin,
	typename DPin,
	typename OEPin,
	typename LatPin,
	typename ClkPin
>
struct Scan16Pins
{
	static constexpr unsigned R1PinIndex = R1Pin::Index;
	static constexpr unsigned R2PinIndex = R2Pin::Index;
	static constexpr unsigned G1PinIndex = G1Pin::Index;
	static constexpr unsigned G2PinIndex = G2Pin::Index;
	static constexpr unsigned B1PinIndex = B1Pin::Index;
	static constexpr unsigned B2PinIndex = B2Pin::Index;
	static constexpr unsigned APinIndex = APin::Index;
	static constexpr unsigned BPinIndex = BPin::Index;
	static constexpr unsigned CPinIndex = CPin::Index;
	static constexpr unsigned DPinIndex = DPin::Index;
	static constexpr unsigned OEPinIndex = OEPin::Index;
	static constexpr unsigned LatPinIndex = LatPin::Index;
	static constexpr unsigned ClkPinIndex = ClkPin::Index;
	
	static void init()
	{
		R1Pin::conf_out_push_pull(hl::PinSpeed::High);
		R2Pin::conf_out_push_pull(hl::PinSpeed::High);
		G1Pin::conf_out_push_pull(hl::PinSpeed::High);
		G2Pin::conf_out_push_pull(hl::PinSpeed::High);
		B1Pin::conf_out_push_pull(hl::PinSpeed::High);
		B2Pin::conf_out_push_pull(hl::PinSpeed::High);

		APin::conf_out_push_pull(hl::PinSpeed::High);
		BPin::conf_out_push_pull(hl::PinSpeed::High);
		CPin::conf_out_push_pull(hl::PinSpeed::High);
		DPin::conf_out_push_pull(hl::PinSpeed::High);

		OEPin::conf_out_push_pull(hl::PinSpeed::High);
		LatPin::conf_out_push_pull(hl::PinSpeed::High);
		ClkPin::conf_out_push_pull(hl::PinSpeed::High);
	}
	
	static uint_fast16_t get_row_mask(unsigned row)
	{
		uint_fast16_t mask = 0;
		if (row & 1) mask |= (1 << APinIndex);
		if (row & 2) mask |= (1 << BPinIndex);
		if (row & 4) mask |= (1 << CPinIndex);
		if (row & 8) mask |= (1 << DPinIndex);
		return mask;
	}
};

template <
	typename R1Pin,
	typename R2Pin,
	typename G1Pin,
	typename G2Pin,
	typename B1Pin,
	typename B2Pin,
	typename APin,
	typename BPin,
	typename CPin,
	typename DPin,
	typename EPin,
	typename OEPin,
	typename LatPin,
	typename ClkPin
>
struct Scan32Pins : public Scan16Pins<
	R1Pin, R2Pin, 
	G1Pin, G2Pin, 
	B1Pin, B2Pin, 
	APin, BPin, CPin, DPin, 
	OEPin, LatPin, ClkPin>
{
	using Base = Scan16Pins<
		R1Pin, R2Pin, 
		G1Pin, G2Pin, 
		B1Pin, B2Pin, 
		APin, BPin, CPin, DPin, 
		OEPin, LatPin, ClkPin
	>;
	
	using Base::APinIndex;
	using Base::BPinIndex;
	using Base::CPinIndex;
	using Base::DPinIndex;
	static constexpr unsigned EPinIndex = EPin::Index;
	
	static void init()
	{
		Base::init();
		
		EPin::conf_out_push_pull(hl::PinSpeed::High);
	}
	
	static uint_fast16_t get_row_mask(unsigned row)
	{
		uint_fast16_t mask = 0;
		if (row &  1) mask |= (1 << APinIndex);
		if (row &  2) mask |= (1 << BPinIndex);
		if (row &  4) mask |= (1 << CPinIndex);
		if (row &  8) mask |= (1 << DPinIndex);
		if (row & 16) mask |= (1 << EPinIndex);
		return mask;
	}
};


template <
	typename Port,
	typename Pins,
	typename DmaStream,
	int8_t   DmaChanIndex,
	int8_t   DmaTimerChan
>
class Hub75LineHardware
{
public:
	template <typename DmaStransTimer>
	static void init()
	{
		init_gpio();
		init_dma_stream();
		conf_trans_dma_timer_chan<DmaStransTimer>();
	}
	
	template <typename Screen>
	static void fill_data_for_row_into_buffer(
		const Screen        &screen, 
		UnitBamData<Screen> *bam_data, 
		unsigned            cur_row, 
		unsigned            row_offset)
	{
		volatile uint16_t* bam_data_fill_ptr = bam_data->bam_data_to_fill;
		for (unsigned stage = 0; stage < Screen::color_depth; stage++)
		{
			screen.fill_data_for_row_into_buffer(
				bam_data_fill_ptr, bam_data_fill_ptr+Screen::row_buffer_len,
				cur_row, 0, stage,
				Pins::R1PinIndex, Pins::R2PinIndex, 
				Pins::G1PinIndex, Pins::G2PinIndex, 
				Pins::B1PinIndex, Pins::B2PinIndex,
				Pins::OEPinIndex, Pins::LatPinIndex, Pins::ClkPinIndex,
				&Pins::get_row_mask
			);
			bam_data_fill_ptr += Screen::row_buffer_len;
		}
	}
	
	template <typename Screen>
	static void start_dma_transfer(UnitBamData<Screen> *bam_data)
	{
		DmaStream::disable();
		DmaStream::clear_transfer_complete_flag();
		DmaStream::clear_transfer_error_flag();
		DmaStream::set_memory_address((uintptr_t)bam_data->bam_data_send_ptr);
		DmaStream::set_number_of_data(Screen::row_buffer_len);
		DmaStream::enable();
		
		bam_data->bam_data_send_ptr += Screen::row_buffer_len;
	}
	
	template <typename Screen>
	static void make_row_off(unsigned row)
	{
		Port::set(Screen::get_row_off_value(row, Pins::OEPinIndex, &Pins::get_row_mask));
	}
	
	
private:
	static void init_gpio()
	{
		Port::clock_on();
		Port::reset();
		
		Pins::init();
	}
	
	static void init_dma_stream()
	{
		DmaStream::disable();
		DmaStream::set_direction(hl::DMADirection::MemToPeriph);
		DmaStream::set_peripheral_address(Port::ODR_ADDR);
		DmaStream::set_peripheral_size(hl::DMADataSize::_16bit);
		DmaStream::disable_peripheral_increment();
		DmaStream::set_memory_size(hl::DMADataSize::_16bit);
		DmaStream::enable_memory_increment();
		if constexpr (DmaChanIndex != -1)
			DmaStream::set_channel(DmaChanIndex);
	}
	
	template <typename DmaStransTimer>
	static void conf_trans_dma_timer_chan()
	{
		if constexpr (DmaTimerChan != -1)
		{
			DmaStransTimer::template conf_pwm<DmaTimerChan>(
				hl::PWMMode::_1, 
				hl::PWMPolarity::High, 
				hl::PWMPreload::Disable
			);
			DmaStransTimer::template set_pwm_value<DmaTimerChan>(1);
			DmaStransTimer::template enable_cc_dma_request<DmaTimerChan>();
		}
		else
		{
			DmaStransTimer::enable_update_dma_request();
		}
	}
};

template <typename ... Items>
class Hub75LineHardwareItems
{
public:
	static constexpr unsigned count = sizeof ... (Items);

	template <typename DmaStransTimer>
	static void init()
	{
		init_internal<DmaStransTimer, Items ...>();
	}
	
	template <typename Screen>
	static void fill_data_for_row_into_buffer(
		const Screen        &screen, 
		UnitBamData<Screen> *bam_data, 
		unsigned            cur_row)
	{
		fill_data_for_row_into_buffer_internal<Screen, Items ...>(screen, bam_data, cur_row, 0);
	}
	
	template <typename Screen>
	static void make_row_off(unsigned row)
	{
		make_row_off_internal<Screen, Items ...>(row);
	}
	
	template <typename Screen>
	static void start_dma_transfer(UnitBamData<Screen> *bam_data)
	{
		start_dma_transfer_internal<Screen, Items ...>(bam_data);
	}
	
	
private:
	template <typename DmaStransTimer, typename ItemToInit, typename ... OtherItems>
	static void init_internal()
	{
		ItemToInit::template init<DmaStransTimer>();
		if constexpr ((sizeof ... (OtherItems)) != 0)
			init_internal<DmaStransTimer, OtherItems...>();
	}
	
	template <typename Screen, typename Item, typename ... OtherItems>
	static void fill_data_for_row_into_buffer_internal(
		const Screen        &screen, 
		UnitBamData<Screen> *bam_data, 
		unsigned            cur_row, 
		unsigned            row_offset)
	{
		Item::fill_data_for_row_into_buffer(screen, bam_data, cur_row, row_offset);
		if constexpr (sizeof ... (OtherItems) != 0)
		{
			bam_data++;
			row_offset += Screen::unit_height;
			fill_data_for_row_into_buffer_internal<Screen, OtherItems ...>(
				screen, 
				bam_data, 
				cur_row, 
				row_offset
			);
		}
	}
	
	template <typename Screen, typename Item, typename ... OtherItems>
	static void make_row_off_internal(unsigned row)
	{
		Item::template make_row_off<Screen>(row);
		if constexpr (sizeof ... (OtherItems) != 0)
		{
			make_row_off_internal<Screen, OtherItems ...>(row);
		}
	}
	
	template <typename Screen, typename Item, typename ... OtherItems>
	static void start_dma_transfer_internal(UnitBamData<Screen> *bam_data)
	{
		Item::start_dma_transfer(bam_data);
		if constexpr (sizeof ... (OtherItems) != 0)
		{
			bam_data++;
			start_dma_transfer_internal<Screen, OtherItems ...>(bam_data);
		}
	}
};

template <
	unsigned TimersClock,
	unsigned RowScanPeriod,
	typename Screen,
	typename LineHardwareItems,
	typename TransDma,
	typename DmaStransTimer,
	typename RowScanTimer,
	typename BamTimer,
	unsigned DataTranslatePeriod
>
class Hub75Controller
{
	static_assert(Screen::vert_units == LineHardwareItems::count);
	
public:
	Hub75Controller(const Screen &screen) : 
		screen_(screen)
	{}

	static void init()
	{
		TransDma::clock_off();
		TransDma::clock_on();
		
		DmaStransTimer::clock_on();
		DmaStransTimer::reset();
		DmaStransTimer::set_auto_reload_value(2);
		DmaStransTimer::generate_update();
		DmaStransTimer::clear_all_flags();
		DmaStransTimer::set_counter(0);
		
		LineHardwareItems::template init<DmaStransTimer>();
		
		DmaStransTimer::start();
		
		RowScanTimer::clock_on();
		RowScanTimer::reset();
		RowScanTimer::set_prescaler(TimersClock/1000000-1);
		RowScanTimer::set_auto_reload_value(RowScanPeriod - 1);
		RowScanTimer::generate_update();
		RowScanTimer::clear_all_flags();
		RowScanTimer::enable_update_interrupt();
		NVIC_SetPriority(RowScanTimer::IRQn, 2);
		NVIC_EnableIRQ(RowScanTimer::IRQn);
		
		BamTimer::clock_on();
		BamTimer::reset();
		BamTimer::set_prescaler(TimersClock/1000000-1);
		BamTimer::set_auto_reload_value(RowScanPeriod);
		BamTimer::enable_one_pulse_mode();
		BamTimer::generate_update();
		BamTimer::clear_all_flags();
		BamTimer::template conf_pwm<1>(hl::PWMMode::_1, hl::PWMPolarity::High, hl::PWMPreload::Disable);
		BamTimer::template enable_cc_interrupt<1>();
		NVIC_SetPriority(BamTimer::IRQn, 1);
		NVIC_EnableIRQ(BamTimer::IRQn);
	}
	
	static void start()
	{
		RowScanTimer::start();
	}
	
	void process_row_scan_timer_interrupt()
	{
		if (RowScanTimer::get_update_interrupt_flag())
		{
			for (auto &b : unit_bam_)
			{
				b.bam_data_send_ptr = (cur_row_ & 1) ? b.data1 : b.data2;
				b.bam_data_to_fill = (cur_row_ & 1) ? b.data2 : b.data1;
			}
				
			LineHardwareItems::start_dma_transfer(unit_bam_);
				
			constexpr unsigned LedsOnBamPeriod = 
				RowScanPeriod 
				- DataTranslatePeriod*Screen::color_depth;
				
			constexpr unsigned InitialBamPeriod = 
				LedsOnBamPeriod + (LedsOnBamPeriod >> Screen::color_depth);

			bam_period_ = brightness_ * InitialBamPeriod / (2*16);
			bam_pwm_value_ = bam_period_ + DataTranslatePeriod;
			bam_stage_ = 0;
			
			BamTimer::stop();
			BamTimer::template disable_cc_interrupt<1>();
			BamTimer::set_counter(0);
			BamTimer::template set_pwm_value<1>(bam_pwm_value_);
			BamTimer::generate_update();
			BamTimer::clear_all_flags();
			BamTimer::template clear_cc_flag<1>();
			BamTimer::template enable_cc_interrupt<1>();
			BamTimer::start();
			
			LineHardwareItems::fill_data_for_row_into_buffer(screen_, unit_bam_, cur_row_);
			
			row_to_off_ = cur_row_;
			
			cur_row_++;
			if (cur_row_ >= Screen::unit_scan_height)
				cur_row_ = 0;
			
			RowScanTimer::clear_update_interrupt_flag();
		} 
	}
	
	void process_bam_timer_interrupt()
	{
		if (BamTimer::template get_cc_flag<1>())
		{
			bam_period_ = bam_period_ / 2;
			bam_pwm_value_ = bam_pwm_value_ + bam_period_ + DataTranslatePeriod;
				
			BamTimer::template set_pwm_value<1>(bam_pwm_value_);
				
			bam_stage_++;
					
			if ((bam_stage_ != Screen::color_depth) && (bam_period_ > 4))
				LineHardwareItems::start_dma_transfer(unit_bam_);
			else
			{
				LineHardwareItems::template make_row_off<Screen>(row_to_off_);
				BamTimer::stop();
				BamTimer::template disable_cc_interrupt<1>();
			}
			
			BamTimer::template clear_cc_flag<1>();
		}
	}
	
	void set_brightness(uint8_t brightness)
	{
		if (brightness < 1) brightness = 1;
		if (brightness > 16) brightness = 16;
		brightness_ = brightness;
	}
	
private:
	const Screen &screen_;
	UnitBamData<Screen> unit_bam_[Screen::vert_units];
	volatile uint16_t cur_row_ = 0;
	volatile unsigned bam_stage_ = 0;
	volatile unsigned bam_period_ = 0;
	volatile unsigned bam_pwm_value_ = 0;
	volatile uint8_t brightness_ = 16;
	volatile uint16_t row_to_off_ = 0;
};

} // namespace ledmatrix
