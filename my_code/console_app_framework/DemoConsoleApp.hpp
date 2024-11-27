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

#ifndef DEMOCONSOLEAPP_HPP
#define DEMOCONSOLEAPP_HPP

#include "ConsoleApp.hpp"

class DemoConsoleApp : public ConsoleApp,
                        public std::enable_shared_from_this<DemoConsoleApp> {
public :
    DemoConsoleApp(std::string appName, std::string cursor);
    ~DemoConsoleApp();
    void init();

private:
    void initConsole();
    void rilCommand(std::vector<std::string> userInput);
    void dial(std::vector<std::string> userInput);
    void hangup(std::vector<std::string> userInput);
    void answer(std::vector<std::string> userInput);
    void getImei(std::vector<std::string> userInput);
    void getImsi(std::vector<std::string> userInput);
    void sendSMS(std::vector<std::string> userInput);
    void connectDataCall(std::vector<std::string> userInput);
    void disconnectDataCall(std::vector<std::string> userInput);
};

#endif  // DEMOCONSOLEAPP_HPP