#ifndef INCLUDED_STDIO_
#define INCLUDED_STDIO_
#include <stdio.h>
#endif

#ifndef INCLUDED_STDLIB_
#define INCLUDED_STDLIB_
#include <stdlib.h>
#endif

#ifndef INCLUDED_STRING_
#define INCLUDED_STRING_
#include <string.h>
#endif

#ifndef INCLUDED_CTYPE_
#define INCLUDED_CTYPE_
#include <ctype.h>
#endif

#ifndef INCLUDED_MATH_
#define INCLUDED_MATH_
#include <math.h>
#endif

#ifndef INCLUDED_FCNTL_
#define INCLUDED_FCNTL_
#include <fcntl.h>
#endif

#ifndef INCLUDED_TIME_
#define INCLUDED_TIME_
#include <time.h>
#endif

#ifndef INCLUDED_ERROR_
#define INCLUDED_ERROR_
#include <error.h>
#endif

#ifndef INCLUDED_ERRNO_
#define INCLUDED_ERRNO_
#include <errno.h>
#endif

#ifndef INCLUDED_NETINET_TCP_
#define INCLUDED_NETINET_TCP_
#include <netinet/tcp.h>
#endif

#ifndef INCLUDED_NETINET_IN_
#define INCLUDED_NETINET_IN_
#include <netinet/in.h>
#endif

#ifndef INCLUDED_SYS_TYPES_
#define INCLUDED_SYS_TYPES_
#include <sys/types.h>
#endif

#ifndef INCLUDED_SYS_SOCKET_
#define INCLUDED_SYS_SOCKET_
#include <sys/socket.h>
#endif

#ifndef INCLUDED_ARPA_INET_
#define INCLUDED_ARPA_INET_
#include <arpa/inet.h>
#endif

#define YES    1
#define NO     0
#define TRUE 	1
#define FALSE 	0

#define DEBUG    NO

#define GPWAIT 1

int DMM_E1;
int DMM_E2;
int LIA_E1;
int LIA_E2;
int RF_E1;
int RF_E2;
int PDC_E1;
int PDC_E2;

#define Ether_DMM_VOAC7602_C   "10.0.251.155"
#define Ether_DMM_VOAC7602_R   "10.0.251.156"
#define Ether_DMM_VOAC7602_PORT  	5198

#define Ether_LIA_SR7270_C   "10.0.251.152"
#define Ether_LIA_SR7270_R   "10.0.251.153"
#define Ether_LIA_SR7270_PORT  50000

#define Ether_RF_N5172B_C    "10.0.251.150"
#define Ether_RF_N5172B_PORT    5025

#define Ether_RF_E8257D_R    "10.0.251.151"
#define Ether_RF_E8257D_PORT    5025

#define Ether_PDC_ADC6156_C      "10.0.251.103"
#define Ether_PDC_ADC6156_R      "10.0.251.104"
#define Ether_PDC_ADC6156_PORT   1234

#define Ether_GasCell_TEMP "10.0.251.101"
#define Ether_VCSEL_TEMP "10.0.251.105"
#define Prologix_Ether_PORT 1234


int etheropen(char *ip, uint16_t port);
int etherwrite(int, char*);
int etherread(int, char*);
int etherreadi(int);
double etherreadd(int);
double RF_read_freq();
double RF_set_freq();
double RF_read_power();
double RF_set_power();
double RF_read_FMdev();
double RF_read_FMrate();
