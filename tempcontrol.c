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

#ifndef INCLUDED_TIME_
#define INCLUDED_TIME_
#include <time.h>
#endif

#ifndef INCLUDED_MATH_
#define INCLUDED_MATH_
#include <math.h>
#endif

void exit_handler(int);
volatile int exit_flag = 0;

void exit_handler(int signo)
{
  exit_flag = 1;
}

int prologix_initialize(int sock, int addr);

double get_resi_keithley2000(int sock, int addr);
int set_current_y7651(int sock, int addr, double value);

#define B_param 3930.0
#define R0 10.0e3
#define T0 25.0
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

int set_current_y7651(int sock, int addr, double value)
{
  char ad[10];
  char comm[256];
  sprintf(ad, "++addr %d", addr);
  etherwrite(sock, ad);

  sprintf(comm, "S+%e", value);
  etherwrite(sock, ad);
  etherwrite(sock, "E");

  return 0;
}

int main(int argc, char *argv[]){
  int	i;
  double target_gascelltemp = 80.0, target_vcseltemp = 70.0;
  double curr_gascell_temp, curr_vcseltemp;
  double err_gascell, err_vcsel;
  double norm_gascell, norm_vcsel;
  double sum_gascell = 0.0, sum_vcsel = 0.0;
  double control_gascell, control_vcsel;
  double p_vcsel = 7.0, i_vcsel = 0.01;
  double p_gascell = 10.0, i_gascell = 0.01; 
  int GASCELL_CONTROL, VCSEL_CONTROL;

  /* nano sec */
  long t_start, t_elapsed;
  long t_interval = 1000000000;
  struct timespec ts;
  struct timespec wait;

  for(i=1;i<argc;i++){
    if(*argv[i]=='-' && strlen(argv[i])>1 ){
      switch(*(argv[i]+1))
      {
        case 'v':
          sscanf(argv[i], "-v%lf", &target_vcseltemp);
          break;
        case 'g':
          sscanf(argv[i], "-g%lf", &target_gascelltemp);
          break;
        default :
          printf("Undefined parameter.\n");
      }
    }
  }

  printf("VCSEL target temperature: %lf.\n", target_vcseltemp);
  printf("Gas Cell target temperature: %lf.\n", target_gascelltemp);


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

  /*
  etherwrite(GASCELL_CONTROL, "++addr 1");
  etherwrite(GASCELL_CONTROL, ":MEAS:RES?");
  curr_gascell_temp = etherreadd(GASCELL_CONTROL);

  printf("Current gascell temp: %E\n", curr_gascell_temp);
  */

  while(exit_flag == 0)
  {
    clock_gettime(CLOCK_MONOTONIC, &ts);
    t_start = ts.tv_nsec;
    
    /* Get current temp */
    curr_gascell_temp = thermistor_conversion(get_resi_keithley2000(GASCELL_CONTROL, 1));
    printf("Current gas cell temperature: %lf\n", curr_gascell_temp);
    curr_vcseltemp = thermistor_conversion(get_resi_keithley2000(VCSEL_CONTROL, 5));
    printf("Current VCSEL temperature: %lf\n", curr_vcseltemp);

    /* Error */
    err_gascell = target_gascelltemp - curr_gascell_temp;
    err_vcsel = target_vcseltemp - curr_vcseltemp;

    /* Normalize */
    norm_gascell = err_gascell / (90 - 20);
    norm_vcsel = err_vcsel / (90 - 20);

    /* Integral */
    sum_gascell += norm_gascell;
    sum_vcsel += norm_vcsel;

    /* PI control */
    control_gascell = norm_gascell * p_gascell + sum_gascell * i_gascell;
    control_vcsel = norm_vcsel * p_vcsel + sum_vcsel + i_vcsel;

    /* -0.5 - 0.5 to 0 - 1.0 */
    if (control_gascell > 0.5) control_gascell = 0.5;
    else if (control_gascell < -0.5) control_gascell = -0.5;
    control_gascell += 1.0;

    if (control_vcsel > 0.5) control_vcsel = 0.5;
    else if(control_vcsel < -0.5) control_vcsel = -0.5;
    control_vcsel += 1.0;

    set_current_y7651(GASCELL_CONTROL, 2, control_gascell);
    set_current_y7651(VCSEL_CONTROL, 6, control_vcsel);

    clock_gettime(CLOCK_MONOTONIC, &ts);
    t_elapsed = ts.tv_nsec - t_start;
    if (t_interval - t_elapsed < 0)
    {
      printf("Interval too slow!!\n");
    }
    else
    {
      wait.tv_nsec = t_interval - t_elapsed;
      nanosleep(&wait, NULL);
    }
  }

  return 0;
}
