#include <cstdio>
#include <cerrno>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
using namespace std;

#define FAIL    -1

// OpenConnection - create socket and connect to server.
int OpenConnection(const char *hostname, int port) {
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;

    if ( (host = gethostbyname(hostname)) == NULL ) {
        perror(hostname);
        abort();
    }

    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);

    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 ) {
        close(sd);
        perror(hostname);
        abort();
    }
    return sd;
}

// InitCTX - initialize the SSL engine.
SSL_CTX* InitCTX(void) {
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();      // Load cryptos, et.al.
    SSL_load_error_strings();   // Bring in and register error messages
//    method = TLSv1_2_client_method();  // Create new client-method instance
    method = SSLv23_method();
    ctx = SSL_CTX_new(method);         // Create new context
    if ( ctx == NULL ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

// ShowCerts - print out the certificates.
void ShowCerts(SSL* ssl) {
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl); // get the server's certificate
    if ( cert != NULL ) {
        cout << "Server certificates:" << endl;
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        cout << "Subject: " << line << endl;
        free(line);          // free the malloc'ed string
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        cout << "Issuer: " << line << endl;
        free(line);          // free the malloc'ed string
        X509_free(cert);     // free the malloc'ed certificate copy
    }
    else
        cout << "Info: No client certificates configured." << endl;
}

// main - create SSL context and connect
int main(int count, char *strings[]) {
    SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char buf[1024];
    int bytes;
    char *hostname, *portnum;

    if ( count != 3 ) {
        cout << "usage: " << strings[0] << " <hostname> <portnum>" << endl;
        exit(0);
    }
    SSL_library_init();
    hostname=strings[1];
    portnum=strings[2];

    ctx = InitCTX();
    server = OpenConnection(hostname, atoi(portnum));
    ssl = SSL_new(ctx);               // create new SSL connection state
    SSL_set_fd(ssl, server);          // attach the socket descriptor
    if ( SSL_connect(ssl) == FAIL )   // perform the connection
        ERR_print_errors_fp(stderr);
    else {
        char acMsg[16] = {0};
        const char *msg = "%s";

        cout << "Enter message" << endl;
        cin >> acMsg;

        cout << "Connected with " << SSL_get_cipher(ssl) << 
            " encryption" << endl;
        ShowCerts(ssl);                          // get any certs
        SSL_write(ssl, acMsg, strlen(acMsg));        // encrypt & send message
        bytes = SSL_read(ssl, buf, sizeof(buf)); // get reply & decrypt
        buf[bytes] = 0;
        cout<< "Received: \"" << buf << "\"" << endl;
        SSL_free(ssl);        // release connection state
    }
    close(server);            // close socket
    SSL_CTX_free(ctx);        // release context
}
