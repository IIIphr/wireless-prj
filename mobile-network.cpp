#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int main(){
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
    
    return 0;
}