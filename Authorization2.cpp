﻿#include "Authorization2.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <iostream>

//利用时间授权
int InitTime()
{
}
//利用时间授权
int InitTime2()
{
}
//利用网卡信息授权
int InitAdapter()
{
}

//利用时间授权
int InitTime3()
{
//char *hostname=(char *)"163.117.202.33";
//char *hostname=(char *)"pool.ntp.br";
// 这里使用的时间服务器是129.132.2.21，更多地址请参考
//servAddr.sin_addr.S_un.S_addr = inet_addr("202.120.2.101");
char    *hostname=(char *)"200.20.186.76";
int portno=123;     //NTP is port 123
int maxlen=1024;        //check our buffers
int i;          // misc var i
unsigned char msg[48]={010,0,0,0,0,0,0,0,0};    // the packet we send
unsigned long  buf[maxlen]; // the buffer we get back
//struct in_addr ipaddr;        //  
struct protoent *proto;     //
struct sockaddr_in server_addr;
int s;  // socket
long tmit;   // the time -- This is a time_t sort of
long beginAuthTime=1558998662;//2019-05-28 07:11:02
long validityIn30Days=3592000;//30 days
//long beginAuthTime=1558987200;//2019-05-28 07:11:02
//std::cout << "init time is " << ctime(&beginAuthTime)  << std::endl;
//long validityIn30Days=360000;//30 days

//use Socket;
//
//#we use the system call to open a UDP socket
//socket(SOCKET, PF_INET, SOCK_DGRAM, getprotobyname("udp")) or die "socket: $!";
proto=getprotobyname("udp");
s=socket(PF_INET, SOCK_DGRAM, proto->p_proto);
if(s<=0)
{
	//perror("socket");
	return INTERNET_FAILED;
}
//
//#convert hostname to ipaddress if needed
//$ipaddr   = inet_aton($HOSTNAME);
memset( &server_addr, 0, sizeof( server_addr ));
server_addr.sin_family=AF_INET;
server_addr.sin_addr.s_addr = inet_addr(hostname);
//argv[1] );
//i   = inet_aton(hostname,&server_addr.sin_addr);
server_addr.sin_port=htons(portno);
//printf("ipaddr (in hex): %x\n",server_addr.sin_addr);

/*
 * build a message.  Our message is all zeros except for a one in the
 * protocol version field
 * msg[] in binary is 00 001 000 00000000 
 * it should be a total of 48 bytes long
*/

// send the data
//printf("sending data..\n");//xszhu: it is unnecessary
i=sendto(s,msg,sizeof(msg),0,(struct sockaddr *)&server_addr,sizeof(server_addr));
if(i<=0)
{
	//perror("sendto");
	return INTERNET_FAILED;
}

// get the data back
struct sockaddr saddr;
socklen_t saddr_l = sizeof (saddr);
i=recvfrom(s,buf,48,0,&saddr,&saddr_l);
if(i<=0)
{
	//perror("recvfr:");
	return INTERNET_FAILED;
}
//We get 12 long words back in Network order
/*
for(i=0;i<12;i++) {
    //printf("%d\t%-8x\n",i,ntohl(buf[i]));
    long tmit2=ntohl((time_t)buf[i]);
    std::cout << "Round number " << i << " time is " << ctime(&tmit2)  << std::endl;
}
*/
/*
 * The high word of transmit time is the 10th word we get back
 * tmit is the time in seconds not accounting for network delays which
 * should be way less than a second if this is a local NTP server
 */

//tmit=ntohl((time_t)buf[10]);    //# get transmit time
tmit=ntohl((time_t)buf[4]);    //# get transmit time
//printf("tmit=%d\n",tmit);

/*
 * Convert time to unix standard time NTP is number of seconds since 0000
 * UT on 1 January 1900 unix time is seconds since 0000 UT on 1 January
 * 1970 There has been a trend to add a 2 leap seconds every 3 years.
 * Leap seconds are only an issue the last second of the month in June and
 * December if you don't try to set the clock then it can be ignored but
 * this is importaint to people who coordinate times with GPS clock sources.
 */

//tmit-= 2208988800U;
//tmit-= 2209017600U;
tmit-= 2208988800U;
//printf("tmit=%d\n",tmit);
/* use unix library function to show me the local time (it takes care
 * of timezone issues for both north and south of the equator and places
 * that do Summer time/ Daylight savings time.
 */

//#compare to system time
//printf("Time: %s",ctime(&tmit));


std::cout << "time is " << ctime(&tmit)  << std::endl;
/*
i=time(0);
//printf("%d-%d=%d\n",i,tmit,i-tmit);
//printf("System time is %d seconds off\n",(i-tmit));
std::cout << "System time is " << (i-tmit) << " seconds off" << std::endl;*/

if(tmit-beginAuthTime>validityIn30Days)
{
//已过有效期
return VALIDITY_FAILED;
}
return 0;
}

void ntpdate() {
//char *hostname=(char *)"163.117.202.33";
//char *hostname=(char *)"pool.ntp.br";
// 这里使用的时间服务器是129.132.2.21，更多地址请参考
//servAddr.sin_addr.S_un.S_addr = inet_addr("202.120.2.101");
char    *hostname=(char *)"200.20.186.76";
int portno=123;     //NTP is port 123
int maxlen=1024;        //check our buffers
int i;          // misc var i
unsigned char msg[48]={010,0,0,0,0,0,0,0,0};    // the packet we send
unsigned long  buf[maxlen]; // the buffer we get back
//struct in_addr ipaddr;        //  
struct protoent *proto;     //
struct sockaddr_in server_addr;
int s;  // socket
long tmit;   // the time -- This is a time_t sort of

//use Socket;
//
//#we use the system call to open a UDP socket
//socket(SOCKET, PF_INET, SOCK_DGRAM, getprotobyname("udp")) or die "socket: $!";
proto=getprotobyname("udp");
s=socket(PF_INET, SOCK_DGRAM, proto->p_proto);
perror("socket");
//
//#convert hostname to ipaddress if needed
//$ipaddr   = inet_aton($HOSTNAME);
memset( &server_addr, 0, sizeof( server_addr ));
server_addr.sin_family=AF_INET;
server_addr.sin_addr.s_addr = inet_addr(hostname);
//argv[1] );
//i   = inet_aton(hostname,&server_addr.sin_addr);
server_addr.sin_port=htons(portno);
//printf("ipaddr (in hex): %x\n",server_addr.sin_addr);

/*
 * build a message.  Our message is all zeros except for a one in the
 * protocol version field
 * msg[] in binary is 00 001 000 00000000 
 * it should be a total of 48 bytes long
*/

// send the data
//printf("sending data..\n");//xszhu: it is unnecessary
i=sendto(s,msg,sizeof(msg),0,(struct sockaddr *)&server_addr,sizeof(server_addr));
perror("sendto");
// get the data back
struct sockaddr saddr;
socklen_t saddr_l = sizeof (saddr);
i=recvfrom(s,buf,48,0,&saddr,&saddr_l);
perror("recvfr:");

//We get 12 long words back in Network order
/*
for(i=0;i<12;i++) {
    //printf("%d\t%-8x\n",i,ntohl(buf[i]));
    long tmit2=ntohl((time_t)buf[i]);
    std::cout << "Round number " << i << " time is " << ctime(&tmit2)  << std::endl;
}
*/
/*
 * The high word of transmit time is the 10th word we get back
 * tmit is the time in seconds not accounting for network delays which
 * should be way less than a second if this is a local NTP server
 */

//tmit=ntohl((time_t)buf[10]);    //# get transmit time
tmit=ntohl((time_t)buf[4]);    //# get transmit time
//printf("tmit=%d\n",tmit);

/*
 * Convert time to unix standard time NTP is number of seconds since 0000
 * UT on 1 January 1900 unix time is seconds since 0000 UT on 1 January
 * 1970 There has been a trend to add a 2 leap seconds every 3 years.
 * Leap seconds are only an issue the last second of the month in June and
 * December if you don't try to set the clock then it can be ignored but
 * this is importaint to people who coordinate times with GPS clock sources.
 */

//tmit-= 2208988800U;
tmit-= 2209017600U;
//printf("tmit=%d\n",tmit);
/* use unix library function to show me the local time (it takes care
 * of timezone issues for both north and south of the equator and places
 * that do Summer time/ Daylight savings time.
 */

//#compare to system time
//printf("Time: %s",ctime(&tmit));

std::cout << "time is " << ctime(&tmit)  << std::endl;
i=time(0);
//printf("%d-%d=%d\n",i,tmit,i-tmit);
//printf("System time is %d seconds off\n",(i-tmit));
std::cout << "System time is " << (i-tmit) << " seconds off" << std::endl;
}

