#include <unistd.h>
#include <sys/reboot.h>
 
int main(){
    
    int reboot(int cmd);

    
    reboot(RB_AUTOBOOT);
    
    return 0;


}

