#include <unistd.h>
#include <termio.h>
#include <bitset>
#include <thread> 
#include <unistd.h>
#include "opencv2/opencv.hpp"
#include <iostream>
#include "LinuxSerial.hpp"
#include <chrono>
 #include <stdio.h>  
 #include <termios.h>  
 
using std::bitset;
std::chrono::steady_clock::time_point speed_test_start_begin_time;
CLinuxSerial serial(0,115200);
int control,num1=1,//1云台控制 0放弃控制
            num2=0,//0 底盘不跟随 1 底盘跟随
            num3=0,//0 不射 1慢射2中射3快射
            num4=0,//1 底盘控制 0放弃控制
            num5=0,//1 左旋 0右旋 
            num6=0,//1 a 0d
            num7=0,//1 w 0s
            i=0;
char xe,ex;
 char get1char(void)  
{  
  
#ifdef _WIN32  
        // Do nothing  
#else   // 保存并修改终端参数  
    struct termios stored_settings;  
    struct termios new_settings;  
    tcgetattr (0, &stored_settings);  
    new_settings = stored_settings;  
    new_settings.c_lflag &= (~ICANON);  
    new_settings.c_cc[VTIME] = 0;  
    new_settings.c_cc[VMIN] = 1;  
    tcsetattr (0, TCSANOW, &new_settings);  
#endif  
  
    int ret = 0;  
    char c;  
  
#ifdef _WIN32  
    c = getch();  
#else  
    c = getchar();  
    putchar('\b'); // 删除回显  
#endif  
  
    printf("input:  [%c]\n", c);  
  
#ifdef _WIN32  
    // Do nothing  
#else  
    tcsetattr (0, TCSANOW, &stored_settings); // 恢复终端参数  
#endif  
  
    return c;   
}  

/*     
void listen()
{
    while(1)
    {
        uchar data[] = {0xDA,0x00,0x00,0x00,0x00,0xDB};
        serial.ReadData(data,6);
        short Yaw = (data[1]<<8) + data[2];
        short Pitch = (data[3]<<8) + data[4];
        std::cout << "\tYaw: "<< Yaw/100.0 << "\tPitch: "<< Pitch/100.0 << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
} 
*/
void listen()//规律变化发送的指令
{
    while(1)
    {
      i++;
      printf("%d\n",i);
        if(i%1000==0)
        {
            if(num2==0)
              num2=1;
             else
             
              num2=0;
        
        
        }
        if(i%1000==0)
        {
            if(num5==0)
                 
            num5=num6=num7=1;
                  
            else 
            num5=num6=num7=0;
        }
               
        if(i%2000==0)
        {  if(num4==0)
            num4=1;
            else
            num4=0;
        }
    std::this_thread::sleep_for(std::chrono::milliseconds(20));}
}
void publish()
{
    while(1)
    {   
        speed_test_begin();
        unsigned char send_bytes[] = { 0xFF,0x00,0x00,0x00,0x00,0x00,0xFE};
        double send_data[]={1,1};
        /*if(start==0){*/
    //    if
    //    (ex=get1char()=='1')
    //   num1=1 ;
       //start ++;
       //}
      //  xe=ex;
       // printf("初始化");start++;}
       // if (ex=='q')
        //printf("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq");
        short* data_ptr = (short *)(send_bytes + 2);
        if(num1=1)
        {
           if(num2==0)
              {printf("底盘不跟随云台");
            }
           else
              {
            printf("底盘跟随云台");
            }
        
        }
        if(num4==1)
        {
            if(num5==0)
            printf("底盘左旋");
            else
            printf("底盘右旋");
            if(num6==0)
            printf("右平移");
            else
            printf("左平移");
            if(num7==0)
            printf("向后走");
            else
            printf("向前走");
            
            
            
            
        }
          //  [0] = (short)send_data[0];  
        data_ptr[0] = (short)send_data[0];    
        data_ptr[1] = (short)send_data[1];  
        control=(num1<<7)+(num2<<6)+(num3<<4)+(num4<<3)+(num5<<2)+(num6<<1)+num7;
      float  yaw=send_data[0];//((send_bytes[2]<<8)+send_bytes[3])/100;
      float  pitch = send_data[1];
       
        send_bytes[1]=(unsigned char)control;
        //bitset<10>(control)
        serial.WriteData(send_bytes,7);
        std::cout << "\tsend_data..."<<bitset<8>(control)<<std::endl;
        std::cout <<"yaw"<<yaw<<"pitch"<<pitch<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        speed_test_end("time:","ms");
    }
}

int main(int argc, char * argv[]){

//system("stty raw");
    std::thread task1(&publish);
    std::thread task2(&listen);
 //if char get1char(void)  
  
  

 //ex=get1char();
     //  if (ex=='1')
    // {  num1=1 ;printf("%d",num1);};
    task1.join();
    task2.join();
//publish();
//system("stty cooked");  
	return EXIT_SUCCESS;
   
}

