#ifndef _MRR_CONFIGURE_H
#define	_MRR_CONFIGURE_H

#define MAX_IPSTRING_LEN	(16)
#define MAX_MACSTRING_LEN	(18)

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


#ifdef __cplusplus
extern "C"
{
#endif


int search_mrr_device(SEARCH_RESULT_T **ppresult_out);

void safe_delete_search_result(SEARCH_RESULT_T* presult);

int configure_mrr_device(const char* mac, const char* dest_ip, int dest_port, 
	const char* local_ip, int local_port, const char* mask_ip, const char* gateway);


#ifdef __cplusplus
}
#endif

#endif
