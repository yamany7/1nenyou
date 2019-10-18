#ifndef MOUSE_H
#define MOUSE_H

#include "machine.h"

class Test : public Machine
{
public:
    Switch *Sw1;
    Switch *Sw2;
    Switch *Sw3;
    Switch *Sw4;

    Test()
    {
        Sw1 = new Switch(p5, PullUp);
        Sw2 = new Switch(p6, PullUp);
        Sw3 = new Switch(p7, PullUp);
        Sw4 = new Switch(p8, PullUp);
        Fsen = new DistanceSensor(p16);
        Lsen = new DistanceSensor(p15);
        Rsen = new DistanceSensor(p17);
        //motor no pin shitei
        //(clock, reset, cw, b_cw,  m1,  m2, m3)
        Lmotor = new StepMotor(p24, p26, p25, true, p14, p22, p23);
        Rmotor = new StepMotor(p10, p12, p11, false, p13, p21, p9);

        Lmotor -> kill();
        Rmotor -> kill();
        Lmotor -> update(0);
        Rmotor -> update(0);
    }

    void pCtrl() //p seigyo
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

};

#endif
