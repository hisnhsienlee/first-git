extern "C" {
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <netinet/in.h>
#include <poll.h>

#include <sys/un.h>
#include <sys/socket.h>
}
#include <thread>
class Ril {
public:
   static Ril &getInstance();
   int init();
   void ReaderThread();
   Ril();
   ~Ril();
private:
    //std::thread Reader_;
    std::thread *thread;
    Ril *ril_;
};
Ril::Ril()
{
    thread = new std::thread(&Ril::ReaderThread, this);
}
void Ril::ReaderThread()
{
    while(1)
    {
        sleep(1);
        printf("ReaderThread\n");
    }
}
Ril &Ril::getInstance() {
   static Ril instance;
   instance.init();
   return instance;
}
int Ril::init() {
    //Reader_ = std::thread{[=] { ReaderThread(); }};
    //Reader_ = std::thread(&Ril::ReaderThread, this);
}
Ril::~Ril(){
    thread->join();
}
extern "C" void showMsg() {
    printf("showMsg pid=%d\n", getpid());
    printf("hello, this is mytest\n");
    
    Ril::getInstance();
}