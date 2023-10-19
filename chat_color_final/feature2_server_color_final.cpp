#pragma comment(lib, "ws2_32.lib") //������� ���̺귯���� ��ũ. ���� ���̺귯�� ����

#include <WinSock2.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <vector>
#include <mysql/jdbc.h>
#include <algorithm>
#include <cstdio> // connect error Ȯ��

#define MAX_SIZE 1024 //������ ������ �� �ӽ÷� �����ϴ� ��
#define MAX_CLIENT 100 //client ��� ����

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

std::vector<SOCKET_INFO> sck_list; // ����� Ŭ���̾�Ʈ ���ϵ��� ������ �迭 ����.
//std::list<SOCKET_INFO> sck_list;
SOCKET_INFO server_sock; // ���� ���Ͽ� ���� ������ ������ ���� ����.
std::vector <string> login_list = {};

int client_count = 0; // ���� ������ �ִ� Ŭ���̾�Ʈ�� count �� ���� ����.
std::vector <string> bad_word = { "�ٺ�", "��û��" };

void server_init(); // socket �ʱ�ȭ �Լ�. socket(), bind(), listen() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void add_client(); // ���Ͽ� ������ �õ��ϴ� client�� �߰�(accept)�ϴ� �Լ�. client accept() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void send_msg(const char* msg); // send() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void recv_msg(int idx); // recv() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void del_client(int idx); // ���Ͽ� ����Ǿ� �ִ� client�� �����ϴ� �Լ�. closesocket() �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void direct_msg(string nickname, string msg);

int main() {
    WSADATA wsa;

    int code = WSAStartup(MAKEWORD(2, 2), &wsa);
    // Winsock�� �ʱ�ȭ�ϴ� �Լ�. MAKEWORD(2, 2)�� Winsock�� 2.2 ������ ����ϰڴٴ� �ǹ�.
    // ���࿡ �����ϸ� 0��, �����ϸ� �� �̿��� ���� ��ȯ.
    // 0�� ��ȯ�ߴٴ� ���� Winsock�� ����� �غ� �Ǿ��ٴ� �ǹ�.

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

        closesocket(server_sock.sck);
    }
    else {
        cout << "���α׷� ����. (Error code : " << code << ")";
    }

    WSACleanup();

    return 0;
}

void server_init() { //���� �ʱ�ȭ
    server_sock.sck = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
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
}// ���� �ʱ�ȭ

void add_client() {
    SOCKADDR_IN addr = {};
    int addrsize = sizeof(addr);
    char buf[MAX_SIZE] = { };

    ZeroMemory(&addr, addrsize); // addr�� �޸� ������ 0���� �ʱ�ȭ

    SOCKET_INFO new_client = {};

    new_client.sck = accept(server_sock.sck, (sockaddr*)&addr, &addrsize);
    recv(new_client.sck, buf, MAX_SIZE, 0); // Winsock2�� recv �Լ�. client�� ���� �г����� ����.
    new_client.user = string(buf);

    string msg = "[����] " + new_client.user + " ���� �����߽��ϴ�.";
    cout << msg << endl;
    sck_list.push_back(new_client); // client ������ ��� sck_list �迭�� ���ο� client �߰�

    std::thread th(recv_msg, client_count); //������ ���� �� �ٸ� ������ ����.

    // �ٸ� �����κ��� ���� �޽����� ����ؼ� ���� �� �ִ� ���·� ����� �α�.
    //client�� 2���̸� ����thread�� 4���� ����? client���� ���� 1����? 

    client_count++; // client �� ����.
    cout << "[����] ���� ������ �� : " << client_count << "��" << endl;
    send_msg(msg.c_str()); // c_str : string Ÿ���� const chqr* Ÿ������ �ٲ���.

    string client_admission = "1"; // ���� ���� �� ���� ä�� ������ �����ֱ� ����.
    send(new_client.sck, client_admission.c_str(), client_admission.size(), 0);

    th.join();
}

void send_msg(const char* msg) {
    for (int i = 0; i < client_count; i++) { // ������ �ִ� ��� client���� �޽��� ����
        send(sck_list[i].sck, msg, MAX_SIZE, 0);
    }
}

void color_msg(SOCKET ss,const char* msg) { // "/color �� �г����� Ŭ���̾�Ʈ�κ��� �ް�, list�� �ش� �г����� �����ϸ� socket������ �ѱ��.
        std::stringstream s(msg);
       //SOCKET s;
        string color, nick, method;
        s >> method >> color >> nick;
        string msg1 = "�ش� ������ �������� �ʽ��ϴ�.\n";

        for (SOCKET_INFO s : sck_list) {    //���� ����Ʈ���� ã�´�.
            if (s.user == nick) {
                send(ss, msg, MAX_SIZE, 0);
                return;
            }
        }
        send(ss, msg1.c_str(), msg1.length(), 0);
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
            for (SOCKET_INFO s : sck_list) {    //���� ����Ʈ���� ã�´�. ���� ���� �ִ���.
                if (s.user == sub) {
                    send(s.sck, send_msg.c_str(), MAX_SIZE, 0);
                    cur_flag = 1; break;
                }
            }
            if (cur_flag == 0) {
                send_msg = "���� " + sub + "�� �������� �ʽ��ϴ�. \n";
                for (SOCKET_INFO s : sck_list) {    //���� ����Ʈ���� ã�´�.
                    if (s.user == nickname) {
                        send(s.sck, send_msg.c_str(), MAX_SIZE, 0);
                        break;
                    }
                }
            }break;
        }
    }
    if (db_flag == 0) {
        send_msg = "ä�� �����ڰ� �ƴմϴ�.\n";
        for (SOCKET_INFO s : sck_list) {    //���� ����Ʈ���� ã�´�.
            if (s.user == nickname) {
                send(s.sck, send_msg.c_str(), MAX_SIZE, 0);
                break;
            }
        }
    }
}

void recv_msg(int idx) {
    char buf[MAX_SIZE] = { };
    string msg = "";

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(sck_list[idx].sck, buf, MAX_SIZE, 0) > 0) { // ������ �߻����� ������ recv�� ���ŵ� ����Ʈ ���� ��ȯ. 0���� ũ�ٴ� ���� �޽����� �Դٴ� ��.
            if (string(buf).substr(0, 4) == "/dm ") {
                direct_msg(sck_list[idx].user, buf);
                continue;
            }
            if (string(buf).substr(0, 5) == "/back") {
                msg = "[����] " + sck_list[idx].user + " ���� �����߽��ϴ�.";
                cout << msg << endl;
                send_msg(msg.c_str());
                del_client(idx);
                return;
            }
            if (string(buf).substr(0, 6) == "/color") {
                color_msg(sck_list[idx].sck, buf);
                continue;
            }
            for (string bad : bad_word) {
                int i = sizeof(buf);
                if (string(buf).substr().find(bad) != string::npos) {
                    msg = "[����] " + sck_list[idx].user + " ���� �弳�� ���� ����Ǿ����ϴ�.";
                    cout << msg << endl;
                    send_msg(msg.c_str());
                    del_client(idx);
                    return;
                }
            }
            msg = sck_list[idx].user + " : " + buf;
            cout << msg << endl;
            send_msg(msg.c_str());
        }
        else { //�׷��� ���� ��� ���忡 ���� ��ȣ�� �����Ͽ� ���� �޽��� ����
            msg = "[����] " + sck_list[idx].user + " ���� �����߽��ϴ�.";
            cout << msg << endl;
            send_msg(msg.c_str());
            del_client(idx); // Ŭ���̾�Ʈ ����
            return;
        }
    }
}

void del_client(int idx) {
    //int cnt = 0;
    //std::list<SOCKET_INFO>::iterator it = sck_list.begin();
    //for (it; it != sck_list.end(); ++it) {
    //    if (cnt == idx) break;
    //    cnt++;
    //}
    //closesocket(it->sck);
    //sck_list.erase(it); // �迭���� Ŭ���̾�Ʈ�� �����ϰ� �� ��� index�� �޶����鼭 ��Ÿ�� ���� �߻�....��

    closesocket(sck_list[idx].sck);
    sck_list.erase(sck_list.begin() + idx);
    client_count--;
}
