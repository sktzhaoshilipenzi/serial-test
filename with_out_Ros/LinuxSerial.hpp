#pragma once

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

#include <chrono>
#define speed_test_begin() speed_test_start_begin_time = std::chrono::steady_clock::now()
#define speed_test_end(info, unit) std::cout << info << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - speed_test_start_begin_time).count()  << " " << unit << std::endl;
#define speed_test_reset() speed_test_start_begin_time = std::chrono::steady_clock::now()

class CLinuxSerial
{
public:
    CLinuxSerial();
    CLinuxSerial(UINT portNo = 0 , UINT baudRate = 115200 );
    ~CLinuxSerial();

    bool InitPort(UINT portNo = 0, UINT baudRate = 115200);
    UINT ReadData(UCHAR *data, UINT length);
    UINT WriteData(UCHAR *data, UINT length);
    UINT GetBytesInCom();

private:
    int m_iSerialID; 
    bool OpenPort(UINT portNo);
    void ClosePort();
};

