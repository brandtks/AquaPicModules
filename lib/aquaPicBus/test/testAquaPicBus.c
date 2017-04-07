#include <stdio.h>
#include <stdlib.h>
#include "../aquaPicBus.h"

void apbMessageHandler(void);

struct apbObjStruct apbInstStruct;
apbObj apbInst = &apbInstStruct;

#define NUM_CHANNELS 4
uint16_t ct[] = {0x1122, 0x3344, 0x5566, 0x7788};

int main(int argc, char** argv) {
    int i;
    
    apb_init (apbInst, 
            &apbMessageHandler, 
            0x01,
            5,
            NULL,
            0);
    
    apb_framing(apbInst);
    apb_framing(apbInst);
    
    {
        uint8_t message[] = {1, 10, 6, 1, 0, 0};
        uint8_t crc[2];
        apb_crc16(message, crc, 6);
        message[4] = crc[0];
        message[5] = crc[1];

        printf("Receiving message ");
        for (i = 0; i < 6; ++i) {
            printf("0x%02x ", message[i]);
            apb_run(apbInst, message[i]);
        }
    }
    
    {
        uint8_t message[] = {1, 10, 8, 1, 0x44, 0x33, 0, 0};
        uint8_t crc[2];
        apb_crc16(message, crc, 8);
        message[6] = crc[0];
        message[7] = crc[1];
        printf("Expected message  ");
        for (i = 0; i < 8; ++i) {
            printf("0x%02x ", message[i]);
            apb_run(apbInst, message[i]);
        }
        printf("\n");
    }
    
    
    apb_framing(apbInst);
    apb_framing(apbInst);
    
    {
        uint8_t message[] = {1, 1, 5, 0, 0};
        uint8_t crc[2];
        apb_crc16(message, crc, 5);
        message[3] = crc[0];
        message[4] = crc[1];

        printf("Receiving message ");
        for (i = 0; i < 5; ++i) {
            printf("0x%02x ", message[i]);
            apb_run(apbInst, message[i]);
        }
    }
    
    {
        uint8_t message[] = {1, 1, 5, 0, 0};
        uint8_t crc[2];
        apb_crc16(message, crc, 5);
        message[3] = crc[0];
        message[4] = crc[1];
        printf("Expected message  ");
        for (i = 0; i < 5; ++i) {
            printf("0x%02x ", message[i]);
            apb_run(apbInst, message[i]);
        }
        printf("\n");
    }
    
    return (EXIT_SUCCESS);
}


void apbMessageHandler(void) {
    printf("\n");
    switch (apbInst->function) {
        case 10: { //read single channel value
            uint8_t channel = apbInst->message[3];
            uint16_t value  = ct[channel];
            
            apb_initResponse(apbInst);
            apb_appendToResponse(apbInst, channel);
            apb_addToResponse(apbInst, &value, sizeof(uint16_t));
            apb_sendResponse(apbInst);
 
            break;
        }
        case 1: {
            apb_sendDefualtResponse(apbInst);
            break;
        }
        default:
            break;
    }
}
