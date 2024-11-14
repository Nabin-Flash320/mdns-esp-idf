

#ifndef _PROJ_MDNS_H_
#define _PROJ_MDNS_H_

typedef enum e_mdns_service_context
{
    MDNS_SERVICE_CONTEXT_BOARD = 0,
    MDNS_SERVICE_CONTEXT_USER,
    MDNS_SERVICE_CONTEXT_PASSWORD,
    MDNS_SERVICE_CONTEXT_MAX,
} e_mdns_service_context_t;

void initialise_mdns();

#endif // _PROJ_MDNS_H_
