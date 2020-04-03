#include "PacketSync.h"
#include "openssl/md5.h"

void CPacketSync::recvdata(const unsigned char* data, size_t len)
{ //���յ����ݣ������ݱ�����circulebuffer_
	size_t iret = 0;
	while (iret < len || truepacketlen >= NET_PACKAGE_HEADLEN + 2) {
		if (PARSE_NOTHING == parsetype) {//δ������head
			if (thread_readdata.len - truepacketlen >= len - iret) {
				memcpy(thread_readdata.base + truepacketlen, data + iret, len - iret);
				truepacketlen += len - iret;
				iret  = len;
			} else {
				memcpy(thread_readdata.base + truepacketlen, data + iret, thread_readdata.len - truepacketlen);
				iret += thread_readdata.len - truepacketlen;
				truepacketlen = thread_readdata.len;
			}
			headpt = (char*)memchr(thread_readdata.base, HEAD, truepacketlen);
			if (!headpt) {//1
				fprintf(stdout, "��ȡ%d���ݣ��Ҳ�����ͷ\n", truepacketlen);
				truepacketlen = 0;//���thread_readdata�������Ϊ��Ч
				continue;
			}
			headpos = headpt - thread_readdata.base;
			if (truepacketlen - headpos - 1 < NET_PACKAGE_HEADLEN) { //2.2
				if (headpos != 0) {
					fprintf(stdout, "��ȡ%d���ݣ��ҵ���ͷ,λ��%d,���ݲ�������֡ͷ���Ȼ���\n", truepacketlen, headpos);
					memmove(thread_readdata.base, thread_readdata.base + headpos, truepacketlen - headpos);
					truepacketlen -= headpos;
				}
				continue;
			}
			//��֡ͷ
			headpt = &thread_readdata.base[headpos + 1];
			//fprintf(stdout,"recv netpacketchar:\n");;
			//for (int i=0; i<NET_PACKAGE_HEADLEN; ++i) {
			//    fprintf(stdout,"%02X",(unsigned char)(headpt)[i]);
			//}
			//fprintf(stdout,"\n");
			CharToNetPacket((const unsigned char*)(headpt), theNexPacket);
			if (theNexPacket.header != HEAD || theNexPacket.tail != TAIL || theNexPacket.datalen < 0) {//֡ͷ���ݲ��Ϸ�(֡������Ϊ0)
				fprintf(stdout, "��ȡ%d����,��ͷλ��%d. ֡���ݲ��Ϸ�(head:%02x,tail:%02x,datalen:%d)\n",
					truepacketlen, headpos, theNexPacket.header, theNexPacket.tail, theNexPacket.datalen);
				memmove(thread_readdata.base, thread_readdata.base + headpos + 1, truepacketlen - headpos - 1); //2.4
				truepacketlen -= headpos + 1;
				continue;
			}
			parsetype = PARSE_HEAD;
			//��֡����
			if (thread_packetdata.len < (size_t)theNexPacket.datalen + 1) { //��������tail
				thread_packetdata.base = (char*)realloc(thread_packetdata.base, theNexPacket.datalen + 1);
				thread_packetdata.len = theNexPacket.datalen + 1;
			}
			getdatalen = (std::min)((int)(truepacketlen - headpos - 1 - NET_PACKAGE_HEADLEN), (int)(theNexPacket.datalen + 1));
			//�ȴ�thread_readdata��ȡ
			if (getdatalen > 0) {
				memcpy(thread_packetdata.base, thread_readdata.base + headpos + 1 + NET_PACKAGE_HEADLEN, getdatalen);
			}
		}

		//������head,�ڽ���data
		if (getdatalen < theNexPacket.datalen + 1) {
			if (getdatalen + (len - iret) < theNexPacket.datalen + 1) {
				memcpy(thread_packetdata.base + getdatalen, data + iret, len - iret);
				getdatalen += len - iret;
				iret = len;
				return;//�ȴ���һ�ֵĶ�ȡ
			} else {
				memcpy(thread_packetdata.base + getdatalen, data + iret, theNexPacket.datalen + 1 - getdatalen);
				iret += theNexPacket.datalen + 1 - getdatalen;
				getdatalen = theNexPacket.datalen + 1;
			}
		}
		//���У���������һλ
		if (thread_packetdata.base[theNexPacket.datalen] != TAIL) {
			fprintf(stdout, "�����ݳ�%d, ��β���ݲ��Ϸ�(tail:%02x)\n", theNexPacket.datalen,
				(unsigned char)(thread_packetdata.base[theNexPacket.datalen]));
			if (truepacketlen - headpos - 1 - NET_PACKAGE_HEADLEN >= theNexPacket.datalen + 1) {//thread_readdata�����㹻
				memmove(thread_readdata.base, thread_readdata.base + headpos + 1, truepacketlen - headpos - 1); //2.4
				truepacketlen -= headpos + 1;
			} else {//thread_readdata���ݲ���
				if (thread_readdata.len < NET_PACKAGE_HEADLEN + theNexPacket.datalen + 1) {//��������tail
					thread_readdata.base = (char*)realloc(thread_readdata.base, NET_PACKAGE_HEADLEN + theNexPacket.datalen + 1);
					thread_readdata.len = NET_PACKAGE_HEADLEN + theNexPacket.datalen + 1;
				}
				memmove(thread_readdata.base, thread_readdata.base + headpos + 1, NET_PACKAGE_HEADLEN); //2.4
				truepacketlen = NET_PACKAGE_HEADLEN;
				memcpy(thread_readdata.base + truepacketlen, thread_packetdata.base, theNexPacket.datalen + 1);
				truepacketlen += theNexPacket.datalen + 1;
			}
			parsetype = PARSE_NOTHING;//��ͷ����
			continue;
		}
		if (0 == theNexPacket.datalen) { //����Ϊ0��md5Ϊ��d41d8cd98f00b204e9800998ecf8427e����Ϊȫ0
			memset(md5str, 0, sizeof(md5str));
		} else {
			MD5_CTX md5;
			MD5_Init(&md5);
			MD5_Update(&md5, thread_packetdata.base, theNexPacket.datalen); //�����ݵ�У��ֵ
			MD5_Final(md5str, &md5);
		}
		if (memcmp(theNexPacket.check, md5str, MD5_DIGEST_LENGTH) != 0) {
			fprintf(stdout, "��ȡ%d����, У���벻�Ϸ�\n", NET_PACKAGE_HEADLEN + theNexPacket.datalen + 2);
			if (truepacketlen - headpos - 1 - NET_PACKAGE_HEADLEN >= theNexPacket.datalen + 1) {//thread_readdata�����㹻
				memmove(thread_readdata.base, thread_readdata.base + headpos + 1, truepacketlen - headpos - 1); //2.4
				truepacketlen -= headpos + 1;
			} else {//thread_readdata���ݲ���
				if (thread_readdata.len < NET_PACKAGE_HEADLEN + theNexPacket.datalen + 1) {//��������tail
					thread_readdata.base = (char*)realloc(thread_readdata.base, NET_PACKAGE_HEADLEN + theNexPacket.datalen + 1);
					thread_readdata.len = NET_PACKAGE_HEADLEN + theNexPacket.datalen + 1;
				}
				memmove(thread_readdata.base, thread_readdata.base + headpos + 1, NET_PACKAGE_HEADLEN); //2.4
				truepacketlen = NET_PACKAGE_HEADLEN;
				memcpy(thread_readdata.base + truepacketlen, thread_packetdata.base, theNexPacket.datalen + 1);
				truepacketlen += theNexPacket.datalen + 1;
			}
			parsetype = PARSE_NOTHING;//��ͷ����
			continue;
		}
		if (truepacketlen - headpos - 1 - NET_PACKAGE_HEADLEN >= theNexPacket.datalen + 1) {//thread_readdata�����㹻
			memmove(thread_readdata.base, thread_readdata.base + headpos + NET_PACKAGE_HEADLEN + theNexPacket.datalen + 2,
				truepacketlen - (headpos + NET_PACKAGE_HEADLEN + theNexPacket.datalen + 2)); //2.4
			truepacketlen -= headpos + NET_PACKAGE_HEADLEN + theNexPacket.datalen + 2;
		} else {
			truepacketlen = 0;//���¿�ʼ��ȡ����
		}
		//�ص�֡���ݸ��û�
		if (this->packet_cb_) {
			thread_packetdata.base[theNexPacket.datalen] = '\0';
			this->packet_cb_(theNexPacket, (const unsigned char*)thread_packetdata.base, this->packetcb_userdata_);
		}
		parsetype = PARSE_NOTHING;//��ͷ����
	}
}

std::string PacketData2( NetPacket& packet, const unsigned char* data )
{
	if (packet.datalen == 0 || data == NULL) {//����Ϊ0��md5Ϊ��d41d8cd98f00b204e9800998ecf8427e����Ϊȫ0
		memset(packet.check, 0, sizeof(packet.check));
	} else {
		MD5_CTX md5;
		MD5_Init(&md5);
		MD5_Update(&md5, data, packet.datalen);
		MD5_Final(packet.check, &md5);
	}
	unsigned char packchar[NET_PACKAGE_HEADLEN];
	NetPacketToChar(packet, packchar);

	std::string retstr;
	retstr.append(1, packet.header);
	retstr.append((const char*)packchar, NET_PACKAGE_HEADLEN);
	retstr.append((const char*)data, packet.datalen);
	retstr.append(1, packet.tail);
	return std::move(retstr);
}
