
#include <string.h>
#include <stdlib.h>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "mdns.h"

#include "proj_mdns.h"

#define EXAMPLE_MDNS_INSTANCE "esp-instance"
#define EXAMPLE_HOST_NAME "mdns-host"

static const char *TAG = "mdns-test";
static char *generate_hostname(void);
static mdns_txt_item_t *prepare_mdns_service_contexts(size_t *mdns_context_size);
static void add_mdns_service_context_to_array(mdns_txt_item_t *mdns_service_context, e_mdns_service_context_t context);

void initialise_mdns(void)
{
    char *hostname = generate_hostname();

    // initialize mDNS
    ESP_ERROR_CHECK(mdns_init());
    // set mDNS hostname (required if you want to advertise services)
    ESP_ERROR_CHECK(mdns_hostname_set(hostname));
    ESP_LOGI(TAG, "mdns hostname set to: [%s]", hostname);
    // set default mDNS instance name
    ESP_ERROR_CHECK(mdns_instance_name_set(EXAMPLE_MDNS_INSTANCE));

    size_t mdns_context_array_size = 0;
    mdns_txt_item_t *mdns_service_context_array = prepare_mdns_service_contexts(&mdns_context_array_size);
    ESP_LOGE(TAG, "Mdns context size: %d", mdns_context_array_size);

    // initialize service
    esp_err_t err = mdns_service_add("mockmdns", "_mock", "_tcp", 80, mdns_service_context_array, mdns_context_array_size);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to add mDNS service: %s", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGI(TAG, "mDNS service added successfully.");
    }
    free(mdns_service_context_array);
    free(hostname);
}

static char *generate_hostname(void)
{
    uint8_t mac[6];
    char *hostname;
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    if (-1 == asprintf(&hostname, "%s-%02X%02X%02X", EXAMPLE_HOST_NAME, mac[3], mac[4], mac[5]))
    {
        abort();
    }
    return hostname;
}

static mdns_txt_item_t *prepare_mdns_service_contexts(size_t *mdns_context_size)
{
    *mdns_context_size = MDNS_SERVICE_CONTEXT_MAX;

    mdns_txt_item_t *mdns_service_context_array = (mdns_txt_item_t *)malloc(sizeof(mdns_txt_item_t) * MDNS_SERVICE_CONTEXT_MAX);
    if (mdns_service_context_array)
    {
        memset(mdns_service_context_array, 0, sizeof(mdns_txt_item_t) * MDNS_SERVICE_CONTEXT_MAX);
        for (int i = 0; i < MDNS_SERVICE_CONTEXT_MAX; i++)
        {
            add_mdns_service_context_to_array(&mdns_service_context_array[i], i);
        }
    }
    return mdns_service_context_array;
}

static void add_mdns_service_context_to_array(mdns_txt_item_t *mdns_service_context, e_mdns_service_context_t context)
{
    switch (context)
    {
    case MDNS_SERVICE_CONTEXT_BOARD:
    {
        mdns_service_context->key = "board";
        mdns_service_context->value = "esp32";
        break;
    }
    case MDNS_SERVICE_CONTEXT_USER:
    {
        mdns_service_context->key = "u";
        mdns_service_context->value = "user";
        break;
    }
    case MDNS_SERVICE_CONTEXT_PASSWORD:
    {
        mdns_service_context->key = "p";
        mdns_service_context->value = "password";
        break;
    }
    default:
    {
        break;
    }
    }
}
