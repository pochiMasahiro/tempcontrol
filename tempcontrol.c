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


    /* Initial setting */
    GASCELL_CONTROL = etheropen(Ether_GasCell_TEMP, Prologix_Ether_PORT);
    VCSEL_CONTROL = etheropen(Ether_VCSEL_TEMP, Prologix_Ether_PORT);
    etherwrite(GASCELL_CONTROL, "++mode 1");
    etherwrite(GASCELL_CONTROL, "++auto 1");
    etherwrite(GASCELL_CONTROL, "++addr 1");
    etherwrite(GASCELL_CONTROL, "++eos 1");
    etherwrite(GASCELL_CONTROL, "++eot_enable 1");
    etherwrite(GASCELL_CONTROL, "++savecfg 0");
    etherwrite(VCSEL_CONTROL, "++mode 1");
    etherwrite(VCSEL_CONTROL, "++auto 1");
    etherwrite(VCSEL_CONTROL, "++addr 1");
    etherwrite(VCSEL_CONTROL, "++eos 1");
    etherwrite(VCSEL_CONTROL, "++eot_enable 1");
    etherwrite(VCSEL_CONTROL, "++savecfg 0");

    etherwrite(GASCELL_CONTROL, "++addr 1");
    etherwrite(GASCELL_CONTROL, ":MEAS:RES?");
    curr_gascell_temp = etherreadd(GASCELL_CONTROL);

    printf("Current gascell temp: %E\n", curr_gascell_temp);

    /*
    double svol, vol;

    etherwrite(PDC_E1,"SOI?");
    etherwrite(PDC_E1,"++read eoi");
    svol = etherreadd(PDC_E1);
    printf("%E\n", svol);
    */

    return 0;
}