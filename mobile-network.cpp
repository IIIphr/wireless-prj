#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <ctime>
#include <cstdlib>

using namespace std;
double message_count = 0;
double loss_count = 0;
int loss_probability = 0;

int main(){
    printf("Enter the loss probability (integer, 0 to 100): ");
    scanf("%d", &loss_probability);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1){
        printf("Couldn't create the server socket.\n");
        return 1;
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8088);
    server_address.sin_addr.s_addr = INADDR_ANY;
    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if(listen(server_socket, 5) == -1){
        printf("Couldn't listen in port 8088.\n");
        return 2;
    }
    printf("Server is listening on port 8088.\n");
    
    printf("Waiting for the actor to connect...\n");
    int actor_socket = accept(server_socket, nullptr, nullptr);
    while(actor_socket == -1){
        printf("Connection failed, try again...\n");
        actor_socket = accept(server_socket, nullptr, nullptr);
    }
    printf("Actor connected!\n");

    printf("Waiting for the sensor to connect...\n");
    int sensor_socket = accept(server_socket, nullptr, nullptr);
    while(sensor_socket == -1){
        printf("Connection failed, try again...\n");
        sensor_socket = accept(server_socket, nullptr, nullptr);
    }
    printf("Sensor connected!\n");

    srand(time(0));
    while(true){
        char buffer[1024] = {0};
        switch(recv(sensor_socket, buffer, sizeof(buffer), 0)){
            case -1:{
                printf("Failed to receive data from sensor. Shutting down the actor...\n");
                send(actor_socket, buffer, strlen(buffer), 0);
                printf("Message count: %lf\n", message_count);
                printf("Loss count: %lf\n", loss_count);
                printf("Total count: %lf\n", loss_count + message_count);
                return 3;
            }
            case 0: break;
            default: {
                char * input = strtok(buffer, "\n");
                char output_buffer[1024] = {0};
                while(input){
                    if(input[0] == 'q') {
                        printf("Shutdown message received, shuting down the actor...\n");
                        send(actor_socket, input, strlen(input), 0);
                        printf("Message count: %lf\n", message_count);
                        printf("Loss count: %lf\n", loss_count);
                        printf("Total count: %lf\n", loss_count + message_count);
                        return 0;
                    }
                    printf("Sensor message: <%s>\n", input);
                    if(rand() % 100 < loss_probability){
                        printf("The above message was lost.\n");
                        loss_count++;
                    }
                    else{
                        sprintf(output_buffer, "%s\n", input);
                        send(actor_socket, output_buffer, strlen(output_buffer), 0);
                        message_count++;
                    }
                    input = strtok(NULL, "\n");
                }
            }
        }
    }

    return 0;
}