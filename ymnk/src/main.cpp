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

                while(1)
                {
                    pc.printf("\r\b\r\b\r");
                    pc.printf(" LS | L3 | F | R3 | RS |  \r\n");
                    pc.printf(" %4d | %4d | %4d | %4d |\r\n", machine.Lsen.get_val(), machine.Flsen.get_val(), machine.Frsen.get_val(), machine.Rsen.get_val());
                }
            break;

            case 3:
                machine.start();
                wait(2);

                machine.daikei(10, 'F');
                machine.detailedBlock();

                while(true)
                {
                    if(machine.isOpenedWall('F'))
                    {
                        machine.halfBlock();
                        machine.halfBlock();
                    }
                    else if(machine.isOpenedWall('L'))
                    {
                        machine.daikei(10, 'D');
                        machine.turn90('L');
                        machine.daikei(10, 'A');
                    }
                    else if(machine.isOpenedWall('R'))
                    {
                        machine.daikei(10, 'D');
                        machine.turn90('R');
                        machine.daikei(10, 'A');
                    }
                    else if(machine.isOpenedWall('F') && machine.isOpenedWall('L') && machine.isOpenedWall('R'))
                    {
                        machine.daikei(10, 'D');
                        machine.turn90('L');
                        machine.turn90('L');
                        machine.daikei(10, 'A');
                    }
                    else { mode = 1; break; }

                }

            break;

            }
    }

}
