#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAXBUF 1024











/************���ڱ��ĵ�********************************************
// *filename: sync-client.c
*purpose: ��ʾ����ͬ��ͨѶ�����ǿͻ��˳���
*wrote by: zhoulifa(zhoulifa@163.com) ������(http://zhoulifa.bokee.com)
Linux������ Linux֪ʶ������ SOHO�� ������ ���ó�C����
*date time:2007-01-25 20:32
*Note: �κ��˿������⸴�ƴ��벢������Щ�ĵ�����Ȼ���������ҵ��;
* ������ѭGPL
*Thanks to: Google.com
*Hope:ϣ��Խ��Խ����˹����Լ���������Ϊ��ѧ������չ����
* �Ƽ�վ�ھ��˵ļ���Ͻ������죡��л�п�Դǰ���Ĺ��ף�



����������������
gcc -Wall sync-server.c -o server
gcc -Wall sync-client.c -o client
�ֱ���������������
./server 7838 1
./client 127.0.0.1 7838
ͬ������������£�
1������˳�����
2���ͻ��˳����������ͻ������ӷ�������
3�����������û�������Ϣ�����͸��ͻ���
4���ͻ����յ���Ϣ���ͻ����û�������Ϣ�����͸���������
5�����������յ���Ϣ�����������û�������Ϣ�����͸��ͻ���
6���κ�һ���˳���ǰ���켴��ֹһ���������
7���������˼����ȴ���һ��������������ʼ�µ��������
*********************************************************************/
int main(int argc, char **argv)
{
int sockfd, len;
struct sockaddr_in dest;
char buffer[MAXBUF + 1];
if (argc != 3) {
printf
("������ʽ������ȷ�÷����£�\n\t\t%s IP��ַ �˿�\n\t����:\t%s 127.0.0.1 80\n�˳���
������ĳ�� IP ��ַ�ķ�����ĳ���˿ڽ������ MAXBUF ���ֽڵ���Ϣ",
argv[0], argv[0]);
exit(0);
}
/* ����һ�� socket ���� tcp ͨ�� */
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
perror("Socket");
exit(errno);
}

printf("socket created\n");
/* ��ʼ���������ˣ��Է����ĵ�ַ�Ͷ˿���Ϣ */
bzero(&dest, sizeof(dest));
dest.sin_family = AF_INET;
dest.sin_port = htons(atoi(argv[2]));
if (inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr) == 0) {
perror(argv[1]);
exit(errno);
}
printf("address created\n");
/* ���ӷ����� */
if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
perror("Connect ");
exit(errno);
}
printf("server connected\n");
while (1) {
/* ���նԷ�����������Ϣ�������� MAXBUF ���ֽ� */
bzero(buffer, MAXBUF + 1);
/* ���շ�����������Ϣ */
len = recv(sockfd, buffer, MAXBUF, 0);
if (len > 0)
printf("������Ϣ�ɹ�:'%s'����%d���ֽڵ�����\n",
buffer, len);
else {
if (len < 0)
printf
("��Ϣ����ʧ�ܣ����������%d��������Ϣ��'%s'\n",
errno, strerror(errno));
else
printf("�Է��˳��ˣ�������ֹ��\n");
break;
}
bzero(buffer, MAXBUF + 1);
printf("������Ҫ���͸��Է�����Ϣ��");
fgets(buffer, MAXBUF, stdin);
if (!strncasecmp(buffer, "quit", 4)) {
printf("�Լ�������ֹ���죡\n");
break;
}
/* ����Ϣ�������� */

len = send(sockfd, buffer, strlen(buffer) - 1, 0);
if (len < 0) {
printf
("��Ϣ'%s'����ʧ�ܣ����������%d��������Ϣ��'%s'\n",
buffer, errno, strerror(errno));
break;
} else
printf
("��Ϣ��%s\t���ͳɹ�����������%d���ֽڣ�\n",
buffer, len);
}
/* �ر����� */
close(sockfd);
return 0;
}