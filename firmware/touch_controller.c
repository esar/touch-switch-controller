#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 4000000UL
#include <util/delay.h>

#define GET_BIT(value, bit)    ((value) & (1 << (bit)))
#define SET_BIT(value, bit)    ((value) |= 1 << (bit))
#define CLR_BIT(value, bit)    ((value) &= ~(1 << (bit)))
#define BIT(bit)               (1 << (bit))
#define PULSE_BIT(value, bit)  SET_BIT(value, bit); _delay_ms(100); CLR_BIT(value, bit)

#define DPRT_SCENE1   DDRA
#define DPIN_SCENE1   DDA5
#define PRT_SCENE1    PORTA
#define PIN_SCENE1    PORTA5

#define DPRT_SCENE2   DDRA
#define DPIN_SCENE2   DDA4
#define PRT_SCENE2    PORTA
#define PIN_SCENE2    PORTA4

#define DPRT_SCENE3   DDRA
#define DPIN_SCENE3   DDA3
#define PRT_SCENE3    PORTA
#define PIN_SCENE3    PORTA3

#define DPRT_SCENE4   DDRA
#define DPIN_SCENE4   DDA2
#define PRT_SCENE4    PORTA
#define PIN_SCENE4    PORTA2

#define DPRT_OFF      DDRA
#define DPIN_OFF      DDA1
#define PRT_OFF       PORTA
#define PIN_OFF       PORTA1

#define DPRT_RAISE    DDRA
#define DPIN_RAISE    DDA0
#define PRT_RAISE     PORTA
#define PIN_RAISE     PORTA0

#define DPRT_LOWER    DDRB
#define DPIN_LOWER    DDB3
#define PRT_LOWER     PORTB
#define PIN_LOWER     PORTB3


int pressed = 0;

ISR(ANA_COMP_vect)
{
}

int get()
{
	int count = 0;
	int last = 0;

	for(;;)
	{
		int now = ACSRA & (1 << ACO);
		if(now != last)
		{
			count = 0;
			last = now;
		}
		else
		{
			if(++count >= 50)
				return now;
		}
	}
}

int main()
{
	CLR_BIT(PRT_SCENE1, PIN_SCENE1);
	CLR_BIT(PRT_SCENE2, PIN_SCENE2);
	CLR_BIT(PRT_SCENE3, PIN_SCENE3);
	CLR_BIT(PRT_SCENE4, PIN_SCENE4);
	CLR_BIT(PRT_OFF,    PIN_OFF);
	CLR_BIT(PRT_RAISE,  PIN_RAISE);
	CLR_BIT(PRT_LOWER,  PIN_LOWER);

	SET_BIT(DPRT_SCENE1, DPIN_SCENE1);
	SET_BIT(DPRT_SCENE2, DPIN_SCENE2);
	SET_BIT(DPRT_SCENE3, DPIN_SCENE3);
	SET_BIT(DPRT_SCENE4, DPIN_SCENE4);
	SET_BIT(DPRT_OFF,    DPIN_OFF);
	SET_BIT(DPRT_RAISE,  DPIN_RAISE);
	SET_BIT(DPRT_LOWER,  DPIN_LOWER);

	SET_BIT(DDRB, DDB6);
	SET_BIT(PORTB, PORTB6);

	cli();
	ACSRA = (1 << ACIE);
	sei();

	for(;;)
	{
		static int last = 0;
		static int now = 0;
		static int on = 0;

		now = get();
		if(now && !last)
		{
			if(on == 0)
			{
				PULSE_BIT(PRT_SCENE1, PIN_SCENE1);
				on = 1;
			}
			else
			{
				PULSE_BIT(PRT_OFF, PIN_OFF);
				on = 0;
				
			}
		}
		last = now;
	}
}

