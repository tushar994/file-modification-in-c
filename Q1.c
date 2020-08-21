
// C program to read nth byte of a file and 
// copy it to another file using lseek 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <fcntl.h> 
#include <string.h>
#include <sys/stat.h>


long long int RAM = 1000000;

void print_loader(double percentage){
    
    char value[5];

    value[4] = (char)(((int)(percentage*((double)100)))%10)+ '0';
    value[3] = (char)(((int)(percentage*((double)10)))%10) + '0';
    value[2] = (char)(((int)(percentage))%10) + '0';
    value[1]= (char)(((int)(percentage/((double)10)))%10) + '0';
    value[0]= (char)(((int)(percentage/((double)99)))%10) + '0';

    
    char okay[10];

    for(int i=1;i<=3;i++){
        okay[i] = value[i-1];
    }
    okay[4] = '.';
    okay[5] = value[3];
    okay[6] = value[4];

    okay[7] = '%';
    okay[0] = '\r';
    okay[8] = '[';
    okay[9] = '\0';
    // if(percentage/100 != 0){
    //     okay[1] = (char)(percentage/100) + '0';
    //     okay[2] = (char)((percentage%100)/10) + '0';
    //     okay[3] = (char)((percentage%10)) + '0';
    // }
    // else if((percentage%100)/10 != 0){
    //     okay[2] = (char)((percentage%100)/10)+ '0';
    //     okay[3] = (char)((percentage%10))+ '0';
    // }
    // else if((percentage%10) != 0){
    //     okay[3] = (char)((percentage%10))+ '0';
    // }
    write(STDOUT_FILENO, okay, strlen(okay));

    for(int i=1;i<=percentage/5;i++){
        write(STDOUT_FILENO, "#", strlen("#"));
    }
    for(int i=(percentage/5)+1;i<=20;i++){
        write(STDOUT_FILENO, " ", strlen(" "));
    } 
    write(STDOUT_FILENO, "]", strlen("]"));
}


int func(char arr[],char arr2[], int n, char path[]) 
{ 
    // Open the file for READ only. 
    int f_write = open(path, O_RDONLY); 

    if(f_write<1){
        perror("r2");
        return 1;
    }
    // this is to find the name of the file that has been given as input
    // first we find the starting index at which the name of the file is
    int path_size = strlen(path);
    int actual_path_size = path_size;
    path_size--;
    while(path_size>0){
        if(path[path_size]=='/'){
            path_size++;
            break;
        }
        path_size--;
    }
    // here we copy the name of the file into the array of characters called name;
    // ./Assignment/
    char start[13] = "./Assignment/";

    char name[actual_path_size - path_size + 1 + 13];

    for(int i=0;i<13;i++){
        name[i] = start[i];
    }

    for(int i=0;i<actual_path_size-path_size;i++){
        name[i+13] = path[path_size+i];
    }
    name[actual_path_size - path_size + 13] = '\0';
    // we now have the name of the file, along with it's path !!!


    
    // Open the file for WRITE and READ only. 
    int f_read = open(name, O_RDWR | O_CREAT  | O_TRUNC, 0600); 

    if(f_read<0){
        perror("r67");
        return 1;
    }
    // we find the size of the file in bytes
    long long int size = lseek(f_write, 0, SEEK_END);

    if(size<0){
        perror("r1");
        return 1;
    }
    // we start at the approrpiate place on the file
    long long int intial_back;
    if(size>RAM){
        intial_back = -1 *RAM;
    }
    else{
        intial_back = -1 * size;
    }
    lseek (f_write, intial_back, SEEK_END); 

    long long int count = size; 



    while ( size>0 ) 
    {   
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
        read(f_write, arr, current_read);

        // we flip te contents of arr, and store this into arr2
        for(int i=0;i<current_read;i++){
            arr2[current_read-1-i] = arr[i];
        }
        // we write contents of arr2 into the file
        write (f_read, arr2, current_read); 
        


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
        // we see how much is done, and show in the loading thing
        double percentage = ((double)((count-size)*100))/((double)(count));
        print_loader(percentage);

        
    } 

    close(f_write); 
    close(f_read);
    return 0;

} 
  
// Driver code 
int main(int argc, char *argv[]) 
{ 

    // I think this works
    mkdir("Assignment", 0700); 
  
    // use argc to see how many arguments have been passed
    // only valid case is two arguments
    // use argv[1] to access the pointer to the path string

    if(argc<2){
        write(STDOUT_FILENO, "Have not passed enough arguments\n", strlen("Have not passed enough arguments\n"));
        return 0;
    }
    else if(argc>2){
        write(STDOUT_FILENO, "Have passed too many arguments\n", strlen("Have passed too many arguments\n"));
        return 0;
    }


    char arr[RAM+7];
    char arr2[RAM+7]; 
    int n; 
    n = -2; 
  
    // Calling for the function 
    func(arr,arr2, n,argv[1]); 
    return 0; 
}