#ifndef _INC_AuthAPI2
#define _INC_AuthAPI2

// 函数原型


#define ADAPTER_GET_FAILED 2
#define INTERNET_FAILED 2
#define VALIDITY_FAILED 1
#define ADAPTER_MATCH_FAILED 1

//利用时间授权
int InitTime();
//利用时间授权
int InitTime2();

//利用网卡信息授权
int InitAdapter();

//利用时间授权
int InitTime3();

void ntpdate();

#endif //!_INC_AuthAPI2