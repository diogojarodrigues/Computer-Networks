#include "protocol.hpp"

//TODO: Create a function to send a tcp image, just like we have to receive a tcp image

/* ####################### UDP MESSAGE #######################  */

string send_udp_message(string message) {
    
    int fd,errcode; 
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    char buffer[8192];

    fd=socket(AF_INET,SOCK_DGRAM,0);    //UDP socket
    if (fd==-1) exit(1);                /*error*/

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;            //IPv4
    hints.ai_socktype=SOCK_DGRAM;       //UDP socket
    
    errcode=getaddrinfo(SERVER, PORT, &hints, &res);
    if(errcode!=0) exit(1);            /*error*/
    
    n=sendto(fd, message.c_str(), message.size(), 0, res->ai_addr, res->ai_addrlen);
    if(n==-1) exit(1);                 /*error*/

    addrlen=sizeof(addr);
    n=recvfrom(fd,buffer, 8192, 0, (struct sockaddr*) &addr, &addrlen);
    if(n==-1) exit(1);                 /*error*/

    freeaddrinfo(res);
    close(fd);

    return buffer;
}

/* ####################### TCP MESSAGE #######################  */

void receive_tcp_image(int fd){

    ssize_t n;
    char buffer[2048];
    n=read(fd,buffer,2048);
    if(n==-1) exit(1);                  /*error*/
    string response = buffer;
    string r = response.substr(0, 7);
    if(r == "RSA NOK"){
        cout << "asset does not exist" << endl;
    }else if(r == "RSA OK "){
        string fname, fsize;
        int i=7;
        int k=0;
        while (1){
            if(buffer[i]==' '){
            k++;
            i++;
            }else if(k==0){
                fname += buffer[i];
                i++;
            }else if(k==1){
                fsize += buffer[i];
                i++;
            }
            else if(k==2){
                break;
            }
        }
        fstream FileName;               
        FileName.open(fname, ios::out);    
        if (!FileName)
            cout<<"Error while creating the file";    
        FileName.write(buffer+i, n-(i+1));
        while(1){
            n=read(fd,buffer,2048);
            if(n==-1) 
                exit(1);                  /*error*/
            if(n==0)
                break;
            FileName.write(buffer, n);
        }
        FileName.close();
        cout << "asset was saved in file " << fname << " " << fsize << endl;
    }
}

string send_tcp_message(string message, string type, ifstream* file) {

    int fd,errcode;
    ssize_t n;
    // socklen_t addrlen;               //TODO: WHY DONT WE NEED THIS?
    struct addrinfo hints,*res;
    // struct sockaddr_in addr;         //TODO: WHY DONT WE NEED THIS?
    
    const int server_awmser_size = 8192;
    char server_awnser[server_awmser_size] = "";

    fd=socket(AF_INET,SOCK_STREAM,0);   //TCP socket
    if (fd==-1) exit(1);                //error
    
    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;            //IPv4
    hints.ai_socktype=SOCK_STREAM;      //TCP socket
    
    errcode=getaddrinfo(SERVER, PORT, &hints, &res);     //TODO: CHANGE THIS TO ONLY HAPPEN ONE TIME
    if(errcode!=0) exit(1);             /*error*/

    n=connect(fd,res->ai_addr,res->ai_addrlen);
    if(n==-1) exit(1);                  /*error*/
    

    n=write(fd, message.c_str(), message.size());

    //Send the message to the server
    if (file != nullptr && file->is_open()) {
        const int file_data_size = 2048;
        char file_data[file_data_size] = "";


        int bytes_read = 0;
        while ((bytes_read = file->read(file_data, sizeof(file_data)).gcount()) > 0) {
            
            n=write(fd, file_data, bytes_read);
            if(n==-1) exit(1);                  /*error*/

            memset(file_data, 0, file_data_size);            // Clear the buffer
        }
        n=write(fd, "\n", 1);
    }

    n=read(fd, server_awnser, server_awmser_size);
    if(n==-1) exit(1);                  /*error*/
    
    freeaddrinfo(res);                  //TODO: CHANGE THIS TO ONLY HAPPEN ONE TIME
    close(fd);

    return server_awnser;
}
