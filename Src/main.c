#include <stdint.h>
#include <stdio.h>

int main(void)
{
    //C(column) - INPUT pinleri : PA5, PA6, PA7, PA8
	//R(row) - OUTPUT pinleri: PB3, PB4, PB5, PB6



	//adresler

	//GPIO pinleri için clock adresi
	uint32_t volatile *const clock_ctrl_reg = (uint32_t*) 0x40023830;


	//GPIOA pinleri mod adresi
	uint32_t volatile *const gpioa_mode_reg = (uint32_t*) 0x40020000;

	//GPIOB pinleri mod adresi
	uint32_t volatile *const gpiob_mode_reg = (uint32_t*) 0x40020400;


	//GPIOA pinleri için INPUT adresi
	uint32_t volatile *const gpioa_input_reg = (uint32_t*) 0x40020010;

	//GPIOB pinleri için OUTPUT adresi
	uint32_t volatile *const gpiob_output_reg = (uint32_t*) 0x40020414;

	//GPIOA pinleri için pull-up/pull-down adresi
	uint32_t volatile *const gpioa_pull_reg = (uint32_t*) 0x4002000C;


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
					for(uint32_t delay = 0; delay < 300000; delay++); //150ms delay
					printf("key is %c\n", keypad[i][j]);
				}
			}
		}
	}
}
