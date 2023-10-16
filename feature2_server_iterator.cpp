#pragma comment(lib, "ws2_32.lib") //������� ���̺귯���� ��ũ. ���� ���̺귯�� ����
//��ũ�ϴ°ŷ� winsocket�� �̹� �������.

#include <WinSock2.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <vector>
#include <mysql/jdbc.h>
#include <algorithm>

#define MAX_SIZE 1024 //������ ������ �� �ӽ÷� �����ϴ� ��
#define MAX_CLIENT 3 //client ��� ����

using std::cout;
using std::cin;
using std::endl;
using std::string;

sql::mysql::MySQL_Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

const string db_server = "tcp://127.0.0.1:3306"; // �����ͺ��̽� �ּ�
const string db_username = "root"; // �����ͺ��̽� �����
const string db_password = "Aa103201579!"; // �����ͺ��̽� ���� ��й�ȣ

struct SOCKET_INFO { // ����� ���� ������ ���� Ʋ ����
    SOCKET sck; //Ŭ���̾�Ʈ ������ ���� ���� ����.
    string user;
};

//std::vector<SOCKET_INFO> sck_list; // ����� Ŭ���̾�Ʈ ���ϵ��� ������ �迭 ����.
std::list<SOCKET_INFO> sck_list;
SOCKET_INFO server_sock; // ���� ���Ͽ� ���� ������ ������ ���� ����.


int client_count = 0; // ���� ������ �ִ� Ŭ���̾�Ʈ�� count �� ���� ����.

void server_init(); // socket �ʱ�ȭ �Լ�. socket(), bind(), listen() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void add_client(); // ���Ͽ� ������ �õ��ϴ� client�� �߰�(accept)�ϴ� �Լ�. client accept() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void send_msg(const char* msg); // send() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void recv_msg(int idx); // recv() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void del_client(int idx); // ���Ͽ� ����Ǿ� �ִ� client�� �����ϴ� �Լ�. closesocket() �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void direct_msg(string nickname, string msg);

// i = 0;

int main() {
    WSADATA wsa;

    // Winsock�� �ʱ�ȭ�ϴ� �Լ�. MAKEWORD(2, 2)�� Winsock�� 2.2 ������ ����ϰڴٴ� �ǹ�.
    // ���࿡ �����ϸ� 0��, �����ϸ� �� �̿��� ���� ��ȯ.
    // 0�� ��ȯ�ߴٴ� ���� Winsock�� ����� �غ� �Ǿ��ٴ� �ǹ�.
    int code = WSAStartup(MAKEWORD(2, 2), &wsa);

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(db_server, db_username, db_password);
    }
    catch (sql::SQLException& e) {
        std::cout << "Could not connect to server. Error message: " << e.what() << endl;
        exit(1);
    }

    // �����ͺ��̽� ����
    con->setSchema("chat");

    // db �ѱ� ������ ���� ���� 
    stmt = con->createStatement();
    stmt->execute("set names euckr");
    if (stmt) { delete stmt; stmt = nullptr; }

    if (!code) {
        server_init();

        std::thread th1[MAX_CLIENT];
        for (int i = 0; i < MAX_CLIENT; i++) {
            // �ο� �� ��ŭ thread �����ؼ� ������ Ŭ���̾�Ʈ�� ���ÿ� ������ �� �ֵ��� ��.
            th1[i] = std::thread(add_client); //new �Ⱦ��� overload ��ü ����?
        }
        
        //std::thread th1(add_client); // �̷��� �ϸ� �ϳ��� client�� �޾���...

        while (1) { // ���� �ݺ����� ����Ͽ� ������ ����ؼ� ä�� ���� �� �ִ� ���¸� ����� ��. �ݺ����� ������� ������ �� ���� ���� �� ����.
            string text, msg = "";

            std::getline(cin, text);
            const char* buf = text.c_str();
            msg = server_sock.user + " : " + buf;
            send_msg(msg.c_str()); //�ٸ� client���� �޼��� ����
        }

        for (int i = 0; i < MAX_CLIENT; i++) {
            th1[i].join();
            //join : �ش��ϴ� thread ���� ������ �����ϸ� �����ϴ� �Լ�.
            //join �Լ��� ������ main �Լ��� ���� ����Ǿ thread�� �Ҹ��ϰ� ��.
            //thread �۾��� ���� ������ main �Լ��� ������ �ʵ��� ����.
        }
        //th1.join();

        closesocket(server_sock.sck);
    }
    else {
        cout << "���α׷� ����. (Error code : " << code << ")";
    }

    WSACleanup();

    return 0;
}

void server_init() { //���� �ʱ�ȭ
    server_sock.sck = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //tcp ��� �� sock_stream���?
    // Internet�� Stream ������� ���� ����
   // SOCKET_INFO�� ���� ��ü�� socket �Լ� ��ȯ��(��ũ���� ����)
   // ���ͳ� �ּ�ü��, ��������, TCP �������� �� ��. 

    SOCKADDR_IN server_addr = {}; // ���� �ּ� ���� ����
    // ���ͳ� ���� �ּ�ü�� server_addr

    server_addr.sin_family = AF_INET; // ������ Internet Ÿ�� 
    server_addr.sin_port = htons(7777); // ���� ��Ʈ ���� -> ��Ʈ�� ���� �ϱ� ����. �ٲ㵵 ��
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // �����̱� ������ local �����Ѵ�. 
    //Any�� ���� ȣ��Ʈ�� 127.0.0.1�� ��Ƶ� �ǰ� localhost�� ��Ƶ� �ǰ� ���� �� ����ϰ� �� �� �ִ�. �װ��� INADDR_ANY�̴�.
    //ip �ּҸ� ������ �� server_addr.sin_addr.s_addr -- ������ ���?

    //������ �帧. socket -> bind

    bind(server_sock.sck, (sockaddr*)&server_addr, sizeof(server_addr)); // ������ ���� ������ ���Ͽ� ���ε��Ѵ�.
    listen(server_sock.sck, SOMAXCONN); // ������ ��� ���·� ��ٸ���.*
    server_sock.user = "server";

    cout << "Server On" << endl;
}//������� ���� �ʱ�ȭ

void add_client() {
    SOCKADDR_IN addr = {};
    int addrsize = sizeof(addr);
    char buf[MAX_SIZE] = { };

    ZeroMemory(&addr, addrsize); // addr�� �޸� ������ 0���� �ʱ�ȭ

    SOCKET_INFO new_client = {};

    new_client.sck = accept(server_sock.sck, (sockaddr*)&addr, &addrsize);
    recv(new_client.sck, buf, MAX_SIZE, 0); //client�� ���1�� ǥ����. ����Ǹ� ���Ƿ� �ٷ� �г����� ������ ����.
    // Winsock2�� recv �Լ�. client�� ���� �г����� ����.
    new_client.user = string(buf);
    
    cout << new_client.sck << endl;
    string msg = "[����] " + new_client.user + " ���� �����߽��ϴ�.";
    cout << msg << endl;
    sck_list.push_back(new_client); // client ������ ��� sck_list �迭�� ���ο� client �߰�
 /*   cout << sck_list[0].sck << endl;
    cout << sck_list[1].sck << endl;
    cout << client_count << endl;
    */

    std::thread th(recv_msg, client_count); //������ ���� �� �ٸ� ������ ����.

    // �ٸ� �����κ��� ���� �޽����� ����ؼ� ���� �� �ִ� ���·� ����� �α�.
    //client�� 2���̸� ����thread�� 4���� ����? client���� ���� 1����? 

    //std::thread tth(dm_recv_msg, client_count); dm ���� �� thread.

    client_count++; // client �� ����.�����ڸ�
    cout << "[����] ���� ������ �� : " << client_count << "��" << endl;
    send_msg(msg.c_str()); // c_str : string Ÿ���� const chqr* Ÿ������ �ٲ���.
    
    string first = "1";
    send(new_client.sck, first.c_str(), first.size(), 0);
    
    th.join();
    //thh.join();//
}

void send_msg(const char* msg) {
    for (std::list<SOCKET_INFO>::iterator it = sck_list.begin(); it!=sck_list.end(); it++) { // ������ �ִ� ��� client���� �޽��� ����
        send(it->sck, msg, MAX_SIZE, 0);
    }
}

void direct_msg(string nickname, string msg) { //���̷�Ʈ �޼���
    std::stringstream stream;
    stream.str(msg);
    string sub, send_msg;
    while (stream >> sub) {
        if (sub != "/dm") {
            break;
        }
    }
    pstmt = con->prepareStatement("select * from user");
    pstmt->execute();
    result = pstmt->executeQuery();

    int loc = msg.find(sub) + sub.length();
    int db_flag = 0, cur_flag = 0;
  
    while (result->next()) {
        if (result->getString(3).compare(sub) == 0) { //���� dm ���� ����� ���̵� DB�� ����ȰͰ� ���ٸ�
            db_flag = 1;
            send_msg = "�ӼӸ� [" + nickname + "] " + ":" + msg.substr(loc);
            for (std::list<SOCKET_INFO>::iterator it = sck_list.begin(); it != sck_list.end(); it++) {    //���� ����Ʈ���� ã�´�. ���� ���� �ִ���.
                if (it->user == sub) {
                    send(it->sck, send_msg.c_str(), MAX_SIZE, 0);
                    cur_flag = 1; break;
                }
            }


            if (cur_flag == 0) {
                send_msg = "���� " + sub + "�� �������� �ʽ��ϴ�. \n";
                for (std::list<SOCKET_INFO>::iterator it = sck_list.begin(); it != sck_list.end(); it++) {    //���� ����Ʈ���� ã�´�.
                    if (it->user == nickname) {
                        send(it->sck, send_msg.c_str(), MAX_SIZE, 0);
                        break;
                    }
                }
            }
            break;
        }
    }

    if (db_flag == 0) {
        send_msg = "ä�� �����ڰ� �ƴմϴ�.\n";
        for (std::list<SOCKET_INFO>::iterator it = sck_list.begin(); it != sck_list.end(); it++) {    //���� ����Ʈ���� ã�´�.
            if (it->user == nickname) {
                send(it->sck, send_msg.c_str(), MAX_SIZE, 0);
                break;
            }
        }
    }

}

//
//void all_chat() {
//    string all_msg = "";
//    pstmt = con->prepareStatement("select * from chatting");
//    pstmt->execute();
//    result = pstmt->executeQuery();
//
//    /*result->next();
//    all_msg = result->getString(2) + " " + result->getString(3) + "\n";
//    send(sck_list[client_count - 1].sck, all_msg.c_str(), MAX_SIZE, 0);*/
//
//    while (result->next()) {
//        all_msg = result->getString(2) + " " + result->getString(3) + "\n";
//        send(sck_list[client_count - 1].sck, all_msg.c_str(), MAX_SIZE, 0);//���� �����ϴ� client���Ը� ���� ä�ó���.
//    }
//}


void recv_msg(int idx) {
    std::list<SOCKET_INFO>::iterator it=sck_list.begin();
    int cnt = 0;
    for (; it != sck_list.end(); ++it) {
        if (cnt == idx) break;
        cnt++;
    }
    char buf[MAX_SIZE] = { };
    string msg = "";
   // cout << sck_list[idx].sck << endl;

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(it->sck, buf, MAX_SIZE, 0) > 0) { // ������ �߻����� ������ recv�� ���ŵ� ����Ʈ ���� ��ȯ. 0���� ũ�ٴ� ���� �޽����� �Դٴ� ��.
            if (string(buf).substr(0, 4) == "/dm ") {
                direct_msg(it->user, buf);
                continue;
            }
            if (string(buf).substr(0, 5) == "/back") {
                del_client(idx);
                return;
            }
            msg = it->user + " : " + buf;
            cout << msg << endl;
            send_msg(msg.c_str());
           // store_chat(msg, check_timestamp(), sck_list[idx].user); //10_11 
        }
        else { //�׷��� ���� ��� ���忡 ���� ��ȣ�� �����Ͽ� ���� �޽��� ����
            msg = "[����] " + it->user + " ���� �����߽��ϴ�.";
            cout << msg << endl;
            send_msg(msg.c_str());
            del_client(idx); // Ŭ���̾�Ʈ ����
            return;
        }
    }
}


void del_client(int idx) {
    std::list<SOCKET_INFO>::iterator it = sck_list.begin();
    int cnt = 0;
    for (; it != sck_list.end(); ++it) {
        if (cnt == idx) break;
        cnt++;
    }
    closesocket(it->sck);
    sck_list.erase(it); // �迭���� Ŭ���̾�Ʈ�� �����ϰ� �� ��� index�� �޶����鼭 ��Ÿ�� ���� �߻�....��
    client_count--;
   /* pstmt = con->prepareStatement("DELETE from dm where client_sock = ?");
    pstmt->setInt(1, sck_list[idx].sck);
    pstmt->execute();
    delete pstmt;*/
}