#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <time.h>

#define INF 1e100

#define err_quit(fmt, msg) {\
  fprintf(stderr, fmt, msg);\
  exit(1);\
}

#ifndef DEBUG
#define DEBUG 1
#endif
#if DEBUG
#define debug_print printf
#else
void
debug_print(char* msg,...)
{}
#endif

#define TIME_OUT     0
#define SOCKET_READY 1

#define RMSG_SIZE 65536

// type

typedef char nodename_tp[8];
typedef unsigned int state_tp;
typedef double distance_tp;

// 发送的信息的单个节点
typedef struct {
  nodename_tp name;
  distance_tp distance;
  state_tp state;
} distance_msg_tp;

// 发送的信息
typedef struct {
  size_t length;
  nodename_tp name;
  unsigned int node_num;
  distance_msg_tp vector[0];
} msg_tp;

// 路由表中的单个节点
typedef struct {
  distance_msg_tp dest;
  nodename_tp next_node;
} routing_table_tp;

// 路由表的链表
// 保证第一个数据是自己
struct routing_table_tp_l {
  routing_table_tp v;
  struct routing_table_tp_l* n;
};
typedef struct routing_table_tp_l routing_table_tp_l;

// 保存邻居的sockaddr_in
// 用来发送信息
typedef struct {
  unsigned int num;
  struct sockaddr_in ad[0];
} s_neighbors_tp;

// 保存邻居的距离信息的节点
typedef struct {
  nodename_tp nodename;
  distance_tp distance;
} neighbors_tp_v;

// 邻居的距离数组
typedef struct {
  // 邻居节点数量
  int num;
  neighbors_tp_v val[0];
} neighbors_tp;

// 用来给邻居发送消息用
s_neighbors_tp *s_neighbors;
// 邻居的距离
neighbors_tp* neighbors;
// socket
int sockfd;
// r=收到，s=发送
msg_tp *rmsg, *smsg;
// 路由表
routing_table_tp_l *dvl;
// 文件名
char filename[128];
// 记录上一次发送数据的时间
time_t timer;
// 刷新速度
int TIME_OUT_T = 3;

// 文件初始化
void
init_file(fp)
  FILE *fp;
{
  int i, n;
  nodename_tp name;
  char node[128];
  distance_tp d;
  int port;
  routing_table_tp_l *p, *q;
  struct hostent *ptrh;

  // n 节点数
  // name 自己的名字
  fscanf(fp, "%d %s", &n, name);
  debug_print("myname = %s, neighbors num = %d\n", name, n);

  // 初始化邻居节点
  neighbors = (neighbors_tp*)
    malloc(sizeof(neighbors_tp)+(n+1)*sizeof(neighbors_tp_v));
  neighbors->num = n;
  debug_print("neighbors->num = %d\n", neighbors->num);

  // 初始化发送消息用的数组
  s_neighbors = (s_neighbors_tp*)
    malloc(sizeof(s_neighbors_tp)+n*sizeof(struct sockaddr_in));
  s_neighbors->num = n;
  debug_print("s_neighbors->num = %d\n", s_neighbors->num);

  // 将自己的信息添加到路由表表头
  dvl=(routing_table_tp_l*)malloc(sizeof(routing_table_tp_l));
  strcpy(dvl->v.dest.name, name);
  strcpy(dvl->v.next_node, name);
  dvl->v.dest.distance = 0;
  dvl->v.dest.state = 2;
  p = dvl;
  debug_print("dvl->v.dest.name = %s, dvl->v.dest.distance = %lf, dvl->v.dest.state = %d\n",
    dvl->v.dest.name, dvl->v.dest.distance, dvl->v.dest.state);

  for (i = 0; i < n; i++) {
    // name 目标节点名字
    // node 目标节点位置
    // d 目标节点距离
    // port 目标节点使用的端口号
    fscanf(fp, "%s %s %lf %d", name, node, &d, &port);
    debug_print("node =  %s, distance = %lf, on %s using %d\n", name, d, node, port);
    if (d < 0) d = INF;
    
    // 邻居数组
    strcpy(neighbors->val[i].nodename, name);
    neighbors->val[i].distance = d;

    // 发送消息的列表
    memset((char *)(s_neighbors->ad+i),0,sizeof(struct sockaddr_in));
    s_neighbors->ad[i].sin_family = AF_INET;
    s_neighbors->ad[i].sin_port = htons((u_short)port);
    ptrh = gethostbyname(node);
    if ( ((char *)ptrh) == NULL ) err_quit("invalid host: %s\n", node);
    memcpy(&s_neighbors->ad[i].sin_addr, ptrh->h_addr, ptrh->h_length);

    // 路由表信息加入该节点
    q = (routing_table_tp_l*)malloc(sizeof(routing_table_tp_l));
    strcpy(q->v.dest.name, name);
    strcpy(q->v.next_node, name);
    q->v.dest.distance = d;
    q->v.dest.state = 0;
    p->n = q; p = q;
  }

  p->n = NULL;

  debug_print("file reading done.\n");
}

// 添加端口监听
void
listen_port(port)
  u_short port;
{
  struct sockaddr_in ad;

  debug_print("listen port %d\n", port);
  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
    err_quit("%s\n","socket creation failed");
  debug_print("socked creation done\n");

  memset((char *)&ad, 0, sizeof(ad));
  ad.sin_family = AF_INET;
  ad.sin_addr.s_addr = INADDR_ANY;
  ad.sin_port = htons((u_short)port);
  if (bind(sockfd, (struct sockaddr *)&ad, sizeof(ad)) < 0)
    err_quit("%s\n","bind failed");
  debug_print("listen port %d done.\n", port);
}

// 处理参数
void
init(argc, argv)
  int argc;
  char **argv;
{
  FILE *fp;

  if (argc != 3 && argc != 4) goto ARG_FAIL;
  int port = atoi(argv[1]);
  if (port <= 0) goto ARG_FAIL;
  if (argc == 4) {
    TIME_OUT_T = atoi(argv[3]);
    if (TIME_OUT_T <= 0) goto ARG_FAIL;
  }
  strcpy(filename,argv[2]);
  fp = fopen(filename, "r");
  if (!fp) goto ARG_FAIL;
  init_file(fp);
  fclose(fp);
  listen_port(port);
  rmsg = (msg_tp*)malloc(RMSG_SIZE);
  return;
ARG_FAIL:
  err_quit("Useage:\n\t%s port dat_filename [second_to_refresh]\n", argv[0]);
}


// 接收消息，保存在rmsg内
void
recv_msg()
{
  socklen_t alen;
  size_t length;

  debug_print("recive data...\n");
  alen = sizeof(struct sockaddr);
  length = recv(sockfd, rmsg, RMSG_SIZE, 0);
  debug_print("recived %d bytes.\n", length);
}

// 将smsg发送出去
void
send_msg()
{
  int i, alen;
  alen = sizeof(struct sockaddr);
  for (i=0;i<s_neighbors->num;i++) {
    sendto(sockfd, smsg, smsg->length, 0,
      (struct sockaddr *)(s_neighbors->ad+i), alen );
    debug_print("sending %d bytes to #%d neighbor.\n", smsg->length, i);
  }
  timer = time(NULL);
}

// 等待超时或收到socket信息
// 返回值：SOCKET_READY / TIME_OUT
int
wait_socket()
{
  int     maxfdp1;
  fd_set  rset;
  struct timeval tv;

  tv.tv_sec = (TIME_OUT_T - (time(NULL) - timer));
  tv.tv_usec = 0;
  debug_print("waiting for input or timout(%ds).\n", (int)tv.tv_sec);

  FD_SET(sockfd, &rset);
  maxfdp1 = sockfd + 1;

  select(maxfdp1, &rset, NULL, NULL, &tv);

  if (FD_ISSET(sockfd, &rset)) {
    debug_print("socket ready.\n");
    return SOCKET_READY;
  }
  timer = time(NULL);
  debug_print("time out.\n");
  return TIME_OUT;
}

// 发送消息
void
send_vec()
{
  static unsigned long print_out_num = 0;
  int i, length;
  routing_table_tp_l *p;

  if (smsg) free(smsg);

  for (length = 0, p = dvl; p; length++, p = p->n);
  debug_print("known nodes: %d\n", length);

  smsg = (msg_tp*)malloc(sizeof(msg_tp)+length*sizeof(distance_msg_tp));
  smsg->length = sizeof(msg_tp)+length*sizeof(distance_msg_tp);
  strcpy(smsg->name, dvl->v.dest.name);
  smsg->node_num = length;
  debug_print("smsg->length = %d\n", smsg->length);

  printf("\n## print-out number %ld\n", ++print_out_num);
  for (i = 0, p = dvl; i < length; i++, p = p->n) {
    memcpy(smsg->vector+i, &(p->v.dest), sizeof(distance_msg_tp));
    printf("shortest path to node %s (seq# %d): the next hop is %s and the cost is ",
      smsg->vector[i].name, smsg->vector[i].state, p->v.next_node
    );
    if (smsg->vector[i].distance >= INF) printf("INF");
    else printf("%0.1lf", smsg->vector[i].distance);
    printf(".\n");
  }
  debug_print("smsg copy done.\n");

  send_msg();
}

void
timeout()
{
  int i, n;
  nodename_tp name;
  char node[128];
  distance_tp d;
  int port;
  routing_table_tp_l *p, *q;
  FILE *fp;

  fp = fopen(filename, "r");
  if (!fp) return;

  p = dvl;

  debug_print("reload file...\n");

  // n 节点数
  // name 自己的名字
  fscanf(fp, "%d %s", &n, name);
  debug_print("myname = %s, neighbors num = %d\n", name, n);

  for (i = 0; i < n; i++) {

    p = p->n;

    // name 目标节点名字
    // node 目标节点位置
    // d 目标节点距离
    // port 目标节点使用的端口号
    if (fscanf(fp, "%s %s %lf %d", name, node, &d, &port) != 4) goto ERR_RETURN;
    debug_print("node =  %s, distance = %lf, on %s using %d\n", name, d, node, port);
    if (d < 0) d = INF;

    // 检查节点名称是否符合
    // 不符合则直接退出
    if (strcmp(name, neighbors->val[i].nodename)) goto ERR_RETURN;

    debug_print("distance0 for %s = %lf\n",
      neighbors->val[i].nodename, neighbors->val[i].distance);

    // 邻居数组
    if (neighbors->val[i].distance > d) {
      debug_print("distance changed ~ o\n");
      neighbors->val[i].distance = d;
      for (q = dvl; q; q = q->n) {
        if (!strcmp(q->v.next_node, name))
        if (q->v.dest.distance > d) {
          q->v.dest.distance = d;
          strcpy(q->v.next_node, name);
        }
      }
      if (p->v.dest.distance > d) p->v.dest.distance = d;
    } else if (neighbors->val[i].distance < d) {
      debug_print("distance changed ~ x\n");
      neighbors->val[i].distance = d;
      p->v.dest.state = (p->v.dest.state + 2) | 1;
      p->v.dest.distance = d;
      for (q = dvl; q; q = q->n) {
        if (!strcmp(q->v.next_node, name)) {
          q->v.dest.state = (q->v.dest.state + 2) | 1;
          q->v.dest.distance = INF;
        }
      }
    } else debug_print("distance for node %s not changed\n", p->v.dest.name);

  }
  fclose(fp);
  debug_print("reload file done.\n");
  return;
ERR_RETURN:
  fclose(fp);
  err_quit("%s\n", "Unsupport for network topology changes.");
}

int
update_rt()
{
  int i, j, changed = 0;
  routing_table_tp_l *p, *q;
  distance_tp sender_distance;

  debug_print("sender node %s.\n", rmsg->name);
  for (i = 0; i < neighbors->num; i++)
    if (!strcmp(neighbors->val[i].nodename, rmsg->name)) break;
  if (i == neighbors->num) err_quit("%s", "Unsupport graphic.");
  sender_distance = neighbors->val[i].distance;
  debug_print("distance[sender] = %lf.\n", sender_distance);

  debug_print("package length   = %d.\n", rmsg->length);
  debug_print("package node_num = %d.\n", rmsg->node_num);
  for (i = 0; i < rmsg->node_num; i++) {

    debug_print("@%02d/%02d: ", i, rmsg->node_num);
    debug_print("node %s : ", rmsg->vector[i].name);

    if (!strcmp(dvl->v.dest.name, rmsg->vector[i].name)) {
      debug_print("(myself) ");
      debug_print("satate0 = %d, satae\' = %d.\n", dvl->v.dest.state, rmsg->vector[i].state);
      if (rmsg->vector[i].state > dvl->v.dest.state) {
        dvl->v.dest.state = (rmsg->vector[i].state + 2) & ~1U;
        debug_print("change mystate to %d.", dvl->v.dest.state);
        changed = 1;
      }
      continue;
    }


    for (j=0, p = dvl; p->n; j++, p = p->n)
      if (!strcmp(p->v.dest.name, rmsg->vector[i].name)) break;
    if (!strcmp(p->v.dest.name, rmsg->vector[i].name)) {
      // 节点在记录中
      if (
        // 记录的seq比较小
        (p->v.dest.state < rmsg->vector[i].state) ||
        (
         // 记录的seq与收到的相同，取距离较小值
         (p->v.dest.state == rmsg->vector[i].state) &&
         (p->v.dest.distance > rmsg->vector[i].distance + sender_distance)
        )
      ) {
        // 更新距离和状态
        p->v.dest.distance = rmsg->vector[i].distance + sender_distance;
        p->v.dest.state = rmsg->vector[i].state;
        strcpy(p->v.next_node, rmsg->name);
        debug_print("state0 = %d, state = %d.\n", p->v.dest.state, rmsg->vector[i].state);
        debug_print("distance changed to %lf (= %lf + %lf).\n",
          rmsg->vector[i].distance, rmsg->vector[i].distance, sender_distance);
        changed = 1;
      } else if (p->v.dest.state > rmsg->vector[i].state)
        debug_print("known state larger.\n");
      else debug_print("distance' = %lf + %lf >= %lf .\n",
        rmsg->vector[i].distance, sender_distance, p->v.dest.distance);
    } else {
      // 一个之前不知道的节点
      debug_print("(new) ");
      q = (routing_table_tp_l*)malloc(sizeof(routing_table_tp_l));
      strcpy(q->v.dest.name, rmsg->vector[i].name);
      strcpy(q->v.next_node, rmsg->name);
      q->v.dest.distance = rmsg->vector[i].distance + sender_distance;
      q->v.dest.state = rmsg->vector[i].state;
      q->n = NULL;
      p->n = q; p = q;
      debug_print("state = %d, distance = %lf (= %lf + %lf).\n",
        q->v.dest.state, q->v.dest.distance, rmsg->vector[i].distance, sender_distance);
      changed = 1;
    }
  }
  return changed;
}

int
main(argc, argv)
  int argc;
  char** argv;
{
  debug_print("aloha~_~\n\n");
  init(argc, argv);
  send_vec();

  for (;;) {
    if (wait_socket()==SOCKET_READY) {
      recv_msg();
      if (update_rt()) send_vec();
    } else {
      timeout();
      send_vec();
    }
  }

  return 0;
}
