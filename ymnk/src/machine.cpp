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

    while(true)
    {
        pCtrl();
        if((Lmotor->pulse_cnt()>HALF_SEC)||(Rmotor->pulse_cnt()>HALF_SEC)) break;
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
void Machine::detailedBlock(int speed) //shitei no palse susumu
{
    Lmotor->reset_cnt();
    Rmotor->reset_cnt();
    Lmotor->update(speed);
    Rmotor->update(speed);

    while(true)
    {
        if(Lmotor->pulse_cnt()>=START_PULSE) break;
    }

}

void Machine::turn(char direction) //magaru,mawaru
{
    int speed;
    int a;
    int x;
    int half_val;
    Lmotor->reset_cnt();
    Rmotor->reset_cnt();

    if(direction == 'L') //hidari ni magaru
    {
        speed = 0;
        a = (SPEED - 30)/20;
        x = TURN90/20;
        half_val = TURN90/20;

        speed = 30;

        while(speed < SPEED/2-a)
        {
            while(true){
                Lmotor->update(-speed);
                Rmotor->update(speed);

                if(Rmotor->pulse_cnt() > half_val){
                    speed += a;
                    Lmotor->reset_cnt();
                    Rmotor->reset_cnt();
                    break;
                }
            }

        }
        speed = SPEED/2;

        while(speed > 30)
        {
            while(true)
            {
                Lmotor->update(-speed);
                Rmotor->update(speed);

                if(Rmotor->pulse_cnt() > half_val)
                {
                    speed -= a;
                    Lmotor->reset_cnt();
                    Rmotor->reset_cnt();
                    break;
                }
            }
        }


    }
    else if(direction == 'R') //migi ni magaru
    {
        speed = 0;
        a = (SPEED - 30)/20;
        x = TURN90/20;
        half_val = TURN90/20;

        speed = 30;

        while(speed < SPEED/2-a)
        {
            while(true){
                Lmotor->update(speed);
                Rmotor->update(-speed);

                if(Lmotor->pulse_cnt() > half_val){
                    speed += a;
                    Lmotor->reset_cnt();
                    Rmotor->reset_cnt();
                    break;
                }
            }

        }
        speed = SPEED/2;

        while(speed > 30)
        {
            while(true)
            {
                Lmotor->update(speed);
                Rmotor->update(-speed);

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
    else if(direction == 'T') //180do magaru
    {
        speed = 0;
        a = (SPEED - 30)/20;
        x = TURN90/20;
        half_val = TURN90/20;

        speed = 30;

        while(speed < SPEED/2-a)
        {
            while(true){
                Lmotor->update(speed);
                Rmotor->update(-speed);

                if(Lmotor->pulse_cnt() > half_val){
                    speed += a;
                    Lmotor->reset_cnt();
                    Rmotor->reset_cnt();
                    break;
                }
            }

        }
        speed = SPEED/2-10;

        while(speed > 30)
        {
            while(true)
            {
                Lmotor->update(speed);
                Rmotor->update(-speed);

                if(Lmotor->pulse_cnt() > half_val)
                {
                    speed -= a;
                    Lmotor->reset_cnt();
                    Rmotor->reset_cnt();
                    break;
                }
            }
        }

        Lmotor->update(-50);
        Rmotor->update(-50);
        wait(2);
        Lmotor->update(0);
        Rmotor->update(0);
        detailedBlock(50);
        Lmotor->reset_cnt();
        Rmotor->reset_cnt();

        speed = 0;
        a = (SPEED - 30)/20;
        x = TURN90/20;
        half_val = TURN90/20;

        speed = 30;

        while(speed < SPEED/2-a)
        {
            while(true){
                Lmotor->update(speed);
                Rmotor->update(-speed);

                if(Lmotor->pulse_cnt() > half_val){
                    speed += a;
                    Lmotor->reset_cnt();
                    Rmotor->reset_cnt();
                    break;
                }
            }

        }
        speed = SPEED/2-10;

        while(speed > 30)
        {
            while(true)
            {
                Lmotor->update(speed);
                Rmotor->update(-speed);

                if(Lmotor->pulse_cnt() > half_val)
                {
                    speed -= a;
                    Lmotor->reset_cnt();
                    Rmotor->reset_cnt();
                    break;
                }
            }
        }
        Lmotor->update(-50);
        Rmotor->update(-50);
        wait(2);
        Lmotor->update(0);
        Rmotor->update(0);
        Lmotor->reset_cnt();
        Rmotor->reset_cnt();

    }else{}
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
    int s_sensor_sum = 180;
    int Lspeed, Rspeed, Ltemp, Rtemp;
    int L_sensor = Lsen->get_val();
    int R_sensor = Rsen->get_val();


    if(R_sensor > 150 && L_sensor > 150){
        Rtemp = s_sensor_sum/2;
        Ltemp = s_sensor_sum/2;
    }else if(R_sensor > 150){
		   	Ltemp = L_sensor;
		   	Rtemp = 180 - Ltemp;
		}else if(L_sensor > 150){
		   	Rtemp = R_sensor;
		   	Ltemp = 180 - Rtemp;
	  }else{
   			Rtemp = R_sensor;
	   		Ltemp = L_sensor;
    }

   	Lmotor->update(SPEED + KP * (Rtemp - Ltemp));
   	Rmotor->update(SPEED + KP * (Ltemp - Rtemp));
   	wait_ms(3);

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
