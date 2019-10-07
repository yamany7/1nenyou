#include <mslm/mslm.h>
#include <parameter.h>

//#define UnitPart 440

class Machine
{
public:
    StepMotor *Lmotor, *Rmotor;
    DistanceSensor Flsen, Frsen, Lsen, Rsen;

    //sensor pin shitei
    Machine():Flsen(p15), Frsen(p19), Lsen(p16), Rsen(p18)
    {
        //motor no pin shitei
        //(clock, reset, cw, b_cw,  m1,  m2, m3)
        Lmotor = new StepMotor(p10, p12, p11, false, p13, p21, p9);
        Rmotor = new StepMotor(p24, p26, p25, true, p14, p22, p23);

        Lmotor -> kill();
        Rmotor -> kill();
        Lmotor -> update(0);
        Rmotor -> update(0);
    }

    inline void start()
    {
        Lmotor->Start();
        Rmotor->Start();
    }
    inline void kill(){
        Lmotor->kill();
        Rmotor->kill();
    }

    void halfBlock()
    {
        Lmotor->reset_cnt();
        Rmotor->reset_cnt();
        Lmotor->update(SPEED);
        Rmotor->update(SPEED);

        while(true)
        {
            if(Lmotor->pulse_cnt()>=HALF_SEC)
            {
                Lmotor->update(0);
                Rmotor->update(0);
                break;
            }
        }
    }

    void turn90(char direction)
    {
        Lmotor->reset_cnt();
        Rmotor->reset_cnt();

        if(direction == 'L')
        {
            Lmotor->update(-SPEED);
            Rmotor->update(SPEED);
            while(true)
            {
                if(Rmotor->pulse_cnt() >= TURN90)
                {
                    Lmotor->update(0);
                    Rmotor->update(0);
                    break;
                }
            }

        }
        else if(direction == 'R')
        {
            Lmotor->update(SPEED);
            Rmotor->update(-SPEED);
            while(true)
            {
                if(Lmotor->pulse_cnt() >= TURN90) break;
            }
        }
        else if(direction == 'T')
        {
            Lmotor->update(SPEED);
            Rmotor->update(-SPEED);
            while(true)
            {
                if(Lmotor->pulse_cnt() >= TURN90*2) break;
            }

        }
        else{}
    }

    void daikei(int interval, char direction)
    {
        Lmotor->reset_cnt();
        Rmotor->reset_cnt();

        int speed = 0;
        int a = (SPEED - 30)/interval;
        int x = HALF_SEC/interval;
        int half_val = HALF_SEC/interval;
        int i = 0;

        if(direction == 'A')
        {
            speed = 30;

            while(speed < SPEED)
            {
                Lmotor->update(SPEED);
                Rmotor->update(SPEED);

                if(Lmotor->pulse_cnt() > half_val){
                    speed += a;
                    Lmotor->reset_cnt();
                    Rmotor->reset_cnt();
                }
            }

        }
        else if(direction == 'D')
        {
            speed = SPEED;

            while(speed < 30)
            {
                Lmotor->update(speed);
                Rmotor->update(speed);

                if(Lmotor->pulse_cnt() > half_val)
                {
                    speed -= a;
                    Lmotor->reset_cnt();
                    Rmotor->reset_cnt();
                }
            }
        }
    }

    void pCtrl()
    {
        int cf = Flsen.get_val();
        int ls = Lsen.get_val();
        int rs = Rsen.get_val();

        const int sensorSum = ls + rs;

        if(sensorSum > RYOU_KABE)
        {
            if(ls > KATA_KABE && rs > KATA_KABE)
            {
                ls = rs = 0;
            }
            else if(ls > KATA_KABE)
            {
                ls = RYOU_KABE - rs;
            }
            else if(rs > KATA_KABE)
            {
                rs = RYOU_KABE - ls;
            }
        }

        if(cf < 500) ls = rs = 0;

        const int sensorDiff = ls - rs;
        Lmotor->update(SPEED - (sensorDiff * KP));
        Rmotor->update(SPEED + (sensorDiff * KP));
    }

    bool isOpenedWall(char direction)
    {
        if(direction=='F')
        {
            if(Flsen.get_val()>DISTANCE_FRONT_WALL && Frsen.get_val()>DISTANCE_FRONT_WALL) return true;
            else return false;
        }
        else if(direction=='L')
        {
            if(Lsen.get_val()>DISTANCE_SIDE_WALL) return true;
            else return false;
        }
        else if(direction=='R')
        {
            if(Rsen.get_val()>DISTANCE_SIDE_WALL) return true;
            else return false;
        }
    }

    void detailedBlock()
    {
        Lmotor->reset_cnt();
        Rmotor->reset_cnt();
        Lmotor->update(SPEED);
        Rmotor->update(SPEED);

        while(true)
        {
            if(Lmotor->pulse_cnt()>=START_PALSE) break;
        }
    }
};
