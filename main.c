#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define tprintf(n, args...) do{for(int TPFL=(n);TPFL>1;--TPFL){printf(" │  ");}if(n){printf(" ├──");}printf(args);}while(0)

void print_dir(int, char*);

int main(int argc, char** argv)
{
    if(argc > 1) print_dir(0, argv[1]);
    else print_dir(0, "./");
}

void print_dir(int r, char* directory)
{
    int err;
    DIR* d;
    struct dirent* entry;
    char cwd[1024];

    getcwd(cwd, sizeof(cwd));

    d = opendir(directory);
    if(d)
    {
        if(directory[0] == '.') tprintf(r, "(H) %s\n", directory);
        else tprintf(r, "[D] %s\n", directory);
        while((entry = readdir(d)))
        {
            if(entry->d_type == DT_DIR)
            {
                if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
                {
                    tprintf(r + 1, "(H) %s\n", entry->d_name);
                } else 
                {
                    chdir(directory);
                    print_dir(r + 1, entry->d_name);
                    chdir(cwd);
                }
            }
        }

        rewinddir(d);

        while((entry = readdir(d)))
        {
            if(entry->d_type == DT_REG)
            {
                tprintf(r + 1, "%s\n", entry->d_name);
            }
        }

        closedir(d);
    }
    else
    {
        err = errno;
        tprintf(r, "{E} Unable to Open: %s [%d: %s]\n", directory, err, strerror(err));
    }

}