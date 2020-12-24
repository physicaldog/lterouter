#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h>  
#include "./uci.h" 

int get_config(char*buff,char *option,char *section,char *package) 
{ 
	struct uci_context *c; 
	struct uci_ptr p; 
	char path[64] = {0};

	sprintf(path,"%s.%s.%s",package,section,option); 

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

int set_config(char*buff,char *option,char *section,char *package) 
{ 
	struct uci_context *c; 
	struct uci_ptr p; 
	char path[64] = {0};
	int ret = 0;

	sprintf(path,"%s.%s.%s",package,section,option); 

	c = uci_alloc_context(); 
	uci_set_confdir(c, "/opt/config");

	if((UCI_OK == uci_lookup_ptr(c, &p, path, true)) && (p.flags & UCI_LOOKUP_COMPLETE)) { 
		p.value = buff;
		ret = uci_set(c,&p);
		if(ret == 0){
			uci_commit(c,&p.p,false);
		}
	} 
	else{
		uci_perror(c, "no found!\n"); 
		return -1; 
	}

	uci_free_context(c); 
	return(0); 
}

int main()
{
	char buff[64] = {0};
	get_config(buff,"ip","lan","config");
	printf("buff=%s\n",buff);
	set_config("255.255.255.0","netmask","lan","config");
	get_config(buff,"netmask","lan","config");
	printf("buff=%s\n",buff);

}
