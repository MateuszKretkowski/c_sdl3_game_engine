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
int peerPort;

int sock;
struct sockaddr_in my_address;
struct sockaddr_in peer_address;

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
    char buf2[64];

    
    printf("Host IP: %s\n", get_host_ip());

    // printf("isHost (1 or 0): ");
    // fflush(stdout);
    // if (fgets(buf, sizeof(buf), stdin)) {
    //     isHost = atoi(buf);
    // }

    printf("Set Port:");
    fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin)) {
        port = atoi(buf);
    }

    printf("Set Peer Port:");
    fflush(stdout);
    if (fgets(buf2, sizeof(buf2), stdin)) {
        peerPort = atoi(buf2);
    }

    // if (!isHost) {
        printf("Set IP:");
        fflush(stdout);
        if (fgets(ip, sizeof(ip), stdin)) {
            ip[strcspn(ip, "\r\n")] = '\0';
        }
    // }
    
    // INITIALIZING SOCKETS =================

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        printf("Creating the socket failed (%d): %d\n\n", sock, WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    my_address.sin_family = AF_INET;
    my_address.sin_port = htons(port);
    my_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*) &my_address, sizeof(my_address)) == SOCKET_ERROR) {
        printf("bind() failed: %d\n", WSAGetLastError());
    }
    if (listen(sock, 5) == SOCKET_ERROR) {
        printf("listen() failed: %d\n", WSAGetLastError());
    }

    peer_address.sin_family = AF_INET;
    peer_address.sin_port = htons(peerPort);
    if (inet_pton(AF_INET, ip, &peer_address.sin_addr) != 1) {
        printf("inet_pton() failed for IP '%s': %d\n", ip, WSAGetLastError());
    }

    isReady = true;
}

bool connected = false;

void network_manager_update(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;

    if (!isReady || !ip || !port) {
        return;
    }

    char message[64] = "eloelo320";

    if (sendto(sock, message, strlen(message), 0, (struct sockaddr*) &peer_address, sizeof(peer_address)) == SOCKET_ERROR) {
        printf("sendto() failed: %d\n", WSAGetLastError());
    }

    char buff[256];
    struct sockaddr_in from_addr;
    int from_len = sizeof(from_addr);
    int received = recvfrom(sock, buff, sizeof(buff), 0, (struct sockaddr*) &from_addr, &from_len);
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
    if (received == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) {
            printf("recvfrom() failed: %d\n", err);
        }
        return;
    }
    peer_address = from_addr;



    printf("od: %s:%d (%d bajtow)\n buff: %s\n", inet_ntoa(from_addr.sin_addr), ntohs(from_addr.sin_port), received, buff);
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
