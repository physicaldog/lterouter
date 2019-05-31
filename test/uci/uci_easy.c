#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h>  
#include "./uci.h" 

int get_config(char *package,char *section,char *option,char *buff) 
{ 
	struct uci_context *c; 
	struct uci_ptr p; 
	char path[64] = {0};

	printf("%s\n",__FUNCTION__);
	sprintf(path,"%s.%s.%s",package,section,option); 
	printf("%s\n",path);

	c = uci_alloc_context(); 
	uci_set_confdir(c, "/opt/config");

	if((UCI_OK == uci_lookup_ptr(c, &p, path, true)) && (p.flags & UCI_LOOKUP_COMPLETE)) { 

		strcpy(buff,p.o->v.string);
	} 
	else{
		uci_perror(c, "no found!\n"); 
		return -1; 
	}

	uci_free_context(c); 
	return(0); 
}

int set_config(char *package,char *section,char *option,char *value,bool commit) 
{ 
	struct uci_context *c; 
	struct uci_ptr p; 
	char path[64] = {0};
	int ret = 0;

	printf("%s\n",__FUNCTION__);
	sprintf(path,"%s.%s.%s",package,section,option); 
	printf("%s\n",path);

	c = uci_alloc_context(); 
	uci_set_confdir(c, "/opt/config");

	if((UCI_OK == uci_lookup_ptr(c, &p, path, true))) { 
		p.value = value;
		ret = uci_set(c,&p);
		uci_save(c,p.p);
		if(ret == 0){
			if(commit){
				uci_commit(c,&p.p,false);
				printf("commit\n");
			}
		}
	} 
	else{
		uci_perror(c, "no found!\n"); 
		return -1; 
	}

	uci_free_context(c); 
	return(0); 
}
/*a.out get option section package*/
/*a.out set option section package value ture/false*/
int main(int argc,char *argv[])
{
	char buff[64] = {0};
	if(argc < 5){
		printf("no argc!!!\n");
		return -1;
	}

	if(!strcmp(argv[1],"get")){
		get_config(argv[2],argv[3],argv[4],buff);
		printf("%s\n",buff);
	}
	if(!strcmp(argv[1],"set")){
		if(argc < 7){
			printf("no argc!!!\n");
			return -1;
		}
		if(!strcmp(argv[6],"commit"))
			set_config(argv[2],argv[3],argv[4],argv[5],true);
		else
			set_config(argv[2],argv[3],argv[4],argv[5],false);
	}
}
