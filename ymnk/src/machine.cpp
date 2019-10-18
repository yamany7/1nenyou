#include "machine.h"

void Machine::start() //reizi suru
{
    Lmotor->Start();
    Rmotor->Start();
}

void Machine::kill() //reizi kiru
{
    Lmotor->kill();
    Rmotor->kill();
}

void Machine::halfBlock() //hanmasu susumu
{
    Lmotor->reset_cnt();
    Rmotor->reset_cnt();
    Lmotor->update(SPEED);
    Rmotor->update(SPEED);

    while(true)
    {
        pCtrl();
        if((Lmotor->pulse_cnt()>HALF_SEC)||(Lmotor->pulse_cnt()>HALF_SEC)) break;
    }
}

void Machine::detailedBlock() //shitei no palse susumu
{
    Lmotor->reset_cnt();
    Rmotor->reset_cnt();
    Lmotor->update(SPEED);
    Rmotor->update(SPEED);

    while(true)
    {
        if(Lmotor->pulse_cnt()>=START_PULSE) break;
    }

}

void Machine::turn(char direction) //magaru,mawaru
{
    Lmotor->reset_cnt();
    Rmotor->reset_cnt();

    if(direction == 'L') //hidari ni magaru
    {
        Lmotor->update(-SPEED/2);
        Rmotor->update(SPEED/2);

        while(true)
        {
        	if(Rmotor->pulse_cnt() >= TURN90) break;
        }
        Lmotor->update(0);
        Rmotor->update(0);

    }
    else if(direction == 'R') //migi ni magaru
    {
        Lmotor->update(SPEED/2);
        Rmotor->update(-SPEED/2);

        while(true)
        {
            if(Lmotor->pulse_cnt() >= TURN90) break;
        }
        Lmotor->update(0);
        Rmotor->update(0);
    }
    else if(direction == 'T') //180do magaru
    {
        Lmotor->update(SPEED/2);
        Rmotor->update(-SPEED/2);

        while(true)
        {
            if(Lmotor->pulse_cnt() >= TURN90*2) break;
        }
        Lmotor->update(0);
        Rmotor->update(0);

    }
    else{}
}

void Machine::daikei(int interval, char direction) //daikei de susumu
{
    Lmotor->reset_cnt();
    Rmotor->reset_cnt();

    int speed = 0;
    int a = (SPEED - 30)/interval;
    int x = HALF_SEC/interval;
    int half_val = HALF_SEC/interval;

    if(direction == 'A') //kasoku
    {
        speed = 30;

        while(speed < SPEED)
        {
            while(true){
                Lmotor->update(speed);
                Rmotor->update(speed);

                if(Lmotor->pulse_cnt() > half_val){
                    speed += a;
                    Lmotor->reset_cnt();
                    Rmotor->reset_cnt();
                    break;
                }
            }

        }

    }
    else if(direction == 'D') //gensoku
    {
        speed = SPEED;

        while(speed > 30)
        {
            while(true)
            {
                Lmotor->update(speed);
                Rmotor->update(speed);

                if(Lmotor->pulse_cnt() > half_val)
                {
                    speed -= a;
                    Lmotor->reset_cnt();
                    Rmotor->reset_cnt();
                    break;
                }
            }
        }
    }
}

void Machine::pCtrl() //p seigyo
{
    int cf = Fsen->get_val();
    int ls = Lsen->get_val();
    int rs = Rsen->get_val();

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

    const int sensorDiff = ls - rs;
    Lmotor->update(500 - (sensorDiff * KP));
    Rmotor->update(500 + (sensorDiff * KP));
    wait(0.005);
}

bool Machine::isOpenedWallF() //mae kabe check
{
    return (Fsen->get_val()>DISTANCE_FRONT_WALL);
}
bool Machine::isOpenedWallL() //hidari kabe check
{
    return (Lsen->get_val()>DISTANCE_SIDE_WALL);
}
bool Machine::isOpenedWallR() // migi kabe check
{
    return (Rsen->get_val()>DISTANCE_SIDE_WALL);
}
