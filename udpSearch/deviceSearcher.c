#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>


#define DEST_PORT 	(1500)
#define LOCAL_PORT 	(1500)
#define MAX_BUFFER_LEN	(512)

#define MAX_IPSTRING_LEN	(16)
#define MAX_MACSTRING_LEN	(18)
#define SEARCH_REPLY_LEN	(35)
#define CONF_PASSWORD_LEN	(7)	

#define DEFAULT_CONF_PASSWORD ("110415")

typedef struct _search_result_raw
{
	unsigned char mac[6];
	unsigned char version;
	unsigned char dest_ip[4];
	unsigned char dest_port[2];
	unsigned char local_ip[4];
	unsigned char local_port[2];
	unsigned char gateway[4];
	unsigned char work_mode;	//1. TcpClient;  0.	UDP; 2.	UDPServer;	3.	TcpServer 
	unsigned char bit_rate[3];	
	unsigned char com_flag;		//03
	unsigned char id[3];		
	unsigned char mask_ip[4];
}SEARCH_RESULT_RAW;

typedef struct _search_result_t
{
	char mac[MAX_MACSTRING_LEN];
	char version[4];
	char dest_ip[MAX_IPSTRING_LEN];
	char local_ip[MAX_IPSTRING_LEN];
	char gateway[MAX_IPSTRING_LEN];
	char mask_ip[MAX_IPSTRING_LEN];
	unsigned short dest_port;
	unsigned short local_port;
	unsigned short work_mode;
	unsigned int bit_rate;
	unsigned short com_flag;
	unsigned int id;
	struct _search_result_t * next_result;
}SEARCH_RESULT_T;


typedef struct _device_conf_param_raw
{
	unsigned char mac[6];
	unsigned char password[6];	//万能密码：110415

	/*-- configure param  --*/
	unsigned char dest_ip[4];
	unsigned char dest_port[2];
	unsigned char local_ip[4];
	unsigned char local_port[2];
	unsigned char gateway[4];
	unsigned char work_mode;	//1. TcpClient;  0.	UDP; 2.	UDPServer;	3.	TcpServer 
	unsigned char bit_rate[3];	
	unsigned char com_flag;		//03
	/*--*/

	unsigned char id[3];
	unsigned char mask_ip[4];
}DEVICE_CONF_PARAM_RAW;

typedef struct _device_conf_param_t
{
	char mac[MAX_MACSTRING_LEN];
	char password[CONF_PASSWORD_LEN];	//110415
	char dest_ip[MAX_IPSTRING_LEN];
	char local_ip[MAX_IPSTRING_LEN];
	char gateway[MAX_IPSTRING_LEN];
	char mask_ip[MAX_IPSTRING_LEN];
	unsigned short dest_port;
	unsigned short local_port;
	unsigned short work_mode;
	unsigned int bit_rate;
	unsigned short com_flag;
	unsigned int id;
}DEVICE_CONF_PARAM_T;

void convert_bytes_to_ip(const unsigned char* ip_bytes, unsigned char* ip_string, int ip_string_len)
{
	assert(ip_string);
	assert(ip_string_len >= MAX_IPSTRING_LEN);
	sprintf((char*)ip_string, "%d.%d.%d.%d", ip_bytes[3], ip_bytes[2], ip_bytes[1], ip_bytes[0]);
}

void convert_ip_to_bytes(const unsigned char* ip_string, unsigned char* ip_bytes, int ip_bytes_len)
{
	assert(ip_bytes);
	assert(ip_bytes_len == 4);
	int ip_temp[4];

	sscanf((char*)ip_string, "%d.%d.%d.%d", &ip_temp[3], &ip_temp[2], &ip_temp[1], &ip_temp[0]);

	for(int i = 0; i < ip_bytes_len; i++)
	{
		ip_bytes[i] = (unsigned char)ip_temp[i];
	}
}

void display_search_result(const SEARCH_RESULT_T *result)
{
	if(result->next_result)
	{
		display_search_result(result->next_result);
	}

	printf("========search result start=========\n");
	printf("mac: \t%s\n", result->mac);
	printf("version: \t%s\n",result->version);
	printf("dest_ip: \t%s\n", result->dest_ip);
	printf("local_ip: \t%s\n", result->local_ip);
	printf("gateway: \t%s\n", result->gateway);
	printf("mask_ip: \t%s\n", result->mask_ip);
	printf("local port: \t%d\n", result->local_port);
	printf("dest port: \t%d\n", result->dest_port);
	printf("work mode: \t%d\n", result->work_mode);
	printf("bit rate: \t%d\n", result->bit_rate);
	printf("com flag: \t%d\n", result->com_flag);
	printf("id: \t%d\n", result->id);
	printf("========search result end =========\n");

}

void display_configure_param(const DEVICE_CONF_PARAM_RAW *raw_data)
{
	printf("---------------------configure------------------\n");
	printf("mac：\t");
	for(int i = 0; i < 6; i++)
	{
		printf("%02X ", raw_data->mac[i]);
	}
	printf("\n");

	printf("password：\t");
	for(int i = 0; i < 6; i++)
	{
		printf("%02X ", raw_data->password[i]);
	}
	printf("\n");

	printf("dest-ip: \t");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X ", raw_data->dest_ip[i]);
	}
	printf("\n");

	printf("dest-port: \t");
	for(int i = 0; i < 2; i++)
	{
		printf("%02X ", raw_data->dest_port[i]);
	}
	printf("\n");

	printf("local-ip: \t");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X ", raw_data->local_ip[i]);
	}
	printf("\n");

	printf("local-port: \t");
	for(int i = 0; i < 2; i++)
	{
		printf("%02X ", raw_data->local_port[i]);
	}
	printf("\n");

	printf("mask-ip: \t");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X ", raw_data->mask_ip[i]);
	}
	printf("\n");

	printf("gateway-ip: \t");
	for(int i = 0; i < 4; i++)
	{
		printf("%02X ", raw_data->gateway[i]);
	}
	printf("\n");


	printf("work_mode: \t%02X \n", raw_data->work_mode);

	printf("bit_rate: \t");
	for(int i = 0; i < 3; i++)
	{
		printf("%02X ", raw_data->bit_rate[i]);
	}
	printf("\n");

	printf("com_flag: \t%02X \n", raw_data->com_flag);

	printf("id: \t");
	for(int i = 0; i < 3; i++)
	{
		printf("%02X ", raw_data->id[i]);
	}
	printf("\n");
	printf("---------------------end------------------\n");
}

void convert_search_raw_result(const SEARCH_RESULT_RAW *raw, SEARCH_RESULT_T *result)
{
	// convert mac
	sprintf(result->mac, "%02X-%02X-%02X-%02X-%02X-%02X",
		raw->mac[0],
		raw->mac[1],
		raw->mac[2],
		raw->mac[3],
		raw->mac[4],
		raw->mac[5]);

	// convert version
	sprintf(result->version, "%d.%d", raw->version / 16, raw->version % 16);

	// convert ip
	convert_bytes_to_ip(raw->dest_ip, (unsigned char*)result->dest_ip, MAX_IPSTRING_LEN);
	convert_bytes_to_ip(raw->local_ip, (unsigned char*)result->local_ip, MAX_IPSTRING_LEN);
	convert_bytes_to_ip(raw->gateway, (unsigned char*)result->gateway, MAX_IPSTRING_LEN);
	convert_bytes_to_ip(raw->mask_ip, (unsigned char*)result->mask_ip, MAX_IPSTRING_LEN);

	// convert port
	result->local_port = raw->local_port[0] + (raw->local_port[1]<<8);
	result->dest_port = raw->dest_port[0] + (raw->dest_port[1]<<8);

	// convert work_mode
	result->work_mode = raw->work_mode;

	// convert bitrate
	result->bit_rate = raw->bit_rate[0] + (raw->bit_rate[1]<<8) + (raw->bit_rate[2]<<16);

	// convert com flag
	result->com_flag = raw->com_flag;

	// convert id
	result->id = raw->id[0] + (raw->id[1]<<8) + (raw->id[2]<<16);

}

void convert_conf_param(const DEVICE_CONF_PARAM_T *param, DEVICE_CONF_PARAM_RAW*raw)
{
	// convert mac
	int mac_temp[6];
	sscanf(param->mac, "%02X-%02X-%02X-%02X-%02X-%02X",
		&mac_temp[0], &mac_temp[1], &mac_temp[2], &mac_temp[3], &mac_temp[4], &mac_temp[5]);

	for(int i = 0; i < 6; i++)
	{
		raw->mac[i] = (unsigned char)mac_temp[i];
	}

	// convert password
	sscanf(param->password, "%c%c%c%c%c%c",
		&raw->password[0], &raw->password[1], &raw->password[2], &raw->password[3], &raw->password[4], &raw->password[5]);


	// convert ip
	convert_ip_to_bytes((unsigned char*)param->dest_ip, raw->dest_ip, 4);
	convert_ip_to_bytes((unsigned char*)param->local_ip, raw->local_ip, 4);
	convert_ip_to_bytes((unsigned char*)param->gateway, raw->gateway, 4);
	convert_ip_to_bytes((unsigned char*)param->mask_ip, raw->mask_ip, 4);

	// convert port
	raw->local_port[0] = (unsigned char)param->local_port;
	raw->local_port[1] = (unsigned char)(param->local_port >> 8);

	raw->dest_port[0] = (unsigned char)param->dest_port;
	raw->dest_port[1] = (unsigned char)(param->dest_port >> 8);

	// convert work_mode
	raw->work_mode = (unsigned char)param->work_mode;

	// convert bitrate
	//param->bit_rate = raw->bit_rate[0] + (raw->bit_rate[1]<<8) + (raw->bit_rate[2]<<16);
	raw->bit_rate[0] = (unsigned char)param->bit_rate;
	raw->bit_rate[1] = (unsigned char)(param->bit_rate >> 8);
	raw->bit_rate[2] = (unsigned char)(param->bit_rate >> 16);

	// convert com flag
	raw->com_flag = (unsigned char)param->com_flag;

	// convert id
	raw->id[0] = (unsigned char)param->id;
	raw->id[1] = (unsigned char)(param->id >> 8);
	raw->id[2] = (unsigned char)(param->id >> 16);
}

int send_boardcast_cmd(int sock_fd, const struct sockaddr* paddr_dest, const unsigned char* cmd, int cmd_length)
{
	int ret = -1;
	ret=sendto(sock_fd, cmd, cmd_length, 0, paddr_dest, sizeof(struct sockaddr));

	return ret;
}

int recv_reply(int sock_fd, struct sockaddr* paddr_dest, unsigned char * recv_buf)
{
	int ret = 0;
	ret = recvfrom(sock_fd, recv_buf, MAX_BUFFER_LEN, 0, paddr_dest, (socklen_t*)&ret);

	return ret;
}

int parse_search_result(const unsigned char *buf, int buf_len, SEARCH_RESULT_T **ppresult)
{
	//assert(*ppresult);
	assert(buf_len % SEARCH_REPLY_LEN == 0 && buf_len != 0);

	int ret = 0;

////
//待修改20160317
////
	//SEARCH_RESULT_T* first_result = (SEARCH_RESULT_T*)malloc(sizeof(SEARCH_RESULT_T));

	for(int i = 0; i < buf_len / SEARCH_REPLY_LEN; i++)
	{
		SEARCH_RESULT_RAW reply = {0};
		memcpy(&reply, buf + SEARCH_REPLY_LEN * i, SEARCH_REPLY_LEN);


		SEARCH_RESULT_T *result = (SEARCH_RESULT_T*)malloc(sizeof(SEARCH_RESULT_T));
		memset(result, 0, sizeof(SEARCH_RESULT_T));
		
		//convert reply to result
		convert_search_raw_result(&reply, result);
		*ppresult = result;
	}


	return ret;
}

int create_udp_socket()
{
	int sock = -1;
	const int opt = 1;
	int ret = 0;

	struct sockaddr_in addr_local;
	bzero(&addr_local, sizeof(struct sockaddr_in));
	addr_local.sin_family = AF_INET;
	addr_local.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_local.sin_port = htons(LOCAL_PORT);

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{   
		printf("create socket failed with error %d\n", sock);
		return -1;
	}   
	
	//设置该套接字为广播类型，
	ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	if(ret == -1)
	{
		printf("set socket opt error \n");
		return -1;
	}

	if(bind(sock,(struct sockaddr *)&(addr_local), sizeof(addr_local)) == -1) 
	{   
		printf("bind error...\n");
		return -1;
	}

	return sock;
}

int close_udp_socket(int sock_fd)
{
	int ret = 0;
	ret = close(sock_fd);
	return ret;
}

int search_mrr_device(SEARCH_RESULT_T **ppresult_out)
{
	unsigned char reply_buf[MAX_BUFFER_LEN] = {0};

	struct sockaddr_in addr_dest;
	bzero(&addr_dest, sizeof(struct sockaddr_in));
	addr_dest.sin_family=AF_INET;
	addr_dest.sin_addr.s_addr=htonl(INADDR_BROADCAST);
	addr_dest.sin_port=htons(DEST_PORT);

	int sock = create_udp_socket();

	//从广播地址发送搜索指令
	int recv_len = 0;
	unsigned char cmd[] = {"0123456789012345678901234567890123456789"};

	int ret = send_boardcast_cmd(sock, (struct sockaddr*)&addr_dest, cmd, strlen((char*)cmd));
	if(ret<0)
	{
		printf("send search cmd error....\n");
		return -1;
	}
	else
	{		
		printf("send search cmd ok!\n");	
	}

	while(1)
	{
		recv_len = recv_reply(sock, (struct sockaddr*)&addr_dest, reply_buf);
		if(recv_len <= 0)
		{
			printf("recv error....\n");
			break;
		}
		else if(recv_len % SEARCH_REPLY_LEN == 0)
		{
			ret = parse_search_result(reply_buf, recv_len, ppresult_out);
			if(ret >= 0)
				break;
		}

		sleep(1);

	}

	close_udp_socket(sock);

	return 0;
}

void safe_delete_search_result(SEARCH_RESULT_T* presult)
{
	if(presult->next_result)
	{
		safe_delete_search_result(presult->next_result);
	}

	free (presult);
	presult = NULL;
}

int configure_device(const DEVICE_CONF_PARAM_T *conf_param)
{
	DEVICE_CONF_PARAM_RAW raw_data;
	convert_conf_param(conf_param, &raw_data);

	//display on screen
	display_configure_param(&raw_data);

	//save to buffer
	unsigned char* pcmdbuffer = (unsigned char*)malloc(sizeof(DEVICE_CONF_PARAM_RAW));
	memcpy(pcmdbuffer, &raw_data, sizeof(DEVICE_CONF_PARAM_RAW));

	//create udp socket
	struct sockaddr_in addr_dest;
	bzero(&addr_dest, sizeof(struct sockaddr_in));
	addr_dest.sin_family=AF_INET;
	addr_dest.sin_addr.s_addr=htonl(INADDR_BROADCAST);
	addr_dest.sin_port=htons(DEST_PORT);

	int sock = create_udp_socket();

	//从广播地址发送配置指令
	int ret = send_boardcast_cmd(sock, (struct sockaddr*)&addr_dest, pcmdbuffer, sizeof(DEVICE_CONF_PARAM_RAW));
	if(ret<0)
	{
		printf("send configure cmd error....\n");
		return -1;
	}
	else
	{		
		printf("send configure cmd ok!\n");	
	}

	close_udp_socket(sock);
	free (pcmdbuffer);
	pcmdbuffer = NULL;

	return ret;
}

int configure_mrr_device(const char* mac, const char* dest_ip, int dest_port, 
	const char* local_ip, int local_port, const char* mask_ip, const char* gateway)
{
	DEVICE_CONF_PARAM_T conf_param;
	sprintf(conf_param.mac, "%s", mac);
	sprintf(conf_param.password, "%s", DEFAULT_CONF_PASSWORD);

	sprintf(conf_param.dest_ip, "%s", dest_ip);
	sprintf(conf_param.local_ip, "%s", local_ip);
	sprintf(conf_param.mask_ip, "%s", mask_ip);
	sprintf(conf_param.gateway, "%s", gateway);

	conf_param.dest_port = dest_port;
	conf_param.local_port = local_port;
	conf_param.work_mode = 1;
	conf_param.bit_rate = 9600;
	conf_param.id = 0;
	conf_param.com_flag = 3;

	return configure_device(&conf_param);
}

/**
*	usage
*
*	1. usage of search mrr device
*	2. usage of configure mrr device
*
*/
int usage_search()
{
	int ret = 0;

	//you must declare a pointer to SEARCH_RESULT_T firstly
	SEARCH_RESULT_T * presult = NULL;

	//call search_mrr_device() to search, the result will be saved in struct presult
	ret = search_mrr_device(&presult);


	/////////////////////////////////////////////////////////////////
	//模拟搜索出多个设备
	SEARCH_RESULT_T * result2 = (SEARCH_RESULT_T*)malloc(sizeof(SEARCH_RESULT_T));
	memcpy(result2, presult, sizeof(SEARCH_RESULT_T));
	result2->com_flag = 5;

	SEARCH_RESULT_T * result3 = (SEARCH_RESULT_T*)malloc(sizeof(SEARCH_RESULT_T));
	memcpy(result3, presult, sizeof(SEARCH_RESULT_T));
	result3->com_flag = 6;

	presult->next_result = result2;
	result2->next_result = result3;
	/////////////////////////////////////////////////////////////////

	//output the search result on screen
	display_search_result(presult);

	//you must call safe_delete_search_result() to delete the pointer lastly
	safe_delete_search_result(presult);

	return ret;
}

int usage_configure()
{
	int ret = 0;

	// configure method  1
/*
	DEVICE_CONF_PARAM_T conf_param;

	sprintf(conf_param.mac, "%s", "12-23-34-45-56-CF");
	sprintf(conf_param.password, "%s", "110415");

	sprintf(conf_param.dest_ip, "%s", "192.168.200.141");
	sprintf(conf_param.local_ip, "%s", "192.168.28.253");
	sprintf(conf_param.mask_ip, "%s", "255.255.255.0");
	sprintf(conf_param.gateway, "%s", "192.168.28.1");

	conf_param.dest_port = 20108;
	conf_param.local_port = 20108;
	conf_param.work_mode = 1;
	conf_param.bit_rate = 9600;
	conf_param.id = 201022;
	conf_param.com_flag = 3;
	ret = configure_device(&conf_param);
*/

	// configure method 2 : main method
	ret = configure_mrr_device("00-38-AF-94-47-22",
		"192.168.200.141", 20108,
		"192.168.28.253", 20108,
		"255.255.255.0",
		"192.168.28.1" 
		);

	return ret;
}

int main()
{
	int ret = 0;

	// test search
	//ret = usage_search();

	// test configure
	ret = usage_configure();


	return ret;
}
