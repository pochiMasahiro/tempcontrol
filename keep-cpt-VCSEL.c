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
    
    int		i, fnum, num_mesu=10, wait_sec=1.0, r_flag=1;
    int		flag_inc=0;
    int		gas = 0;
    double		speedup=1.0;
    unsigned char	comm[255], filename[100]; 
    long		l, loop_num=0, n=0;

    //int	mnum = 10;

	for(i=1;i<argc;i++){
        if(*argv[i]=='-' && strlen(argv[i])>1 ){
            switch(*(argv[i]+1)){
            case 'g':
                sscanf(argv[i],"-g%d",&gas);
            break;
            case 'u':
                sscanf(argv[i],"-u%lf",&speedup);
            break;
            case 'p':
                flag_inc=1;
            break;
            default :
                printf("No option : -%c\n",*(argv[i]+1));
                exit(1);
            }
        fnum++;
        }
    }
    
    switch(gas){
        case 0:
            LIA_E1 = etheropen(Ether_LIA_SR7270_C, Ether_LIA_SR7270_PORT);
            PDC_E1 = etheropen(Ether_PDC_ADC6156_C, Ether_PDC_ADC6156_PORT);
            etherwrite(PDC_E1, "++mode 1");
            etherwrite(PDC_E1, "++auto 0");
            etherwrite(PDC_E1, "++eos 1");
            etherwrite(PDC_E1, "++eot_enable 1");
            etherwrite(PDC_E1, "++addr 1");
            printf("#GAS is set as Cs\n");
        break;
                
        case 1:
            LIA_E1 = etheropen(Ether_LIA_SR7270_R, Ether_LIA_SR7270_PORT);
            PDC_E1 = etheropen(Ether_PDC_ADC6156_R, Ether_PDC_ADC6156_PORT);  
            etherwrite(PDC_E1, "++mode 1");
            etherwrite(PDC_E1, "++auto 0");
            etherwrite(PDC_E1, "++eos 1");
            etherwrite(PDC_E1, "++eot_enable 1");
            etherwrite(PDC_E1, "++addr 2");
            printf("#GAS is set as Rb\n");
        break;
	}

    double ample, phase, svol, vol;

    char *dat[2048];

    etherwrite(PDC_E1,"SOI?");
    etherwrite(PDC_E1,"++read eoi");
    svol = etherreadd(PDC_E1);
    printf("%E\n", svol);

       
    double target_vol;
    double dark_vgene_step, dark_vgene_vol, dark_vgene_vol2;
    double dark_permited_range, dark_status, dark_target_vol;

    dark_vgene_vol = svol;
    if(svol<-0.5e-3 || svol >4.55e-3) exit(1);
    vol = svol;
    dark_vgene_vol=svol;
    dark_vgene_vol2=svol;
    dark_permited_range = 1.0e-20;
    dark_status = 0;
    dark_target_vol = +00.00e-3;

    double	vol_gain = 1.00e2;
    double	delta_v;
    vol_gain *= speedup;
    /*
    while(1){
		etherwrite(LIA_E2,"X1.?");
		if(gas == 1) dark_target_vol = 0.00e-6;
		ample = etherreadd(LIA_E2);

		printf("#LIA_E2 X: %le\n",ample);

		shmparams->dark_meas1 = ample;
		shmparams->dark_meas2 = phase;
		shmparams->dark_meas3 = sqrt(ample*ample + phase*phase);
		if(ample > shmparams->dark_target_vol + target_vol +shmparams->dark_permited_range/2.0){
			if(flag_inc) delta_v = +shmparams->dark_vgene_step*fabs((ample-shmparams->dark_target_vol)*vol_gain);
			else         delta_v = -shmparams->dark_vgene_step*fabs((ample-shmparams->dark_target_vol)*vol_gain);
			shmparams->dark_status = 0;
		}
		else if(ample < shmparams->dark_target_vol + target_vol - shmparams->dark_permited_range/2.0){
			if(flag_inc) delta_v = -shmparams->dark_vgene_step*fabs((ample-shmparams->dark_target_vol)*vol_gain);
			else         delta_v = +shmparams->dark_vgene_step*fabs((ample-shmparams->dark_target_vol)*vol_gain);
			shmparams->dark_status = 0;
		}
		else  shmparams->dark_status = 1;

		printf("#delta_v = %lf\n", delta_v);

		if(USE_VOL){
			if(flag_limit)
			 	if(fabs(delta_v) > 0.080e-0) delta_v = 0.0;
			else
			 	if(fabs(delta_v) > 0.200e-3) delta_v = 0.0;
			vol += delta_v;
		}
		else{
			if(flag_limit)
			 	if(fabs(delta_v) > 0.080e-3) delta_v = 0.0;
			else
			 	if(fabs(delta_v) > 0.200e-6) delta_v = 0.0;
			vol += delta_v;
		}

		//if(vol > 0.150) vol = 0.150;
		//set_vol_y7651(PDC3, vol);
		//set_mamp_y7651(PDC3, vol);

		set_amp_adc6156(PDC3, vol);

		//set_vol_fixed_mv(Vgene, vol);
		if(gas == 0) shmparams->dark_vgene_vol  = vol;
		if(gas == 1) shmparams->dark_vgene_vol2 = vol;
		if(delta_v >1.0e-9) 
			printf("#X=%5.2lfe-3, Y=%5.2lfe-3, vol=%lf, status=%d +\n", ample*1.0e3, phase*1.0e3, vol*1e3, shmparams->dark_status);
		else if(delta_v <-1.0e-9) 
			printf("#X=%5.2lfe-3, Y=%5.2lfe-3, vol=%lf, status=%d -\n", ample*1.0e3, phase*1.0e3, vol*1e3, shmparams->dark_status);
		else 
			printf("#X=%5.2lfe-3, Y=%5.2lfe-3, vol=%lf, status=%d  \n", ample*1.0e3, phase*1.0e3, vol*1e3, shmparams->dark_status);
		//shmparams->dark_vgene_vol = vol;
		usleep(200000);
		
	}
    */
    return 0;
}  