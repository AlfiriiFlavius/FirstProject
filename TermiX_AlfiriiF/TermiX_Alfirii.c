
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <signal.h>
#include <termios.h>
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <dirent.h>
#include <stddef.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>
#include <limits.h>

#define SIZE 1
#define BUFFERSIZE 1024

#define MAX_COMAND_LENGTH 100
#define MAX_NUMBER_OF_PARAMS 50



#define GRN   "\x1B[33m"
#define RESET "\x1B[0m"



///////////////////////////



char *user;
char *getlogin();



///////////////////////////









char *input;// for history command

int i=0;

char cmd[MAX_COMAND_LENGTH+1];
char *params[MAX_NUMBER_OF_PARAMS+1];
FILE *file_name1;



int status=-1;//pentru deschiderea fisierelor
int status1;

char *file_name;
char line[256];
char *buf;
char prev1[256];
char *prev;
char *current;
int n;
int numberOfByte=0;//pentru afisarea ultimilor n bytes din fisier
int numberOfByteShow=0;

char *istoric;

int count;//contor pentru numarul liniilor din fisier
int count1;// numarul linie de la care incep afisarea
int m;//numarul de linii care trbuie afisate, introdus de catre utilizator
int afisare=0;//incep afisarea
////////////////
//pentru expr///
char semn[1000];
char arg1[1000];
char arg2[1000];
///////////////
char path[1000];//pentru schimbare de director
char pth[1000];
char directory[1000];
char *defaultDirectory="/home/user/";
int size = 1000,i,j;

int fd[100][2];
char string[][100] = {"ls"};

struct termios save_term;
int gg;
//char exec9[50]=" ";
void parse(char *cmd, char **params)
{
    for(i=0; i< MAX_NUMBER_OF_PARAMS;i++){
        params[i]=strsep(&cmd," ");
        if(params[i] == NULL)
        {
            gg=i;
	    //strcat(exec,params[i]);
	    //strcat(exec,";");
            break;
        }
    }
}


//-----------------------------------------------Informatii despre comenzile valabile cu optiuni implementate---------------------------------------------------


void help(void)
{
	printf("\nThese shell commands are defined internally.\n");
	printf("Type 'help' to see this list about available implemented options\n");
	printf("Type 'version' to find more about the shell and comand ");
	printf("authors of this shell.\n");
	printf("Available function:\nexit\nhelp\nversion\nclear\nlogname");
}



//----------------------------------------------------------------Informatii despre autori-----------------------------------------------------------------------


void version(void)
{
	printf("\n AUTHOR");
	printf("\t Written by Flavius & Laurentiu.\n");
	printf("\n SOFTWARE VERSION \n ");
	printf("\t Version 1.3 -this version includes:\n");
	printf("\t \"help\", that will list all available commands with implemented options.\n");
	printf("\t \"version\", that will offer author information, versioning information, and possibility other valuable information\n");
	printf("\t \"info\", that will offer informations about the functions\n");
	printf("\t -Other Comands: \n ");
	printf("\t -The \"cd\".\n");
	printf("\t -The \"logname\".\n");
	printf("\t -The \"clear\".\n");


}


//-----------------------------------------------------------------Comenzile man[command]------------------------------------------------------------------------


void manCd()
{
	printf("\nNAME:\n");
	printf("\tcd - cd command, which stands for 'change directory', changes the \n");
	printf("\tshell's current working directory.\n");
	printf("\nDESCRIPTION\n");
	printf("\tThe cd command is one of the commands you will use the most at the\n"); 		printf("\tcommand line in linux. It allows you to change your working directory.\n");
	printf("\tYou use it to move around within the hierarchy of your file system.\n\n");
}

void manlogname()
{
	printf("\nNAME:\n");
	printf("\tlogname - print user´s login name");
	printf("\nSYNOPSIS:\n");
	printf("\tlogname [OPTION]");
	printf("\nDESCRIPTION:\n");
	printf("\tPrint the name of the current user.\n");
	printf("\n\t--help display this help and exit\n");
	printf("\n\t--version\n");
	printf("\t\toutput version information and exit\n\n");

}
void manclear()
{
	printf("\nNAME:\n");
	printf("\tclear - clear the terminal screen");
	printf("\nSYNOPSIS:\n");
	printf("\tclear\n");
	printf("\nDESCRIPTION:\n");
	printf("\tclear  clears your screen if this is possible, including its scrollback");
	printf("\n\tbuffer (if the extended 'E3' capability is defined).  clear  looks in");
	printf("\n\tthe environment for the terminal type and then in the terminfo database");
	printf("\n\tto determine how to clear the screen.");
	printf("\n\n\tclear ignores any command-line parameters that may be present.\n\n");
}

void manfalse()
{
	printf("\nNAME:\n");
	printf("\tfalse - do nothing, unsuccessfully");
	printf("\nSYNOPSIS:\n");
	printf("\tfalse [ignored command line arguments]\n");
	printf("\tfalse [OPTION]");
	printf("\nDESCRIPTION:\n");
	printf("\tExit with a status code indicating failure.\n");
	printf("\n\t--help display this help and exit\n");
	printf("\n\t--version\n");
	printf("\t\toutput version information and exit\n");
	printf("\tNOTE: your shell may have its own version of false,\n");
	printf("\twhich usually supersedes the version described here.\n");
	printf("\tPlease refer to your shell's documentation for details about the\n");
	printf("\toptions it supports.\n\n");
}

void mancat()
{
	printf("\nNAME:\n");
	printf("\tcat - concatenate files and print on the standard output");
	printf("\nSYNOPSIS:\n");
	printf("\tcat [OPTION]... [FILE]...");
	printf("\nDESCRIPTION:\n");
	printf("\tConcatenate FILE(s), or standard input, to standard output.\n");
	printf("\n\t--help display this help and exit\n");
	printf("\n\t--version\n");
	printf("\t\toutput version information and exit\n");
	printf("\tWith no FILE, or when FILE is -, read standard input.\n\n");
	printf("\t-b\n");
	printf("\t\tnumber nonempty output lines\n");
	printf("\t-c\n");
	printf("\t\tnumber all output lines\n");
	printf("\t-d\n");
	printf("\t\tdisplay $ at end of each line\n");

}

void manfile()
{
	printf("\nNAME:\n");
	printf("\tfile - determine file type");
	printf("\nSYNOPSIS:\n");
	printf("\tfile [-bchikLNnprsvz0] [--apple] [--mime-encoding] [--mime-type]");
	printf("\t[-e testname] [-F separator] [-f namefile] [-m magicfiles] file ...");
	printf("\n\tfile -C [-m magicfiles]");
	printf("\n\tfile [--help]");
	printf("\nDESCRIPTION:\n");
	printf("\tfile tests each argument in an attempt to classify it.  There are three\n");
	printf("\tsets of tests, performed in this order: filesystem tests, magic tests,\n");
	printf("\tand language tests.  The first test that succeeds causes the file type to\n");
	printf("\tbe printed.\n");
	printf("\n\t--help display this help and exit\n");
	printf("\n\t--version\n");
	printf("\t\toutput version information and exit\n");
	printf("\t-b\n");
	printf("\t\tDo not prepend filenames to output lines (brief mode).\n");
}

void manexpr()
{
	printf("\nNAME:\n");
	printf("\texpr - evaluate expressions");
	printf("\nSYNOPSIS:\n");
	printf("\texpr EXPRESSION");
	printf("\nDESCRIPTION:\n");
	printf("\n\t--help display this help and exit\n");
	printf("\n\t--version\n");
	printf("\t\toutput version information and exit\n");
	printf("\tPrint  the  value of EXPRESSION to standard output.  A blank line below");
       	printf("\tseparates increasing precedence groups.  EXPRESSION may be:\n");
	printf("\tARG1 | ARG2\n");
	printf("\t\tARG1 if it is neither null nor 0, otherwise ARG2\n");
	printf("\tARG1 & ARG2\n");
	printf("\t\tARG1 if neither argument is null or 0, otherwise 0\n");
	printf("\tARG1 < ARG2\n");
	printf("\t\tARG1 is less than ARG2\n");
	printf("\tARG1 = ARG2\n");
	printf("\t\tARG1 is equal to ARG2\n");
	printf("\tARG1 != ARG2\n");
	printf("\t\tARG1 is unequal to ARG2\n");
	printf("\tARG1 > ARG2\n");
	printf("\t\tARG1 is greater than ARG2\n");
	printf("\tARG1 + ARG2\n");
	printf("\t\tarithmetic sum of ARG1 and ARG2\n");
	printf("\tARG1 - ARG2\n");
	printf("\t\tarithmetic difference of ARG1 and ARG2\n");
	printf("\tARG1 * ARG2\n");
	printf("\t\tarithmetic product of ARG1 and ARG2\n");
	printf("\tARG1 / ARG2\n");
	printf("\t\tarithmetic quotient of ARG1 divided by ARG2\n");
	printf("\tARG1(%)ARG2\n");
	printf("\t\tarithmetic remainder of ARG1 divided by ARG2\n\n");
	printf("\tExit status is 0 if EXPRESSION is neither null nor 0, 1 if EXPRESSION\n");
	printf("\tis  null  or  0,  2 if EXPRESSION is syntactically invalid, and 3 if an\n");
	printf("\terror occurred.\n");
}



//-------------------------------------------------------------------[command] --help ---------------------------------------------------------------------------


void logh()
{
	printf("Usage: logname [OPTION]\n");
	printf("Print the name of the current user.\n");
	printf("\n\t--help     display this help and exit\n");
	printf("\t--version  output version information and exit\n");
}

void cath()
{
	printf("Usage: cat [OPTION]... [FILE]...\n");
	printf("Concatenate FILE(s), or standard input, to standard output.\n\n");
	printf("-b    number nonempty output lines\n");
	printf("-d    display $ at end of each line\n");
	printf("-c    number all output lines\n");
	printf("\n\t--help     display this help and exit\n");
	printf("\t--version  output version information and exit\n");
}

void exprh()
{
	printf("Usage: expr EXPRESSION\n");
	printf("   or: expr OPTION\n\n");
	printf("\n\t--help     display this help and exit\n");
	printf("\t--version  output version information and exit\n");
	printf("\tPrint  the  value of EXPRESSION to standard output.  A blank line below");
       	printf("\tseparates increasing precedence groups.  EXPRESSION may be:\n");
	printf("\tARG1 | ARG2       ARG1 if it is neither null nor 0, otherwise ARG2\n");
	printf("\tARG1 & ARG2       ARG1 if neither argument is null or 0, otherwise 0\n");
	printf("\tARG1 < ARG2       ARG1 is less than ARG2\n");
	printf("\tARG1 = ARG2       ARG1 is equal to ARG2\n");
	printf("\tARG1 != ARG2      ARG1 is unequal to ARG2\n");
	printf("\tARG1 > ARG2       ARG1 is greater than ARG2\n");
	printf("\tARG1 + ARG2       arithmetic sum of ARG1 and ARG2\n");
	printf("\tARG1 - ARG2       arithmetic difference of ARG1 and ARG2\n");
	printf("\tARG1 * ARG2       arithmetic product of ARG1 and ARG2\n");
	printf("\tARG1 / ARG2       arithmetic quotient of ARG1 divided by ARG2\n");
	printf("\tARG1(%)ARG2       arithmetic remainder of ARG1 divided by ARG2\n\n");
	printf("\tExit status is 0 if EXPRESSION is neither null nor 0, 1 if EXPRESSION\n");
	printf("\tis  null  or  0,  2 if EXPRESSION is syntactically invalid, and 3 if an\n");
	printf("\terror occurred.\n");

}

void fileh()
{
	printf("Usage: file [OPTION...] [FILE...]\n");
	printf("Determine type of FILEs.\n\n");
	printf("\n\t--help     display this help and exit\n");
	printf("\t--version  output version information and exit\n");
	printf("\t-b,	   Do not prepend filenames to output lines (brief mode).\n");
}



//-------------------------------------------------------------------[command] --version-------------------------------------------------------------------------


void logv()
{
	printf("logname (GNU coreutils) 8.21\n");
	printf("Copyright (C) 2013 Free Software Foundation, Inc.\n");
	printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.");
	printf("This is free software: you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n");
}


void catv()
{
	printf("cat (GNU coreutils) 8.21\n");
	printf("Copyright (C) 2013 Free Software Foundation, Inc.\n");
	printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.");
	printf("This is free software: you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n");
}

void exprv()
{
	printf("expr (GNU coreutils) 8.21\n");
	printf("Copyright (C) 2013 Free Software Foundation, Inc.\n");
	printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.");
	printf("This is free software: you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n");
}

void filev()
{
	printf("file (GNU coreutils) 8.21\n");
	printf("Copyright (C) 2013 Free Software Foundation, Inc.\n");
	printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.");
	printf("This is free software: you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n");
}



//------------------------------------------------------------Functiile comenzilor implementate------------------------------------------------------------------


void logname()
{
    	user=getlogin();
    	if(user==NULL)
        	printf("No user logged on.\n");
	else
		printf("Hello %s !\n",user);
}

void expr()
{
    	char semn[]=" + - * / <= >= | & = != % < > ";
    	strcpy(semn,params[2]);
    	strcpy(arg1,params[1]);
    	strcpy(arg2,params[3]);
   	int rezultat;
    	if(strstr(semn, "+")!=NULL)
    	{
        	if(strcmp(strstr(semn, "+"),"+")==0)
        	{
            	rezultat= atoi(arg1)+atoi(arg2);
        	}
	printf("%d\n",rezultat);
    	}
    	else if(strstr(semn, "-")!=NULL)
    	{
        	if(strcmp(strstr(semn, "-"),"-")==0)
        	{
            	rezultat= atoi(arg1)-atoi(arg2);
        	}
	printf("%d\n",rezultat);
    	}
    	else if(strstr(semn, "*")!=NULL)
    	{
        	if(strcmp(strstr(semn, "*"),"*")==0)
        	{
            	rezultat= atoi(arg1)*atoi(arg2);
        	}
	printf("%d\n",rezultat);
    	}
    	else if(strstr(semn, "/")!=NULL)
    	{
        	if(strcmp(strstr(semn, "/"),"/")==0)
        	{
            	rezultat= atoi(arg1)/atoi(arg2);
        }
	printf("%d\n",rezultat);
    	}
   	else if(strstr(semn, "%")!=NULL)
    	{
        	if(strcmp(strstr(semn, "%"),"%")==0)
        	{
            	rezultat= atoi(arg1)%atoi(arg2);
        	}
	printf("%d\n",rezultat);
    	}
    	else if(strstr(semn, ">")!=NULL)
    	{
        	if(strcmp(strstr(semn, ">"),">")==0)
        	{
            		if(atoi(arg1)>atoi(arg2))
                	rezultat = 1;
            		else
                	rezultat = 0;
        	}
	printf("%d\n",rezultat);
    	}
    	else if(strstr(semn, "<")!=NULL)
    	{
        	if(strcmp(strstr(semn, "<"),"<")==0)
        	{
            		if(atoi(arg1)<atoi(arg2))
                		rezultat = 1;
            		else
                		rezultat = 0;
        	}
	printf("%d\n",rezultat);
    	}
    	else if(strstr(semn, "=")!=NULL)
    	{
        	if(strcmp(strstr(semn, "="),"=")==0)
        	{
            		if(atoi(arg1)==atoi(arg2))
                		rezultat = 1;
            		else
                		rezultat = 0;
        	}
	printf("%d\n",rezultat);
    	}
    	else if(strstr(semn, "!=")!=NULL)
    	{
        	if(strcmp(strstr(semn, "!="),"!=")==0)
        	{
            		if(atoi(arg1)!=atoi(arg2))
                		rezultat = 1;
            		else
                		rezultat = 0;
        	}
	printf("%d\n",rezultat);
    	}
    	else if(strstr(semn, "|")!=NULL)
    	{
        	if(strcmp(strstr(semn, "|"),"|")==0)
        	{
            		if((atoi(arg1)!=NULL) || (atoi(arg1)!=0))
                		rezultat = atoi(arg1);
            		else
                		rezultat = atoi(arg2);
        	}
	printf("%d\n",rezultat);
    	}
    	else if(strstr(semn, "&")!=NULL)
    	{
        	if(strcmp(strstr(semn, "&"),"&")==0)
        	{
            		if((atoi(arg1)!=NULL) || (atoi(arg1)!=0)&&(atoi(arg2)!=NULL) || (atoi(arg2)!=0))
                		rezultat = atoi(arg1);
            		else
                		rezultat = 0;
        	}
	printf("%d\n",rezultat);
    	}
    	else
		printf("expr : syntax error\n");
}

void clear()
{
    printf("\n"); printf("\033[2J\033[1;1H");
}

void cd()
{
        strcpy(path,params[1]);
		getcwd(directory,sizeof(directory));
		strcat(directory,"/");
		strcat(directory,path);
		status=chdir(directory);
		if(status !=0)
			//perror("MUE\n");
			printf("bash : cd: %s: no such file or directory\n",params[1]);
}

void file()
{
   int ok=0;

   if(strcmp(params[1],"-b")==0)
   {
    strcpy(pth,params[2]);
    strcpy(path,params[2]);
   }
   else
   {
    strcpy(pth,params[1]);
    strcpy(path,params[1]);
   }

	getcwd(directory,sizeof(directory));
	strcat(directory,"/");
	strcat(directory,path);
	status=chdir(directory);
	if(status ==0)
    {
        printf("%s: Directory\n",path);
        strcpy(directory, defaultDirectory);
        status=chdir(directory);
    }
    else
    {

        //////////////////////////////////////////////////

    char *curr_dir = NULL;DIR *dp = NULL;struct dirent *dptr = NULL;unsigned int count = 0;
    curr_dir = getenv("PWD");
    if(NULL == curr_dir)
    {
        printf("\n ERROR : Could not get the working directory\n");
        //return -1;
    }
    // Open the current directory
    dp = opendir((const char*)curr_dir);
    if(NULL == dp)
    {
        printf("\n ERROR : Could not open the working directory\n");
       // return -1;
    }
    // Go through and display all the names (files or folders)// Contained in the directory.
    for(count = 0; NULL != (dptr = readdir(dp)); count++)
    {
        if(strcmp(path,dptr->d_name)==0)
        {
            ok=1;

            /////////////////////////////
             if(strstr(path, ".txt")!=NULL)
        {
             if(strcmp(strstr(path, ".txt"),".txt")==0)
             {
                 if(strcmp(params[1],"-b")!=0) printf("%s: ASCII text\n",path);
                 else printf("ASCII text\n");

             }
        }
        else if(strstr(path, ".c")!=NULL)
        {
            if(strcmp(strstr(path, ".c"),".c")==0)
            {
                if(strcmp(params[1],"-b")!=0) printf("%s: C source code\n",path);
                else printf("C source code\n");
            }
        }
        else if(strstr(path, ".awk")!=NULL)
        {
            if(strcmp(strstr(path, ".awk"),".awk")==0)
            {
                if(strcmp(params[1],"-b")!=0) printf("%s: AWK script\n",path);
                else printf("AWK script\n");
            }
        }
        else if(strstr(path, ".log")!=NULL)
        {
            if(strcmp(strstr(path, ".log"),".log")==0)
            {
                if(strcmp(params[1],"-b")!=0) printf("%s: application log\n",path);
                else printf("application log\n");
            }
        }
        else if(strstr(path, ".sh")!=NULL)
        {
            if(strcmp(strstr(path, ".sh"),".sh")==0)
            {
                if(strcmp(params[1],"-b")!=0) printf("%s: shell script\n",path);
                else printf("shell script\n");
            }
        }
        else if(strstr(path, ".o")!=NULL)
        {
            if(strcmp(strstr(path, ".o"),".o")==0)
            {
                if(strcmp(params[1],"-b")!=0) printf("%s: object code\n",path);
                else printf("object code\n");
            }
        }
         else if(strstr(path, ".desktop")!=NULL)
        {
            if(strcmp(strstr(path, ".desktop"),".desktop")==0)
            {
                if(strcmp(params[1],"-b")!=0) printf("%s: unicode text\n",path);
                else printf("unicode text\n");
            }
        }
        else
        {
            if(strcmp(params[1],"-b")!=0) printf("%s: exe\n",path);
            else printf("exe\n");
        }
        ///////////////////////////////////

         break;
        }


    }
    if(ok==0) printf("This file is not exist\n");


    }
}

void home()
{
    		strcpy(directory, defaultDirectory);
        	status=chdir(directory);
}


void false()
{
    int i=0;
}

void cat()
{
    int fd4, bytesRead, i3 = 1;
        char* bufff;


        while (i3 < gg)
        {
                fd4 = open(params[i3], O_RDONLY);

                if (fd4 < 0)
                {
                        printf("Could not open file: %s\n", params[i3]);
                        fprintf(stderr, "Error number is %d, error message is: %s\n", errno, strerror(errno));
                }
                else
                {
                        bufff = malloc(sizeof(char *) * SIZE);
                        while ((bytesRead = read(fd4, bufff, SIZE)) > 0)
                        {
                                bufff[bytesRead] = '\0';
                                printf("%s", bufff);
                        }
                        free(bufff);
                }
                close(fd4);
                i3++;
        }

}

void cat_b()
{
    int fd4, bytesRead, i3 = 2,contor=1;
        char* bufff;
////////////////o/////////
        char s4[100];
        FILE *fisier;
        int i5=1;
//////////////////////////////////
        while (i3 < gg)
        {
                fd4 = open(params[i3], O_RDONLY);
                fisier=fopen(params[i3],"r");

                if (fd4 < 0)
                {
                        printf("Could not open file: %s\n", params[i3]);
                        fprintf(stderr, "Error number is %d, error message is: %s\n", errno, strerror(errno));
                }
                else
                {
        //fisier=fopen(params[i3]);

        while(!feof(fisier))
        {
            fgets(s4,sizeof(s4),fisier);
            printf("%d\t%s",i5,s4);
            i5++;
        }
        fclose(fisier);
                }
                close(fd4);
                i3++;
        }

}

void cat_c()
{
    int fd4, bytesRead, i3 = 2,contor=1;
        char* bufff;
/////////////////////////
        char s4[100];
        FILE *fisier;
        int i5=1;
//////////////////////////////////
        while (i3 < gg)
        {
                fd4 = open(params[i3], O_RDONLY);
                fisier=fopen(params[i3],"r");

                if (fd4 < 0)
                {
                        printf("Could not open file: %s\n", params[i3]);
                        fprintf(stderr, "Error number is %d, error message is: %s\n", errno, strerror(errno));
                }
                else
                {
        while(!feof(fisier))
        {
            fgets(s4,sizeof(s4),fisier);
            printf("%s",s4);
            i5++;
        }
        fclose(fisier);
                }
                close(fd4);
                i3++;
        }
        i5--;
        printf("\nNumar de linii: %d\n",i5);

}

void cat_d()
{
    int fd4, bytesRead, i3 = 2,contor=1;
        char* bufff;
/////////////////////////
        char s4[100];
        FILE *fisier;
//////////////////////////////////
        while (i3 < gg)
        {
                fd4 = open(params[i3], O_RDONLY);
                fisier=fopen(params[i3],"r");

                if (fd4 < 0)
                {
                        printf("Could not open file: %s\n", params[i3]);
                        fprintf(stderr, "Error number is %d, error message is: %s\n", errno, strerror(errno));
                }
                else
                {

        while(!feof(fisier))
        {
            fgets(s4,sizeof(s4),fisier);
            printf("$%s",s4);
        }
        fclose(fisier);
                }
                close(fd4);
                i3++;
        }

}

//--------------------------------------------------------------Functia fork pentru procese----------------------------------------------------------------------

int execute(char** params)
{
    // Fork process
    pid_t pid = fork();

    // Error
    if (pid == -1) {
        char* error = strerror(errno);
        printf("fork: %s\n", error);
        return 1;
    }

    // Child process
    else if (pid == 0) {
        // Execute command
        execvp(params[0], params);

        // Error occurred
        char* error = strerror(errno);
        printf("shell: %s: %s\n", params[0], error);
        return 0;
    }

    // Parent process
    else {
        // Wait for child process to finish
        int childStatus;
        waitpid(pid, &childStatus, 0);
        return 1;
    }

}




//------------------------------------------------------------------------Main-----------------------------------------------------------------------------------



int main()
{

char *citire;
char *argv[64];
char *args[2];


    	while(1)
    	{

		char *username = getenv("USER");
        	printf(GRN "%s@Flavius&Laurentiu-shell:~$ ",username);
		printf(RESET);

        	//citeste de pe linia de comanda
        	if(fgets(cmd,sizeof(cmd),stdin)==NULL)
            		break;

        	//elimina terminatorul de sir de pe o linie noua
        	if(cmd[strlen(cmd)-1]=='\n')
            		cmd[strlen(cmd)-1]='\0';

         	parse(cmd,params);
		// functia de descompunere a informatiilor citite de pe linia de comanda

		//comanda cd
        	if(!strcmp(params[0],"cd")&& params[1]!=NULL)
        	{
            		if(!strcmp(params[1],"$HOME"))
                		home();
            		else if(strcmp(params[1],"..")!=0)
                		cd();
        	}
		else if(!strcmp(params[0],"cd"))
            		home();

		//comanda exit
        	else if(!strcmp(params[0],"exit"))
            		exit(0);
           	///////////////////////////


        	//else if(!strcmp(params[0],"pwd"))
            		//printf("This command is not exist\n");

		//comanda expr
		else if(!strcmp(params[0],"expr")&&(params[1]==NULL))
		{
			printf("expr : missing operand\n");
			printf("Try 'expr --help' for more information.\n");
		}
		else if(!strcmp(params[0],"expr")&&(params[1]!=NULL))
		{
			if(!strcmp(params[1],"--help"))
		    		exprh();
			else if(!strcmp(params[1],"--version"))
				exprv();
			else expr();
		}

		//comanda help
        	else if(!strcmp(params[0],"help"))
            		help();

		//comanda version
        	else if(!strcmp(params[0],"version"))
            		version();
		//comanda clear
        	else if(!strcmp(params[0],"clear"))
			clear();

		//comanda logname
		else if(!strcmp(params[0],"logname")&&(params[1]!=NULL))
		{
			if(!strcmp(params[1],"--help"))
		    		logh();
			else if(!strcmp(params[1],"--version"))
				logv();
			else {printf("logname: extra operand ‘%s’\n",params[1]);
			      printf("Try 'logname --help' for more information.\n");}
		}
		else if(!strcmp(params[0],"logname"))
			logname();

		//comanda false
		else if(!strcmp(params[0],"false")&&(params[1]!=NULL))
		{
			if(!strcmp(params[1],"--help"))
		    		false();
			else if(!strcmp(params[1],"--version"))
				false();
			else if(!strcmp(params[1],"ls"))
				false();
			else if(!strcmp(params[1],"logname"))
				false();
			else if(!strcmp(params[1],"cd"))
				false();
		}
		else if(!strcmp(params[0],"false"))
			false();

		//comanda file
        	else if(!strcmp(params[0],"file") &&(params[1]!=NULL))
            	{
			if(!strcmp(params[1],"--help"))
		    		fileh();
			else if(!strcmp(params[1],"--version"))
				filev();
			else file();
		}
        	else if(!strcmp(params[0],"file"))
            	{
			printf("Usage:  file [-bchikLlNnprsvz0] [--apple] [--mime-encoding] [--mime-type]\n");
			printf("\t     [-e testname] [-F separator] [-f namefile] [-m magicfiles] file ...\n");
			printf("\tfile -C [-m magicfiles]\n");
			printf("\tfile [--help]\n");
		}

		//comanda cat
		else if(!strcmp(params[0],"cat") && params[1]!=NULL)
        	{
            		if(!strcmp(params[1],"-b"))
                cat_b();
            		else if(!strcmp(params[1],"-c"))
				cat_c();
            		else if(!strcmp(params[1],"-d"))
				cat_d();
            		else cat();
        	}
		else if(!strcmp(params[0],"cat") && params[1]==NULL)
            		printf("You don't have arguments.\n");

		//comanda man
	       	else if(!strcmp(params[0],"man") && params[1]!=NULL)
		{
			if(!strcmp(params[1],"cd"))
				manCd();
			else if(!strcmp(params[1],"logname"))
				manlogname();
			else if(!strcmp(params[1],"clear"))
				manclear();
			else if(!strcmp(params[1],"false"))
				manfalse();
			else if(!strcmp(params[1],"expr"))
				manexpr();
			else if(!strcmp(params[1],"cat"))
				mancat();
			else if(!strcmp(params[1],"file"))
				manfile();
			else printf("No manual entry for %s\n",params[1]);
		}
		else if(!strcmp(params[0],"man"))
			printf("What manual page do you want? \n");

		else if(params[0]!=NULL && strcmp(params[0],"nano")!=0 && strcmp(params[0],"ls")!=0 && strcmp(params[0],"pwd")!=0)
		{
			printf("This command is not exist.\n");
			printf("Wanna use one of our implemented commands?\n");
			printf("\t- cd, clear, false, file, cat, logname, expr.\n");
			printf("Or one of available commands:\n");
			printf("\t- ls, pwd, nano.\n");
		}

	else execute(params);

	}
    return 0;
}


