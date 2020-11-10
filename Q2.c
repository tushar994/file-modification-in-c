#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <fcntl.h> 
#include <string.h>
#include <sys/stat.h>


long long int RAM = 1000000;


int print_the_line(char user[], char op[], char name[], int condition){
    
    write(STDOUT_FILENO, user, strlen(user));
    write(STDOUT_FILENO, " has ", strlen(" has "));
    write(STDOUT_FILENO, op, strlen(op));
    write(STDOUT_FILENO, " permissions on ", strlen(" permissions on "));
    write(STDOUT_FILENO, name, strlen(name));


    if((condition)){
        write(STDOUT_FILENO, ": Yes\n", strlen(": Yes\n"));
    }
    else{
        write(STDOUT_FILENO, ": No\n", strlen(": No\n"));
    }
    return 0;
}



int print_permissions(char argv[],int index){
    struct stat fileStat;
    if(stat(argv, &fileStat) < 0){    
        perror("r1");
        return 1;
    }

    // i store the name of what im working with in name_of_file
    char name_of_file[10];
    if(index==1){
        char okay[10] = "newfile";

        for(int i=0;i<10;i++){
            name_of_file[i] = okay[i];
        }
    }
    if(index==2){
        char okay[10] = "oldfile";

        for(int i=0;i<10;i++){
            name_of_file[i] = okay[i];
        }
    }
    if(index==3){
        char okay[10] = "directory";

        for(int i=0;i<10;i++){
            name_of_file[i] = okay[i];
        }
    }
    
    // here we start printing permissions
    char user[10] = "user";
    char group[10] = "group";
    char other[10] = "other";
    char read[10] = "read";
    char write[10] = "write";
    char execute[10] = "execute";

    // for user
    print_the_line(user,read,name_of_file,(fileStat.st_mode & S_IRUSR));
    print_the_line(user,write,name_of_file,(fileStat.st_mode & S_IWUSR));
    print_the_line(user,execute,name_of_file,(fileStat.st_mode & S_IXUSR));    

    //for group
    print_the_line(group,read,name_of_file,(fileStat.st_mode & S_IRGRP));
    print_the_line(group,write,name_of_file,(fileStat.st_mode & S_IWGRP));
    print_the_line(group,execute,name_of_file,(fileStat.st_mode & S_IXGRP));

    //for others 
    print_the_line(other,read,name_of_file,(fileStat.st_mode & S_IROTH));
    print_the_line(other,write,name_of_file,(fileStat.st_mode & S_IWOTH));
    print_the_line(other,execute,name_of_file,(fileStat.st_mode & S_IXOTH));

    return 0;



}






int main(int argc, char **argv)
{

    if(argc != 4){
        write(STDOUT_FILENO, "You have not passed exactly 3 paths.", strlen("You have not passed exactly 3 paths."));
        return 1;
    }
    
    char arr1[RAM+ 5];
    char arr2[RAM+ 5];
    char arr3[RAM+ 5];

    int one_does_not_exist=1;

    int f_write = open(argv[1], O_RDONLY); 

    if(f_write<1){
        // perror("r1");
        one_does_not_exist=0;
    }

    struct stat file1Stat;

    int okay1 = stat(argv[1], &file1Stat);
    if(okay1==0 && S_ISDIR(file1Stat.st_mode)){
        one_does_not_exist = 0;
    }


    int two_does_not_exist=1;
    int f_rev = open(argv[2], O_RDONLY); 
    if(f_rev<1){
        // perror("r2");
        two_does_not_exist=0;
    }

    struct stat file2Stat;

    int okay2 = stat(argv[2], &file2Stat);
    if(okay2==0 && S_ISDIR(file2Stat.st_mode)){
        two_does_not_exist = 0;
    }
    // write(STDOUT_FILENO, "okaybro1\n", strlen("okaybro1\n"));

    long long int size = lseek(f_write, 0, SEEK_END);
    long long int size2 = lseek(f_rev, 0, SEEK_END);
    if(size<0){
        // perror("r3");
        one_does_not_exist=0;
        // return 1;
    }
    if(size2<0){
        // perror("r4");
        two_does_not_exist=0;
        // return 1;
    }
    write(STDOUT_FILENO, "\n", strlen("\n"));
    if(one_does_not_exist==0){
        write(STDOUT_FILENO, "First file (first argument) given doesn't exist/can't be opened\n", strlen("First file (first argument) given doesn't exist/can't be opened\n"));
    }
    if(two_does_not_exist==0){
        write(STDOUT_FILENO, "Second file (second argument) given doesn't exist/can't be opened\n", strlen("Second file (second argument) given doesn't exist/can't be opened\n"));
    }
    if(one_does_not_exist && two_does_not_exist){
        if(size!=size2){
            
            write(STDOUT_FILENO, "Whether file contents are reversed in newfile: No\n", strlen("Whether file contents are reversed in newfile: No\n"));
        }
        else{
            int flag = 1;
            long long int intial_back;
            if(size>RAM){
                intial_back = -1 *RAM;
            }
            else{
                intial_back = -1 * size;
            }
            //f_write will be read from reverse 
            lseek (f_write, intial_back, SEEK_END);
            lseek (f_rev, 0, SEEK_SET);
            long long int count = size;

            while ( size>0 && flag){   
                // we see how much we need to read this time
                // it starts at the staring of the place we need to read from
                long long int current_read;
                if(size>RAM){
                    current_read = RAM;
                }
                else{
                    current_read = size;
                }
                // we read the appropriate thing and store it in arr
                read(f_write, arr1, current_read);
                read(f_rev, arr2, current_read);
                // we flip te contents of arr, and store this into arr2
                for(int i=0;i<current_read;i++){
                    arr3[current_read-1-i] = arr2[i];
                }
                for(int i=0;i<current_read;i++){
                    if(arr3[i]!=arr1[i]){
                        flag = 0;
                        break;
                    }
                }


                


                // we decrease size by the apprpriate amount
                size-=current_read;

                // we find the new location to read from
                long long int current_write;
                if(size>RAM){
                    long long int back = 2*RAM;
                    back = -1 * back;
                    lseek (f_write, back, SEEK_CUR); 
                }
                else{
                    lseek (f_write, 0, SEEK_SET); 
                }
                
            }
            if(flag){
                write(STDOUT_FILENO, "Whether file contents are reversed in newfile: Yes\n", strlen("Whether file contents are reversed in newfile: Yes\n"));
            }
            else{
                write(STDOUT_FILENO, "Whether file contents are reversed in newfile: No\n", strlen("Whether file contents are reversed in newfile: No\n"));
            }
            close(f_write); 
            close(f_rev);
        }
    }
    write(STDOUT_FILENO, "\n", strlen("\n"));










    struct stat dirStat;

    int okay = stat(argv[3], &dirStat);
    int isdir = 1;
    if(okay==0 && S_ISDIR(dirStat.st_mode)){
        write(STDOUT_FILENO, "Directory exists: Yes\n", strlen("Directory exists: Yes\n"));
    }
    else{
        write(STDOUT_FILENO, "Directory exists: No\n", strlen("Directory exists: No\n"));
        isdir = 0;
    }


    // here we print permissions
    if(one_does_not_exist){
        write(STDOUT_FILENO, "\nPermissions for newfile (first argument)\n", strlen("\nPermissions for newfile (first argument)\n"));
        print_permissions(argv[1],1);
    }
    if(two_does_not_exist){
        write(STDOUT_FILENO, "\nPermissions for oldfile (second argument)\n", strlen("\nPermissions for oldfile (second argument)\n"));
        print_permissions(argv[2],2);
    }

    if(isdir){
        write(STDOUT_FILENO, "\nPermissions for directory (third argument)\n", strlen("\nPermissions for directory (third argument)\n"));
        print_permissions(argv[3],3);
    }


    return 0;
}