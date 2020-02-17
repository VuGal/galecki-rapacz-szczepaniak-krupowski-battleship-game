#include    <sys/types.h>   /* basic system data types */
#include    <sys/socket.h>  /* basic socket definitions */
#include    <sys/time.h>    /* timeval{} for select() */
#include    <sys/stat.h>
#include    <time.h>                /* timespec{} for pselect() */
#include    <netinet/in.h>  /* sockaddr_in{} and other Internet defns */
#include    <arpa/inet.h>   /* inet(3) functions */
#include 	<fcntl.h>
#include    <errno.h>
#include    <netdb.h>
#include    <signal.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <limits.h>		/* for OPEN_MAX */
#include    <syslog.h>
#include    <sys/epoll.h>
#include    <unistd.h>

#define STATS
#ifdef STATS
#include 	<pthread.h>
#endif
 
#define MAXLINE 1024
#define SA struct sockaddr
#define LISTENQ 2
#define INFTIM -1
#define MAXEVENTS 2000
#define	MAXFD	64


int
daemon_init(const char *pname, int facility, uid_t uid, int socket)
{
	int		i, p;
	pid_t	pid;

	if ( (pid = fork()) < 0)
		return (-1);
	else if (pid)
		exit(0);			/* proces rodzica umiera */

	/* dziecko 1 kontynuuje... */

	if (setsid() < 0)			/* zostaje liderem sesji */
		return (-1);

	signal(SIGHUP, SIG_IGN);
	if ( (pid = fork()) < 0)
		return (-1);
	else if (pid)
		exit(0);			/* dziecko 1 umiera */

	/* dziecko 2 kontynuuje... */

	chdir("/tmp");				/* zmiana folderu roboczego i chroot()*/
	chroot("/tmp");

	/* przekierowanie stdin, stdout i stderr do /dev/null */
	p= open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	openlog(pname, LOG_PID, facility);
	
    syslog(LOG_ERR," STDIN =   %i\n", p);
	setuid(uid); /* zmiana uzytkownika */
	
	return (0);	
}



int evn_r;
int activeconns_r;
long s_r;

	
#ifdef STATS
void *
print_r(void *arg)
{
	while (1){
		syslog(LOG_ERR,"\rService of %6d events in %10ld us : active connections: %6d", evn_r,s_r, activeconns_r);
		sleep(1);
	}
	return(NULL);
}
#endif

int
main(int argc, char **argv)
{
	int debug = 0;
	int		listenfd, listenfd_udp, connfd, sockfd, writefd;
	pid_t		childpid;
	socklen_t	clilen;
	struct sockaddr_in6	cliaddr, servaddr, servaddr_udp;
	int		i, maxi, maxfd, n; 
	int		nready, s;
	int		parity_flag;
	char 		buf[MAXLINE], addr_buf[INET6_ADDRSTRLEN+1];
    char        table_buf[13][13];
	struct 		timeval start, stop;
	int activeconns = 0;
	struct sockaddr_storage peer_addr;
	socklen_t peer_addr_len;
	

	printf("\nUruchomiono serwer!\n\n");
	syslog(LOG_NOTICE, "MAX EVENTS = %d\n", MAXEVENTS-2);
	
	if( argc > 1 )
		debug = atoi(argv[1]);
	
	int epollfd, eventstriggered, currfd;
	struct epoll_event events[MAXEVENTS];
	struct epoll_event ev;
	
	
		/* stworzenie kolejki do epoll() */
	if((epollfd = epoll_create(MAXEVENTS)) == -1){ 	
          syslog(LOG_ERR, "epoll_create() error : %s\n", strerror(errno));
          return -1;
   }
	
	//stworzenie gniazd sluchajacych	
	if ( (listenfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0){
		syslog(LOG_ERR, "TCP socket error : %s\n", strerror(errno));
		return -1;
	}

	int on = 1;               
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
	      syslog(LOG_ERR, "SO_REUSEADDR setsockopt error : %s\n", strerror(errno));
	}

	int off = 0;         
    if (setsockopt(listenfd, SOL_IPV6, IPV6_V6ONLY, off, sizeof(off)) < 0)
	{
        syslog(LOG_ERR, "IPV6_V6ONLY setsockopt error : %s\n", strerror(errno));
    }

	if ( (listenfd_udp = socket(AF_INET6, SOCK_DGRAM, 0)) < 0){
		syslog(LOG_ERR, "UDP socket error : %s\n", strerror(errno));
		return 1;
	}   

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family = AF_INET6;
	servaddr.sin6_addr   = in6addr_any;
	servaddr.sin6_port   = htons(8080);

	if ( bind( listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
		syslog(LOG_ERR, "TCP bind error : %s\n", strerror(errno));
		return 1;
	}

	if ( bind( listenfd_udp, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
		syslog(LOG_ERR, "UDP bind error : %s\n", strerror(errno));
		return 1;
	}

	if ( listen(listenfd, LISTENQ) < 0){
		syslog(LOG_ERR, "listen error : %s\n", strerror(errno));
		return 1;
	}

	activeconns +=2;	
	activeconns_r = activeconns;
	int time_start=0;
	int evn;

	ev.events = EPOLLIN; // ktorego eventu nasluchujemy
	ev.data.fd = listenfd_udp; // dane uzytkownika
	
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd_udp, &ev) == -1){
           syslog(LOG_ERR, "listen error : %s\n", strerror(errno));
           return 1;
	}

	ev.events = EPOLLIN; // ktorego eventu nasluchujemy
	ev.data.fd = listenfd; // dane uzytkownika

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) == -1){
           syslog(LOG_ERR,"listen error : %s\n", strerror(errno));
           return 1;
	}
	

#ifdef STATS
	int ret;
	pthread_t	tid;
	if ( (ret=pthread_create(&tid, NULL, print_r, NULL)) != 0 ) {
	    errno=ret;
	    syslog(LOG_ERR, "pthread_create() error : %s", strerror(ret));
	}
#endif

#define AVG 1000
	long ss[AVG];
	int count=0;
	int iter=0;

	daemon_init(argv[0], LOG_USER, 1000, listenfd);

	syslog (LOG_NOTICE, "Program rozpoczety przez uzytkownika %d", getuid ());
	syslog (LOG_INFO, "Oczekiwanie na klientow ...");

	for ( ; ; ) {


	if( gettimeofday(&stop,0) != 0 ){
		syslog(LOG_ERR, "gettimeofday error : %s\n", strerror(errno));
	}else{
		if(time_start == 1){
			long s =(long)((stop.tv_sec*1000000 + stop.tv_usec) - (start.tv_sec*1000000 + start.tv_usec));
			if(count < 100) count++;
			if(iter > 99) iter=0;
			ss[iter]=s;
			long sum=0;
			int k;
			for(k=0; k < count; k++) sum+=ss[iter];	
			iter++; 
			s_r=sum/count; 	evn_r = evn; 	activeconns_r =  activeconns;
			time_start=0;  
		}
	}
	
    	if( (nready = epoll_wait(epollfd, events, MAXEVENTS, -1)) == -1){
           syslog(LOG_ERR,"epoll_wait error : %s\n", strerror(errno));
           return 1;
		}
		
		if(debug)
			printf( "New events=%d, active connections: %d\n", nready, activeconns);
			

	if( gettimeofday(&start,0) != 0 ){
	      syslog(LOG_ERR, "gettimeofday error : %s\n", strerror(errno));
	}else{
	      time_start=1;
	      evn = nready;
	}

	for (i = 0; i < nready; i++) {	/* sprawdzamy po kolei klientow czy wysylaja dane */

		      
      //gniazdo nasluchujace TCP
	      if (events[i].data.fd == listenfd) { //e
			clilen = sizeof(cliaddr);
			if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
				    //  perror("accept error");
				      continue;
			}

      if(debug)		   
      printf ( "\tnew TCP client: events=%d, sockfd = %d, on socket = %d,  activeconns = %d \n", nready, listenfd, connfd, activeconns);
		      
			bzero(addr_buf, sizeof(addr_buf));
			inet_ntop(AF_INET6, (struct sockaddr  *) &cliaddr.sin6_addr,  addr_buf, sizeof(addr_buf));
			if(debug)
			      printf("\tNew client: %s, port %d\n",	addr_buf, ntohs(cliaddr.sin6_port));

			if ((activeconns+1) >= MAXEVENTS) { //epoll
			      // w przypadku zbyt duzej ilosci polaczen
				      syslog(LOG_ERR, "accept() error - too many connections for this program\n");
				      close(connfd);
				      continue;
			}
			ev.events = EPOLLIN ; // input i nie edge-triggered
			ev.data.fd = connfd;
			if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev)){
				      syslog(LOG_ERR, "epoll_ctl adding new connection error");
				      close(connfd);
				      continue;
			}
			activeconns ++;
			continue;
		  } //koniec epoll
      //koniec gniazda nasluchujacego TCP

      //klienci UDP
	      if (events[i].data.fd == listenfd_udp) { //epoll
if(debug) printf( "UDP connection\n");
			int nread;
			peer_addr_len = sizeof(struct sockaddr_storage);
			nread = recvfrom(listenfd_udp, buf, MAXLINE, 0,
			(struct sockaddr *) &peer_addr, &peer_addr_len);
			if (nread == -1)
			      continue;      // zignoruj nieudany request

			char host[NI_MAXHOST], service[NI_MAXSERV];

			s = getnameinfo((struct sockaddr *) &peer_addr,
			peer_addr_len, host, NI_MAXHOST,
			service, NI_MAXSERV, NI_NUMERICHOST);
			if (s == 0)
			      syslog(LOG_NOTICE, "UDP: Received %ld bytes from %s:%s\n",
				(long) nread, host, service);
			else
			      syslog(LOG_NOTICE, "getnameinfo: %s\n", gai_strerror(s));

			if (sendto(listenfd_udp, buf, nread, 0, (struct sockaddr *) &peer_addr,
									peer_addr_len) != nread)
			      syslog(LOG_ERR, "UDP Error sending response\n");

			continue; 
		}
      //koniec klientow UDP

      //klienci TCP
	    bzero(table_buf, 169);
		currfd = events[i].data.fd;

		if (currfd%2 == 0) 
		{
			writefd = currfd-1;
		}
        else 
		{
			writefd = currfd+1;
		}

if(debug)		printf( "\tRead client\n: events=%d, sockfd = %d\n", nready, currfd);
		if((n = read(currfd, table_buf, 169)) == -1) {
			  // Zamkniecie deskryptora sprawi, ze epoll usunie go ze zbioru monitorowanych deskryptorow
		      syslog(LOG_ERR, "read error - closing connection");
		      close(currfd);
			  bzero(table_buf, 169);
			  table_buf[0][0] = 60;
			  write(writefd, table_buf, 160);
			  close(writefd);
		      activeconns -=2;
		      continue;
		}
			
		 // Read z gniazda sie powiodl. Patrzymy czy dostalismy dane

		if(n == 0) {
		      // Gniazdo wyslalo EOF.
		      // Zamkniecie deskryptora sprawi, ze epoll usunie go ze zbioru monitorowanych deskryptorow
			  // Zamykamy obydwa gniazda z pary graczy

		      close(currfd);
			  bzero(table_buf, 169);
			  table_buf[0][0] = 60;
			  write(writefd, table_buf, 160);
			  close(writefd);
		      activeconns -=2;
		      continue;
		}
		
		if(strlen(table_buf) == 169) {
			if( write(writefd, table_buf, 169) == -1) 
			{	
				// Obsluga bledu
				syslog(LOG_ERR, "Write error (table_buf strlen : 169)\n");
				close(currfd);
				activeconns --;
				continue;
			}
		}

		if(strlen(table_buf) == 1) {
			if( write(writefd, table_buf, 160) == -1) 
			{
				// Obsluga bledu
				syslog(LOG_ERR, "Write error (table_buf strlen : 1)\n");
				close(currfd);
				activeconns --;
				continue;
			}
		}

		if(strlen(table_buf) != 1 && strlen(table_buf) != 169) {
			write(writefd, table_buf, sizeof(table_buf)) == -1;
			bzero(table_buf, 169);
		}
        
      //koniec epoll
	    }
	}
}