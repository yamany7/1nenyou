#include <mbed.h>
#include <machine.h>

AnalogOut Ref(p18);
BusOut myled(LED4, LED3, LED2, LED1);
Serial pc(USBTX, USBRX);
Switch Sw1(p5, PullUp); //ika switch no pin
Switch Sw2(p6, PullUp);
Switch Sw3(p7, PullUp);
Switch Sw4(p8, PullUp);

int main()
{
    Machine machine;
    int mode = 1, led_num = 1;
    Ref = 0.15/3.3;

    while(1)
    {
        switch (mode)
        {
            case 1://mode

                while(true)
                {

                    if(Sw1.update())
                    {
                        mode = led_num;
                        break;
                    }
                    else if(Sw2.update())
                    {
                        led_num++;
                        if(led_num > 15) led_num = 1;
                    }
                    else if(Sw3.update())
                    {
                        led_num--;
                        if(led_num < 1) led_num = 15;
                    }
                    else{}
                    myled = led_num;
                }

                break;

            case 2:
            myled = 0;
            wait(1);

                while(1)
                {
                    if(machine.isOpenedWallF()){
                        myled=0b0110;
                    }
                    else if(machine.isOpenedWallL()){
                        myled=0b1100;
                    }
                    else if (machine.isOpenedWallR()){
                        myled=0b0011;
                    }else{
                        myled=0b1111;
                    }
                }
                break;

            case 3:
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
                        machine.turn90('L');
                        machine.daikei(10, 'A');
                    }
                    else if(machine.isOpenedWallR())
                    {
                        myled=0b0011;
                        machine.daikei(10, 'D');
                        machine.turn90('R');
                        machine.daikei(10, 'A');
                    }
                    else {
                        myled=0b1001;
                        machine.daikei(10, 'D');
                        machine.turn90('T');
                        machine.daikei(10, 'A');
                    }

                }

                if(Sw4.update()){
                  myled = 1;
                  wait(0.5);
                  mode = 1;
                  break;
                }

                break;

            case 4:
                while(1)
                {
                    pc.printf("\r\b\r\b\r");
                  //  pc.printf(" LS | L3 | F | R3 | RS |  \r\n");
                    pc.printf(" LS=%4d | FS=%4d | RS=%4d |\r\n", machine.Lsen.get_val(), machine.Fsen.get_val(), machine.Rsen.get_val());
                    wait(1);

                    if(Sw4.update()){
                      myled = 1;
                      wait(0.5);
                      mode = 1;
                      break;
                    }
                }
            case 5:
                machine.start();
                machine.halfBlock();


            }
    }

}
