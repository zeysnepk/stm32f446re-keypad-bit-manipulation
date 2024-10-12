#include <stdint.h>
#include <stdio.h>


#define DELAY_COUNT       ( 300000UL )

#define CLCK_CTRL_REG     ( (uint32_t*) 0x40023830 )
#define GPIOA_MODE_REG    ( (uint32_t*) 0x40020000 )
#define GPIOB_MODE_REG    ( (uint32_t*) 0x40020400 )
#define GPIOA_INPUT_REG   ( (uint32_t*) 0x40020010 )
#define GPIOB_OUTPUT_REG  ( (uint32_t*) 0x40020414 )
#define GPIOA_PULL_REG    ( (uint32_t*) 0x4002000C )



int main(void)
{
    //C(column) - INPUT pinleri : PA5, PA6, PA7, PA8
	//R(row) - OUTPUT pinleri: PB3, PB4, PB5, PB6



	//adresler

	//GPIO pinleri için clock adresi
	uint32_t volatile *const clock_ctrl_reg = CLCK_CTRL_REG;


	//GPIOA pinleri mod adresi
	uint32_t volatile *const gpioa_mode_reg = GPIOA_MODE_REG;

	//GPIOB pinleri mod adresi
	uint32_t volatile *const gpiob_mode_reg = GPIOB_MODE_REG;


	//GPIOA pinleri için INPUT adresi
	uint32_t volatile *const gpioa_input_reg = GPIOA_INPUT_REG;

	//GPIOB pinleri için OUTPUT adresi
	uint32_t volatile *const gpiob_output_reg = GPIOB_OUTPUT_REG;

	//GPIOA pinleri için pull-up/pull-down adresi
	uint32_t volatile *const gpioa_pull_reg = GPIOA_PULL_REG;


	//konfigürasyon

	//GPIOA pinleri için clock ayarı
	*clock_ctrl_reg |= 1;

	//GPIB pinleri için clock ayarı
	*clock_ctrl_reg |= 2;


	//GPIOA pinleri için mode ayarı
	/* PA5 --> 00(Input) */ *gpioa_mode_reg &= ~(3 << 10);
	/* PA6 --> 00(Input) */ *gpioa_mode_reg &= ~(3 << 12);
	/* PA7 --> 00(Input) */ *gpioa_mode_reg &= ~(3 << 14);
	/* PA8 --> 00(Input) */ *gpioa_mode_reg &= ~(3 << 16);

	//GPIOB pinleri için mode ayarı
	/* PB3 --> 01(Output) */ *gpiob_mode_reg &= ~(3 << 6);  *gpiob_mode_reg |= (1 << 6);
	/* PB4 --> 01(Output) */ *gpiob_mode_reg &= ~(3 << 8);  *gpiob_mode_reg |= (1 << 8);
	/* PB5 --> 01(Output) */ *gpiob_mode_reg &= ~(3 << 10); *gpiob_mode_reg |= (1 << 10);
	/* PB6 --> 01(Output) */ *gpiob_mode_reg &= ~(3 << 12); *gpiob_mode_reg |= (1 << 12);


	//GPIOA pinleri için pull-up ayarı
	/* PA5 --> 00(Input) */ *gpioa_pull_reg &= ~(3 << 10);  *gpioa_pull_reg |= (1 << 10);
	/* PA6 --> 00(Input) */ *gpioa_pull_reg &= ~(3 << 12);  *gpioa_pull_reg |= (1 << 12);
	/* PA7 --> 00(Input) */ *gpioa_pull_reg &= ~(3 << 14);  *gpioa_pull_reg |= (1 << 14);
	/* PA8 --> 00(Input) */ *gpioa_pull_reg &= ~(3 << 16);  *gpioa_pull_reg |= (1 << 16);


	//keypad matrix
	uint8_t keypad[4][4] = {
			{ '1', '2', '3', 'A' },
			{ '4', '5', '6', 'B' },
			{ '7', '8', '9', 'C' },
			{ '*', '0', '#', 'D' }
	};


	while(1){
		//GPIOB pinleri HIGH ayarlama
		/* PB3 --> 1(HIGH) */ *gpiob_output_reg |= (1 << 3);
		/* PB4 --> 1(HIGH) */ *gpiob_output_reg |= (1 << 4);
		/* PB5 --> 1(HIGH) */ *gpiob_output_reg |= (1 << 5);
		/* PB6 --> 1(HIGH) */ *gpiob_output_reg |= (1 << 6);

		for(int i = 0; i < 4; i++){
			*gpiob_output_reg &= ~(1 << (3 + i));
			for(int j = 0; j < 4; j++){
				if(!(*gpioa_input_reg & (1 << (5 + j)))){
					//tuşa basıldı
					for(uint32_t delay = 0; delay < DELAY_COUNT; delay++); //150ms delay
					printf("key is %c\n", keypad[i][j]);
				}
			}
		}
	}
}
