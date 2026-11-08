/* ---- create ---- */
int fd = socket(AF_INET, SOCK_STREAM, 0);              /* -1 on error */

/* ---- allow fast restart: BEFORE bind ---- */
int yes = 1;
setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

/* ---- fill an IPv4 address ---- */
struct sockaddr_in addr;
memset(&addr, 0, sizeof addr);          /* C++: sockaddr_in addr{}; */
addr.sin_len         = sizeof addr;     /* macOS/BSD only field */
addr.sin_family      = AF_INET;
addr.sin_port        = htons(8080);     /* host → network order */
addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* 0.0.0.0: all interfaces */
/* or from text: inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
   returns 1 = ok, 0 = invalid string (errno NOT set!), -1 = error */

/* ---- bind + listen ---- */
bind(fd, (struct sockaddr*)&addr, sizeof addr);
listen(fd, SOMAXCONN);                  /* backlog: queue of pending handshakes */

/* ---- accept: blocks until a client connects ---- */
struct sockaddr_in peer;
socklen_t len = sizeof peer;            /* IN: buffer size, OUT: bytes written */
int cfd = accept(fd, (struct sockaddr*)&peer, &len);

/* ---- address → text ---- */
char ip[INET_ADDRSTRLEN];
inet_ntop(AF_INET, &peer.sin_addr, ip, sizeof ip);
unsigned short port = ntohs(peer.sin_port);

/* ---- the address actually bound (needed when you bind port 0) ---- */
struct sockaddr_in local;  socklen_t llen = sizeof local;
getsockname(fd, (struct sockaddr*)&local, &llen);

/* ---- per-socket options ---- */
setsockopt(cfd, SOL_SOCKET, SO_NOSIGPIPE, &yes, sizeof yes);  /* macOS: no SIGPIPE */
fcntl(fd, F_SETFD, FD_CLOEXEC);          /* don't leak fd into exec'd children */
int fl = fcntl(fd, F_GETFL, 0);
fcntl(fd, F_SETFL, fl | O_NONBLOCK);     /* non-blocking (kqueue phase) */