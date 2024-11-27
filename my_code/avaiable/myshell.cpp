
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#define MAXLINE 512

enum {
    SND_DEVICE_NONE = 0,

    /* Playback devices */
    SND_DEVICE_MIN,
    SND_DEVICE_OUT_BEGIN = SND_DEVICE_MIN,
    SND_DEVICE_OUT_HANDSET = SND_DEVICE_OUT_BEGIN,
    SND_DEVICE_OUT_SPEAKER,
    SND_DEVICE_OUT_SPEAKER_EXTERNAL_1,
    SND_DEVICE_OUT_SPEAKER_EXTERNAL_2,
    SND_DEVICE_OUT_SPEAKER_REVERSE,
    SND_DEVICE_OUT_SPEAKER_SAFE,
    SND_DEVICE_OUT_SPEAKER_VBAT,
    SND_DEVICE_OUT_LINE,
    SND_DEVICE_OUT_HEADPHONES,
    SND_DEVICE_OUT_HEADPHONES_DSD,
    SND_DEVICE_OUT_HEADPHONES_44_1,
    SND_DEVICE_OUT_SPEAKER_AND_HEADPHONES,
    SND_DEVICE_OUT_SPEAKER_SAFE_AND_HEADPHONES,
    SND_DEVICE_OUT_SPEAKER_AND_LINE,
    SND_DEVICE_OUT_SPEAKER_SAFE_AND_LINE,
    SND_DEVICE_OUT_SPEAKER_AND_HEADPHONES_EXTERNAL_1,
    SND_DEVICE_OUT_SPEAKER_AND_HEADPHONES_EXTERNAL_2,
    SND_DEVICE_OUT_VOICE_HANDSET,
    SND_DEVICE_OUT_VOICE_SPEAKER,
    SND_DEVICE_OUT_VOICE_SPEAKER_STEREO,
    SND_DEVICE_OUT_VOICE_SPEAKER_VBAT,
    SND_DEVICE_OUT_VOICE_SPEAKER_2,
    SND_DEVICE_OUT_VOICE_SPEAKER_2_VBAT,
    SND_DEVICE_OUT_VOICE_HEADPHONES,
    SND_DEVICE_OUT_VOICE_LINE,
    SND_DEVICE_OUT_HDMI,
    SND_DEVICE_OUT_SPEAKER_AND_HDMI,
    SND_DEVICE_OUT_DISPLAY_PORT,
    SND_DEVICE_OUT_SPEAKER_AND_DISPLAY_PORT,
    SND_DEVICE_OUT_BT_SCO,
    SND_DEVICE_OUT_BT_SCO_WB,
    SND_DEVICE_OUT_BT_A2DP,
    SND_DEVICE_OUT_SPEAKER_AND_BT_A2DP,
    SND_DEVICE_OUT_SPEAKER_SAFE_AND_BT_A2DP,
    SND_DEVICE_OUT_VOICE_HANDSET_TMUS,
    SND_DEVICE_OUT_SPEAKER_AND_BT_SCO,
    SND_DEVICE_OUT_SPEAKER_SAFE_AND_BT_SCO,
    SND_DEVICE_OUT_SPEAKER_AND_BT_SCO_WB,
    SND_DEVICE_OUT_SPEAKER_SAFE_AND_BT_SCO_WB,
    SND_DEVICE_OUT_VOICE_TTY_FULL_HEADPHONES,
    SND_DEVICE_OUT_VOICE_TTY_VCO_HEADPHONES,
    SND_DEVICE_OUT_VOICE_TTY_HCO_HANDSET,
    SND_DEVICE_OUT_VOICE_TTY_FULL_USB,
    SND_DEVICE_OUT_VOICE_TTY_VCO_USB,
    SND_DEVICE_OUT_VOICE_HAC_HANDSET,
    SND_DEVICE_OUT_VOICE_TX,
    SND_DEVICE_OUT_VOICE2_TX,
    SND_DEVICE_OUT_VOICE_MUSIC_TX,
    SND_DEVICE_OUT_VOICE_SPEAKER_HFP,
    SND_DEVICE_OUT_AFE_PROXY,
    SND_DEVICE_OUT_USB_HEADSET,
    SND_DEVICE_OUT_USB_HEADPHONES,
    SND_DEVICE_OUT_SPEAKER_AND_USB_HEADSET,
    SND_DEVICE_OUT_SPEAKER_SAFE_AND_USB_HEADSET,
    SND_DEVICE_OUT_VOICE_USB_HEADPHONES,
    SND_DEVICE_OUT_VOICE_USB_HEADSET,
    SND_DEVICE_OUT_USB_HEADSET_SPEC,
    SND_DEVICE_OUT_TRANSMISSION_FM,
    SND_DEVICE_OUT_ANC_HEADSET,
    SND_DEVICE_OUT_ANC_FB_HEADSET,
    SND_DEVICE_OUT_VOICE_ANC_HEADSET,
    SND_DEVICE_OUT_VOICE_ANC_FB_HEADSET,
    SND_DEVICE_OUT_SPEAKER_AND_ANC_HEADSET,
    SND_DEVICE_OUT_SPEAKER_AND_ANC_FB_HEADSET,
    SND_DEVICE_OUT_ANC_HANDSET,
    SND_DEVICE_OUT_SPEAKER_PROTECTED,
    SND_DEVICE_OUT_VOICE_SPEAKER_PROTECTED,
    SND_DEVICE_OUT_VOICE_SPEAKER_2_PROTECTED,
    SND_DEVICE_OUT_VOICE_SPEAKER_STEREO_PROTECTED,
    SND_DEVICE_OUT_SPEAKER_PROTECTED_VBAT,
    SND_DEVICE_OUT_VOICE_SPEAKER_PROTECTED_VBAT,
    SND_DEVICE_OUT_VOICE_SPEAKER_2_PROTECTED_VBAT,
    SND_DEVICE_OUT_SPEAKER_WSA,
    SND_DEVICE_OUT_VOICE_SPEAKER_WSA,
    SND_DEVICE_OUT_VOICE_SPEAKER_2_WSA,
    SND_DEVICE_OUT_SPEAKER_PROTECTED_RAS,
    SND_DEVICE_OUT_SPEAKER_PROTECTED_VBAT_RAS,
    SND_DEVICE_OUT_VOICE_SPEAKER_AND_VOICE_HEADPHONES,
    SND_DEVICE_OUT_VOICE_SPEAKER_AND_VOICE_ANC_HEADSET,
    SND_DEVICE_OUT_VOICE_SPEAKER_AND_VOICE_ANC_FB_HEADSET,
    SND_DEVICE_OUT_VOICE_SPEAKER_STEREO_AND_VOICE_HEADPHONES,
    SND_DEVICE_OUT_VOICE_SPEAKER_STEREO_AND_VOICE_ANC_HEADSET,
    SND_DEVICE_OUT_VOICE_SPEAKER_STEREO_AND_VOICE_ANC_FB_HEADSET,
    SND_DEVICE_OUT_BUS_MEDIA,
    SND_DEVICE_OUT_BUS_SYS,
    SND_DEVICE_OUT_BUS_NAV,
    SND_DEVICE_OUT_BUS_PHN,
    SND_DEVICE_OUT_SPEAKER2,
    SND_DEVICE_OUT_SPEAKER3,
    SND_DEVICE_OUT_SPEAKER_DSD,
    SND_DEVICE_OUT_VOICE_DL_TX,
    SND_DEVICE_OUT_ECALL_HANDSET,
    SND_DEVICE_OUT_ECALL_SPEAKER,
    SND_DEVICE_OUT_ECALL_HEADPHONES,
    SND_DEVICE_OUT_END,

    /*
     * Note: IN_BEGIN should be same as OUT_END because total number of devices
     * SND_DEVICES_MAX should not exceed MAX_RX + MAX_TX devices.
     */
    /* Capture devices */
    SND_DEVICE_IN_BEGIN = SND_DEVICE_OUT_END,
    SND_DEVICE_IN_HANDSET_MIC  = SND_DEVICE_IN_BEGIN,
    SND_DEVICE_IN_HANDSET_MIC_EXTERNAL,
    SND_DEVICE_IN_HANDSET_MIC_AEC,
    SND_DEVICE_IN_HANDSET_MIC_NS,
    SND_DEVICE_IN_HANDSET_MIC_AEC_NS,
    SND_DEVICE_IN_HANDSET_DMIC,
    SND_DEVICE_IN_HANDSET_DMIC_AEC,
    SND_DEVICE_IN_HANDSET_DMIC_NS,
    SND_DEVICE_IN_HANDSET_DMIC_AEC_NS,
    SND_DEVICE_IN_SPEAKER_MIC,
    SND_DEVICE_IN_SPEAKER_MIC_AEC,
    SND_DEVICE_IN_SPEAKER_MIC_NS,
    SND_DEVICE_IN_SPEAKER_MIC_AEC_NS,
    SND_DEVICE_IN_SPEAKER_DMIC,
    SND_DEVICE_IN_SPEAKER_DMIC_AEC,
    SND_DEVICE_IN_SPEAKER_DMIC_NS,
    SND_DEVICE_IN_SPEAKER_DMIC_AEC_NS,
    SND_DEVICE_IN_HEADSET_MIC,
    SND_DEVICE_IN_HEADSET_MIC_AEC,
    SND_DEVICE_IN_HEADSET_MIC_FLUENCE,
    SND_DEVICE_IN_VOICE_SPEAKER_MIC,
    SND_DEVICE_IN_VOICE_SPEAKER_MIC_HFP,
    SND_DEVICE_IN_VOICE_SPEAKER_MIC_HFP_MMSECNS,
    SND_DEVICE_IN_VOICE_HEADSET_MIC,
    SND_DEVICE_IN_SPDIF,
    SND_DEVICE_IN_HDMI_MIC,
    SND_DEVICE_IN_HDMI_ARC,
    SND_DEVICE_IN_BT_SCO_MIC,
    SND_DEVICE_IN_BT_SCO_MIC_NREC,
    SND_DEVICE_IN_BT_SCO_MIC_WB,
    SND_DEVICE_IN_BT_SCO_MIC_WB_NREC,
    SND_DEVICE_IN_BT_A2DP,
    SND_DEVICE_IN_CAMCORDER_MIC,
    SND_DEVICE_IN_VOICE_DMIC,
    SND_DEVICE_IN_VOICE_DMIC_TMUS,
    SND_DEVICE_IN_VOICE_SPEAKER_DMIC,
    SND_DEVICE_IN_VOICE_SPEAKER_TMIC,
    SND_DEVICE_IN_VOICE_SPEAKER_QMIC,
    SND_DEVICE_IN_VOICE_TTY_FULL_HEADSET_MIC,
    SND_DEVICE_IN_VOICE_TTY_VCO_HANDSET_MIC,
    SND_DEVICE_IN_VOICE_TTY_HCO_HEADSET_MIC,
    SND_DEVICE_IN_VOICE_TTY_FULL_USB_MIC,
    SND_DEVICE_IN_VOICE_TTY_HCO_USB_MIC,
    SND_DEVICE_IN_VOICE_REC_MIC,
    SND_DEVICE_IN_VOICE_REC_MIC_NS,
    SND_DEVICE_IN_VOICE_REC_MIC_AEC,
    SND_DEVICE_IN_VOICE_REC_MIC_AEC_NS,
    SND_DEVICE_IN_VOICE_REC_DMIC_STEREO,
    SND_DEVICE_IN_VOICE_REC_DMIC_FLUENCE,
    SND_DEVICE_IN_VOICE_REC_HEADSET_MIC,
    SND_DEVICE_IN_VOICE_RX,
    SND_DEVICE_IN_USB_HEADSET_MIC,
    SND_DEVICE_IN_USB_HEADSET_MIC_AEC,
    SND_DEVICE_IN_VOICE_USB_HEADSET_MIC,
    SND_DEVICE_IN_UNPROCESSED_USB_HEADSET_MIC,
    SND_DEVICE_IN_VOICE_RECOG_USB_HEADSET_MIC,
    SND_DEVICE_IN_USB_HEADSET_MULTI_CHANNEL_MIC,
    SND_DEVICE_IN_USB_HEADSET_MULTI_CHANNEL_MIC_AEC,
    SND_DEVICE_IN_UNPROCESSED_USB_HEADSET_MULTI_CHANNEL_MIC,
    SND_DEVICE_IN_VOICE_RECOG_USB_HEADSET_MULTI_CHANNEL_MIC,
    SND_DEVICE_IN_CAPTURE_FM,
    SND_DEVICE_IN_AANC_HANDSET_MIC,
    SND_DEVICE_IN_QUAD_MIC,
    SND_DEVICE_IN_HANDSET_DMIC_STEREO,
    SND_DEVICE_IN_SPEAKER_DMIC_STEREO,
    SND_DEVICE_IN_CAPTURE_VI_FEEDBACK,
    SND_DEVICE_IN_CAPTURE_VI_FEEDBACK_MONO_1,
    SND_DEVICE_IN_CAPTURE_VI_FEEDBACK_MONO_2,
    SND_DEVICE_IN_VOICE_SPEAKER_DMIC_BROADSIDE,
    SND_DEVICE_IN_SPEAKER_DMIC_BROADSIDE,
    SND_DEVICE_IN_SPEAKER_DMIC_AEC_BROADSIDE,
    SND_DEVICE_IN_SPEAKER_DMIC_NS_BROADSIDE,
    SND_DEVICE_IN_SPEAKER_DMIC_AEC_NS_BROADSIDE,
    SND_DEVICE_IN_VOICE_FLUENCE_DMIC_AANC,
    SND_DEVICE_IN_HANDSET_QMIC,
    SND_DEVICE_IN_HANDSET_QMIC_AEC,
    SND_DEVICE_IN_SPEAKER_QMIC_AEC,
    SND_DEVICE_IN_SPEAKER_QMIC_NS,
    SND_DEVICE_IN_SPEAKER_QMIC_AEC_NS,
    SND_DEVICE_IN_VOICE_REC_QMIC_FLUENCE,
    SND_DEVICE_IN_THREE_MIC,
    SND_DEVICE_IN_HANDSET_TMIC_FLUENCE_PRO,
    SND_DEVICE_IN_HANDSET_TMIC,
    SND_DEVICE_IN_HANDSET_TMIC_AEC,
    SND_DEVICE_IN_HANDSET_TMIC_NS,
    SND_DEVICE_IN_HANDSET_TMIC_AEC_NS,
    SND_DEVICE_IN_SPEAKER_TMIC_AEC,
    SND_DEVICE_IN_SPEAKER_TMIC_NS,
    SND_DEVICE_IN_SPEAKER_TMIC_AEC_NS,
    SND_DEVICE_IN_VOICE_REC_TMIC,
    SND_DEVICE_IN_UNPROCESSED_MIC,
    SND_DEVICE_IN_UNPROCESSED_STEREO_MIC,
    SND_DEVICE_IN_UNPROCESSED_THREE_MIC,
    SND_DEVICE_IN_UNPROCESSED_QUAD_MIC,
    SND_DEVICE_IN_UNPROCESSED_HEADSET_MIC,
    SND_DEVICE_IN_HANDSET_6MIC,
    SND_DEVICE_IN_HANDSET_8MIC,
    SND_DEVICE_IN_EC_REF_LOOPBACK_MONO,
    SND_DEVICE_IN_EC_REF_LOOPBACK_STEREO,
    SND_DEVICE_IN_HANDSET_GENERIC_QMIC,
    SND_DEVICE_IN_INCALL_REC_RX,
    SND_DEVICE_IN_INCALL_REC_TX,
    SND_DEVICE_IN_INCALL_REC_RX_TX,
    SND_DEVICE_IN_LINE,
    SND_DEVICE_IN_EC_REF_LOOPBACK_QUAD,
    SND_DEVICE_IN_BUS,
    SND_DEVICE_IN_EC_REF_LOOPBACK,
    SND_DEVICE_IN_HANDSET_DMIC_AND_EC_REF_LOOPBACK,
    SND_DEVICE_IN_HANDSET_QMIC_AND_EC_REF_LOOPBACK,
    SND_DEVICE_IN_HANDSET_6MIC_AND_EC_REF_LOOPBACK,
    SND_DEVICE_IN_HANDSET_8MIC_AND_EC_REF_LOOPBACK,
    SND_DEVICE_IN_HANDSET_GENERIC_DMIC,
    SND_DEVICE_IN_HANDSET_GENERIC_6MIC,
    SND_DEVICE_IN_HANDSET_GENERIC_8MIC,
    SND_DEVICE_IN_HANDSET_GENERIC_DMIC_AND_EC_REF_LOOPBACK,
    SND_DEVICE_IN_HANDSET_GENERIC_QMIC_AND_EC_REF_LOOPBACK,
    SND_DEVICE_IN_HANDSET_GENERIC_6MIC_AND_EC_REF_LOOPBACK,
    SND_DEVICE_IN_HANDSET_GENERIC_8MIC_AND_EC_REF_LOOPBACK,
    SND_DEVICE_IN_HDMI_MIC_DSD,
    SND_DEVICE_IN_ECALL_HANDSET_MIC,
    SND_DEVICE_IN_ECALL_SPEAKER_MIC,
    SND_DEVICE_IN_ECALL_HEADSET_MIC,
    SND_DEVICE_IN_ECALL_VOICE_DMIC,
    SND_DEVICE_IN_ECALL_HANDSET_DMIC,
    SND_DEVICE_IN_ECALL_HANDSET_DMIC_AEC,
    SND_DEVICE_IN_ECALL_HANDSET_DMIC_NS,
    SND_DEVICE_IN_ECALL_HANDSET_DMIC_AEC_NS,
    SND_DEVICE_IN_ECALL_SPEAKER_DMIC,
    SND_DEVICE_IN_ECALL_SPEAKER_DMIC_AEC,
    SND_DEVICE_IN_ECALL_SPEAKER_DMIC_NS,
    SND_DEVICE_IN_ECALL_SPEAKER_DMIC_AEC_NS,
    SND_DEVICE_IN_END,
    SND_DEVICE_MAX = SND_DEVICE_IN_END,
};
int waitMRCD()
{
    int rc = 0;
    int status = 1;
    int count = 0;
    const char* string = "ps | grep mrc-daemon | grep system";
    char result_buf[MAXLINE]={0};
    while (status)
    {
        if(count > 10)
        {
            status = 0;
            printf("\r[ERROR]wait MRCD....%ds\r\n", count);
            break;
        }
        memset(result_buf, 0,sizeof(result_buf));
        FILE *fp;

        //fp = popen(string, "r");
        //if (NULL == fp)
        //{
        //    return 1;
        //}
        printf("\rwait MRCD....%ds", count);
        fflush(stdout);
        /*while (fgets(result_buf, sizeof(result_buf), fp) != NULL)
        {
            if ('\n' == result_buf[strlen(result_buf)-1]) {
                result_buf[strlen(result_buf)-1] = '\0';
            }
            if (strstr(result_buf, "/sbin/mrc-daemon")) {
                status = 0;
                printf("[OK]wait MRCD....%ds\r\n", count);
                sleep(5);
            }
        }*/
        count ++;
        //pclose(fp);
        sleep(1);
    }
    return 0;
}
static int exec_prog(const char **argv)
{
    pid_t   my_pid;
    int     status, timeout /* unused ifdef WAIT_FOR_COMPLETION */;

    if (0 == (my_pid = fork())) {
            if (-1 == execve(argv[0], (char **)argv , NULL)) {
                    perror("child process execve failed [%m]");
                    return -1;
            }
    }

#ifdef WAIT_FOR_COMPLETION
    timeout = 1000;

    while (0 == waitpid(my_pid , &status , WNOHANG)) {
            if ( --timeout < 0 ) {
                    perror("timeout");
                    return -1;
            }
            sleep(1);
    }

    printf("%s WEXITSTATUS %d WIFEXITED %d [status %d]\n",
            argv[0], WEXITSTATUS(status), WIFEXITED(status), status);

    if (1 != WIFEXITED(status) || 0 != WEXITSTATUS(status)) {
            perror("%s failed, halt system");
            return -1;
    }
#endif
    return 0;
}
#define MODE (S_IRWXU | S_IRWXG | S_IRWXO) 
#define MAXLINE 512
int command(char* string)
{
     printf("[Hsin]command\r\n");
    char result_buf[MAXLINE]={0};
	int rc = 0;
	FILE *fp;

	fp = popen(string, "r");
	if (NULL == fp)
	{
        printf("[Hsin]%s\r\n", result_buf);
        return 1;
	}
    printf("[Hsin]1\r\n");
	while (fgets(result_buf, sizeof(result_buf), fp) != NULL)
	{
            printf("[Hsin]2\r\n");
		if('\n' == result_buf[strlen(result_buf)-1]) {
			result_buf[strlen(result_buf)-1] = '\0';
		}
		printf("[Hsin]%s\r\n", result_buf);
        if (strstr(string, "mkdir")) {
            if (strstr(result_buf, "nonexistent directory")) {
                rc = 1;
            }
        }
        if (strstr(string, "echo")) {
            if (strstr(result_buf, "Permission deniend")) {
                rc = 1;
            }
        }
        if (strstr(string, "cat")) {
            if (strstr(result_buf, "No such file or directory")) {
                rc = 1;
            }
        }
        if (strstr(string, "recovery")) {
            if (strstr(result_buf, "Recovery exiting, upgrade success")) {
                rc = 0;
            } else if (strstr(result_buf, "Recovery exiting, upgrade failed")) {
                rc = 1;
            }
        }
	}
        printf("[Hsin]3\r\n");
    pclose(fp);
    return rc;
}
int checkRecoveryStatus()
{

    int value = 0;
    const char* filename = "/data/recovery";
    FILE *fp = fopen(filename, "r");
    char str[4] ={0};


	while(fread(str ,1 ,1, fp)==1) {
        value = atoi(str);
        return value;
	}
    fclose(fp);
    return 0;
}

void buffer()
{
    size_t  size = 10;
    const char *mData = "HelloWorld";
    char buffer[size];
    memcpy(&buffer,mData, size);
    
    for (int i = 0 ; i < size ;i++)
    {
       printf("Buffer index = %d  %d\n",i , buffer[i]);
    }
}
void print_device()
{
    printf("SND_DEVICE_OUT_HANDSET%d \n", SND_DEVICE_OUT_HANDSET);
    printf("SND_DEVICE_IN_HANDSET_MIC%d \n", SND_DEVICE_IN_HANDSET_MIC);
}
void write_file()
{
   /* const char* filename = "/home/hsin/123";
    u_int16_t x[1] = {0x2000};
    char c[2] = {0x20, 0x00};
    char s[2] = {0x88, 0x88};
    FILE *fp1 = fopen(filename, "w");
    
    fwrite(c, 2, 1, fp1);
    write(s, 2, 1, fp1);

    fclose(fp1);*/
}

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

void ril_init()
{
    typedef void (*myShowMsg)();
    void *handle;
    char* error;
    void* handle1 = 0;
    myShowMsg show_msg = 0;

    /* Clear any existing error */
    //dlerror();

    printf("main pid=%d\n", getpid());
    /* 開啟 shared library 'libmyfunc.so' */
    if ((handle1 = dlopen("./libtest.so", RTLD_NOW)) == NULL) {
        printf("open libmyfunc.so error.the handle1 is [%p]\n", handle1);
        exit(1);
    }

    /* 在 handle 指向的 shared library 裡找到 "showMsg" 函數,
     *      * 並傳回他的 memory address
     **/
    if ((*(void **)(&show_msg) = dlsym(handle1, "showMsg")) == NULL) {
        printf("can not find showMsg.the showMsg is [%p]\n", *(void**)&show_msg);
        exit(2);
    }

    if ((error = dlerror()) != NULL) {
        printf("error message:[%s]\n", error);
        exit(3);
    }


    /* indirect function call (函數指標呼叫),
     * 呼叫所指定的函數
     */
    show_msg();
    printf("main pid=%d\n", getpid());
    dlclose(handle1);
}
#include <string>
#include <iostream>


#include <sstream>
#include <string>

std::string getSwappedNibbleBcdString(const std::string &data);

std::string getSwappedNibbleBcdString(const std::string &data) {
   std::string hexString;
   std::string text(data);

   // Ensure text length is a multiple of 2 else pad with 'F'
   if(text.length() % 2) {
      text.append("F");
   }

   // writes string as SMS PDU, by converting them as hex char
   for(unsigned int i = 0; i < text.length(); i += 2) {
      hexString.push_back(text[i + 1]);
      hexString.push_back(text[i]);
   }
   return hexString;
}
static void
memsetString (char *s) {
    if (s != NULL) {
        memset (s, 0, strlen(s));
    }
}

int sendSms()
{
        char delimiter = '\n';
    std::string address;
    std::cout << "Enter the address: ";
    std::getline(std::cin, address, delimiter);
    //11 00 0A 81 9073755516 00 00 FF 04 D4E2940A
    std::string bstr = "11000A81";
    std::string number = "9073755516";
    std::string estr = "0000FF04D4E2940A";
    std::string pdu = bstr + number + estr;
    std::cout << "pdu: " << pdu << std::endl;

    std::string smsc = "";
    char **pStrings;
    size_t datalen = 0;
    int32_t countStrings = 2;
    datalen = sizeof(char *) * countStrings;

    pStrings = (char **)calloc(countStrings, sizeof(char *));
    if (pStrings == NULL) {

        return 0 ;
    }
    
    int smscLen = smsc.length();
    char * smsc_cstr = (char *) malloc(sizeof(char) * (smscLen + 1));
    if (smsc_cstr != NULL) {
        strncpy(smsc_cstr, smsc.c_str(), sizeof(char) * (smscLen + 1));
    }
    int pduLen = pdu.length();
    char * pdu_cstr = (char *) malloc(sizeof(char) * (pduLen + 1));
    if (pdu_cstr != NULL) {
        strncpy(pdu_cstr, pdu.c_str(), sizeof(char) * (pduLen + 1));
    }
    
    pStrings[0] = smsc_cstr;
    pStrings[1] = pdu_cstr;
    if (pStrings != NULL) {
        for (int i = 0 ; i < countStrings ; i++) {

            memsetString (pStrings[i]);
            free(pStrings[i]);
        }
        memset(pStrings, 0, datalen);
        free(pStrings);
    }
}

#include <iostream>
#include <string>
#include <vector>

// Decode a GSM 7-bit PDU SMS message
std::string decodeGsm7bitPdu(const std::string& pdu)
{
    // Convert hex string to bytes
    std::vector<unsigned char> bytes;
    for (std::size_t i = 0; i < pdu.length(); i += 2)
    {
        std::string byteString = pdu.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
        bytes.push_back(byte);
    }

    // Decode 7-bit data
    std::string result;
    unsigned char buffer = 0;
    int shift = 0;
    for (std::size_t i = 1; i < bytes.size(); ++i)
    {
        unsigned char next = bytes[i];

        // Shift bits and add to buffer
        buffer |= (next >> shift);
        result.push_back(buffer);

        // Handle overflow
        if (shift == 6)
        {
            buffer = 0;
            shift = 0;
            continue;
        }

        buffer = (next << (8 - shift));
        shift += 1;
    }

    return result;
}

void stringtochar(){
    /*char interface_id[2] = {0};
    sprintf(interface_id, "%d", (0) + 1);
    interface_id[1] = '\0';
     printf("main pid=%d\n", interface_id[0]);
        printf("main pid=%d\n", interface_id[1]);
        
    //same   
    std::string val = "1";
    char* ww= (char*)val.c_str();
    printf("main pid=%d\n", ww[0]);
    printf("main pid=%d\n", ww[1]);*/
}
typedef int (*Callback) (std::string );
Callback g_callback = NULL;
int getNmea(std::string nmea)
{
     std::cout << " Nmea String : " << nmea << std::endl;
}
// callback function
void B(Callback callback)
{
    g_callback = callback;
    std::string str ="11111";
    callback (str);
}

void test_callback()
{
    int (*func) (std::string ) = &getNmea;
    std::string str ="22222";
    //(*func)(str);
    //loc(func);
    B(func);

    g_callback (str);
}

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

using namespace std;

bool shellCmd(const string &cmd, string &result) {
    char buffer[512];
    result = "";

    // Open pipe to file
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return false;
    }

    // read till end of process:
    while (!feof(pipe)) {
        // use buffer to read and add to result
        if (fgets(buffer, sizeof(buffer), pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);
    return true;
}

int get_hw_cmd(const string &cmd, string &result) {
    char buffer[512] = {0};
    result = "";

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return 0;
    }

    while (!feof(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) != NULL)
        {
            printf("%s \r\n",buffer);
            if (strcmp(buffer, "Platform ID") == 0)
            {
                result += buffer;
            } 
            else if (strcmp(buffer, "Subtype") == 0)
            {
                result += buffer;
            }
            else if (strcmp(buffer, "Major") == 0)
            {
                result += buffer;
            }
            else if (strcmp(buffer, "Minor") == 0)
            {
                result += buffer;
            }
        }
    }

    pclose(pipe);
    return 1;
}
int main() {
    std::string result;
    //shellCmd("cat ./sa515m_version", result);
    cout << result;
    
    get_hw_cmd("cdt_tool.sh -r", result);
    cout << result;
    return 0;
}
