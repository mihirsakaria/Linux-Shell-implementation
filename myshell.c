#include<stdio.h>
#include<sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

char* findpwd()
{
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));
        //printf("%s\n",cwd);
        char* s = cwd;
        //printf("%s\n",s);
        return s;
}

int check_basic_commands(char** tokens)
{
            char* List[4];
            List[0] = "exit";
            List[1] = "cd";
            List[2] = "pwd";
            List[3] = "mkdir";
            if(strcmp(List[0],tokens[0])==0)
            {
                exit(0);
            }
            else if(strcmp(List[1],tokens[0])==0)
            {
                chdir(tokens[1]);
                return 1;

            }
            else if(strcmp(List[2],tokens[0])==0)
            {
                // char cwd[1024];
                // getcwd(cwd,sizeof(cwd));
                // printf("%s\n",cwd);
                printf("%s\n",findpwd());
                return 1;
            }
            else if(strcmp(List[3],tokens[0])==0)
            {
                int flag = mkdir(tokens[1]);
                if(!flag)
                    printf("The directory could not be made\n");
                else   
                    printf("Directory  %s Made\n",tokens[1]);
                return 1;
            }
            return 0;

}

void applyfunctions(char **tokens)
{
    pid_t x;
    x = fork();
    int stat_loc;

    if(x==0)
    {
        if(execvp(tokens[0],tokens)<0)
        {
            // perror(tokens[0]);
            printf("Something wrong with %s \n",tokens[0]);
        }
        printf("This won't be printed if execvp is successful\n");
    }
    else if(x<0)
    {
        printf("program failed\n ##### SORRY ###\n");
    }
    else{
        waitpid(x,&stat_loc,WUNTRACED);
    }
}

void applyfunctions_background(char **tokens)
{
    pid_t x;
    x = fork();
    int stat_loc;

    if(x==0)
    {
        if(execvp(tokens[0],tokens)<0)
        {
            // perror(tokens[0]);
            printf("Something wrong with %s \n",tokens[0]);
        }
        printf("This won't be printed if execvp is successful\n");
    }
    else if(x<0)
    {
        printf("program failed\n ##### SORRY ###\n");
    }
    else{
        //waitpid(x,&stat_loc,WUNTRACED);
    }
}

int checkpipes(char **tokens)
{
    for(int i =0;tokens[i]!=NULL;i++)
    {
        if(strcmp(tokens[i],"|")==0)
            return 1;
    }
    return 0;
}

int foreground(char** tokens)
{
    for(int i=0;tokens[i]!= NULL;i++)
    {
        if(strcmp(tokens[i],"&")==0)
            return 1;
    }
    return 0;
}

void makepipes(char ** tokens,int n)
{
    int p[2];
    pid_t p1[n+1];
    int stat_loc;
    int j=0;
    for(int i=0;i<=n;i++)
    {
        p1[i] = fork();
        if(p1[i]<0)
        {
            printf("couldn't fork\n");
        }  
        if(p1[i] == 0)
        {
            if(i==0)
            {
                close(p[0]);
                dup2(p[1],STDOUT_FILENO);
                close(p[1]);
            }
            else if(i!=n)
            {
                dup2(p[0],STDOUT_FILENO);
                dup2(p[1],STDOUT_FILENO);
            }
            else{
                close(p[1]);
                dup2(p[0],STDOUT_FILENO);
                close(p[0]);
            }

            char **tar = (char**)malloc(MAX_TOKEN_SIZE*sizeof(char*));
            int k = 0;
            for(;strcmp(tokens[j],"|") && tokens[j]!=NULL;j++)
            {
                tar[k] = tokens[j];
                k++;
            }
            j++;
            if(execvp(tar[0],tar)<0)
            {
                printf("Could not execute.\n");
            }
        }
        else
        {
            waitpid(p1[i],&stat_loc,WUNTRACED);
        }
        
    }
}

void printpipes(char ***pipes)
{
    for(int i=0;pipes[i]!=NULL;i++)
    {
        for(int j=0;pipes[i][j]!=NULL;j++)
        {
            printf("this is in pipes %s\n",pipes[i][j]);
        }
    }
}

int number_pipes(char ** tokens)
{
    int n=0;
    for(int i=0;tokens[i]!= NULL;i++)
    {
        if(strcmp(tokens[i],"|")==0)
            n++;
    }
    return n;
}


int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;

	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;	
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
			printf("%s $ ",findpwd());
			scanf("%[^\n]", line);
			getchar();
		}
		// printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

       //do whatever you want with the commands, here we just print them

		// for(i=0;tokens[i]!=NULL;i++){
		// 	printf("found token %s (remove this debug output later)\n", tokens[i]);
		//     }
        int y = checkpipes(tokens);
        printf("%d\n",y);
        if(y)
        {
            // char **pipes[MAX_NUM_TOKENS];
            int n = number_pipes(tokens);
            printf("%d\n",n);
            makepipes(tokens,n);
            // printpipes(pipes);
        }

        if(check_basic_commands(tokens))
            continue;
        
        if(foreground(tokens))
        {
            printf("hello\n");    
            applyfunctions_background(tokens);
        }
        else
            applyfunctions(tokens);
        
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
