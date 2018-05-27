#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#define VERSION "1.0"
#define ERR_DIR EISDIR
#define DYES 1
#define DNO 0

#define C_ERR "\033[31m"
#define C_WAR "\033[33m"
#define C_INF "\033[32m"
#define C_CLE "\033[0m"

#define ERR_QUIT(msg) {printf("%s%s%s: %s%s%s\n",C_INF,msg,C_CLE,C_ERR,strerror(errno),C_CLE);exit(-1);}
#define ERR_CONTINUE(msg) {printf("%s%s%s: %s%s%s\n",C_INF,msg,C_CLE,C_ERR,strerror(errno),C_CLE);continue;}
#define ERR_BREAK(msg) {printf("%s%s%s: %s%s%s\n",C_INF,msg,C_CLE,C_ERR,strerror(errno),C_CLE);break;}
#define ERR_RETURN(msg,ret) {printf("%s%s%s: %s%s%s\n",C_INF,msg,C_CLE,C_ERR,strerror(errno),C_CLE);return ret;}

#define INTERACTIVE_TEST(func,arg) \
{\
	bool yes=false;\
	char c;\
	if((opt->i && opt->interactive.type != NEVER) && !opt->f)\
	{\
		while(true)\
		{\
			printf("rm: remove file '%s%s%s'?%s[y/n]%s",C_INF,real_path(arg,opt->path),C_CLE,C_WAR,C_CLE);\
			c=getchar();\
			while(getchar() != '\n')\
				continue;\
			if(c == 'y' || c == 'Y')\
				yes=true;\
			else if(c == 'n' || c == 'N')\
				yes=false;\
			else\
				continue;\
			break;\
		}\
		if(yes)\
		{\
			if(opt->v)\
				printf("remove '%s%s%s' . . .\n",C_INF,real_path(arg,opt->path),C_CLE);\
			return func(arg);\
		}\
		else\
			return 0;\
	}\
}

enum rm_interactive
{
	NEVER,
	ONCE,
	ALWAYS
};

typedef struct option OPT;
typedef struct
{
	bool f;
	bool i;
	bool I;
	struct
	{
		bool t;
		enum rm_interactive type;
	}interactive;
	bool preserve_root;
	bool r;
	bool d;
	bool v;

	char *path;
}RM;

void usage(void);
void rm_options_init(RM *opt);
int rm_interactive_set_arg(char *arg,enum rm_interactive *type);
int remove_file(char *filename,RM *opt);
int remove_dir(char *dir_name,RM *opt);
int _remove_file(char *filename,RM *opt);
char *real_path(char *path,char *p);
int display_test(char *file,int num,RM *opt);
char *dir_name(char *path);

int main(int argc,char **argv)
{
	static OPT opt[]={
		{"force",no_argument,NULL,'f'},
		{"interactive",optional_argument,NULL,'t'},
		{"no-preserve-root",no_argument,NULL,'n'},
		{"preserve-root",no_argument,NULL,'p'},
		{"recursive",no_argument,NULL,'r'},
		{"dir",no_argument,NULL,'d'},
		{"verbose",no_argument,NULL,'v'},
		{"help",no_argument,NULL,'h'},
		{"version",no_argument,NULL,'V'},
		{0,0,0,0}
	};
	char *optstring="fiIrRdvVh";
	int ch;
	RM rm_opt;
	char *filename;

	if(argc < 2)
		usage();
	rm_options_init(&rm_opt);

	while((ch=getopt_long(argc,argv,optstring,opt,NULL)) != -1)
	{
		switch(ch)
		{
			case 'f':
				rm_opt.f=true;
				break;
				
			case 'i':
				rm_opt.i=true;
				break;

			case 'I':
				rm_opt.I=true;
				break;

			case 'r':
			case 'R':
				rm_opt.r=true;
				break;

			case 'd':
				rm_opt.d=true;
				break;
				
			case 'v':
				rm_opt.v=true;
				break;

			case 'V':
				printf("rm %s%s%s by %sBrisk%s\nLicense: GPL\nEmail: %sbriskgreen@163.com%s\n",C_INF,VERSION,C_CLE,C_ERR,C_CLE,C_ERR,C_CLE);
				return 0;

			case 'h':
				usage();

			case 't':
				rm_opt.interactive.t=true;
				if(rm_interactive_set_arg(optarg,&rm_opt.interactive.type) == -1)
				{
					printf("Unknown %s%s%s WHEN options!\n",C_ERR,optarg,C_CLE);
					exit(-1);
				}
				break;

			case 'n':
				rm_opt.preserve_root=false;
				break;

			case 'p':
				rm_opt.preserve_root=true;
				break;

			case '?':
				break;

			default:
				printf("?? getopt returned character code %s0%o%s ??\n",C_ERR,ch,C_CLE);
		}
	}

//	filename=argv[optind];
	if(argv[optind] == NULL)
		usage();

	if(rm_opt.interactive.t)
	{
		switch(rm_opt.interactive.type)
		{
			case NEVER:
				rm_opt.I=false;
				break;
				
			case ONCE:
				rm_opt.I=true;
				break;

			case ALWAYS:
				rm_opt.i=true;
				break;
		}
	}

	if(rm_opt.I && !rm_opt.i)
	{
		if(display_test(argv[optind],argc-optind,&rm_opt) == DYES)
		{
			bool yes=false;
			char c;

			while(true)
			{
				printf("rm: remove?%s[y/n]%s",C_WAR,C_CLE);
				c=getchar();
				while(getchar() != '\n')
					continue;

				if(c == 'y' || c == 'Y')
					yes=true;
				else if(c == 'n' || c == 'N')
					yes=false;
				else
					continue;
				
				break;
			}

			if(!yes)
				return 0;
		}
	}
/*	if(remove_file(filename,&rm_opt) == -1)
		ERR_QUIT(filename);*/
	while(argv[optind])
	{
		if((strcmp(argv[optind],"/") == 0) && rm_opt.preserve_root)
		{
			printf("rm: %sit is dangerous to operate recursively on '/'%s\n",C_ERR,C_CLE);
			printf("rm: %suse --no-preserve-root to override this failsafe%s\n",C_ERR,C_CLE);
			++optind;
			continue;
		}

		rm_opt.path=dir_name(argv[optind]);

		if(remove_file(argv[optind],&rm_opt) == -1)
			printf("%s%s%s: %s%s%s\n",C_INF,argv[optind],C_CLE,C_ERR,strerror(errno),C_CLE);

		++optind;
	}

	return 0;
}

void usage(void)
{
	char *help="Usage: rm [OPTION]... FILE...\nRemove (unlink) the FILE(s).\n\n  -f, --force\tignore nonexistent files and arguments, never prompt\n  -i\tprompt before every removal\n  -I\tprompt once before removing more than three files, or when removing recursively; less intrusive than -i,while still giving protection against most mistakes\n      --interactive[=WHEN]  prompt according to WHEN: never, once (-I), or always (-i); without WHEN, prompt always\n      --no-preserve-root  do not treat \'/\' specially\n      --preserve-root   do not remove \'/\' (default)\n  -r, -R, --recursive\tremove directories and their contents recursively\n  -d, --dir\tremove empty directories\n  -v, --verbose\texplain what is being done\n  -h, --help\tdisplay this help and exit\n  -V, --version\toutput version information and exit";

	puts(help);
	exit(0);
} 

void rm_options_init(RM *opt)
{
	opt->f=false;
	opt->i=false;
	opt->I=false;
	opt->interactive.t=false;
	opt->interactive.type=ALWAYS;
	opt->preserve_root=true;
	opt->r=false;
	opt->d=false;
	opt->v=false;
}

int rm_interactive_set_arg(char *arg,enum rm_interactive *type)
{
	if(arg == NULL)
		*type=ALWAYS;
	else if(strcmp(arg,"never") == 0)
		*type=NEVER;
	else if(strcmp(arg,"once") == 0)
		*type=ONCE;
	else if(strcmp(arg,"always") == 0)
		*type=ALWAYS;
	else
		return -1;

	return 0;
}

int remove_file(char *filename,RM *opt)
{
	struct stat buf;

	if(stat(filename,&buf) == -1)
		return -1;

	if(S_ISDIR(buf.st_mode))
	{
		if(opt->d)
		{
			INTERACTIVE_TEST(rmdir,filename);

			if(opt->v)
				printf("remove '%s%s%s' . . .\n",C_INF,filename,C_CLE);
			return rmdir(filename);
		}
		
		if(!opt->r)
		{
			errno=ERR_DIR;
			return -1;
		}

		return remove_dir(filename,opt);
	}

	return _remove_file(filename,opt);
}

int remove_dir(char *dir_name,RM *opt)
{
	DIR *dirp;
	struct dirent *dir;
	struct stat buf;
	char *current_path;

	current_path=getcwd(NULL,0);
	if(current_path == NULL)
		ERR_RETURN(dir_name,-1);

	if((dirp=opendir(dir_name)) == NULL)
		ERR_RETURN(dir_name,-1);
	if(chdir(dir_name) == -1)
		ERR_RETURN(dir_name,-1);

	while((dir=readdir(dirp)) != NULL)
	{
		if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0)
			continue;

		if(stat(dir->d_name,&buf) == -1)
			ERR_BREAK(dir->d_name);

		if(S_ISDIR(buf.st_mode))
		{
			if(remove_dir(dir->d_name,opt) == -1)
				ERR_CONTINUE(dir->d_name);
		}
		else
		{
			if(_remove_file(dir->d_name,opt) == -1)
				ERR_CONTINUE(dir->d_name);
		}
	}

	closedir(dirp);
	if(chdir(current_path) == -1)
		ERR_QUIT(dir_name);

	INTERACTIVE_TEST(rmdir,dir_name);
	if(opt->v)
	{
		if(strcmp(dir_name,opt->path) == 0)
			printf("remove '%s%s%s' . . .\n",C_INF,opt->path,C_CLE);
		else
			printf("remove '%s%s%s' . . .\n",C_INF,real_path(dir_name,opt->path),C_CLE);
	}

	return rmdir(dir_name);
}

int _remove_file(char *filename,RM *opt)
{
	INTERACTIVE_TEST(remove,filename);

	if(opt->v)
		printf("remove '%s%s%s' . . .\n",C_INF,real_path(filename,opt->path),C_CLE);

	return remove(filename);
}

char *real_path(char *path,char *p)
{
	char *current_path;
	static char res[PATH_MAX];
	struct stat buf;
	char *ps;

	current_path=getcwd(NULL,0);
	if(current_path == NULL)
		ERR_QUIT("getcwd");

	memset(res,0,PATH_MAX);
	if(stat(path,&buf) == -1)
		ERR_QUIT(path);

	if(S_ISDIR(buf.st_mode))
		snprintf(res,PATH_MAX,"%s",strstr(current_path,p));
	else
		snprintf(res,PATH_MAX,"%s",path);

	return res;
}

int display_test(char *file,int num,RM *opt)
{
	struct stat buf;
	int i=0;
	DIR *dirp;
	struct dirent *dir;

	if(num >= 3)
		return DYES;

	if(stat(file,&buf) == -1)
		ERR_QUIT(file);

	if(S_ISDIR(buf.st_mode))
	{
		if(opt->r)
			i=1;

		if((dirp=opendir(file)) == NULL)
			ERR_QUIT(file);

		while((dir=readdir(dirp)) != NULL)
		{
			if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0)
				continue;

			++i;
			if(i >= 3)
			{
				closedir(dirp);
				return DYES;
			}
		}

		closedir(dirp);
	}

	return DNO;
}

char *dir_name(char *path)
{
	static char res[PATH_MAX];
	size_t len;
	struct stat buf;

	if(stat(path,&buf) == -1)
		ERR_QUIT(path);

	if(S_ISDIR(buf.st_mode))
	{
		char *p;
		
		p=path+strlen(path)-1;
		while(*p == '/')
			--p;
		*(p+1)=0;

		return path;
	}
	else
	{
		len=strlen(path)-1;

		while(len && path[len] != '/')
			--len;

		if(!len)
			return path;
	
		memset(res,0,PATH_MAX);
		snprintf(res,len+1,path);
		return res;
	}
}
