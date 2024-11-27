#ifndef COMMON_HPP
#define COMMON_HPP

#define RIL_REQUEST_VENDOR_BASE 2000
/* Constants for response types */
#define RESPONSE_SOLICITED 0
#define RESPONSE_UNSOLICITED 1

#define NUM_ELEMS(a)     (sizeof (a) / sizeof (a)[0])

typedef struct {
    int requestNumber;
    void (*dispatchFunction) (Parcel &p, struct RequestInfo *pRI);
    void(*responseFunction) (RIL_Token t, RIL_Errno e, void *response, size_t responselen);
} CommandInfo;

typedef struct RequestInfo {
    int32_t token;      //this is not RIL_Token
    CommandInfo *pCI;
    struct RequestInfo *p_next;
    char cancelled;
    char local;         // responses to local commands do not go back to command process
    RIL_SOCKET_ID socket_id;
} RequestInfo;

typedef struct {
    char* name;            /* User printable name of the function. */
    int  request;            /* User printable name of the function. */
    void (*dispacthfunc)();        /* Function to call to do the job. */
    void (*responsefunc)(RIL_Token t, RIL_Errno e, void *response, size_t responselen);        /* Function to call to do the job. */
} COMMAND;

#endif  // COMMON_HPP