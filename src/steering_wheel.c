#include <steering_wheel.h>

int16_t turn_to = 0;
bool turned = false;
float Dima_speed = 0;
int16_t prev_err = 0;
double I = 0;
int16_t PID = 0;
float P = 0;
float D = 0;
int8_t max_err = 0;
int16_t now_err = 0;
bool turn_enable = true;
bool turn_dir = false;
float kpcur = 15;
float kicur = 15;
float kp = 0;
float kd = 0;
float ki = 0;

void start_motor(void)
{
    turn_enable = true;
}

void stop_motor(void)
{
    turn_enable = false;
}

PWMConfig pwm1conf = {
    .frequency = 4000000,
    .period    = 10000,
    .callback  = NULL,
    .channels  = {
                  {.mode = PWM_OUTPUT_ACTIVE_HIGH, .callback = NULL}, // k wtyke
                  {.mode = PWM_OUTPUT_ACTIVE_HIGH, .callback = NULL}, // ot wtuke
                  {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL}
                  },
    .cr2        = 0,
    .dier       = 0
};


void lldMotorInit(void)
{
    palSetLineMode( PWM1_P9,  PWM1_MODE_P9  );
    palSetLineMode( PWM1_P11,  PWM1_MODE_P11 );
    pwmStart( PWMDriver1, &pwm1conf );
    pwmEnableChannel( PWMDriver1, PWM1_CHANNEL_P9, 5840 );
    chThdSleepMilliseconds(5000);
    chThdCreateStatic(waTURNING_WHEELS, sizeof(waTURNING_WHEELS), NORMALPRIO, TURNING_WHEELS, NULL);
}
int16_t count = 0;

void steerUnitCSSetPosition(int32_t position)
{
    if(abs(position - get_wheel_pos())>9)
    {


        turn_to = CLIP_VALUE(position, -100, 100);
        //    turn_to = CLIP_VALUE(position+7*abs(position)/position, -100, 100);
        turned = false;
    }
}
bool almost_turned = false;


void lldSetPowerMotor(int8_t powerPrct) //WORKING     (откатывать до сюдaaaa)
{
    powerPrct = CLIP_VALUE(powerPrct,-100,100);
    if(powerPrct == 0)
    {
        pwmEnableChannel( PWMDriver1, PWM1_CHANNEL_P9, 6000 - 160);
    }
    else if(powerPrct > 0 && powerPrct <= 100)
    {
        pwmEnableChannel( PWMDriver1, PWM1_CHANNEL_P9, 6000+20*powerPrct - 160 );
        turn_dir = true;
    }
    else if(powerPrct < 0 && powerPrct >= -100)
    {
        if(turn_dir == true)
        {
            turn_dir = false;
            pwmEnableChannel( PWMDriver1, PWM1_CHANNEL_P9, 6000-1000 - 160);
            chThdSleepMilliseconds(10);
            pwmEnableChannel( PWMDriver1, PWM1_CHANNEL_P9, 6000 - 160);
            chThdSleepMilliseconds(10);
        }
        pwmEnableChannel( PWMDriver1, PWM1_CHANNEL_P9, 6000+20*powerPrct - 160 );
    }

}
int16_t Curr =0;
float CurrP = 0;
float CurrI = 0;
int8_t TurnMotor(void)
{

    if(turn_enable == false)
    {
        return 0;
    }

    now_err = turn_to - get_wheel_pos();
    P = kp*now_err;
    D = kd*(now_err - prev_err);
    I += ki*now_err;
    prev_err = now_err;
    if(abs(now_err)<1)
    {
        almost_turned = true;
    }
    if(almost_turned == true)
    {
        I = I-I/40-ki*now_err;
        if(I<5 && I>-5)
        {
            almost_turned = false;
            turned = true;
            CurrI = 0;
            I = 0;
            P = 0;
            D = 0;
        }
    }
    PID = P+D+I;
    CurrP = kpcur*CLIP_VALUE(abs(get_current()/1000)-11,0,100);
    CurrI += kicur*CurrP/kpcur;
    Curr = (CurrP+CurrI)*abs(PID)/PID;
    PID = PID - CLIP_VALUE(Curr,(-1)*abs(PID),abs(PID));

    lldSetPowerMotor(PID - CLIP_VALUE(Curr,(-1)*abs(PID),abs(PID)));
}

systime_t time_potok = 0;

static THD_WORKING_AREA(waTURNING_WHEELS, 256);        // just for checking
static THD_FUNCTION(TURNING_WHEELS, arg)
{
    int cnt = 0;
     time_potok= chVTGetSystemTime();
    (void)arg;
    while (true)
    {
        if(turn_enable == true)
        {
            if(turned == false)
            {
                if(get_speed() > 1.5)
                {
                    kp = 0.15;
                    kd = 0;
                    ki = 0.005;
                    max_err = 3;
                    TurnMotor();
                }
                else if(get_speed() <= 1.5)
                {
                    kp = 0.15;
                    kd = 0;
                    ki = 0.05;
                    max_err = 3;
                    TurnMotor();
                }
                cnt++;
        //        if (cnt >= 10)
        //        {
        //            chprintf(((BaseSequentialStream *)&SD7), "i%d    ", turn_to);
        //            chprintf(((BaseSequentialStream *)&SD7), "i%d    ", get_wheel_pos());
        //            chprintf(((BaseSequentialStream *)&SD7), "i%d    ", now_err);
        //            chprintf(((BaseSequentialStream *)&SD7), "i%d    ", P);
        //            chprintf(((BaseSequentialStream *)&SD7), "i%d    ", D);
        //            chprintf(((BaseSequentialStream *)&SD7), "i%d    ", (int)I);
        //            chprintf(((BaseSequentialStream *)&SD7), "i%d    \r\n", PID);
        //
        //            cnt = 0;
        //        }
            }
        }
        else
        {
            lldSetPowerMotor(0);
        }
//        pwmEnableChannel( PWMDriver1, PWM1_CHANNEL_P9, 5840 );
        time_potok = chThdSleepUntilWindowed(time_potok,time_potok+MS2ST(20));
    }
}

