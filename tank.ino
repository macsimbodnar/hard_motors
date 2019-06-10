#define PIN_EN_1 3 
#define PIN_IA_1 7
#define PIN_IB_1 4

#define PIN_EN_2 5
#define PIN_IA_2 2
#define PIN_IB_2 13

#define PIN_EN_3 6
#define PIN_IA_3 8
#define PIN_IB_3 12

#define PIN_EN_4 9
#define PIN_IA_4 A4
#define PIN_IB_4 A5

#define PIN_EN_5 11
#define PIN_IA_5 A2
#define PIN_IB_5 A3

#define PIN_EN_6 10
#define PIN_IA_6 A0
#define PIN_IB_6 A1

#define PIN_AN_1 A6
#define PIN_AN_2 A7

#define PIN_RX   0   
#define PIN_TX   1


typedef enum { FORWARD, BACKWARD, STOP } direction_t;

typedef struct
{
    int pin_enable;
    int pin_in_a;
    int pin_in_b;
} motor_data_t;

static const motor_data_t motors_data[] = 
{
    {PIN_EN_1, PIN_IA_1, PIN_IB_1},
    {PIN_EN_2, PIN_IA_2, PIN_IB_2},
    {PIN_EN_3, PIN_IA_3, PIN_IB_3},
    {PIN_EN_4, PIN_IA_4, PIN_IB_4},
    {PIN_EN_5, PIN_IA_5, PIN_IB_5},
    {PIN_EN_6, PIN_IA_6, PIN_IB_6}
};


void setup() {
    
    for (unsigned int i = 0; i < sizeof(motors_data) / sizeof(motors_data[0]); i++) {
        set_up_motor(motors_data[i]);
        
        set_motor(motors_data[i], FORWARD, 125);    
    }

    set_up_pwm();
}

void loop() {
  // put your main code here, to run repeatedly:

}


void set_up_motor(motor_data_t motor)
{
    pinMode(motor.pin_enable, OUTPUT);
    pinMode(motor.pin_in_a, OUTPUT);
    pinMode(motor.pin_in_b, OUTPUT);
}

void set_motor(motor_data_t motor, direction_t dir, unsigned char spd)
{
    switch (dir) {
        case FORWARD:
            digitalWrite(motor.pin_in_a, LOW);
            digitalWrite(motor.pin_in_b, HIGH);
            break;
            
        case BACKWARD:
            digitalWrite(motor.pin_in_a, HIGH);
            digitalWrite(motor.pin_in_b, LOW);
            break;
            
        case STOP:
            digitalWrite(motor.pin_in_a, LOW);
            digitalWrite(motor.pin_in_b, LOW);
            break;
    }
    analogWrite(motor.pin_enable, spd);
}


void set_up_pwm()
{
    //---------------------------------------------- Set PWM frequency for D5 & D6 -------------------------------

    //TCCR0B = TCCR0B & B11111000 | B00000001;    // set timer 0 divisor to     1 for PWM frequency of 62500.00 Hz
      TCCR0B = (TCCR0B & B11111000) | B00000010;    // set timer 0 divisor to     8 for PWM frequency of  7812.50 Hz
    //TCCR0B = TCCR0B & B11111000 | B00000011;    // set timer 0 divisor to    64 for PWM frequency of   976.56 Hz (The DEFAULT)
    //TCCR0B = TCCR0B & B11111000 | B00000100;    // set timer 0 divisor to   256 for PWM frequency of   244.14 Hz
    //TCCR0B = TCCR0B & B11111000 | B00000101;    // set timer 0 divisor to  1024 for PWM frequency of    61.04 Hz
    
    
    //---------------------------------------------- Set PWM frequency for D9 & D10 ------------------------------
    
    //TCCR1B = TCCR1B & B11111000 | B00000001;    // set timer 1 divisor to     1 for PWM frequency of 31372.55 Hz
      TCCR1B = (TCCR1B & B11111000) | B00000010;    // set timer 1 divisor to     8 for PWM frequency of  3921.16 Hz
    //TCCR1B = TCCR1B & B11111000 | B00000011;    // set timer 1 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
    //TCCR1B = TCCR1B & B11111000 | B00000100;    // set timer 1 divisor to   256 for PWM frequency of   122.55 Hz
    //TCCR1B = TCCR1B & B11111000 | B00000101;    // set timer 1 divisor to  1024 for PWM frequency of    30.64 Hz
    
    //---------------------------------------------- Set PWM frequency for D3 & D11 ------------------------------
    
    //TCCR2B = TCCR2B & B11111000 | B00000001;    // set timer 2 divisor to     1 for PWM frequency of 31372.55 Hz
      TCCR2B = (TCCR2B & B11111000) | B00000010;    // set timer 2 divisor to     8 for PWM frequency of  3921.16 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000011;    // set timer 2 divisor to    32 for PWM frequency of   980.39 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000100;    // set timer 2 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
    //TCCR2B = TCCR2B & B11111000 | B00000101;    // set timer 2 divisor to   128 for PWM frequency of   245.10 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000110;    // set timer 2 divisor to   256 for PWM frequency of   122.55 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000111;    // set timer 2 divisor to  1024 for PWM frequency of    30.64 Hz
}
