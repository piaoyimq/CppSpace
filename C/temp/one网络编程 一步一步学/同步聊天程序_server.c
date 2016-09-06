#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAXBUF 1024
/************���ڱ��ĵ�********************************************
*filename: sync-server.c
*purpose: ��ʾ����ͬ��ͨѶ�����Ƿ������˳���
*wrote by: zhoulifa(zhoulifa@163.com) ������(http://zhoulifa.bokee.com)
Linux������ Linux֪ʶ������ SOHO�� ������ ���ó�C����
*date time:2007-01-25 20:26
*Note: �κ��˿������⸴�ƴ��벢������Щ�ĵ�����Ȼ���������ҵ��;
* ������ѭGPL
*Thanks to: Google.com
*Hope:ϣ��Խ��Խ����˹����Լ���������Ϊ��ѧ������չ����
* �Ƽ�վ�ھ��˵ļ���Ͻ������죡��л�п�Դǰ���Ĺ��ף�
*********************************************************************/
int main(int argc, char **argv)
{
int sockfd, new_fd;
socklen_t len;
struct sockaddr_in my_addr, their_addr;
unsigned int myport, lisnum;
char buf[MAXBUF + 1];
if (argv[1])
myport = atoi(argv[1]);
else
myport = 7838;
if (argv[2])
lisnum = atoi(argv[2]);
else
lisnum = 2;
if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
perror("socket");
exit(1);
}
bzero(&my_addr, sizeof(my_addr));
my_addr.sin_family = PF_INET;
my_addr.sin_port = htons(myport);
if (argv[3])
my_addr.sin_addr.s_addr = inet_addr(argv[3]);
else
my_addr.sin_addr.s_addr = INADDR_ANY;
if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr))
== -1) {
perror("bind");
exit(1);
}
if (listen(sockfd, lisnum) == -1) {
perror("listen");
exit(1);
}
while (1) {
printf
("\n----�ȴ��µ����ӵ�����ʼ��һ�����졭��\n");
len = sizeof(struct sockaddr);
if ((new_fd =
accept(sockfd, (struct sockaddr *) &their_addr,
&len)) == -1) {
perror("accept");
exit(errno);
} else
printf("server: got connection from %s, port %d, socket %d\n",
inet_ntoa(their_addr.sin_addr),
ntohs(their_addr.sin_port), new_fd);

/* ��ʼ����ÿ���������ϵ������շ� */
while (1) {
bzero(buf, MAXBUF + 1);
printf("������Ҫ���͸��Է�����Ϣ��");
fgets(buf, MAXBUF, stdin);
if (!strncasecmp(buf, "quit", 4)) {
printf("�Լ�������ֹ���죡\n");
break;
}
len = send(new_fd, buf, strlen(buf) - 1, 0);
if (len > 0)
printf
("��Ϣ:%s\t���ͳɹ�����������%d���ֽڣ�\n",
buf, len);
else {
printf
("��Ϣ'%s'����ʧ�ܣ����������%d��������Ϣ��'%s'\n",
buf, errno, strerror(errno));
break;
}
bzero(buf, MAXBUF + 1);
/* ���տͻ��˵���Ϣ */
len = recv(new_fd, buf, MAXBUF, 0);
if (len > 0)
printf
("������Ϣ�ɹ�:'%s'����%d���ֽڵ�����\n",
buf, len);
else {
if (len < 0)
printf
("��Ϣ����ʧ�ܣ����������%d��������Ϣ��'%s'\n",
errno, strerror(errno));
else
printf("�Է��˳��ˣ�������ֹ\n");
break;
}
}
close(new_fd);
/* ����ÿ���������ϵ������շ����� */
}
close(sockfd);
return 0;
}