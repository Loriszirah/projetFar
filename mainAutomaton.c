#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/ssl.h>

char RFID[100]="1002018";
//gcc mainAutomaton.c -o mainAutomaton

int main ( int argc, char **argv ) {
     int state = 1;
     while(state == 0){
     	//Reception des infos concernant la partie
     	state = partieToDweet();
	     if(state == 0){
	     	printf("I am inactive\n");
	     }
	 }

	 sendIpDweet();

     return 0;
}


void Error(char *mess, int sock){
	  fprintf(stderr,"%s\n",mess);
	  if (sock) close(sock);
	  exit(-1);
}

int sendIpDweet(){
	int sock;
	
	/* Saisie des infos de connexion */
	  char host[100]="dweet.io";
	  int port=80;

	  /* Calcul de l'IP de l'hote */
	  struct hostent *hostinfo;
	  hostinfo = gethostbyname(host); 
	  if (hostinfo) {
	    struct in_addr  **pptr;
	    pptr = (struct in_addr **)hostinfo->h_addr_list;
	    printf("Adresse IP de l'hote : %s\n",inet_ntoa(**(pptr)));
	  } 

	  /* if ( hostinfo = gethostbyname(host) )
	    memcpy((void*)&sin.sin_addr,(void*)hostinfo->h_addr,hostinfo->h_length);
	  else if( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
	    Error("unknown host");
	  */

	  /* Creation de la socket */
	  if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) <0 )
	    Error("can't create socket", sock);
	 
	  /* Configuration de la connexion */

	  struct sockaddr_in sin;
	 
	  sin.sin_addr = *(struct in_addr *) hostinfo->h_addr;
	  sin.sin_family = AF_INET;
	  if ( (sin.sin_port = htons(port)) == 0)
	    Error("unknown service", sock);

	  /* Tentative de connexion au serveur */
	  if (connect(sock, (struct sockaddr*)&sin,sizeof(sin))<0)
	    Error("can't connect", sock);
	  else printf("Connexion a %s sur le port %d\n", inet_ntoa(sin.sin_addr),
	         htons(sin.sin_port));
	   
	  /* Envoie de l'ip à DWEET */ 
      char buffer[1024]="";
      strcat(buffer,"GET /dweet/for/partieFar?type_msg=IP&type_ent=RJ&num=");
      strcat(buffer,RFID);
      strcat(buffer,"&data=192.168.1.10");
      strcat(buffer, " HTTP/1.1\nHost: ");
      strcat(buffer, host);
  	  strcat(buffer, "\n\n");
  	  printf("%s\n",buffer );
  	  send(sock, buffer, 1024, 0);
}

int partieToDweet(){
	SSL_load_error_strings ();

  SSL_library_init ();

  SSL_CTX *ssl_ctx = SSL_CTX_new (SSLv23_client_method ());

  if (ssl_ctx==NULL) {printf("PBM creation objet ssl_ctx\n");exit(-1);}

  else {printf("SSL_CTX object successfully created\n");}
	int sock;
	
	/* Saisie des infos de connexion */
	  char host[100]="dweet.io";
	  int port=80;

	  /* Calcul de l'IP de l'hote */
	  struct hostent *hostinfo;
	  hostinfo = gethostbyname(host); 
	  if (hostinfo) {
	    struct in_addr  **pptr;
	    pptr = (struct in_addr **)hostinfo->h_addr_list;
	    printf("Adresse IP de l'hote : %s\n",inet_ntoa(**(pptr)));
	  } 

	  /* if ( hostinfo = gethostbyname(host) )
	    memcpy((void*)&sin.sin_addr,(void*)hostinfo->h_addr,hostinfo->h_length);
	  else if( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
	    Error("unknown host");
	  */

	  /* Creation de la socket */
	  if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) <0 )
	    Error("can't create socket", sock);
	 
	  /* Configuration de la connexion */

	  struct sockaddr_in sin;
	 
	  sin.sin_addr = *(struct in_addr *) hostinfo->h_addr;
	  sin.sin_family = AF_INET;
	  if ( (sin.sin_port = htons(port)) == 0)
	    Error("unknown service", sock);

	  /* Tentative de connexion au serveur */
	  if (connect(sock, (struct sockaddr*)&sin,sizeof(sin))<0)
	    Error("can't connect", sock);
	  else printf("Connexion a %s sur le port %d\n", inet_ntoa(sin.sin_addr),
	         htons(sin.sin_port));

	  SSL *SSLconn = SSL_new(ssl_ctx);

 	 if (SSLconn==NULL) {printf("PBM: structure de donnees SSL non initialisee\n");exit(-1);}
  	 else {printf("Structure de donnees SSL initialisee\n");}
  	 if (SSL_set_fd(SSLconn, sock)==0) {  printf("PBM in connecting the SSL object with the socket file descriptor\n");exit(-1); }
 	 else {printf("Connecting the SSL object with the socket file descriptor: DONE\n");}
 	 int retConn = SSL_connect(SSLconn);  /* perform the connection */
     printf("retConn = %d\n",retConn); //fflush(stdout);
 	 if (retConn>0) {printf("Connexion SSL ok\n");}
     else {printf("Erreur connexion SSL : code %d\n",retConn);} 
	  /* Envoi de donnees au serveur */    
	    /*char buffer[1024] = "GET ";
	    strcat(buffer, file);
	    strcat(buffer, " HTTP/1.1\nHost: ");
	    strcat(buffer, host);
	    strcat(buffer, "\n\n");
	    printf("Requete : \n%s", buffer);*/
	    
	      char buffer[1024]="";
	      strcat(buffer,"GET /get/latest/dweet/for/partieFar");
	      strcat(buffer, " HTTP/1.1\nHost: ");
	      strcat(buffer, host);
      	  strcat(buffer, "\n\n");
      	  printf("%s\n",buffer );

	      // TO DO : test whether this suceeds or Error("write error on socket")
	      char commande[256]="";
		  //int s=send(sock, buffer, 1024, 0);
		  SSL_write(SSLconn, buffer, 1024); 

	      //int res = recv(sock,commande,255,0);
	      SSL_read(SSLconn, commande, 255);
	      //commande[res] = '\0';
	      printf("Resultat : %s\n",commande);
	      char *depart;
	      //depart = strtok(commande,"{");
	      //strtok(commande,"{");
	      //strcpy(depart,strtok(commande,"{"));
	      //printf("%s\n",depart);

	      /* Fermeture de la socket client */
	      //close(sock);

	      /*if(depart[10]!='0'){
	      	return 1;
	      }
	      else{
	      	return 0;
	      }
	      */
	}//main