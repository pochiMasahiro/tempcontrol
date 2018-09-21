#ifndef INCLUDED_ETHER_PROG_
#define INCLUDED_ETHER_PROG_
#include	"ether_prog.h"
#endif

int etheropen(char *ip, uint16_t port)
{
	int ethersocket;
	struct	sockaddr_in MyAddress;

        if((ethersocket = socket(PF_INET, SOCK_STREAM,0)) < 0) {
                perror("socket"); exit(1);
        }

	// 構造体全体をゼロに初期化
	memset((char*)&MyAddress,0,sizeof(struct sockaddr_in));
	// その後、個々のフィールドを設定
	MyAddress.sin_family=PF_INET; // IPv4
	MyAddress.sin_port=htons(port); // ほとんどの測定器が使用するポート番号
	//printf("MyAddress.sin_port = %d\n", MyAddress.sin_port);
	MyAddress.sin_addr.s_addr = inet_addr(ip); // IP アドレス
	//printf("MyAddress.sin_addr.s_addr = %d\n", inet_addr(ip));
	// TCP 接続を確立
	if(connect(ethersocket,(struct sockaddr *)&MyAddress, sizeof(struct sockaddr_in)) < 0) {
		perror("connect"); exit(1);
	}

	return ethersocket;

}
int etherwrite(int ether_address, char *comand)
{
	char comm[2048];
	char fchar[2];

	fchar[0] = '\n';
	fchar[1] = '\0';

	strcpy(comm, comand);
	strcat(comm, fchar);

	//printf("comm = %s", comm);

	//if(send(ether_address, comm,(long)strlen(comm)) < 0 ){
	//if(send(ether_address, comm,2,0) < 0 ){
	if(send(ether_address, comm, strlen(comm), 0) < 0 ){
		printf("Write Error!!\n");
		return 1;
	}
	
	return 0;
}

int etherread(int ether_address, char *comand)
{
	int	actual;
	char	readstr[2048];

	actual = recv(ether_address, readstr, 2048,0);
	readstr[actual] = 0;
	strcpy(comand,readstr);
	return 0;
}

int etherreadi(int ether_address)
{
	int	actual;
	int	data;
	char	readstr[2048];

	actual = recv(ether_address, readstr, 2048,0);
	readstr[actual] = 0;
	sscanf(readstr, "%d", &data);

	return data;
}

double etherreadd(int ether_address)
{
	int	actual;
	double	data;
	char	readstr[2048];
	char *p = readstr;

	actual = recv(ether_address, readstr, 2048,0);
	readstr[actual] = '\0';
	while(isalpha(*p)) p++;
	data = strtod(p, NULL);
	return data;
}

