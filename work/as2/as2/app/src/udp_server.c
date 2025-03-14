// udp_server.c

#include "udp_server.h"
#include "sampler.h" 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>


// Constants
#define UDP_PORT 12345
#define BUFFER_SIZE 1024 // Ethernet is 1500 byte packet
#define MAX_ETHERNET_BYTES 1500

// Global variables
static pthread_t udpThread;
static bool isInitialized = false;
static bool isUdpRunning = false;
static int sockfd;
static char lastCommand[BUFFER_SIZE] = ""; // Stores last input, used for <enter>

// Helper function to trim user input for robustness
static void trimInput(char *input) {
    char *pos;
    if ((pos = strchr(input, '\n')) != NULL) *pos = '\0';  // Remove newline
    if ((pos = strchr(input, '\r')) != NULL) *pos = '\0';  // Remove carriage return
}

bool UdpServer_isOnline(void) {
    return isUdpRunning;
}

// Function to handle UDP commands
static void handleCommand(int sockfd, struct sockaddr_in *clientAddr, socklen_t addrLen, const char *command) {
    char response[BUFFER_SIZE];
    response[0] = '\0';  // Initialize response buffer

    // Trim client input
    char trimmedInput[BUFFER_SIZE];
    strncpy(trimmedInput, command, BUFFER_SIZE);
    trimInput(trimmedInput);

    if (strlen(trimmedInput) == 0) {
        if (strlen(lastCommand) == 0) {
            // No previous command
            snprintf(response, BUFFER_SIZE, "No previous input exists.");
        } else {
            // Repeat last command
            strncpy(trimmedInput, lastCommand, BUFFER_SIZE);
        }
    }

    if (strcmp(trimmedInput, "help") == 0 || strcmp(trimmedInput, "?") == 0) {
        snprintf(response, BUFFER_SIZE,
                 "Accepted command examples:\n"
                 "count -- get the total number of samples taken.\n"
                 "length -- get the number of samples taken in the previously completed second.\n"
                 "dips -- get the number of dips in the previously completed second.\n"
                 "history -- get all the samples in the previously completed second.\n"
                 "stop -- cause the server program to end.\n"
                 "<enter> -- repeat last command.\n");
        strncpy(lastCommand, trimmedInput, BUFFER_SIZE); // update last command
        sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)clientAddr, addrLen);
    } else if (strcmp(trimmedInput, "count") == 0) {
        long long count = Sampler_getNumSamplesTaken();
        snprintf(response, BUFFER_SIZE, "# samples taken total: %lld\n", count);
        strncpy(lastCommand, trimmedInput, BUFFER_SIZE); // update last command
        sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)clientAddr, addrLen);
    } else if (strcmp(trimmedInput, "length") == 0) {
        int length = Sampler_getHistorySize();
        snprintf(response, BUFFER_SIZE, "# samples taken last second: %d\n", length);
        strncpy(lastCommand, trimmedInput, BUFFER_SIZE); // update last command
        sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)clientAddr, addrLen);
    } else if (strcmp(trimmedInput, "dips") == 0) {
        int dips = Sample_getDipsCount();
        snprintf(response, BUFFER_SIZE, "# Dips: %d\n", dips);
        strncpy(lastCommand, trimmedInput, BUFFER_SIZE); // update last command
        sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)clientAddr, addrLen);
    } else if (strcmp(trimmedInput, "history") == 0) {
        int size;
        double *history = Sampler_getHistory(&size);

        // No history data
        if (size <= 0 || !history) {
            snprintf(response, BUFFER_SIZE, "No history data.\n");
            sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)clientAddr, addrLen);
            free(history);
            return;
        }

        char historyResponse[MAX_ETHERNET_BYTES];
        int responseDisplace = 0;
        for (int i = 0; i < size; i++) {
            int sent = snprintf(historyResponse + responseDisplace, MAX_ETHERNET_BYTES - responseDisplace, "%.3f", history[i]);
            responseDisplace += sent;

            if (i < size - 1) {
                if ((i+1) % 10 == 0) {
                    historyResponse[responseDisplace++] = '\n';
                } else {
                    historyResponse[responseDisplace++] = ',';
                    historyResponse[responseDisplace++] = ' ';
                }
            }

            // Buffer is full or final char
            if (responseDisplace > MAX_ETHERNET_BYTES - 50 || i == size-1) {
                historyResponse[responseDisplace] = '\0';
                sendto(sockfd, historyResponse, strlen(historyResponse), 0, (struct sockaddr *)clientAddr, addrLen);
                responseDisplace = 0; // Reset position
            }
        }

        if (responseDisplace > 0 && historyResponse[responseDisplace-1] != '\n') {
            //missing newline
            sendto(sockfd, "\n", strlen(historyResponse), 0, (struct sockaddr *)clientAddr, addrLen);
        }
        
        free(history);
        strncpy(lastCommand, trimmedInput, BUFFER_SIZE); // update last command
    } else if (strcmp(trimmedInput, "stop") == 0) {
        snprintf(response, BUFFER_SIZE, "Program terminating.\n");
        sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)clientAddr, addrLen);
        isUdpRunning = false;
    } else {
        snprintf(response, BUFFER_SIZE, "Unknown command. Type 'help' or '?' for a command list.\n");
        sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)clientAddr, addrLen);
        
    }
}

// Thread to run the UDP server
static void* udpThreadFunc(void* none) {
    (void)none; // Suppress unused parameter warning

    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Socket creation UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP_server: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    // https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/notes/files/06-LinuxProgramming-c.pdf
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(UDP_PORT);

    // Setting server address to bind the socket
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP server started on port %d\n", UDP_PORT);

    // Main server loop
    while (isUdpRunning) {
        // Receive a command from the client
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &addrLen);
        buffer[n] = '\0';  // Null-terminate the received data

        // Handle the command
        handleCommand(sockfd, &clientAddr, addrLen, buffer);
    }

    // Finish
    printf("UDP server stopped\n");
    return NULL;
}

// Start the UDP server
void UdpServer_start(void) {
    assert(!isInitialized);

    isUdpRunning = true;
    isInitialized = true;

    // Start UDP server thread
    if (pthread_create(&udpThread, NULL, udpThreadFunc, NULL) != 0) {
        perror("Error when attempting to start up UDP server thread");
        exit(EXIT_FAILURE);
    }
}

// Stop the UDP server
void UdpServer_stop(void) {
    assert(isInitialized);

    isUdpRunning = false;
    isInitialized = false;

    // Clean up thread
    close(sockfd);
    pthread_cancel(udpThread);
    pthread_join(udpThread, NULL);
}