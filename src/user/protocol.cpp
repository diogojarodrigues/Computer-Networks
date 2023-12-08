#include "protocol.hpp"

/* ####################### UDP MESSAGE #######################  */

const char* server_ip = SERVER_IP;
const char* port = PORT;

string send_udp_message(string message) {
    
    int sockett,errcode; 
    ssize_t aux;
    socklen_t addrlen;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    char buffer[8192];

    sockett=socket(AF_INET,SOCK_DGRAM,0);    //UDP socket
    if (sockett==-1) exit(1);                /*error*/

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;            //IPv4
    hints.ai_socktype=SOCK_DGRAM;       //UDP socket

    errcode=getaddrinfo(server_ip, port, &hints, &res);
    if(errcode!=0) exit(1);            /*error*/

    aux=sendto(sockett, message.c_str(), message.size(), 0, res->ai_addr, res->ai_addrlen);
    if(aux==-1) exit(1);                 /*error*/

    addrlen=sizeof(addr);
    aux=recvfrom(sockett,buffer, 8192, 0, (struct sockaddr*) &addr, &addrlen);
    if(aux==-1) exit(1);                 /*error*/

    freeaddrinfo(res);
    close(sockett);

    return buffer;
}

/* ####################### TCP MESSAGE #######################  */

void receive_tcp_image(int sockett){

    ssize_t aux;
    char buffer[2048];
    int i=0;
    int k=0;
    string response, fname, fsize;
    while (1){
        aux=read(sockett,buffer,1);
        if(aux==-1) exit(1);
        if (buffer[i]==' ' || buffer[i]=='\n'){
            k++;
            if(k==2 && response == "RSA NOK" ){
                cout << "asset does not exist" << endl;
                return;
            }
            if(k==1){
                response+=' ';
            }
            continue;
        }
        if(k<2){
            response+= buffer[i];
        }
        if(k==2){
            fname+=buffer[i];
        }
        if(k==3){
            fsize+=buffer[i];
        }
        if(k>3){
            break;
        }
    }

    fstream FileName;               
    FileName.open(fname, ios::out);    
    if (!FileName){
        cout<<"Error while creating the file";
        return;
    }    
    while(1){
        aux=read(sockett,buffer,2048);
        if(aux==-1) exit(1);                 /*error*/
        if(aux==0)
            break;
        FileName.write(buffer, aux);
    }
    FileName.close();
    cout << "asset was saved in file " << fname << " with " << fsize << " bytes"<<endl;


}

void send_tcp_image(int sockett, ifstream* file) {

    if (file != nullptr && file->is_open()) {
        const int file_data_size = 2048;
        char file_data[file_data_size] = "";

        int bytes_read = 0;
        int aux = 0;
        
        while ((bytes_read = file->read(file_data, sizeof(file_data)).gcount()) > 0) {
            
            aux=write(sockett, file_data, bytes_read);
            if(aux==-1) exit(1);                  /*error*/

            memset(file_data, 0, file_data_size);            // Clear the buffer
            
        }

        aux=write(sockett, "\n", 1);
        if(aux==-1) exit(1);                  /*error*/
    }
}

string send_tcp_message(string message, type type, ifstream* file) {

    int sockett, aux;
    // socklen_t addrlen;               //TODO: WHY DONT WE NEED THIS?
    struct addrinfo hints,*res;
    // struct sockaddr_in addr;         //TODO: WHY DONT WE NEED THIS?
    
    const int server_awmser_size = 8192;
    char server_awnser[server_awmser_size] = "";

    // Initializing connection
    sockett=socket(AF_INET,SOCK_STREAM,0);   //TCP socket
    if (sockett==-1) exit(1);                //error
    
    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;                //IPv4
    hints.ai_socktype=SOCK_STREAM;          //TCP socket
    
    aux=getaddrinfo(server_ip, port, &hints, &res);     //TODO: CHANGE THIS TO ONLY HAPPEN ONE TIME
    if(aux!=0) exit(1);                     /*error*/

    aux=connect(sockett,res->ai_addr,res->ai_addrlen);
    if(aux==-1) exit(1);                    /*error*/

    //Sending message
    aux=write(sockett, message.c_str(), message.size());
    if(aux==-1) exit(1);                    /*error*/           //TODO: TEMOS DE MUDAR DEPOIS ESTES ERROS

    if (type == RECEIVE_TCP_IMAGE) {
        printf("Receiving image\n");
        receive_tcp_image(sockett);
        return " ";
    } else if (type == SEND_TCP_IMAGE) {
        printf("Sending image\n");
        send_tcp_image(sockett, file);
    }

    //Receiving message
    aux=read(sockett, server_awnser, server_awmser_size);
    if(aux==-1) exit(1);                /*error*/
    
    // Closing connection
    freeaddrinfo(res);                  //TODO: CHANGE THIS TO ONLY HAPPEN ONE TIME
    close(sockett);

    return server_awnser;
}



/*aux=read(sockett,buffer,1);
    if(aux==-1) exit(1);                  error
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
        if (!FileName){
            cout<<"Error while creating the file";
            return;
        }    
        FileName.write(buffer+i, aux-(i+1));
        while(1){
            aux=read(sockett,buffer,2048);
            if(aux==-1) 
                exit(1);                  error
            if(aux==0)
                break;
            FileName.write(buffer, aux);
        }
        FileName.close();
        cout << "asset was saved in file " << fname << " whith " << fsize << " bytes"<<endl;
    }*/
