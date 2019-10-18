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
        Lmotor -> kill();
        Rmotor -> kill();
        Lmotor -> update(0);
        Rmotor -> update(0);
    }

    void start(); //reizi suru
    void kill(); //reizi kiru

    void halfBlock(); //hanmasu susumu
    void detailedBlock(); //shitei no palse susumu
    void turn(char direction); //magaru,mawaru
    void daikei(int interval, char direction); //daikei de susumu
    void pCtrl(); //p seigyo

    bool isOpenedWallF(); //mae kabe check
    bool isOpenedWallL(); //hidari kabe check
    bool isOpenedWallR(); // migi kabe check

};

#endif
