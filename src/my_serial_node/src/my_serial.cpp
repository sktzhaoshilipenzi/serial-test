#include <ros/ros.h> 
#include <serial/serial.h> //ROS已经内置了的串口包 
#include <std_msgs/String.h> 
#include <std_msgs/Empty.h> 
#include <iostream>
#include <bitset>
using std::bitset;
unsigned char buffer[7] = { 0xFF,0x00,0x00,0x00,0x00,0x00,0xFE};
int control,num1=0,//1云台控制 0放弃控制
            num2=0,//0 底盘不跟随 1 底盘跟随
            num3=2,//0 不射 1慢射2中射3快射
            num4=0,//1 底盘控制 0放弃控制
            num5=0,//1 左旋 0右旋 
            num6=1,//1 a 0d
            num7=0;//1 w 0s
int main(int argc, char** argv)
{
    ros::init(argc, argv, "serial_port");
    //创建句柄（虽然后面没用到这个句柄，但如果不创建，运行时进程会出错）
    ros::NodeHandle nh;
    ros::Rate loop_rate(10);
   
    serial::Serial sp;
    //创建timeout
    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    //设置要打开的串口名称
    sp.setPort("/dev/ttyUSB0");
    //设置串口通信的波特率
    sp.setBaudrate(115200);
    //串口设置timeout
    sp.setTimeout(to);
 
    try
    {
        //打开串口
        sp.open();
    }
    catch(serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port.");
        return -1;
    }
    
    //判断串口是否打开成功
    //sp.open();
    if(sp.isOpen())
    {
        ROS_INFO_STREAM("/dev/ttyUSB0 is opened.");
    }
    else
    {   ROS_INFO_STREAM("can not open");
        //return -1;
    }
    
    //ros::Rate loop_rate(500);
    while(ros::ok())
    {   double send_data[]={1,1};
        //获取缓冲区内的字节数
       // size_t n = sp.available();
     //   printf("%d",n);
       // if(n!=0)
        short* data_ptr = (short *)(buffer + 2);
         data_ptr[0] = (short)send_data[0];    
         data_ptr[1] = (short)send_data[1];
         nh.param("num1", num1, 0);
         nh.param("num2", num2, 0);
         nh.param("num3", num3, 0);
         nh.param("num4", num4, 0);
         nh.param("num5", num5, 0);
         nh.param("num6", num6, 0);
         nh.param("num7", num7, 0);
         control=(num1<<7)+(num2<<6)+(num3<<4)+(num4<<3)+(num5<<2)+(num6<<1)+num7;
         buffer[1]=(unsigned char)control;
         //unsigned char buffer[1024];
            //读出数据
          //  n = sp.read(buffer, n);
           // ROS_INFO_STREAM("send_data...");
           std::cout << "send_data..."<<bitset<8>(control)<<std::endl;
           
           float Yaw = send_data[0];
           float Pitch = send_data[1];
             std::cout <<Yaw<<""<<Pitch<<"\n"<<std::endl;
                //16进制的方式打印到屏幕
          //      std::cout << std::hex << (buffer[i] & 0xff) << " ";
            
            
            //把数据发送回去
            sp.write(buffer, 7);
        
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    //关闭串口
    sp.close();
 
    return 0;
}


