#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>

#include "network_manager_component.h"
#include "component_registry.h"
#include "components.h"
#include "core/gameObject.h"
#include "json_utils.h"
#include "engine/input.h"

#include <cglm/cglm.h>

bool isHost;
bool isGameActive;
bool isReady;

int port;
char ip[64];

char* get_host_ip(void);

void network_manager_awake(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {    
        fprintf(stderr, "WSAStartup failed\n");
        exit(1);
    }

    isHost = false;
    isGameActive = false;
    isReady = false;
}

void network_manager_start(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;

    char buf[64];

    
    // printf("Host IP: %s\n", get_host_ip());

    // printf("isHost (1 or 0): ");
    // fflush(stdout);
    // if (fgets(buf, sizeof(buf), stdin)) {
    //     isHost = atoi(buf);
    // }

    printf("Set Port: ");
    fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin)) {
        port = atoi(buf);
    }

    // if (!isHost) {
        printf("Set IP: ");
        fflush(stdout);
        if (fgets(ip, sizeof(ip), stdin)) {
            ip[strcspn(ip, "\r\n")] = '\0';
        }
    // }
    
    isReady = true;
}

void network_manager_update(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;

    if (!isReady || !ip || !port) {
        return;
    }

    // for peer-to-peer it is needed to have both server and client on both devices
    
    // server

    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Creating the socket failed (%d)\n\n", server_socket);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = port;
    server_address.sin_addr.s_addr = INADDR_ANY;

    printf("SERVER_ADDRESS %s", server_address.sin_addr.s_addr);

    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    listen(server_socket, 5);

    // client

    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (network_socket < 0) {
        printf("Creating the socket failed (%d)\n\n", network_socket);
        exit(EXIT_FAILURE);
    }

    int connection_status = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    int client_socket = accept(server_socket, NULL, NULL);

    char server_message[256] = "You have reached the server";
    send(client_socket, server_message, sizeof(server_message), 0);

    char server_response[256];
    recv(network_socket, server_response, sizeof(server_response), 0);
    printf("server_response: %s\n", server_response);
}

void network_manager_destroy(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;
}

void IP_formatter(char *IPbuffer) { 
   if (NULL == IPbuffer) {
      perror("inet_ntoa");
      exit(1);
   }
}

char* get_host_ip() {
    char host[256];
    struct hostent *host_entry;

    if (gethostname(host, sizeof(host)) != 0) {
        return NULL;
    }

    host_entry = gethostbyname(host);
    if (host_entry == NULL || host_entry->h_addr_list[0] == NULL) {
        return NULL;
    }

    return inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
}

network_manager_component *create_network_manager_component() {
    network_manager_component* comp = malloc(sizeof(network_manager_component));
    comp->base.id = strdup("network_manager_component");
    comp->base.name = strdup("network_manager");
    comp->base.isActive = true;
    comp->base.gameObject = NULL;
    comp->base.size = sizeof(network_manager_component);
    comp->base.standard_voids = malloc(sizeof(component_standard_voids));
    comp->base.standard_voids->awake = network_manager_awake;
    comp->base.standard_voids->start = network_manager_start;
    comp->base.standard_voids->update = network_manager_update;
    comp->base.standard_voids->destroy = network_manager_destroy;

    return comp;
}

Component* network_manager_from_json(cJSON *json) {
    return (Component*)create_network_manager_component();
}

__attribute__((constructor))
static void register_network_manager_component() {
    component_registry_register("network_manager_component", network_manager_from_json);
}
