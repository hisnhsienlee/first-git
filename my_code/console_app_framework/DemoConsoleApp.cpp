/*
 *  Copyright (c) 2019-2021, The Linux Foundation. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *    * Neither the name of The Linux Foundation nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 *  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 *  ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 *  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file       DemoConsoleApp.cpp
 *
 * @brief      
 *             
 */

#include <iostream>
#include <memory>
#include <chrono>

extern "C" {
#include <cxxabi.h>
#include <execinfo.h>
#include <signal.h>
#include<string.h>

#include <dlfcn.h>
#include <getopt.h>
}

#include "ril.h"
//QTI
#include "Parcel2.h"
#include "print_info.h"
#include "common.h"
#include "DemoConsoleApp.hpp"
CommandInfo* find_command (int request);
int readCommandLine(int argc, char **argv);
std::string getSwappedNibbleBcdString(const std::string &data);
RequestInfo* creatRILInfoAndInit(int request, int mode, RIL_SOCKET_ID soc_id);
void initRequestInfo(RequestInfo *pRI, int request, int mode, RIL_SOCKET_ID soc_id);
//StateManager
void responseGetIMEI(RIL_Token t, RIL_Errno e, void *response, size_t responselen);
//SIM
void responseGetIMSI(RIL_Token t, RIL_Errno e, void *response, size_t responselen);
//SMS
void responseSendSMS(RIL_Token t, RIL_Errno e, void *response, size_t responselen);
void responseSmscAddress(RIL_Token t, RIL_Errno e, void *response, size_t responselen);
//Dial
void responseDial(RIL_Token t, RIL_Errno e, void *response, size_t responselen);

//StateManager
void dispatchGetIMEI(Parcel &p, struct RequestInfo *pRI)
{
    std::cout << "dispatchGetIMEI" << std::endl;
    return;
}

void responseGetIMEI(RIL_Token t, RIL_Errno e, void *response, size_t responselen)
{
    std::cout << "responseGetIMEI" << std::endl;
    std::string imei = (char *)response;
    std::cout << "imei: "<< imei << std::endl;
    return;
}
//SIM
void dispatchGetIMSI(Parcel &p, struct RequestInfo *pRI)
{
    std::cout << "dispatchGetIMSI" << std::endl;
    return;
}

void responseGetIMSI(RIL_Token t, RIL_Errno e, void *response, size_t responselen)
{
    std::cout << "responseGetIMSI" << std::endl;
    std::string imsi = (char *)response;
    std::cout << "imsi: "<< imsi << std::endl;
    return;
}
//SMS
void dispatchSendSMS(Parcel &p, struct RequestInfo *pRI)
{
    std::cout << "dispatchSendSMS" << std::endl;
    return;
}

void responseSendSMS(RIL_Token t, RIL_Errno e, void *response, size_t responselen)
{
    std::cout << "responseSendSMS" << std::endl;
    return;
}
void dispatchSmscAddress(Parcel &p, struct RequestInfo *pRI)
{
    std::cout << "dispatchSmscAddress" << std::endl;
    return;
}

void responseSmscAddress(RIL_Token t, RIL_Errno e, void *response, size_t responselen)
{
    std::cout << "responseSmscAddress" << std::endl;
    std::string address = (char *)response;
    std::cout << "SMSC address: "<< address << std::endl;
    return;
}

//Dial
void dispatchDial(Parcel &p, struct RequestInfo *pRI)
{
    std::cout << "dispatchDial" << std::endl;
    return;
}

void responseDial(RIL_Token t, RIL_Errno e, void *response, size_t responselen)
{
    std::cout << "responseDial" << std::endl;
    return;
}

//RIL
void RIL_register (const RIL_RadioFunctions *callbacks);
extern "C" void RIL_onRequestComplete(RIL_Token t, RIL_Errno e,
                           void *response, size_t responselen);
                           
RIL_RadioFunctions s_callbacks = {0, NULL, NULL, NULL, NULL, NULL};

#if defined(ANDROID_MULTI_SIM)
extern "C" void RIL_onUnsolicitedResponse(int unsolResponse, const void *data,
                                size_t datalen, RIL_SOCKET_ID socket_id);
#else
extern "C" void RIL_onUnsolicitedResponse(int unsolResponse, const void *data,
                                size_t datalen);
#endif

extern "C" void
RIL_onRequestComplete(RIL_Token t, RIL_Errno e, void *response, size_t responselen)
{
    RequestInfo *pRI  = (RequestInfo *)t;
    
    std::cout << "RIL_onRequestComplete"<< std::endl;
    if (pRI->cancelled == 0) {
        if (response != NULL) {
            if (pRI->pCI->responseFunction != NULL)
            {
                pRI->pCI->responseFunction(t, e, response, responselen);
            } else {
                std::cout << "Response function is null" << std::endl;
            }
        }
    }
}

#if defined(ANDROID_MULTI_SIM)
extern "C"
void RIL_onUnsolicitedResponse(int unsolResponse, const void *data,
                                size_t datalen, RIL_SOCKET_ID socket_id)
#else
extern "C"
void RIL_onUnsolicitedResponse(int unsolResponse, const void *data,
                                size_t datalen)
#endif
{
    RequestInfo *pRI;
    switch (unsolResponse)
    {
        case RIL_UNSOL_RESPONSE_NEW_SMS_ON_SIM:
            std::cout << "RIL_onUnsolicitedResponse RIL_UNSOL_RESPONSE_NEW_SMS_ON_SIM" << std::endl;
            pRI = creatRILInfoAndInit(RIL_REQUEST_SMS_ACKNOWLEDGE, 1, (RIL_SOCKET_ID)(0));
            s_callbacks.onRequest(RIL_REQUEST_SMS_ACKNOWLEDGE, NULL, 0, pRI);
            break;
        case RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED:
            std::cout << "RIL_onUnsolicitedResponse RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED" << std::endl;
            pRI = creatRILInfoAndInit(RIL_REQUEST_GET_CURRENT_CALLS, 1, (RIL_SOCKET_ID)(0));
            s_callbacks.onRequest(RIL_REQUEST_GET_CURRENT_CALLS, NULL, 0, pRI);
            break;
        case RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED:
            std::cout << "RIL_onUnsolicitedResponse RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED" << std::endl;
            pRI = creatRILInfoAndInit(RIL_REQUEST_VOICE_REGISTRATION_STATE, 1, (RIL_SOCKET_ID)(0));
            s_callbacks.onRequest(RIL_REQUEST_VOICE_REGISTRATION_STATE, NULL, 0, pRI);
            break;
        default:
            break;
    }
}
int s_token = 0;
static struct RIL_Env s_rilEnv = {
    RIL_onRequestComplete,
    RIL_onUnsolicitedResponse,
    NULL,
    NULL
};

static CommandInfo s_commands[] = {
#include "commands.h"
};

char *s_rilLibPath = NULL;


//DemoConsoleApp
DemoConsoleApp::DemoConsoleApp(std::string appName, std::string cursor)
    : ConsoleApp(appName, cursor) {
}

DemoConsoleApp::~DemoConsoleApp() {

}

void DemoConsoleApp::init() {
    std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
    startTime = std::chrono::system_clock::now();
    
    initConsole();
    
    endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime = endTime - startTime;
    std::cout << "Elapsed Time for Demo app to ready : " << elapsedTime.count() << "s"
            << std::endl;
}
int g_debug =0;
void DemoConsoleApp::initConsole() {
    
    std::vector<std::shared_ptr<ConsoleAppCommand>> mainMenuCommands;
    int index = 1;
    if (g_debug) 
    {
        std::shared_ptr<ConsoleAppCommand> rilCommand = std::make_shared<ConsoleAppCommand>(ConsoleAppCommand(std::to_string(index++), "RIL_COMMAND", {}, std::bind(&DemoConsoleApp::rilCommand, this, std::placeholders::_1)));
        mainMenuCommands.push_back(rilCommand);
    } else 
    {
        std::shared_ptr<ConsoleAppCommand> imsi = std::make_shared<ConsoleAppCommand>(ConsoleAppCommand(std::to_string(index++), "Get IMSI", {}, std::bind(&DemoConsoleApp::getImsi, this, std::placeholders::_1)));
            mainMenuCommands.push_back(imsi);
        std::shared_ptr<ConsoleAppCommand> imei = std::make_shared<ConsoleAppCommand>(ConsoleAppCommand(std::to_string(index++), "Get IMEI", {}, std::bind(&DemoConsoleApp::getImei, this, std::placeholders::_1)));
            mainMenuCommands.push_back(imei);
        std::shared_ptr<ConsoleAppCommand> dial = std::make_shared<ConsoleAppCommand>(ConsoleAppCommand(std::to_string(index++), "Dial", {}, std::bind(&DemoConsoleApp::dial, this, std::placeholders::_1)));
            mainMenuCommands.push_back(dial);
        std::shared_ptr<ConsoleAppCommand> hangup = std::make_shared<ConsoleAppCommand>(ConsoleAppCommand(std::to_string(index++), "Hangup", {}, std::bind(&DemoConsoleApp::hangup, this, std::placeholders::_1)));
            mainMenuCommands.push_back(hangup);
        std::shared_ptr<ConsoleAppCommand> answer = std::make_shared<ConsoleAppCommand>(ConsoleAppCommand(std::to_string(index++), "Answer", {}, std::bind(&DemoConsoleApp::answer, this, std::placeholders::_1)));
            mainMenuCommands.push_back(answer);
        std::shared_ptr<ConsoleAppCommand> sendSMS = std::make_shared<ConsoleAppCommand>(ConsoleAppCommand(std::to_string(index++), "Send SMS", {}, std::bind(&DemoConsoleApp::sendSMS, this, std::placeholders::_1)));
            mainMenuCommands.push_back(sendSMS);
        std::shared_ptr<ConsoleAppCommand> connectDataCall = std::make_shared<ConsoleAppCommand>(ConsoleAppCommand(std::to_string(index++), "Connect DataCall", {}, std::bind(&DemoConsoleApp::connectDataCall, this, std::placeholders::_1)));
            mainMenuCommands.push_back(connectDataCall);
        std::shared_ptr<ConsoleAppCommand> disconnectDataCall = std::make_shared<ConsoleAppCommand>(ConsoleAppCommand(std::to_string(index++), "Disconnect DataCall", {}, std::bind(&DemoConsoleApp::connectDataCall, this, std::placeholders::_1)));
            mainMenuCommands.push_back(disconnectDataCall);
    }


    ConsoleApp::addCommands(mainMenuCommands);
    ConsoleApp::displayMenu();
    
}

CommandInfo* find_command (int request)
{
    for (int i = 0; i < (int32_t)NUM_ELEMS(s_commands); i++)
        if (s_commands[i].requestNumber == request)
            return (&s_commands[i]);
    return ((CommandInfo *)NULL);
}
void initRequestInfo(RequestInfo *pRI, int request, int mode, RIL_SOCKET_ID soc_id)
{
    pRI->token = s_token++;;
    if (request < RIL_REQUEST_VENDOR_BASE) { 
        pRI->pCI = find_command(request);
    }
    pRI->socket_id = soc_id;
    pRI->p_next = NULL;
}
RequestInfo* creatRILInfoAndInit(int request, int mode, RIL_SOCKET_ID soc_id)
{
    RequestInfo *pRI  = (RequestInfo *)calloc(1, sizeof(RequestInfo));
    if(pRI ==NULL){
        std::cout <<"%s,memory alloc error!"<< std::endl;
        return NULL;
    }
    initRequestInfo(pRI,request,mode, soc_id);
    return pRI;
}

void RIL_register (const RIL_RadioFunctions *callbacks)
{
    memcpy(&s_callbacks, callbacks, sizeof (RIL_RadioFunctions));
}

int readCommandLine(int argc, char **argv) {
   int ret = 0;
   int c;
   while (1) {
        static struct option long_options[] = {
            {"l", required_argument, 0, 'l'},
            {"help", no_argument, 0, 'h'},
            {"debug", no_argument, 0, 'd'},
            {0, 0, 0, 0}
        };
        // getopt_long stores the option index here
        int option_index = 0;

        c = getopt_long(argc, argv, "l:hd", long_options, &option_index);

        // Detect the end of the options
        if (c == -1) {
            std::cout << "c =-1  "<< std::endl;
            break;
        }

        switch (c) {
           case 'l': {
               if (optarg) {
                   std::string path = std::string(optarg);
                   s_rilLibPath = strdup(path.c_str());
                   std::cout << "path: "<< s_rilLibPath << std::endl;
               } else {
                   std::cout <<" Failed to get the lib path: "<< std::endl;
               }
           } break;
           case 'd': {
               g_debug = 1;
           }break;
           default: {
                std::cout << "default " << std::endl;
           } break;
        }
        std::cout << "back "<< std::endl;
    }
   return ret;
}

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

int main(int argc, char **argv) {
    std::cout << " sizeof (RIL_CDMA_SignalInfoRecord *) = " << sizeof (RIL_CDMA_SignalInfoRecord*) << std::endl;
    std::cout << "sizeof (RIL_CDMA_SignalInfoRecord ) = " << sizeof (RIL_CDMA_SignalInfoRecord) << std::endl;
    #ifdef GG
    std::cout << "GG define " << std::endl;
    #endif
    int ret = readCommandLine(argc, argv);
    if(ret) {
        return -1;
    }
    void *dlHandle;
    const RIL_RadioFunctions *(*rilInit)(const struct RIL_Env *, int, char **);

    const RIL_RadioFunctions *funcs;
    
    std::cout <<"**RIL Started**"<< std::endl;

    const char *rilLibPath = "./libwnc_tel.so";
    
    /*dlHandle = dlopen(rilLibPath, RTLD_NOW);

    rilInit =
        (const RIL_RadioFunctions *(*)(const struct RIL_Env *, int, char **))
        dlsym(dlHandle, "RIL_Init");

    if (rilInit == NULL) {
        std::cout <<"RIL_Init not defined or exported in %s"<< rilLibPath << std::endl;
        dlclose(dlHandle);
        exit(EXIT_FAILURE);
    }*/
    std::cout <<"start rilInit"<< std::endl;
    //funcs = rilInit(&s_rilEnv, 0, NULL);
 
    //RIL_register(funcs);
 
    std::string appName = "Demo console app v1.0.1";//get P I L library version

    auto demoConsoleApp = std::make_shared<DemoConsoleApp>(appName, "DEMO> ");

    demoConsoleApp->init();  // initialize commands and display

    return demoConsoleApp->mainLoop();  // Main loop to continuously read and execute commands*/

}
/**
 * Callee expects const RIL_Dial *
 * Payload is:
 *   String address
 *   int32_t clir
 */
void DemoConsoleApp::dial(std::vector<std::string> userInput) {
    RIL_Dial dial;
    int32_t sizeOfDial;
    char delimiter = '\n';
    std::string address;

    memset (&dial, 0, sizeof(dial));
    std::cout << "Enter the address: ";
    std::getline(std::cin, address, delimiter);

    dial.address = strdup(address.c_str());

    std::cout << "dial.address" << dial.address << std::endl;
    dial.clir = 0;
    dial.uusInfo = NULL;

    sizeOfDial = sizeof(dial);

    RequestInfo *pRI = creatRILInfoAndInit(RIL_REQUEST_DIAL, 1, (RIL_SOCKET_ID)(0));

    s_callbacks.onRequest(RIL_REQUEST_DIAL, &dial, sizeOfDial, pRI);
}

void DemoConsoleApp::hangup(std::vector<std::string> userInput) {
    int callIndex = 1;
    RequestInfo *pRI = creatRILInfoAndInit(RIL_REQUEST_HANGUP, 1, (RIL_SOCKET_ID)(0));
    s_callbacks.onRequest(RIL_REQUEST_HANGUP, &callIndex, sizeof(int), pRI);
}

void DemoConsoleApp::answer(std::vector<std::string> userInput) {
    RequestInfo *pRI = creatRILInfoAndInit(RIL_REQUEST_ANSWER, 1, (RIL_SOCKET_ID)(0));
    s_callbacks.onRequest(RIL_REQUEST_ANSWER, NULL, 0, pRI);
}

void DemoConsoleApp::getImei(std::vector<std::string> userInput) {
    RequestInfo *pRI = creatRILInfoAndInit(RIL_REQUEST_GET_IMEI, 1, (RIL_SOCKET_ID)(0));
    s_callbacks.onRequest(RIL_REQUEST_GET_IMEI, NULL, 0, pRI);
}

void DemoConsoleApp::getImsi(std::vector<std::string> userInput) {
    RequestInfo *pRI = creatRILInfoAndInit(RIL_REQUEST_GET_IMSI, 1, (RIL_SOCKET_ID)(0));
    s_callbacks.onRequest(RIL_REQUEST_GET_IMSI, NULL, 0, pRI);
}

/**
 * Callee expects const RIL_SMS_WriteArgs *
 * Payload is:
 *   int32_t status
 *   String pdu
 */
void DemoConsoleApp::sendSMS(std::vector<std::string> userInput) {
    char delimiter = '\n';
    std::string address;
    std::cout << "Enter the address: ";
    std::getline(std::cin, address, delimiter);
    
    std::string bstr = "11000A81";
    std::string number = getSwappedNibbleBcdString(address);
    std::string estr = "0000FF04D4E2940A";
    std::string pdu = bstr + number + estr;
    
    RIL_SMS_WriteArgs args;
    memset (&args, 0, sizeof(args));
    args.status = 2;
    args.pdu = (char*)pdu.c_str();
    args.smsc = NULL;
    RequestInfo *pRI = creatRILInfoAndInit(RIL_REQUEST_SEND_SMS, 1, (RIL_SOCKET_ID)(0));
    s_callbacks.onRequest(RIL_REQUEST_SEND_SMS, &args, sizeof(args), pRI);
}

void DemoConsoleApp::connectDataCall(std::vector<std::string> userInput) {

}

void DemoConsoleApp::disconnectDataCall(std::vector<std::string> userInput) {

}
//Test
void DemoConsoleApp::rilCommand(std::vector<std::string> userInput) {

}