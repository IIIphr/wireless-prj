#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int desired_temp = 25;
double message_count = 0;

int main(){
    printf("Enter your desired temperature: ");
    scanf("%d", &desired_temp);

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
    printf("Actor is online.\n");

    while(true){
        char buffer[1024] = {0};
        switch(recv(client_socket, buffer, sizeof(buffer), 0)){
            case -1:{
                printf("Failed to receive data from network.\n");
                printf("Message count: %lf\n", message_count);
                return 3;
            }
            case 0: break;
            default: {
                if(buffer[0] == 'q') {
                    printf("Shutdown message received, exiting...\n");
                    printf("Message count: %lf\n", message_count);
                    return 0;
                }
                int current_temp = atoi(buffer);
                if(desired_temp > current_temp) {
                    printf("Heater on (temp: %d)\n", current_temp);
                }
                else if(desired_temp < current_temp) {
                    printf("Cooler on (temp: %d)\n", current_temp);
                }
                else {
                    printf("Heater and Cooler off (temp: %d)\n", current_temp);
                }
                message_count++;
            }
        }
    }

    return 0;
}