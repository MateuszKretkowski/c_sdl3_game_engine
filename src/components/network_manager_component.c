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
bool hasPeer;   // false = waiting to learn the peer address from the first packet

int port;
char ip[64];
int peerPort;

int sock;
struct sockaddr_in my_address;
struct sockaddr_in peer_address;

GameObject **registered_gameObjects;
int registered_gameObjects_length = 0;
int registered_gameObjects_capacity = 3;

char* get_host_ip(void);

void network_manager_awake(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {    
        fprintf(stderr, "WSAStartup failed\n");
        exit(1);
    }
    
    registered_gameObjects = malloc(sizeof(GameObject*) * registered_gameObjects_capacity);

    isHost = false;
    isGameActive = false;
    isReady = false;
    hasPeer = false;
}

void network_manager_start(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;

    char buf[64];


    printf("Host IP: %s\n", get_host_ip());

    printf("Set Port:");
    fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin)) {
        port = atoi(buf);
    }

    printf("Set Peer IP (leave empty to wait for the peer to connect first):");
    fflush(stdout);
    if (fgets(ip, sizeof(ip), stdin)) {
        ip[strcspn(ip, "\r\n")] = '\0';
    }

    if (ip[0] != '\0') {
        printf("Set Peer Port:");
        fflush(stdout);
        if (fgets(buf, sizeof(buf), stdin)) {
            peerPort = atoi(buf);
        }
    }

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

    u_long mode = 1;
    if (ioctlsocket(sock, FIONBIO, &mode) == SOCKET_ERROR) {
        printf("ioctlsocket() failed: %d\n", WSAGetLastError());
    }

    if (ip[0] != '\0') {
        peer_address.sin_family = AF_INET;
        peer_address.sin_port = htons(peerPort);
        if (inet_pton(AF_INET, ip, &peer_address.sin_addr) != 1) {
            printf("inet_pton() failed for IP '%s': %d\n", ip, WSAGetLastError());
        } else {
            hasPeer = true;
            printf("Sending to %s:%d\n", ip, peerPort);
        }
    } else {
        printf("Waiting for the first packet on port %d...\n", port);
    }

    isReady = true;
}

void network_manager_update(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;

    if (!isReady || !port) {
        return;
    }

    if (hasPeer) {
        for (int i=0; i<registered_gameObjects_length; i++) {
            network_gameObject_component *net = get_component(registered_gameObjects[i], network_gameObject_component, "network_gameObject_component");            
            
            bool myOwner = isHost ? 0 : 1;

            if (net->owner != myOwner) {
                continue;
            }

            if (sendto(sock, (char*)net->buffer, sizeof(net->buffer), 0, (struct sockaddr*) &peer_address, sizeof(peer_address)) == SOCKET_ERROR) {
                printf("sendto() failed: %d\n", WSAGetLastError());
            }
        }
    }

    unsigned char buffer[32 + sizeof(Vector3) * 3];
    struct sockaddr_in from_addr;
    int from_len = sizeof(from_addr);
    int received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*) &from_addr, &from_len);
    if (received == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) {
            printf("recvfrom() failed: %d\n", err);
        }
        return;
    }

    if (!hasPeer) {
        peer_address = from_addr;
        hasPeer = true;
        printf("Peer connected: %s:%d\n", inet_ntoa(from_addr.sin_addr), ntohs(from_addr.sin_port));
    } else if (from_addr.sin_addr.s_addr != peer_address.sin_addr.s_addr
            || from_addr.sin_port != peer_address.sin_port) {
        return;
    }

    char gameObject_id[32];
    memcpy(gameObject_id, buffer, 32);
    GameObject *gameObject = scene_get_gameObject_by_name(gameObject_id);
    transform_component *gameObject_tc = get_component(gameObject, transform_component, "transform_component");
    Vector3 pos;
    Vector3 rot;
    Vector3 scale;

    memcpy(&pos, &buffer[32], sizeof(Vector3));
    memcpy(&pos, &buffer[32 + sizeof(Vector3)], sizeof(Vector3));
    memcpy(&pos, &buffer[32 + sizeof(Vector3) * 2], sizeof(Vector3));

    gameObject_tc->position = pos;
    gameObject_tc->rotation = rot;
    gameObject_tc->scale = scale;
}

void network_manager_destroy(Component* self) {
    network_manager_component *comp = (network_manager_component*)self;

    free(registered_gameObjects);
}

void network_manager_register(GameObject *gameObject) {
    if (registered_gameObjects_length == registered_gameObjects_capacity) {
        registered_gameObjects_capacity *= 2;
        registered_gameObjects = realloc(registered_gameObjects, sizeof(GameObject*) * registered_gameObjects_capacity);
    }
    registered_gameObjects[registered_gameObjects_length * sizeof(GameObject*)] = gameObject;
    registered_gameObjects_length++;
}

void network_manager_unregister(GameObject *gameObject) {
    for (int i=0; i<registered_gameObjects_length; i++) {
        if (&registered_gameObjects[i] == gameObject) {
            registered_gameObjects[i] = NULL;
        }
    }
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
