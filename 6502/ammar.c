#include <stdio.h>
#include <stdint.h>

uint16_t a = 1;
uint8_t b = 0b00000001;


int main(){
    
    void write_data(uint8_t ss);
    write_data( b);
   
    
    return 0;
}
void write_data(uint8_t ss){
     for (int i = 7; i >= 0; i--) { 
        if (ss & (1 << i)) {  
            printf("yes\n");
        } else {             
            printf("no\n");
        }
    }
}

