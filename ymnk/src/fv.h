#ifndef FV_H
#define FV_H
#include "mslm/deftype.h"
#include "mslm/sensor.h"
#include "mslm/motor2.h"
#include "mslm/map_mbed.h"
#include "vector.hpp"
#define First_Hz 300
#define FOWARD_CLK 810 //1マス分のクロック数
#define H_C 410
#define RIGHT_CLK 285 //右90度分のクロック数
#define LEFT_CLK 285 //左90度分のクロック数
#define TURN_CLK 570 //180度分のクロック数
#define DISTANCE 150


//Serial pc(USBTX, USBRX);

class MoveMean{
    int size;
public:
    int *value;
    MoveMean(int _size):value(new int[_size]),size(_size){
        for (int i = 0; i < size; ++i) {
            value[i] = 0;
        }
    }
    int push(int val){
        for(int i=size;i>0;i--){
            value[i] = value[i-1];
        }
        value[0] = val;

        int sum = 0;
        for (int j = 0; j < size; ++j) {
            sum += value[j];
        }
        return sum/size;
    }

    inline int get_size(){return size;}

};

class LowPassFilter{
    double a;
public:
    int *value;
    LowPassFilter(double _a):a(_a), value(new int [2]){
    }

    int push(int val){
      value[1] = a * value[0] + (1-a) * val;
      value[0] = value[1];

      return value[0];
    }
};

class fv{
 private:
  int lvec[10]{0};
  int rvec[10]{0};
  int fvec[10]{0};
  int cntcnt = 0;
  int sum;
  bool c_check;
  int s_sensor_sum;


	DistanceSensor Front, L_Side, R_Side, L_30, R_30;
	StepMotor *RMotor, *LMotor;

	Ticker *motor_AB;
  //MoveMean ML, MR;
  LowPassFilter ML, MR;

	short int start_Hz, target_Hz, current_Hz;
	short int Lcurrent_Hz, Rcurrent_Hz;
	int F_Hz_cnt;
	bool Lcw, Rcw;
	bool accel_check = true;
	int target_clock = 1000;
	int integral{0};
 public:

	fv():Front(p15), L_Side(p16), R_Side(p17), L_30(p19), R_30(p20), ML(0.8), MR(0.8){
	}

	void startup(){
    //StepMotor(clock,reset,cw,b_cw,m3)
		//LMotor = new StepMotor(p13, p11, p14, false, p12);
		//RMotor = new StepMotor(p22, p24, p21, true, p23);
    LMotor = new StepMotor(p10, p12, p11, false, p9);
		RMotor = new StepMotor(p24, p26, p25, true, p23);


		RMotor->kill();
		LMotor->kill();
		RMotor->update(0);
		LMotor->update(0);

		current_Hz = 0;
		F_Hz_cnt = 0;
  }
  int calibration_sensor(int dir){
    MoveMean sensor(10);//,l_sensor(50),r_sensor(50);
		int ave;//,l_ave,r_ave;
		for(int i=0;i<11;i++){
			//l_ave = l_sensor.push(L_30.get_val());
			//r_ave = r_sensor.push(R_30.get_val());
      if(dir == 0) ave = sensor.push(L_Side.get_val());
      else if (dir == 1) ave = sensor.push(Front.get_val());
      else if(dir == 2) ave = sensor.push(R_Side.get_val());
			//wait_ms(1);
		}
    if(dir) return ave;
    else return ave;

    //if((r_ave-l_ave)>20) r_ave = 135;

		//R_30.set_corecction(l_ave-r_ave);
    //R_Side.set_corecction(ls_ave-rs_ave);
    //sensor_sum = l_ave + r_ave;
    //s_sensor_sum = ls_ave + rs_ave;
    //while(1) printf("%d | %d | %d | %d \r\n", l_ave, r_ave, R_30.get_corecction(), L_30.get_corecction());
  }

	void motor_start(){
		LMotor->Start();
		RMotor->Start();
	}
	void motor_kill(){
		RMotor->kill();
	  LMotor->kill();
	}


	void motor_ctrl(int speed_Hz){
		if(speed_Hz == 0){
			start_Hz = current_Hz;
			target_Hz = First_Hz;
			motor_update(current_Hz, current_Hz);
			motor_AB->attach(this, &fv::Brake, 0.005);
		}else{
			start_Hz = First_Hz;
			target_Hz = speed_Hz;
			current_Hz = First_Hz;
			motor_update(current_Hz, current_Hz);
			motor_AB->attach(this, &fv::Accel, 0.005);
		}
	}

  /*void PI_ctrl(){
    double KP = 1.0;
    int Lspeed, Rspeed, Ltemp, Rtemp;
    int LS_sensor = ML.push(L_Side.get_val());
    int RS_sensor = MR.push(R_Side.get_val());
    int L_sensor, R_sensor;
    if (Front.get_val()>90){
      L_sensor = ML.push(L_30.get_val());
      R_sensor = MR.push(R_30.get_val());
    }else{
      L_sensor = 700;
      R_sensor = 700;
    }


    if(RS_sensor > 150 && LS_sensor > 150){
      if(L_sensor > R_sensor){
        Ltemp = 5;
        Rtemp = 0;
      }else if(L_sensor < R_sensor){
        Ltemp = 0;
        Rtemp = 5;
      }else if(L_sensor < 120 && R_sensor < 120){
        Rtemp = s_sensor_sum/2;
        Ltemp = s_sensor_sum/2;
      }else{
        Rtemp = 0;
        Ltemp = 0;
      }
    }else if(RS_sensor > 150){
        KP = 2;
        Ltemp = LS_sensor;
        if(R_sensor < 150) Rtemp = s_sensor_sum*(sensor_sum - L_sensor)/sensor_sum;
        else Rtemp = 170 - Ltemp;
    }else if(LS_sensor > 150){
        KP = 2;
        Rtemp = RS_sensor;
        if(L_sensor < 150) Ltemp = s_sensor_sum*(sensor_sum - R_sensor)/sensor_sum;
        else Ltemp = 170 - Rtemp;
    }else{
        Rtemp = RS_sensor;
        Ltemp = LS_sensor;
    }

    Lspeed = current_Hz + KP * (Rtemp - Ltemp);
    Rspeed = current_Hz + KP * (Ltemp - Rtemp);
    motor_update(Lspeed, Rspeed);
    wait_ms(10);
  }*/

	void PI_ctrl(){
		double KP = 1.0;
		int Lspeed, Rspeed, Ltemp, Rtemp;
    int L_sensor = ML.push(L_Side.get_val());
    int R_sensor = MR.push(R_Side.get_val());


    if(R_sensor > 140 && L_sensor > 140){
        Rtemp = s_sensor_sum/2;
        Ltemp = s_sensor_sum/2;
		}else if(R_sensor > 140){
        KP = 1.1;
		   	Ltemp = L_sensor;
		   	Rtemp = 170 - Ltemp;
		}else if(L_sensor > 140){
        KP = 1.1;
		   	Rtemp = R_sensor;
		   	Ltemp = 170 - Rtemp;
	  }else{
        KP = 1.1;
   			Rtemp = R_sensor;
	   		Ltemp = L_sensor;
    }

   	Lspeed = current_Hz + KP * (Rtemp - Ltemp);
   	Rspeed = current_Hz + KP * (Ltemp - Rtemp);
   	motor_update(Lspeed, Rspeed);
   	wait_ms(10);
	}
  /*void H_PI(){
    double KP = 1.0;
		int Lspeed, Rspeed, Ltemp, Rtemp;
    int L_sensor = ML.push(L_Side.get_val());
    int R_sensor = MR.push(R_Side.get_val());

    if(R_sensor < 150 && L_sensor < 150){
        KP=1.0;
   			Rtemp = R_sensor;
	   		Ltemp = L_sensor;
    }

   	Lspeed = current_Hz + KP * (Rtemp - Ltemp);
   	Rspeed = current_Hz + KP * (Ltemp - Rtemp);
   	motor_update(Lspeed, Rspeed);
   	wait_ms(10);
  }*/

  void PI_ctrl_30(){
    double KP = 1.0;
    //bool hantei = 0;

		int Lspeed, Rspeed, Ltemp, Rtemp, Ftemp;
    int F_sensor = get_f_val();
    int L30_sensor = get_l30_val();//ML.push(L_30.get_val());
    int R30_sensor = get_r30_val();//MR.push(R_30.get_val());
    int LS_sensor = get_ls_val();
    int RS_sensor = get_rs_val();
    //int sabun = zettaiti(L_sensor - R_sensor);
    lvec[cntcnt%10]= get_ls_val();
    rvec[cntcnt%10]= get_rs_val();
    fvec[cntcnt%10]= get_f_val();
    cntcnt++;
    if(cntcnt == 99) cntcnt=0;
    if(F_sensor > 130){
      if(L30_sensor < 150 && R30_sensor < 150){
        Rtemp = R30_sensor;
        Ltemp = L30_sensor;
      }else if(R30_sensor > 300 && L30_sensor > 300){
          if(RS_sensor < 140 && LS_sensor < 140){
            Rtemp = RS_sensor;
            Ltemp = LS_sensor;
      		}else if(LS_sensor < 140){
      		   	Ltemp = LS_sensor;
      		   	Rtemp = 170 - Ltemp;
      		}else if(RS_sensor < 140){
      		   	Rtemp = RS_sensor;
      		   	Ltemp = 170 - Rtemp;
      	  }else{
            KP=0;
          }
      }else if(L30_sensor < 150){
          //KP = 1.0;
          Ltemp = L30_sensor;
          Rtemp = 280 - Ltemp;
      }else if(R30_sensor < 150){
          //KP = 1.0;
          Rtemp = R30_sensor;
          Ltemp = 280 - Rtemp;
      }else{
        if(RS_sensor < 140 && LS_sensor < 140){
          Rtemp = RS_sensor;
          Ltemp = LS_sensor;
    		}else if(LS_sensor < 140){
    		   	Ltemp = LS_sensor;
    		   	Rtemp = 170 - Ltemp;
    		}else if(RS_sensor < 140){
    		   	Rtemp = RS_sensor;
    		   	Ltemp = 170 - Rtemp;
    	  }else{
          KP=0;
        }
      }
    }else{
        KP=0;
        //hantei = 1;
    }

    /*if(zettaiti(Rtemp - Ltemp) > 200 && hantei == 0){
        KP = 0;
    }*/

    Lspeed = current_Hz + KP * (Rtemp - Ltemp);
   	Rspeed = current_Hz + KP * (Ltemp - Rtemp);
   	motor_update(Lspeed, Rspeed);
   	wait_ms(10);
  }

	void STD(bool ABcheck){
		if(ABcheck == true){
			motor_AB->detach();
			motor_AB->attach(this, &fv::Accel, 0.005);
		}else{
			motor_update(0, 0);
		}
	}


  int gcd(int a,int b){ //ユークリッドの互除法
    if(a%b==0){
      return(b);
    }else{
      return(gcd(b,a%b));
    }
  }

  void suraromu(int offset, int speed, int direction){ //(オフセット(mm), 車体幅, 平均スピード, 左か右か(左=1 or LEFT_MACHINE, 右=2 or RIGHT_MACHINE))
    int o, i, outside, inside, tread_width = 78;
    outside = 45 + (tread_width / 4) - (offset * 50);
    inside = 45 - (tread_width / 4) - (offset * 50);

    o = gcd(outside, 45);
    i = gcd(45, inside);

    o = speed * (outside / o) / (45 / o);
    i = speed * (inside / i) / (45 / i);

    if(direction == 1){
      motor_cnt_reset();
      motor_update(speed, speed);
      while(!WM_cnt(FOWARD_CLK * offset / 180)){
        PI_ctrl();
      }

      motor_cnt_reset();
      motor_update(i, o);
      while(!RM_cnt(RIGHT_CLK * outside * 4 / tread_width));

      motor_cnt_reset();
      motor_update(speed, speed);
      while(!WM_cnt(FOWARD_CLK * offset / 180)){
        PI_ctrl();
      }

    }else if(direction == 2){
      motor_cnt_reset();
      motor_update(speed, speed);
      while(!WM_cnt(FOWARD_CLK * offset / 18)){
        PI_ctrl();
      }

      motor_cnt_reset();
      motor_update(o, i);
      while(!LM_cnt(LEFT_CLK * outside * 4 / tread_width));

      motor_cnt_reset();
      motor_update(speed, speed);
      while(!WM_cnt(FOWARD_CLK * offset / 18)){
        PI_ctrl();
      }

    }else{}
  }

	void Accel(){
		int Ltemp, Rtemp;
		if(R_Side.get_val() > 120 && L_Side.get_val() >120){
			Rtemp = 90;
		   	Ltemp = 90;
		}else if(R_Side.get_val() > 120){
		   	Ltemp = L_Side.get_val();
		   	Rtemp = 168 - Ltemp;
		}else if(L_Side.get_val() > 120){
		   	Rtemp = R_Side.get_val();
		   	Ltemp = 168 - Rtemp;
	   	}else{
   			Rtemp = R_Side.get_val();
	   		Ltemp = L_Side.get_val();
	   	}
		current_Hz = current_Hz + ( 1.0 / 50 ) * (target_Hz - First_Hz);
		if((fv::LM_clk_cnt() + fv::RM_clk_cnt()) / 2  >= (target_clock / 2.25)) target_Hz = current_Hz;
		if(current_Hz >= target_Hz) current_Hz = target_Hz;
		motor_update(current_Hz + 0.2*Ltemp, current_Hz + 0.2*Rtemp);
		if(current_Hz >= target_Hz){
			motor_AB->detach();
			F_Hz_cnt = (LMotor->pulse_cnt() + RMotor->pulse_cnt()) / 2;
		}
	}
	void Brake(){
		int Ltemp, Rtemp;
		if(R_Side.get_val() > 120 && L_Side.get_val() >120){
			Rtemp = 90;
		   	Ltemp = 90;
		}else if(R_Side.get_val() > 120){
		   	Ltemp = L_Side.get_val();
		   	Rtemp = 168 - Ltemp;
		}else if(L_Side.get_val() > 120){
		   	Rtemp = R_Side.get_val();
		   	Ltemp = 168 - Rtemp;
	   	}else{
   			Rtemp = R_Side.get_val();
	   		Ltemp = L_Side.get_val();
	   	}
		current_Hz = current_Hz - ( 1.0 / 50 ) * (start_Hz - First_Hz);
		if(current_Hz <= First_Hz) current_Hz = 0;
		if(current_Hz != 0) motor_update(current_Hz + 0.2*Ltemp, current_Hz + 0.2*Rtemp);
		else motor_update(0, 0);
		if(current_Hz <= target_Hz){
			motor_AB->detach();
			F_Hz_cnt = 0;
			current_Hz = 0;
			target_clock = 1000;
		}
	}

	void SBrake(){
		current_Hz -= 200;
		motor_update(current_Hz, current_Hz);
		if(current_Hz < First_Hz){
			motor_AB->detach();
			current_Hz = 0;
			motor_update(current_Hz, current_Hz);
		}
	}

	void Stop(){
		motor_AB->detach();
	  motor_AB->attach(this, &fv::SBrake, 0.01);
	}

  void motor_update(int LSpeed_Hz, int RSpeed_Hz){
		RMotor->update(RSpeed_Hz);
		LMotor->update(LSpeed_Hz);
		if(LSpeed_Hz == 0 && RSpeed_Hz == 0)
			current_Hz = 0;
		else if(LSpeed_Hz == RSpeed_Hz)
			current_Hz = LSpeed_Hz;
	}

	unsigned char now_wall_value( void ){
		unsigned char nowwall = 0;
		if(R_Side.get_val() >= 120) nowwall += 1;
    if(Front.get_val() >= 170) nowwall+=2;
		//if(R_Front.get_val() >= 170 || R_Front.get_val() >= 170) nowwall += 2;
		if(L_Side.get_val() >= 120) nowwall += 4;
		return nowwall;
	}

	int LM_clk_cnt(){
		if(LMotor->pulse_cnt() < 0) return - LMotor->pulse_cnt();
		return LMotor->pulse_cnt();
	}
	int RM_clk_cnt(){
		if(RMotor->pulse_cnt() < 0) return - RMotor->pulse_cnt();
		return RMotor->pulse_cnt();
	}
	void target_clk(int tar_clk){
		target_clock = tar_clk;
	}
  bool F_check(unsigned char distance){
    if(Front.get_val() >= distance) return false;
    else return true;
  }
	bool LS_check(unsigned char distance){
		if(L_Side.get_val() >= distance) return false;
		else return true;
	}
	/*bool LF_check(unsigned char distance){
		if(L_Front.get_val() >= distance) return false;
		else return true;
	}
	bool RF_check(unsigned char distance){
		if(R_Front.get_val() >= distance) return false;
		else return true;
	}*/
	bool RS_check(unsigned char distance){
		if(R_Side.get_val() >= distance) return false;
		else return true;
	}
  bool L30_check(unsigned char distance){
    if(L_30.get_val() >= distance) return false;
    else return true;
  }
  bool R30_check(unsigned char distance){
    if(R_30.get_val() >= distance) return false;
    else return true;
  }
	bool LM_cnt(int clk_count){
		if(LMotor->pulse_cnt() >= clk_count) return true;
		else return false;
	}
	bool RM_cnt(int clk_count){
		if(RMotor->pulse_cnt() >= clk_count) return true;
		else return false;
	}
	bool WM_cnt(int clk_count){
		if( (RMotor->pulse_cnt() + LMotor->pulse_cnt()) / 2 >= clk_count) return true;
		else return false;
	}

   void motor_cnt_reset(void){
		RMotor->reset_cnt();
		LMotor->reset_cnt();
	}
	int Firstcnt(void){
		return F_Hz_cnt;
	}
	int nowHz(void){
		return current_Hz;
	}
	int get_RM_palse(void){
		return (RMotor->pulse_cnt());
	}
	int get_LM_palse(void){
		return (LMotor->pulse_cnt());
	}

  inline int get_f_val(){return Front.get_val();}
	inline int get_ls_val(){return L_Side.get_val();}
	inline int get_rs_val(){return R_Side.get_val();}
  inline int get_l30_val(){return L_30.get_val();}
  inline int get_r30_val(){return R_30.get_val();}

  bool check_f(void){
    sum = 0;
    for(int i=0; i<10; i++){
      sum += fvec[i];
    }
    sum /= 10;
    if(sum > 150) return false;
    else return true;
  }
  bool check_l(void){
    sum = 0;
    for(int i=0; i<10; i++){
      sum += lvec[i];
    }
    sum /= 10;
    if(sum > DISTANCE) return false;
    else return true;
  }
  bool check_r(void){
    sum = 0;
    for(int i=0; i<10; i++){
      sum += rvec[i];
    }
    sum /= 10;
    if(sum > DISTANCE) return false;
    else return true;
  }
  int zettaiti(int n){
    if(n<0) return -1*n;
  }
  void turn(int x, int y, int z){
      y-=300;
      double a = y/x/x;
      int gosa = 0;
      int now = 0;

      if(z==0){
        int now_clock = RIGHT_CLK/x;
        //gosa = FOWARD_CLK - now_clock*x;
        for(int i=1; i<=(x-1)/2; i++){
          now = a*i*i;
          motor_cnt_reset();
          motor_update(-(now+300), now+300);
          while(1){
            if(RM_cnt(now_clock)) break;
          }
        }
        for(int i=((x)/2)+1; i>0; i--){
          now = a*i*i;
          motor_cnt_reset();
          motor_update(-(now+300), now+300);
          while(1){
            if(RM_cnt(now_clock)) break;
          }
        }
        motor_update(0, 0);
      }

      if(z==1){
        int now_clock = LEFT_CLK/x;
        //gosa = FOWARD_CLK - now_clock*x;
        for(int i=1; i<=(x-1)/2; i++){
          now = a*i*i;
          motor_cnt_reset();
          motor_update(now+300, -(now+300));
          while(1){
            if(LM_cnt(now_clock)) break;
          }
        }
        for(int i=((x)/2)+1; i>0; i--){
          now = a*i*i;
          motor_cnt_reset();
          motor_update(now+300, -(now+300));
          while(1){
            if(LM_cnt(now_clock)) break;
          }
        }
        motor_update(0, 0);
      }
      if(z==2){
        int now_clock = TURN_CLK/x;
        //gosa = FOWARD_CLK - now_clock*x;
        for(int i=1; i<=(x-1)/2; i++){
          now = a*i*i;
          motor_cnt_reset();
          motor_update(now+300, -(now+300));
          while(1){
            if(LM_cnt(now_clock)) break;
          }
        }
        for(int i=((x)/2)+1; i>0; i--){
          now = a*i*i;
          motor_cnt_reset();
          motor_update(now+300, -(now+300));
          while(1){
            if(LM_cnt(now_clock)) break;
          }
        }
      }
      motor_update(0, 0);
      motor_cnt_reset();

      /*fv.motor_cnt_reset();
      while(1){
        fv.PI_ctrl();
        fv.motor_update(now+200, now+200);
        if(fv.WM_cnt(gosa)) break;
      }*/
  }

  void daikei(int x, int y, int z){
      y-=300;
      double a = y/x/x;
      int gosa = 0;
      int now = 0;
      int now_clock = FOWARD_CLK/x;

      if(z==1){
        //gosa = FOWARD_CLK - now_clock*x;
        for(int i=1; i<10; i++){
          now = a*i*i;
          motor_cnt_reset();
          motor_update(now+300, now+300);
          while(1){
            PI_ctrl_30();
            //fv.PI_ctrl_s();
            if(WM_cnt(now_clock)) break;
          }
        }
      }

      if(z==0){
        //gosa = FOWARD_CLK - now_clock*x;
        for(int i=10; i>0; i--){
          now = a*i*i;
          motor_cnt_reset();
          motor_update(now+300, now+300);
          while(1){
            PI_ctrl_30();
            //fv.PI_ctrl_s();
            if(WM_cnt(now_clock)) break;
          }

        }
        motor_update(0, 0);
      }
      if(z==2){
        //gosa = FOWARD_CLK - now_clock*x;
        for(int i=1; i<5; i++){
          now = a*i*i;
          motor_cnt_reset();
          motor_update(now+300, now+300);
          while(1){
            PI_ctrl_30();
            //fv.PI_ctrl_s();
            if(WM_cnt(now_clock)) break;
          }
        }
        for(int i=5; i>0; i--){
          now = a*i*i;
          motor_cnt_reset();
          motor_update(now+300, now+300);
          while(1){
            PI_ctrl_30();
            //fv.PI_ctrl_s();
            if(WM_cnt(now_clock)) break;
          }
        }
        motor_update(0, 0);
      }

      /*fv.motor_cnt_reset();
      while(1){
        fv.PI_ctrl();
        fv.motor_update(now+200, now+200);
        if(fv.WM_cnt(gosa)) break;
      }*/
  }
};


#endif
