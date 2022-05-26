#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444
#define SIZE 1024

void upload_file(FILE *fp, int clienSocket){
    char data[SIZE] = {0};
    while(fgets(data, SIZE, fp) != NULL){
        if(send(clienSocket, data, sizeof(data), 0) == -1){
            printf("[-]Error in uploading file");
        }
        bzero(data , SIZE);
    }
}

int main(){

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	char *upload = "uploading";

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	FILE *fp;
    char *fileName = "clientFile.txt";

    fp = fopen(fileName, "r");
    if(fp == NULL){
        printf("[-]Error in reading file");
        exit(1);
    }

	upload_file(fp, clientSocket);
	printf("[+]Successfully uploaded file to server.\n");


	while(1){
		printf("Still connected to server type 'exit' to disconnect: \t");
		scanf("%s", &buffer[0]);

		if(strcmp(buffer, "exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			return 0;
		}
	}
}