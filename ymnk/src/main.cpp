#include <mouse.h>

AnalogOut Ref(p18);
BusOut myled(LED4, LED3, LED2, LED1);
Serial pc(USBTX, USBRX);
Switch sw1(p5, PullUp); //orange_l
Switch sw2(p6, PullUp); //orange_r
Switch sw3(p7, PullUp); //blue
Switch sw4(p8, PullUp); //red


int main()
{
    Test machine;
    int mode = 1, led_num = 1;
    Ref = 0.15/3.3;

    while(true)
    {

        while(true)
        {

            if(sw1.update())
            {
                mode = led_num;
                break;
            }
            else if(sw2.update())
            {
                led_num++;
                if(led_num > 15) led_num = 1;
            }
            else if(sw3.update())
            {
                led_num--;
                if(led_num < 1) led_num = 15;
            }
            else{}
            myled = led_num;
        }

        switch(mode)
        {
            case 1: //hidarite tansaku
                machine.start();
                wait(1);
                machine.daikei(10, 'A');
                machine.detailedBlock();
                while(true)
                {

                    if(machine.isOpenedWallF())
                    {
                        myled=0b0110;
                        machine.halfBlock();
                        machine.halfBlock();
                    }
                    else if(machine.isOpenedWallL())
                    {
                        myled=0b1100;
                        machine.daikei(10, 'D');
                        machine.turn('L');
                        machine.daikei(10, 'A');
                    }
                    else if(machine.isOpenedWallR())
                    {
                        myled=0b0011;
                        machine.daikei(10, 'D');
                        machine.turn('R');
                        machine.daikei(10, 'A');
                    }
                    else {
                        myled=0b1001;
                        machine.daikei(10, 'D');
                        machine.turn('T');
                        machine.daikei(10, 'A');
                    }

                }

            break;

            case 2: //sensor test
                while(true)
                {
                    pc.printf("\r\b\r\b\r");
                    //pc.printf(" LS=%4d | FS=%4d | RS=%4d |\r\n", machine.Lsen.get_val(), machine.Fsen.get_val(), machine.Rsen.get_val());
                }

            break;

            case 3: //1masu
                machine.start();
                machine.halfBlock();
            break;

            case 4: //turn test
                machine.start();
                machine.turn('L');
                wait(1);
                machine.turn('R');
                wait(1);
                machine.turn('T');
            break;

            default:
            break;

        }

    }


}
