#ifndef MACHINE_H
#define MACHINE_H

#include <mslm/mslm.h>
#include <parameter.h>

//#define UnitPart 440

class Machine
{
public:
    StepMotor *Lmotor, *Rmotor;
    DistanceSensor *Fsen, *Lsen, *Rsen;

    Machine()
    {
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

    void start(); //reizi suru
    void kill(); //reizi kiru

    void halfBlock(); //hanmasu susumu
    void detailedBlock(); //shitei no palse susumu
    void detailedBlock(int speed);
    void turn(char direction); //magaru,mawaru
    void daikei(int interval, char direction); //daikei de susumu
    void pCtrl(); //p seigyo

    bool isOpenedWallF(); //mae kabe check
    bool isOpenedWallL(); //hidari kabe check
    bool isOpenedWallR(); // migi kabe check

};

#endif
