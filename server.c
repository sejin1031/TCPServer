/* 
   A simple server in the internet domain using TCP
   Usage:./server port (E.g. ./server 10000 )
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>   // definitions of a number of data types used in socket.h and netinet/in.h
#include <sys/socket.h>  // definitions of structures needed for sockets, e.g. sockaddr
#include <netinet/in.h>  // constants and structures needed for internet domain addresses, e.g. sockaddr_in
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[])
{
    int sockfd, newsockfd; //descriptors rturn from socket and accept system calls
     int portno; // port number
     socklen_t clilen;
     
     char buffer[1000];
     
     /*sockaddr_in: Structure Containing an Internet Address*/
     struct sockaddr_in serv_addr, cli_addr;
     
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     
     /*Create a new socket
       AF_INET: Address Domain is Internet 
       SOCK_STREAM: Socket Type is STREAM Socket */
     sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
     if (sockfd < 0) 
        error("ERROR opening socket");
     
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]); //atoi converts from String to Integer
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //for the server the IP address is always the address that the server is running on
     serv_addr.sin_port = htons(portno); //convert from host to network byte order
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) //Bind the socket to the server address
              error("ERROR on binding");
     
     listen(sockfd,5); // Listen for socket connections. Backlog queue (connections to wait) is 5
     
     while(1){
        clilen = sizeof(cli_addr);
     /*accept function: 
       1) Block until a new connection is established
       2) the new socket descriptor will be used for subsequent communication with the newly connected client.
     */
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     
     bzero(buffer,1000);
     n = read(newsockfd,buffer,1000); //Read is a block function. It will read at most 1000 bytes
     if (n < 0) error("ERROR reading from socket");
      printf("Here is the message: %s\n",buffer); // request message를 읽어옴
      char *filename = strtok(buffer," "); // GET /file.a HTTP/1.1에서 file.a 따오기 위해 strtok사용
      filename = strtok(NULL," "); // 위에서 이어짐
      filename = strtok(filename,"/"); // 위에서 이어지고 /제거

      if(access(filename,R_OK) != -1){     //filename의 파일이 존재하는지 확인
         FILE *f;//파일 스트림 f 정의
         f=fopen(filename,"rb"); //rb모드로 filename의 파일 f에 열기
         char buffer[1024] = {0,}; // file을 읽어올 때 임시로 저장할 버퍼
         char *ext = strtok(filename,"."); // 확장자를 떼오기
         ext = strtok(NULL,"."); // 위와 동일

         if(strcmp(ext,"html") == 0){ // 확장자가 html인 경우
            char reply[43] = 
            "HTTP/1.1 200 OK\n"
            "Content-Type: text/html\n" //content type을 text/html로 지정
            "\n"; //header를 reply char 배열에 저장
            send(newsockfd,reply,strlen(reply),0);//header를 send함수로 client에게 전송
            int count = 0; // fread 해 온 크기를 저장

            while(feof(f) == 0){ //EOF에 도달할 때 까지 반복
               count = fread(buffer,sizeof(char),1024,f); // fread를 이용해 f 1024만큼 읽어오기
               send(newsockfd,buffer,sizeof(buffer),0); // 읽어온 데이터가 저장된 buffer의 내용을 send
               memset(buffer,0,sizeof(buffer)); // buffer를 다시 깨끗하게 초기화
            }
            
         } else if(strcmp(ext,"jpg") == 0){ // 확장자가 jpg인 경우
            char reply[44] = 
            "HTTP/1.1 200 OK\n" //HTTP response 임을 나타내주고 200을 통해 정상적인 response
            "Content-Type: image/jpeg\n" //content type을 image/jpeg로 지정
            "\n";
            send(newsockfd,reply,strlen(reply),0);//header를 send함수로 client에게 전송
            int count = 0; // fread 해 온 크기를 저장

            while(feof(f) == 0){ //EOF에 도달할 때 까지 반복
               count = fread(buffer,sizeof(char),1024,f); // fread를 이용해 f 1024만큼 읽어오기
               send(newsockfd,buffer,sizeof(buffer),0); // 읽어온 데이터가 저장된 buffer의 내용을 send
               memset(buffer,0,sizeof(buffer)); // buffer를 다시 깨끗하게 초기화
            }
         } else if(strcmp(ext,"gif") == 0){ // 확장자가 gif인 경우
            char reply[44] = 
            "HTTP/1.1 200 OK\n" //HTTP response 임을 나타내주고 200을 통해 정상적인 response
            "Content-Type: image/gif\n"  //content type을 image/gif로 지정
            "\n";
            send(newsockfd,reply,strlen(reply),0);//header를 send함수로 client에게 전송
            int count = 0; // fread 해 온 크기를 저장

            while(feof(f) == 0){ //EOF에 도달할 때 까지 반복
               count = fread(buffer,sizeof(char),1024,f); // fread를 이용해 f 1024만큼 읽어오기
               send(newsockfd,buffer,sizeof(buffer),0); // 읽어온 데이터가 저장된 buffer의 내용을 send
               memset(buffer,0,sizeof(buffer)); // buffer를 다시 깨끗하게 초기화
            }
         } else if(strcmp(ext,"mp3") == 0){ // 확장자가 mp3인 경우
            char reply[44] = 
            "HTTP/1.1 200 OK\n" //HTTP response 임을 나타내주고 200을 통해 정상적인 response
            "Content-Type: audio/mpeg\n" //content type을 audio/mpeg로 지정
            "\n";
            send(newsockfd,reply,strlen(reply),0); //header를 send함수로 client에게 전송
            int count = 0;// fread 해 온 크기를 저장

            while(feof(f) == 0){  //EOF에 도달할 때 까지 반복
               count = fread(buffer,sizeof(char),1024,f); // fread를 이용해 f 1024만큼 읽어오기
               send(newsockfd,buffer,sizeof(buffer),0); // 읽어온 데이터가 저장된 buffer의 내용을 send
               memset(buffer,0,sizeof(buffer)); // buffer를 다시 깨끗하게 초기화
            }
         } else if(strcmp(ext,"pdf") == 0){ // 확장자가 pdf인 경우
            char reply[60] = 
            "HTTP/1.1 200 OK\n" //HTTP response 임을 나타내주고 200을 통해 정상적인 response
            "Content-Type: application/pdf\n" //content type을 application/pdf로 지정
            "\n";
            send(newsockfd,reply,strlen(reply),0); //header를 send함수로 client에게 전송
            int count = 0;// fread 해 온 크기를 저장

            while(feof(f) == 0){  //EOF에 도달할 때 까지 반복
               count = fread(buffer,sizeof(char),1024,f); // fread를 이용해 f 1024만큼 읽어오기
               send(newsockfd,buffer,sizeof(buffer),0); // 읽어온 데이터가 저장된 buffer의 내용을 send
               memset(buffer,0,sizeof(buffer)); // buffer를 다시 깨끗하게 초기화
            }
         }
         fclose(f); //열었던 file strea을 닫아준다.
      } 
      else{
            continue; //요청받은 파일이 존재하지 않으면 다시 while문의 처음으로 가준다.
         }
      
     
      close(newsockfd); // request의 response를 모두 처리해주면 client의 file descriptor를 닫아준다.

     }
     
     
     close(sockfd); // 서버의 동작이 끝나면 서버의 file descriptor 역시 닫아준다.
     
     return 0; 
}

