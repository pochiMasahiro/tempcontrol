#ifndef INCLUDED_STRING_H_
#define INCLUDED_STRING_H_
#include <string.h>
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

#ifndef INCLUDED_ERRNO_
#define INCLUDED_ERRNO_
#include <errno.h>
#endif

#ifndef INCLUDED_ETHER_PROG_
#define INCLUDED_ETHER_PROG_
#include "ether_prog.h"
#endif

#ifndef INCLUDED_SIGNAL_
#define INCLUDED_SIGNAL_
#include <signal.h>
#endif

void exit_handler(int);
volatile int exit_flag = 0;

void exit_handler(int signo)
{
  exit_flag = 1;
}

int prologix_initialize(int sock, int addr);

double get_resi_keithley2000(int sock, int addr);

#define B_param 3930.0
#define R0 10.0e3
#define T0
double thermistor_conversion(double);

int prologix_initialize(int sock, int addr)
{
  char ad[16];
  sprintf(ad, "++addr %d", addr);

  etherwrite(sock, "++mode 1");
  etherwrite(sock, "++auto 1");
  etherwrite(sock, ad);
  etherwrite(sock, "++eos 1");
  etherwrite(sock, "++eot_enable 1");
  etherwrite(sock, "++savecfg 0");
  return 0;
}

double thermistor_conversion(double resistance)
{
  double temp;
  temp = 1/B_param * log(resistance/R0) + 1/T0;

  return 1 / temp;
}

double get_resi_keithley2000(int sock, int addr)
{
  char ad[16];
  double data;

  sprintf(ad, "++addr %d", addr);
  etherwrite(sock, ad);
  etherwrite(sock, ":MEAS:RES?");
  data = etherreadd(sock);

  return data;
}

int main(int argc, char *argv[]){
  int	i;
  double gascelltemp = 80.0, vcseltemp = 70.0;
  double curr_gascell_temp, curr_vcseltemp;
  int GASCELL_CONTROL, VCSEL_CONTROL;

  for(i=1;i<argc;i++){
    if(*argv[i]=='-' && strlen(argv[i])>1 ){
      switch(*(argv[i]+1))
      {
        case 'v':
          sscanf(argv[i], "-v%lf", &vcseltemp);
          break;
        case 'g':
          sscanf(argv[i], "-g%lf", &gascelltemp);
          break;
        default :
          printf("Undefined parameter.\n");
      }
    }
  }

  printf("VCSEL target temperature: %lf.\n", vcseltemp);
  printf("Gas Cell target temperature: %lf.\n", gascelltemp);


  /* Initialize setting  */
  GASCELL_CONTROL = etheropen(Ether_GasCell_TEMP, Prologix_Ether_PORT);
  VCSEL_CONTROL = etheropen(Ether_VCSEL_TEMP, Prologix_Ether_PORT);
  if (prologix_initialize(GASCELL_CONTROL, 1) != 0)
  {
    printf("Initialize error");
    exit(1);
  }

  if (prologix_initialize(VCSEL_CONTROL, 1) != 0)
  {
    printf("Initialize error");
    exit(1);
  }

  etherwrite(GASCELL_CONTROL, "++addr 1");
  etherwrite(GASCELL_CONTROL, ":MEAS:RES?");
  curr_gascell_temp = etherreadd(GASCELL_CONTROL);

  printf("Current gascell temp: %E\n", curr_gascell_temp);

  while(

  return 0;
}
