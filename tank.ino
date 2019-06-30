#include <SoftwareSerial.h>


/***********************************************************************************
 *                           DEFINES & CONSTANTS
 **********************************************************************************/
#define SETUP_BLUETOOTH_MODULE  0                // 0 disabled | 1 enabled
#define BLUETOOTH_NAME          "nastytooth"
#define BLUETOOTH_PWD           "0000"
#define BLUETOOTH_BAUD          "4"             // id        baud rate             
                                                // 1---------1200
                                                // 2---------2400
                                                // 3---------4800
                                                // 4---------9600
                                                // 5---------19200
                                                // 6---------38400
                                                // 7---------57600
                                                // 8---------115200
                                                // 9---------230400
                                                // A---------460800
                                                // B---------921600
                                                // C---------1382400

// Pins 490Hz -> 3,9,10,11
// Pins 980Hz -> 5,6

#define PIN_EN_1 3 
#define PIN_IA_1 4
#define PIN_IB_1 7

#define PIN_EN_2 5
#define PIN_IA_2 2
#define PIN_IB_2 13

#define PIN_EN_3 6
#define PIN_IA_3 8
#define PIN_IB_3 12

#define PIN_EN_4 9
#define PIN_IA_4 A5
#define PIN_IB_4 A4

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


#define SPEED 150
#define BUFFER_SIZE 11

#define LEFT_FRONT 'a'
#define RIGHT_FRONT 'b'
#define LEFT_BACK 'c'
#define RIGHT_BACK 'd'
#define RIGHT_STOP 'e'
#define LEFT_STOP 'f'
#define GUN_FIRE 'g'
#define GUN_FIRE_STOP 'h'
#define TURRET_LEFT 'i'
#define TURRET_LEFT_STOP 'l'
#define TURRET_RIGHT 'm'
#define TURRET_RIGHT_STOP 'n'
#define GUN_UP 'o'
#define GUN_UP_STOP 'p'
#define GUN_DOWN 'q'
#define GUN_DOWN_STOP 'r'


/***********************************************************************************
 *                                  PROTOCOL
 * 
 **********************************************************************************/



/***********************************************************************************
 *                              DATA STRUCTURES
 **********************************************************************************/
typedef enum { FORWARD, BACKWARD, STOP } direction_t;

typedef struct
{
    int pin_enable;
    int pin_in_a;
    int pin_in_b;
} motor_data_t;

static const motor_data_t motors_data[] = 
{
    {PIN_EN_1, PIN_IA_1, PIN_IB_1}, // M1 T_TURRET
    {PIN_EN_2, PIN_IA_2, PIN_IB_2}, // M2 T_RIGHT_TRUCK
    {PIN_EN_3, PIN_IA_3, PIN_IB_3}, // M3 T_LEFT_TRUCK
    {PIN_EN_4, PIN_IA_4, PIN_IB_4}, // M4 T_CANNON_ELEV
    {PIN_EN_5, PIN_IA_5, PIN_IB_5}, // M5 T_CANNON_FIRE
    {PIN_EN_6, PIN_IA_6, PIN_IB_6}  // M6
};


typedef enum 
{
    T_TURRET = 0,
    T_RIGHT_TRUCK,
    T_LEFT_TRUCK,
    T_CANNON_ELEV,
    T_CANNON_FIRE
} tank_motors_t;


/***********************************************************************************
 *                                   GLOBALS
 **********************************************************************************/
static SoftwareSerial serial(PIN_RX, PIN_TX); // RX | TX



/***********************************************************************************
 *                                  FUNCTIONS
 **********************************************************************************/
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
    //TCCR0B = TCCR0B & B11111000 | B00000010;    // set timer 0 divisor to     8 for PWM frequency of  7812.50 Hz
    //TCCR0B = TCCR0B & B11111000 | B00000011;    // set timer 0 divisor to    64 for PWM frequency of   976.56 Hz (The DEFAULT)
    //TCCR0B = TCCR0B & B11111000 | B00000100;    // set timer 0 divisor to   256 for PWM frequency of   244.14 Hz
    //TCCR0B = TCCR0B & B11111000 | B00000101;    // set timer 0 divisor to  1024 for PWM frequency of    61.04 Hz
    
    
    //---------------------------------------------- Set PWM frequency for D9 & D10 ------------------------------
    
    //TCCR1B = TCCR1B & B11111000 | B00000001;    // set timer 1 divisor to     1 for PWM frequency of 31372.55 Hz
    //TCCR1B = TCCR1B & B11111000 | B00000010;    // set timer 1 divisor to     8 for PWM frequency of  3921.16 Hz
    //TCCR1B = TCCR1B & B11111000 | B00000011;    // set timer 1 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
    //TCCR1B = TCCR1B & B11111000 | B00000100;    // set timer 1 divisor to   256 for PWM frequency of   122.55 Hz
    //TCCR1B = TCCR1B & B11111000 | B00000101;    // set timer 1 divisor to  1024 for PWM frequency of    30.64 Hz
    
    //---------------------------------------------- Set PWM frequency for D3 & D11 ------------------------------
    
    //TCCR2B = TCCR2B & B11111000 | B00000001;    // set timer 2 divisor to     1 for PWM frequency of 31372.55 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000010;    // set timer 2 divisor to     8 for PWM frequency of  3921.16 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000011;    // set timer 2 divisor to    32 for PWM frequency of   980.39 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000100;    // set timer 2 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
    //TCCR2B = TCCR2B & B11111000 | B00000101;    // set timer 2 divisor to   128 for PWM frequency of   245.10 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000110;    // set timer 2 divisor to   256 for PWM frequency of   122.55 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000111;    // set timer 2 divisor to  1024 for PWM frequency of    30.64 Hz
}


void set_up_bluetooth_module()
{
    serial.write("AT+NAME" BLUETOOTH_NAME);
    serial.write("AT+PIN" BLUETOOTH_PWD);
    serial.write("AT+BAUD" BLUETOOTH_BAUD);
}


/***********************************************************************************
 *                                  LOGIC
 **********************************************************************************/
void setup() {
    set_up_pwm();

    for (unsigned int i = 0; i < sizeof(motors_data) / sizeof(motors_data[0]); i++) {
        set_up_motor(motors_data[i]);
        set_motor(motors_data[i], STOP, 0);    
    }

    serial.begin(9600);

#if SETUP_BLUETOOTH_MODULE
    set_up_bluetooth_module();
#endif
}

void loop() {
    while(serial.available()) {
        char c = char(serial.read());

        switch(c) {
            case LEFT_FRONT:        set_motor(motors_data[T_LEFT_TRUCK], FORWARD, 255);     break;
            case RIGHT_FRONT:       set_motor(motors_data[T_RIGHT_TRUCK], FORWARD, 255);    break;
            case LEFT_BACK:         set_motor(motors_data[T_LEFT_TRUCK], BACKWARD, 255);    break;
            case RIGHT_BACK:        set_motor(motors_data[T_RIGHT_TRUCK], BACKWARD, 255);   break;
            case RIGHT_STOP:        set_motor(motors_data[T_RIGHT_TRUCK], STOP, 0);         break;
            case LEFT_STOP:         set_motor(motors_data[T_LEFT_TRUCK], STOP, 0);          break;
            case GUN_FIRE:          set_motor(motors_data[T_CANNON_FIRE], FORWARD, 255);    break;
            case GUN_FIRE_STOP:     set_motor(motors_data[T_CANNON_FIRE], STOP, 0);         break;
            case TURRET_LEFT:       set_motor(motors_data[T_TURRET], FORWARD, 255);         break;
            case TURRET_RIGHT:      set_motor(motors_data[T_TURRET], BACKWARD, 255);        break;
            case TURRET_LEFT_STOP:  set_motor(motors_data[T_TURRET], STOP, 0);              break;
            case TURRET_RIGHT_STOP: set_motor(motors_data[T_TURRET], STOP, 0);              break;
            case GUN_UP:            set_motor(motors_data[T_CANNON_ELEV], FORWARD, 255);    break;
            case GUN_DOWN:          set_motor(motors_data[T_CANNON_ELEV], BACKWARD, 255);   break;
            case GUN_UP_STOP:       set_motor(motors_data[T_CANNON_ELEV], STOP, 0);         break;
            case GUN_DOWN_STOP:     set_motor(motors_data[T_CANNON_ELEV], STOP, 0);         break;
        }
    }
}
