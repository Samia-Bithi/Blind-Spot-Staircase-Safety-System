#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

// ---------------- Font 5x7 ----------------
const uint8_t font5x7[][5] = {
	{0x00,0x00,0x00,0x00,0x00}, // space (0x20)
	{0x00,0x00,0x5F,0x00,0x00}, // !
	{0x00,0x07,0x00,0x07,0x00}, // "
	{0x14,0x7F,0x14,0x7F,0x14}, // #
	{0x24,0x2A,0x7F,0x2A,0x12}, // $
	{0x23,0x13,0x08,0x64,0x62}, // %
	{0x36,0x49,0x55,0x22,0x50}, // &
	{0x00,0x05,0x03,0x00,0x00}, // '
	{0x00,0x1C,0x22,0x41,0x00}, // (
	{0x00,0x41,0x22,0x1C,0x00}, // )
	{0x14,0x08,0x3E,0x08,0x14}, // *
	{0x08,0x08,0x3E,0x08,0x08}, // +
	{0x00,0x50,0x30,0x00,0x00}, // ,
	{0x08,0x08,0x08,0x08,0x08}, // -
	{0x00,0x60,0x60,0x00,0x00}, // .
	{0x20,0x10,0x08,0x04,0x02}, // /
	{0x3E,0x51,0x49,0x45,0x3E}, // 0
	{0x00,0x42,0x7F,0x40,0x00}, // 1
	{0x42,0x61,0x51,0x49,0x46}, // 2
	{0x21,0x41,0x45,0x4B,0x31}, // 3
	{0x18,0x14,0x12,0x7F,0x10}, // 4
	{0x27,0x45,0x45,0x45,0x39}, // 5
	{0x3C,0x4A,0x49,0x49,0x30}, // 6
	{0x01,0x71,0x09,0x05,0x03}, // 7
	{0x36,0x49,0x49,0x49,0x36}, // 8
	{0x06,0x49,0x49,0x29,0x1E}, // 9
	{0x00,0x36,0x36,0x00,0x00}, // :
	{0x00,0x56,0x36,0x00,0x00}, // ;
	{0x08,0x14,0x22,0x41,0x00}, //
	{0x14,0x14,0x14,0x14,0x14}, // =
	{0x00,0x41,0x22,0x14,0x08}, // >
	{0x02,0x01,0x51,0x09,0x06}, // ?
	{0x32,0x49,0x79,0x41,0x3E}, // @
	{0x7E,0x11,0x11,0x11,0x7E}, // A
	{0x7F,0x49,0x49,0x49,0x36}, // B
	{0x3E,0x41,0x41,0x41,0x22}, // C
	{0x7F,0x41,0x41,0x22,0x1C}, // D
	{0x7F,0x49,0x49,0x49,0x41}, // E
	{0x7F,0x09,0x09,0x09,0x01}, // F
	{0x3E,0x41,0x49,0x49,0x7A}, // G
	{0x7F,0x08,0x08,0x08,0x7F}, // H
	{0x00,0x41,0x7F,0x41,0x00}, // I
	{0x20,0x40,0x41,0x3F,0x01}, // J
	{0x7F,0x08,0x14,0x22,0x41}, // K
	{0x7F,0x40,0x40,0x40,0x40}, // L
	{0x7F,0x02,0x0C,0x02,0x7F}, // M
	{0x7F,0x04,0x08,0x10,0x7F}, // N
	{0x3E,0x41,0x41,0x41,0x3E}, // O
	{0x7F,0x09,0x09,0x09,0x06}, // P
	{0x3E,0x41,0x51,0x21,0x5E}, // Q
	{0x7F,0x09,0x19,0x29,0x46}, // R
	{0x46,0x49,0x49,0x49,0x31}, // S
	{0x01,0x01,0x7F,0x01,0x01}, // T
	{0x3F,0x40,0x40,0x40,0x3F}, // U
	{0x1F,0x20,0x40,0x20,0x1F}, // V
	{0x3F,0x40,0x38,0x40,0x3F}, // W
	{0x63,0x14,0x08,0x14,0x63}, // X
	{0x07,0x08,0x70,0x08,0x07}, // Y
	{0x61,0x51,0x49,0x45,0x43}  // Z
};

// ---------------- I2C (TWI) ----------------
#define SSD1306_ADDR 0x3C

void i2c_init(void)
{
	TWSR = 0x00;
	TWBR = 0x02; // ~100kHz @ 1MHz (minimum, will be slower; fine for OLED)
	TWCR = (1 << TWEN);
}

void i2c_start(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void i2c_stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while (TWCR & (1 << TWSTO));
}

void i2c_write(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

// ---------------- SSD1306 ----------------
void ssd1306_cmd(uint8_t cmd)
{
	i2c_start();
	i2c_write(SSD1306_ADDR << 1);
	i2c_write(0x00);
	i2c_write(cmd);
	i2c_stop();
}

void ssd1306_data(uint8_t data)
{
	i2c_start();
	i2c_write(SSD1306_ADDR << 1);
	i2c_write(0x40);
	i2c_write(data);
	i2c_stop();
}

void ssd1306_init(void)
{
	_delay_ms(100);
	ssd1306_cmd(0xAE);
	ssd1306_cmd(0xD5); ssd1306_cmd(0x80);
	ssd1306_cmd(0xA8); ssd1306_cmd(0x3F);
	ssd1306_cmd(0xD3); ssd1306_cmd(0x00);
	ssd1306_cmd(0x40);
	ssd1306_cmd(0x8D); ssd1306_cmd(0x14);
	ssd1306_cmd(0x20); ssd1306_cmd(0x00);
	ssd1306_cmd(0xA1);
	ssd1306_cmd(0xC8);
	ssd1306_cmd(0xDA); ssd1306_cmd(0x12);
	ssd1306_cmd(0x81); ssd1306_cmd(0x80);
	ssd1306_cmd(0xD9); ssd1306_cmd(0xF1);
	ssd1306_cmd(0xDB); ssd1306_cmd(0x40);
	ssd1306_cmd(0xA4);
	ssd1306_cmd(0xA6);
	ssd1306_cmd(0xAF);
}

void ssd1306_set_cursor(uint8_t col, uint8_t page)
{
	ssd1306_cmd(0x21); ssd1306_cmd(col); ssd1306_cmd(127);
	ssd1306_cmd(0x22); ssd1306_cmd(page); ssd1306_cmd(7);
}

void ssd1306_clear(void)
{
	ssd1306_set_cursor(0, 0);
	for (uint16_t i = 0; i < 1024; i++)
	ssd1306_data(0x00);
}

void ssd1306_putchar(char c)
{
	if (c < 0x20 || c > 0x5A) c = ' ';
	uint8_t idx = c - 0x20;
	for (uint8_t i = 0; i < 5; i++)
	ssd1306_data(font5x7[idx][i]);
	ssd1306_data(0x00);
}

void ssd1306_print(const char *str)
{
	while (*str)
	ssd1306_putchar(*str++);
}

void ssd1306_clear_line(uint8_t page)
{
	ssd1306_set_cursor(0, page);
	for (uint8_t i = 0; i < 21; i++)
	ssd1306_print(" ");
}

// ---------------- Servo (PWM on PD5 / OC1A) ----------------
#define SERVO_POS0   1000   // 0 degrees
#define SERVO_POS180 2000   // 180 degrees

void PWM_init(void)
{
	DDRD |= (1 << PD5); // OC1A as output
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10); // Prescaler = 1
	ICR1 = 20000; // 20ms period @ 1MHz, prescaler 1
	OCR1A = SERVO_POS0; // start at 0 degrees
}

// ---------------- Buttons ----------------
// BTN1, BTN2, BTN3 -> PB0, PB1, PB2 (number entry)
// BTN4 -> PB3 (password reset trigger)
#define BTN1 PB0
#define BTN2 PB1
#define BTN3 PB2
#define BTN4 PB3

uint8_t debounce_btn(uint8_t pin)
{
	if (!(PINB & (1 << pin)))
	{
		_delay_ms(20);
		if (!(PINB & (1 << pin)))
		{
			while (!(PINB & (1 << pin))); // wait release
			_delay_ms(20);
			return 1;
		}
	}
	return 0;
}

// ---------------- Password storage ----------------
char password[4] = "123"; // default password, stored in RAM

// ---------------- Helper: read one digit (1/2/3) blocking ----------------
char read_digit(void)
{
	while (1)
	{
		if (debounce_btn(BTN1)) return '1';
		if (debounce_btn(BTN2)) return '2';
		if (debounce_btn(BTN3)) return '3';
		if (debounce_btn(BTN4)) return 'R'; // allow cancel during entry
	}
}

// ---------------- Main ----------------
int main(void)
{
	// Buttons PB0-PB3, input with pull-ups
	DDRB &= ~((1 << BTN1) | (1 << BTN2) | (1 << BTN3) | (1 << BTN4));
	PORTB |= (1 << BTN1) | (1 << BTN2) | (1 << BTN3) | (1 << BTN4);

	i2c_init();
	ssd1306_init();
	ssd1306_clear();

	PWM_init(); // servo on PD5 (OC1A), starts at 0 degrees

	ssd1306_set_cursor(0, 0);
	ssd1306_print("Enter Password:");

	char seq[4] = "";
	uint8_t idx = 0;

	while (1)
	{
		uint8_t pressed = 0;
		char digit = 0;

		// ---- Check BTN4 (reset trigger) first ----
		if (debounce_btn(BTN4))
		{
			idx = 0;
			seq[0] = '\0';

			ssd1306_clear_line(0);
			ssd1306_clear_line(2);
			ssd1306_clear_line(4);
			ssd1306_set_cursor(0, 0);
			ssd1306_print("Enter OLD Pass:");

			char oldSeq[4] = "";
			for (uint8_t i = 0; i < 3; i++)
			{
				char d = read_digit();
				if (d == 'R')
				{
					ssd1306_clear_line(0);
					ssd1306_clear_line(2);
					ssd1306_clear_line(4);
					ssd1306_set_cursor(0, 0);
					ssd1306_print("Enter Password:");
					goto continue_main;
				}
				oldSeq[i] = d;
				oldSeq[i+1] = '\0';

				ssd1306_set_cursor(0, 2);
				ssd1306_print("Old: ");
				ssd1306_print(oldSeq);
				ssd1306_print("    ");
			}

			if (strcmp(oldSeq, password) == 0)
			{
				ssd1306_clear_line(2);
				ssd1306_clear_line(4);
				ssd1306_set_cursor(0, 0);
				ssd1306_print("Enter NEW Pass:");

				char newSeq[4] = "";
				for (uint8_t i = 0; i < 3; i++)
				{
					char d = read_digit();
					if (d == 'R')
					{
						ssd1306_clear_line(0);
						ssd1306_clear_line(2);
						ssd1306_clear_line(4);
						ssd1306_set_cursor(0, 0);
						ssd1306_print("Enter Password:");
						goto continue_main;
					}
					newSeq[i] = d;
					newSeq[i+1] = '\0';

					ssd1306_set_cursor(0, 2);
					ssd1306_print("New: ");
					ssd1306_print(newSeq);
					ssd1306_print("    ");
				}

				strcpy(password, newSeq);

				ssd1306_set_cursor(0, 4);
				ssd1306_print("Password Changed");
				_delay_ms(1500);
			}
			else
			{
				ssd1306_set_cursor(0, 4);
				ssd1306_print("Wrong Password  ");
				_delay_ms(1500);
			}

			ssd1306_clear_line(0);
			ssd1306_clear_line(2);
			ssd1306_clear_line(4);
			ssd1306_set_cursor(0, 0);
			ssd1306_print("Enter Password:");

			idx = 0;
			seq[0] = '\0';
		}

		// ---- Normal password entry ----
		if (debounce_btn(BTN1)) { digit = '1'; pressed = 1; }
		else if (debounce_btn(BTN2)) { digit = '2'; pressed = 1; }
		else if (debounce_btn(BTN3)) { digit = '3'; pressed = 1; }

		if (pressed)
		{
			seq[idx] = digit;
			idx++;
			seq[idx] = '\0';

			ssd1306_set_cursor(0, 2);
			ssd1306_print("Sequence: ");
			ssd1306_print(seq);
			ssd1306_print("    ");

			if (idx == 3)
			{
				if (strcmp(seq, password) == 0)
				{
					ssd1306_set_cursor(0, 4);
					ssd1306_print("Access Granted  ");

					// Servo: 0 -> 180
					OCR1A = SERVO_POS180;
					_delay_ms(1500);

					// Hold at 180 for 4 sec
					_delay_ms(4000);

					// Servo: 180 -> 0
					OCR1A = SERVO_POS0;
					_delay_ms(1500);

					ssd1306_set_cursor(0, 4);
					ssd1306_print("                ");
					ssd1306_set_cursor(0, 0);
					ssd1306_print("Enter Password: ");
				}
				else
				{
					ssd1306_set_cursor(0, 4);
					ssd1306_print("Access Denied   ");
					_delay_ms(1000);
					ssd1306_set_cursor(0, 4);
					ssd1306_print("                ");
				}

				idx = 0;
				seq[0] = '\0';

				ssd1306_set_cursor(0, 2);
				ssd1306_print("Sequence:        ");
			}
		}

		continue_main:;
	}
}