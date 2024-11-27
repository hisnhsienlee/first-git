extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "sms/SMS.h"
#include <locale.h>
#include "sms/utf.h"

#define DSC_to_msg(DSC) (DSC == 0 ? "Bit7" : (DSC == 1 ? "Bit8" : "UCS2"))

int main() {

    char *out;
    int len;
    out = (char*)malloc(1024);

    struct SMS_Struct s = PDUDecoding("0791889623048015040A81907375551600003240313102132302523A");
    
    printf("SMSC address: %s\n", s.SCA);
    printf("Transmit adderss: %s\n", s.OA);
    printf("Timestamps: %s\n", s.SCTS);
    printf("Text: %s\n", s.UD);
    printf("Text Type: %s\n", DSC_to_msg(s.DCS));
    printf("Msg type: %d\n", s.MC);
    
    printf("\n测试编码\n");
    const char *msg = "TEST" ;
    struct PDUS *pdus = PDUEncoding("0937575561", msg, NULL);
    printf("长短信编码：\n");

    for (int i = 0; i < pdus->count; i++) {
        printf("第 %d 条:\n", i + 1);
        printf("%s\n", pdus->PDU[i]);
    }

    return 0;
}
