#include <stdio.h>
#include <sys/socket.h>

using namespace std;

int main(){
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1){
        printf("Couldn't create the server socket.\n");
        return 1;
    }
    return 0;
}