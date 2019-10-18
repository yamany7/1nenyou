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

    void virtual start(); //reizi suru
    void virtual kill(); //reizi kiru

    void virtual halfBlock(); //hanmasu susumu
    void virtual detailedBlock(); //shitei no palse susumu
    void virtual turn(char direction); //magaru,mawaru
    void virtual daikei(int interval, char direction); //daikei de susumu
    void virtual pCtrl(); //p seigyo

    bool virtual isOpenedWallF(); //mae kabe check
    bool virtual isOpenedWallL(); //hidari kabe check
    bool virtual isOpenedWallR(); // migi kabe check

};

#endif
