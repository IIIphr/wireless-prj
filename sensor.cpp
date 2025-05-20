#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

void wait_sec(int sec){
    time_t start, now;
    time(&start);
    time(&now);
    while(difftime(now, start) < sec){
        time(&now);
    }
}

int main(){
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1){
        printf("Couldn't create the client socket.\n");
        return 1;
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8088);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if(connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){
        printf("Couldn't connect to mobile network.\n");
        return 2;
    }
    printf("Sensor is online.\n");

    srand(time(0));
    while(true){
        int random_number = (rand() % 40) + 15;
        char buffer[1024] = {0};
        sprintf(buffer, "%d", random_number); 
        send(client_socket, buffer, strlen(buffer), 0);
        wait_sec(2);
    }
    close(client_socket);
    return 0;
}