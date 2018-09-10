#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<pwd.h>
#include<time.h>
#include<fcntl.h>

#define EXITCODE 9999

char* manual_commands[]={"editfile","newdir","content","info","exitbcse"};

void shellfunc(void);			//accepts commands and executes it and waits until exit
int execute_func(char** args,int count);
int custom_command(char** args,int (*func)(char**,int),int count);
int system_command(char** args,int count);

int newdir(char** args,int count); 					//create a new directory
int editfile(char** args,int count);				//edit a file
int content(char** args,int count); 				//print a file
int info(char** args,int count); 					//display info about a file
int exitbcse(char** args,int count);				//exit the shell

int (*manual_functions[]) (char**,int)={&editfile,&newdir,&content,&info,&exitbcse};


int main(){
	time_t s;
	struct tm* current_time;
	s=time(NULL);
	current_time=localtime(&s);
	//printf ("%d \n",current_time->tm_hour);
	int hour=current_time->tm_hour;
	if(hour>=5&&hour<=12){
		printf("Good morning\n");
	}
	else if(hour<=17){
		printf("Good afternoon\n"); 
	}
	else if(hour<=22){
		printf("Good evening\n");
	}
	else 
		printf("Good night\n");
	shellfunc();
	return 0;
}

void shellfunc(void){
	while(1){
		printf("BCSE !!! ");
		char line[100];
		fgets(line,100,stdin);
		//printf("Input : %s\n",line);

		char** words=malloc(100*sizeof(char*));
		

		int count=0;
		char *token;
		token=strtok(line," \t\r\n");
		while(token!=NULL){
			words[count]=token;
			//printf("%d %s\n", count,words[count]);
			count++;
			token=strtok(NULL," \t\r\n");
		}



		words[count]=NULL;
		//for(int i=0;i<count;i++)
		//	printf("%d %s\n",i,words[i] );
		//clear the line
		int val=execute_func(words,count);
		if(val==EXITCODE) return;
		memset(line,0,sizeof(line));
	}
}

int execute_func(char** args,int count){
	if(args[0]==NULL){
		return 0;
	}
	else{
		int i;
		//printf("%s\n",args[0]);
		for(i=0;i<5;i++)
			if(strcmp(args[0],manual_commands[i])==0){
				return custom_command(args,manual_functions[i],count);
			}
		return system_command(args,count);
	}
	return EXITCODE;
}

int system_command(char** args,int count){
	printf("inside system_command()\n");
	for(int i=0;i<count;i++)
		printf("%s\n",args[i] );
	pid_t pid=fork();
	int background=0;
	if(args[count-1]!=NULL&&strcmp(args[count-1],"&")==0){
		background=1;
		args[count-1]=NULL;
	}
	else if(count>=2&&args[count-2]!=NULL&&strcmp(args[count-2],"&")==0){
		background=1;
		args[count-2]=NULL;
	}
	if(pid==-1){
		printf("forking failed\n");
		return 0;
	}
	//printf(":%s:\n", args[0]);
	if(pid==0&&background==0){
		if(execvp(args[0],args)==-1)				//-1 if failure
		{
			printf("wrong input commands\n");
			return 0;
		}
	}
	else if(pid==0&&background){
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		int x = open("/dev/null", O_RDWR);
		dup(x);
		if(execvp(args[0],args)==-1)				//-1 if failure
		{
			printf("wrong input commands\n");
			return 0;
		}
		kill(getpid(),SIGINT);
	}
	else if(background!=1){
		wait(NULL);
		return 0;
	}
	return 1;
}

int custom_command(char** args,int (*func)(char**,int),int count){
	pid_t pid=fork();
	int background=0;
	if(args[count-1]!=NULL&&strcmp(args[count-1],"&")==0){
		background=1;
		//args[count-1]=NULL;
	}
	else if(count>=2&&args[count-2]!=NULL&&strcmp(args[count-2],"&")==0){
		background=1;
		//args[count-2]=NULL;
	}
	if(pid==-1){
		printf("forking failed\n");
		return 0;
	}
	if(pid==0&&background==0){
		if(func(args,count)==-1){
			printf("wrong input commands\n"); 
			return 0;
		}
	}
	else if(pid==0&&background){
		if(func(args,count)==-1){
			printf("wrong input commands\n");
			return 0;
		}
	}
	else if(background!=1){
		wait(NULL);
		return 0;
	}
	return 1;
}


int editfile(char** args,int count){
	args[0]="gedit";
	return system_command(args,count);
}

int newdir(char** args,int count){
	if(count!=2){
		printf("newdir requires exactly one argument\n"); 
		return 1;
	}
	if(mkdir(args[1],0770)!=0){
		printf("directory already exists\n"); 
		return 1;
	}
	printf("directory created\n"); 
	return 0;
}

int content(char** args,int count){
	if(count!=2){
		printf("newdir requires exactly one argument\n"); 
		return 1;
	}


	FILE *fptr;
	char* filename,c;
	filename=args[1];
	// Open file
	fptr = fopen(filename, "r");
	if (fptr == NULL){
		printf("cannot open %s \n",filename);
		return 0;
	}

	while ((c = fgetc(fptr)) != EOF){
		printf ("%c", c);
	}

	fclose(fptr);
	printf("\n");
	return 1;
}

int info(char** args,int count){
	if(count!=2){
		printf("newdir requires exactly one argument\n"); 
		return 1;
	}
	if(access(args[1],F_OK)==-1){
		printf("Does not exist\n");
		return 1;
	}
	char actualpath[300];
	char *ptr;
	ptr=realpath(args[1],actualpath);
	printf("Path : %s\n",ptr );

	struct stat st;
	stat(args[1],&st);
	int size=st.st_size;
	ptr=ctime(&st.st_mtime);

	printf("Size : %d\n", size);
	printf("Last access : %s",ptr);

	uid_t owner=st.st_uid;
	struct passwd *pwd;
	pwd = getpwuid(owner);
	printf("Owner : %s\n",pwd->pw_name);

	return 0;

}

int exitbcse(char** args,int count){
	printf("Inside exitbcse\n");
	kill(0,SIGTERM);
	exit(0);
	return EXITCODE;
}
