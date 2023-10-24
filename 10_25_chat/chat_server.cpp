#pragma comment(lib, "ws2_32.lib") //������� ���̺귯���� ��ũ. ���� ���̺귯�� ����
#define _CRT_SECURE_NO_WARNINGS //Ȥ�� localtime_s�� ���

#include <WinSock2.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <vector>
#include <mysql/jdbc.h>
#include <algorithm>
#include <ctime>

#define MAX_SIZE 1024 //������ ������ �� �ӽ÷� �����ϴ� ��
#define MAX_CLIENT 3 //client ��� ����

using std::cout;
using std::cin;
using std::endl;
using std::string;

time_t timer;
struct tm* t;

sql::mysql::MySQL_Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

const string db_server = "tcp://127.0.0.1:3306"; //�����ͺ��̽� �ּ�
const string db_username = "root"; //�����ͺ��̽� �����
const string db_password = "Aa103201579!"; //�����ͺ��̽� ���� ��й�ȣ

struct SOCKET_INFO { //����� ���� ������ ���� Ʋ ����
    SOCKET sck; //Ŭ���̾�Ʈ ������ ���� ���� ����
    string user;
};

std::vector<SOCKET_INFO> sck_list; //����� Ŭ���̾�Ʈ ���ϵ��� ������ �迭 ����
SOCKET_INFO server_sock; //���� ���Ͽ� ���� ������ ������ ���� ����
std::vector <string> login_list = {};

int client_count = 0; //���� ������ �ִ� Ŭ���̾�Ʈ�� count �� ���� ����
std::vector <string> bad_word = { "�ٺ�", "��û��" };

void server_init(); //socket �ʱ�ȭ �Լ�. socket(), bind(), listen() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��
void add_client(); //���Ͽ� ������ �õ��ϴ� client�� �߰�(accept)�ϴ� �Լ�. client accept() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��
void send_msg(const char*); //send() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��
void recv_msg(int); //recv() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��
void del_client(int); //���Ͽ� ����Ǿ� �ִ� client�� �����ϴ� �Լ�. closesocket() �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��
void direct_msg(int, string);
string check_timestamp();
void ranking(SOCKET);

namespace my_to_str {
    template < typename T > std::string to_string(const T& n)
    {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

int main() {
    WSADATA wsa;

    int code = WSAStartup(MAKEWORD(2, 2), &wsa);
    //Winsock�� �ʱ�ȭ�ϴ� �Լ�. MAKEWORD(2, 2)�� Winsock�� 2.2 ������ ����ϰڴٴ� �ǹ�
    //���࿡ �����ϸ� 0��, �����ϸ� �� �̿��� ���� ��ȯ
    //0�� ��ȯ�ߴٴ� ���� Winsock�� ����� �غ� �Ǿ��ٴ� �ǹ�

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(db_server, db_username, db_password);
    }
    catch (sql::SQLException& e) {
        std::cout << "Could not connect to server. Error message: " << e.what() << endl;
        exit(1);
    }

    //�����ͺ��̽� ����
    con->setSchema("chat");

    //db �ѱ� ������ ���� ���� 
    stmt = con->createStatement();
    stmt->execute("set names euckr");
    if (stmt) { delete stmt; stmt = nullptr; }

    if (!code) {
        server_init();

        std::thread th1[MAX_CLIENT];
        for (int i = 0; i < MAX_CLIENT; i++) {
            //�ο� �� ��ŭ thread �����ؼ� ������ Ŭ���̾�Ʈ�� ���ÿ� ������ �� �ֵ��� ��
            th1[i] = std::thread(add_client);
        }

        while (1) { //���� �ݺ����� ����Ͽ� ������ ����ؼ� ä�� ���� �� �ִ� ���¸� ����� ��. �ݺ����� ������� ������ �� ���� ���� �� ����
            string text, msg = "";

            std::getline(cin, text);
            const char* buf = text.c_str();
            msg = server_sock.user + " : " + buf;
            send_msg(msg.c_str()); //�ٸ� client���� �޼��� ����
        }

        for (int i = 0; i < MAX_CLIENT; i++) {
            th1[i].join();
            //join : �ش��ϴ� thread ���� ������ �����ϸ� �����ϴ� �Լ�
            //join �Լ��� ������ main �Լ��� ���� ����Ǿ thread�� �Ҹ��ϰ� ��
            //thread �۾��� ���� ������ main �Լ��� ������ �ʵ��� ����
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
    //Internet�� Stream ������� ���� ����
    //SOCKET_INFO�� ���� ��ü�� socket �Լ� ��ȯ��(��ũ���� ����)
    //���ͳ� �ּ�ü��, ��������, TCP �������� �� ��.

    SOCKADDR_IN server_addr = {}; //���� �ּ� ���� ����
    //���ͳ� ���� �ּ�ü�� server_addr

    server_addr.sin_family = AF_INET; //������ Internet Ÿ�� 
    server_addr.sin_port = htons(7777); //���� ��Ʈ ����
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //�����̱� ������ local ����
    //Any�� ���� ȣ��Ʈ�� 127.0.0.1�� ��Ƶ� �ǰ� localhost�� ��Ƶ� �ǰ� ���� �� ���(INADDR_ANY)�ϰ� �� �� ����
    //ip �ּҸ� ������ �� server_addr.sin_addr.s_addr

    //������ �帧. socket -> bind

    bind(server_sock.sck, (sockaddr*)&server_addr, sizeof(server_addr)); //������ ���� ������ ���Ͽ� ���ε�
    listen(server_sock.sck, SOMAXCONN); //������ ��� ���·� ��ٸ�
    server_sock.user = "server";

    cout << "Server On" << endl;
}//���� �ʱ�ȭ

void add_client() {
    SOCKADDR_IN addr = {};
    int addrsize = sizeof(addr);
    char buf[MAX_SIZE] = { };

    ZeroMemory(&addr, addrsize); //addr�� �޸� ������ 0���� �ʱ�ȭ

    SOCKET_INFO new_client = {};

    new_client.sck = accept(server_sock.sck, (sockaddr*)&addr, &addrsize);
    server_sock.sck;

    ranking(new_client.sck);

    recv(new_client.sck, buf, MAX_SIZE, 0); //Winsock2�� recv �Լ�. client�� ���� �г����� ����
    new_client.user = string(buf);

    string msg = "[����] " + new_client.user + " ���� �����߽��ϴ�.";
    cout << msg << endl;
    sck_list.push_back(new_client); //client ������ ��� sck_list �迭�� ���ο� client �߰�

    std::thread th(recv_msg, client_count); //������ ���� �� �ٸ� ������ ����
    //�ٸ� �����κ��� ���� �޽����� ����ؼ� ���� �� �ִ� ���·� ����� �α�

    client_count++; //client �� ����.
    cout << "[����] ���� ������ �� : " << client_count << "��" << endl;
    send_msg(msg.c_str()); //c_str : string Ÿ���� const chqr* Ÿ������ �ٲ���

    string client_admission = "1"; //���� ���� �� ���� ä�� ������ �����ֱ� ����
    send(new_client.sck, client_admission.c_str(), client_admission.size(), 0);

    th.join();
}

void ranking(SOCKET sck) {
    pstmt = con->prepareStatement("select nickname, win, lose from user join win_lose on user.user_id = win_lose.user_id order by win desc limit 3;");
    result = pstmt->executeQuery();
    string msg = "\t@@@@@@@@@@@@@@@@@@@@!�߱����� ��ŷ!@@@@@@@@@@@@@@@@@@@@\n";
    send(sck, msg.c_str(), msg.size(), 0);
    int cnt = 0;
    while (result->next()) {
        cnt++;
        msg = "\t\t\t*" + std::to_string(cnt) + "��*:" + result->getString(1) + " ��:" + result->getString(2) + " ��:" + result->getString(3) + "\n";
        send(sck, msg.c_str(), msg.size(), 0);
    }
    msg = "\n\t@@@@@@@@@@@@@@@@@@@@!�߱����� ��ŷ!@@@@@@@@@@@@@@@@@@@@\n";
    send(sck, msg.c_str(), msg.size(), 0);

    return;
}//�߱����� ��ŷ

void send_msg(const char* msg) {
    for (int i = 0; i < client_count; i++) { //������ �ִ� ��� client���� �޽��� ����
        send(sck_list[i].sck, msg, MAX_SIZE, 0);
    }
}

void color_msg(SOCKET ss, const char* msg) { //'/color' <��> <�г���> ��ɾ Ŭ���̾�Ʈ�κ��� �ް�, vector�� �ش� �г����� �����ϸ� socket������ �ѱ�
    std::stringstream s(msg);
    string color, nick, method;
    s >> method >> color >> nick;
    string msg1 = "�ش� ������ �������� �ʽ��ϴ�.\n";

    for (SOCKET_INFO s : sck_list) {    //���� ����Ʈ���� ã��
        if (s.user == nick) {
            send(ss, msg, MAX_SIZE, 0);
            return;
        }
    }
    send(ss, msg1.c_str(), msg1.length(), 0);
}

void direct_msg(int idx, string msg) { //���̷�Ʈ �޼���
    std::stringstream stream(msg);
    //stream.str(msg);
    string sub, send_msg;

    while (stream >> sub) {
        if (sub != "/dm") {
            break;
        }
    }
    int loc = msg.find(sub) + sub.length();
    int db_flag = 0, cur_flag = 0;

    pstmt = con->prepareStatement("insert into dm values(?,?,?,?)");
    pstmt->setString(1, sck_list[idx].user);
    pstmt->setString(2, sub);
    pstmt->setString(3, msg.substr(loc));
    pstmt->setString(4, check_timestamp());
    pstmt->execute();
    delete pstmt;

    pstmt = con->prepareStatement("select * from user");
    pstmt->execute();
    result = pstmt->executeQuery();

    while (result->next()) {
        if (result->getString(3).compare(sub) == 0) { //���� dm ���� ����� ���̵� DB�� ����ȰͰ� ���ٸ�
            db_flag = 1;
            send_msg = "�ӼӸ� [" + sck_list[idx].user + "] " + ":" + msg.substr(loc);
            for (SOCKET_INFO s : sck_list) {    //���� ����Ʈ���� Ž��. ���� ���� Ȯ��
                if (s.user == sub) {
                    send(s.sck, send_msg.c_str(), MAX_SIZE, 0);
                    cur_flag = 1; break;
                }
            }
            if (cur_flag == 0) {
                send_msg = "���� " + sub + "�� �������� �ʽ��ϴ�. \n";
                for (SOCKET_INFO s : sck_list) {    //���� ����Ʈ���� Ž��
                    if (s.user == sck_list[idx].user) {
                        send(s.sck, send_msg.c_str(), MAX_SIZE, 0);
                        break;
                    }
                }
            }break;
        }
    }
    if (db_flag == 0) {
        send_msg = "ä�� �����ڰ� �ƴմϴ�.\n";
        for (SOCKET_INFO s : sck_list) {    //���� ����Ʈ���� Ž��
            if (s.user == sck_list[idx].user) {
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
        if (recv(sck_list[idx].sck, buf, MAX_SIZE, 0) > 0) { // ������ �߻����� ������ recv�� ���ŵ� ����Ʈ ���� ��ȯ. 0���� ũ�ٴ� ���� �޽����� �Դٴ� ��
            if (string(buf).substr(0, 4) == "/dm ") {
                direct_msg(idx, buf);
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
            if (string(buf).substr(0, 7) == "/dm_msg") { //dm�� �Ϲ�ä�ð� �ٸ� ���̺� ����
                string send_nick, txt, time, all_dm;
                pstmt = con->prepareStatement("select send_nickname, text, send_time from dm where send_nickname = ? or recv_nickname = ?");
                pstmt->setString(1, sck_list[idx].user);
                pstmt->setString(2, sck_list[idx].user);
                pstmt->execute();
                result = pstmt->executeQuery();

                while (result->next()) {
                    send_nick = result->getString(1);
                    txt = result->getString(2);
                    time = result->getString(3);
                    all_dm = all_dm + time + " [" + send_nick + "]: " + txt + "\n";
                }

                send(sck_list[idx].sck, all_dm.c_str(), all_dm.length(), 0);
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
            del_client(idx); //Ŭ���̾�Ʈ ����
            return;
        }
    }
}

string check_timestamp() {
    timer = time(NULL); //1970�� 1�� 1�� 0�� 0�� 0�ʺ��� �����Ͽ� ��������� ��
    t = localtime(&timer); //�������� ���� ����ü�� ����
    string to_timestamp;

    to_timestamp = my_to_str::to_string(t->tm_year + 1900) + "-" + my_to_str::to_string(t->tm_mon + 1) + "-"
        + my_to_str::to_string(t->tm_mday) + "-" + my_to_str::to_string(t->tm_hour)
        + "-" + my_to_str::to_string(t->tm_min) + "-" + my_to_str::to_string(t->tm_sec);

    return to_timestamp;
}

void del_client(int idx) {
    closesocket(sck_list[idx].sck);
    sck_list.erase(sck_list.begin() + idx);
    client_count--;
}