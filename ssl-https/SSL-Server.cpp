// informit.com/articles/article.aspx?p=22078
// cs.utah.edu/~swalton/listings/articles/ssl_server.c
// wiki.openssl.org/index.php/Simple_TLS_Server

#include <cerrno>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <iostream>
using namespace std;

#define FAIL    -1

int isRoot() { return (getuid() != 0) ? 0 : 1; }

// OpenListener - create server socket
int OpenListener(int port) {
    int sd;
    struct sockaddr_in addr;

    sd = socket(PF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if ( bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 ) {
        perror("can't bind port");
        abort();
    }
    if ( listen(sd, 10) != 0 ) {
        perror("Can't configure listening port");
        abort();
    }
    return sd;
}

// InitServerCTX - initialize SSL server  and create context
SSL_CTX* InitServerCTX(void) {
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();      // load & register all cryptos, etc.
    SSL_load_error_strings();          // load all error messages
//    method = TLSv1_2_server_method();  // create new server-method instance
    method = SSLv23_method();
    ctx = SSL_CTX_new(method);         // create new context from method
    if ( !ctx ) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

// LoadCertificates - load from files.
void LoadCertificates(SSL_CTX* ctx, const char* CertFile, const char* KeyFile) {
    // set the local certificate from CertFile
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    // set the private key from KeyFile (may be the same as CertFile)
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    // verify private key
    if ( !SSL_CTX_check_private_key(ctx) ) {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}

// ShowCerts - print out certificates.
void ShowCerts(SSL* ssl) {
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl); // Get certificates (if available)
    if ( cert != NULL ) {
        cout << "Server certificates:" << endl;
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        cout << "Subject: " << line << endl;
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        cout <<"Issuer: " << line << endl;
        free(line);
        X509_free(cert);
    }
    else
        cout << "No certificates." << endl;
}

// Servlet - SSL servlet (contexts can be shared)
void Servlet(SSL* ssl) { // Serve the connection -- threadable
    char buf[1024];
    char reply[1024];
    int sd, bytes;
    const char* HTMLecho = "<html><body><pre>%s</pre></body></html>\n\n";

    if ( SSL_accept(ssl) == FAIL )     // do SSL-protocol accept
        ERR_print_errors_fp(stderr);
    else
    {
        ShowCerts(ssl);        // get any certificates
        bytes = SSL_read(ssl, buf, sizeof(buf));  // get request
        if ( bytes > 0 ) {
            buf[bytes] = 0;
            cout << "Client msg: " << buf << endl;
            sprintf(reply, HTMLecho, buf);        // construct reply
            SSL_write(ssl, reply, strlen(reply)); // send reply
        }
        else
            ERR_print_errors_fp(stderr);
    }
    sd = SSL_get_fd(ssl);   // get socket connection
    SSL_free(ssl);          // release SSL state
    close(sd);              // close connection
}

int main(int count, char *strings[]) {
    SSL_CTX *ctx;
    int server;
    char *portnum;

    if(!isRoot()) {
        cout << "This program must be run as root/sudo user!!" << endl;
        exit(0);
    }

    if ( count != 2 ) {
        cout << "Usage: %s <portnum>" << strings[0] << endl;
        exit(0);
    }
    SSL_library_init();

    portnum = strings[1];
    ctx = InitServerCTX();        // initialize SSL
    LoadCertificates(ctx, "certA.pem", "certA.pem"); // load certs
    server = OpenListener(atoi(portnum));    // create server socket
    while (1) {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        SSL *ssl;

        int client = accept(server, (struct sockaddr*)&addr, &len);  // accept connection as usual
        cout << "Connection: " << inet_ntoa(addr.sin_addr) << ":" << 
            ntohs(addr.sin_port) << endl;
        ssl = SSL_new(ctx);           // get new SSL state with context
        SSL_set_fd(ssl, client);      // set connection socket to SSL state
        Servlet(ssl);                 // service connection
    }
    close(server);          // close server socket
    SSL_CTX_free(ctx);      // release context
}
