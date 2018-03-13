#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int c1pid,c2pid,gc1pid,gc2pid,gc3pid;
int main(){
    c1pid = fork();
    if(c1pid == 0){
        printf("%d: 1st Child, Parent: %d \n",getpid(),getppid());
        sleep(1);
    }
    else{
        c2pid = fork();
        if (c2pid == 0){
            // 2nd child içi

            printf("%d: 2nd Child, Parent: %d \n",getpid(),getppid());
            system("mkdir logs"); // syscall 1
            gc1pid = fork();
            waitpid(gc1pid,NULL,0);
            if (gc1pid == 0){
                printf("%d: 1st GRAND Child, Parent: %d \n",getpid(),getppid());
                system("echo 'First Grand Child log file: pwd' > logs/logB_GC1.log && pwd >> logs/logB_GC1.log"); // syscall 2
                sleep(1);
            }
            else{
                // 2nd child içi
                gc2pid = fork();
                if(gc2pid == 0){
                    printf("%d: 2nd GRAND Child, Parent: %d \n",getpid(),getppid());
                    system("echo 'Second Grand Child log file: pstree' > logs/logB_GC2.log && pstree >> logs/logB_GC2.log"); // syscall 3
                    sleep(2);
                }
                else{
                    // 2nd child içi
                    gc3pid = fork();
                    if(gc3pid == 0){
                        printf("%d: 3rd GRAND Child, Parent: %d \n",getpid(),getppid());
                        system("echo 'Third Grand Child log file: whoami' > logs/logB_GC3.log && whoami >> logs/logB_GC3.log"); // syscall 4
                        sleep(3);
                    }
                    else{

                    }
                }
            }
        }
        else{
        }
        sleep(4);
    }
}
