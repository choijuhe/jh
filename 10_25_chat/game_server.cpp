#pragma comment(lib, "ws2_32.lib") //������� ���̺귯���� ��ũ. ���� ���̺귯�� ����

#include <WinSock2.h>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <mysql/jdbc.h>
#include <sstream>
#include<cstdlib>
#include<ctime>
#include<algorithm>

#define MAX_SIZE 1024
#define MAX_CLIENT 1

using std::cout;
using std::cin;
using std::endl;
using std::string;

sql::mysql::MySQL_Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

const string server = "tcp://127.0.0.1:3306"; // �����ͺ��̽� �ּ�
const string username = "root"; // �����ͺ��̽� �����
const string db_password = "Aa103201579!"; // �����ͺ��̽� ���� ��й�ȣ


struct SOCKET_INFO { // ����� ���� ������ ���� Ʋ ����
    SOCKET sck;
    string user;
};
std::vector<SOCKET_INFO> sck_list; // ����� Ŭ���̾�Ʈ ���ϵ��� ������ �迭 ����.
SOCKET_INFO server_sock; // ���� ���Ͽ� ���� ������ ������ ���� ����.
int client_count = 0; // ���� ������ �ִ� Ŭ���̾�Ʈ�� count �� ���� ����.
std::vector<int> c; //�������� ���� ����
int memo[10];
std::vector<int> p; //�÷��̾ ����� ����
int game_end = 0;

void server_init();
void add_client();
void del_client(int idx);
void send_msg(const char* msg); // send() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void recv_msg(int idx); // recv() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void game_manager(int turn);

namespace my_to_str
{
    template < typename T > std::string to_string(const T& n)
    {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

void baseball_init() {
    srand(time(NULL));
    c.clear(), p.clear();

    for (;;) {
        int r = rand() % 9 + 1;
        c.push_back(r);
        sort(c.begin(), c.end());
        c.erase(unique(c.begin(), c.end()), c.end());
        if (c.size() == 3) break;
    }
    cout << c[0] << ' ' << c[1] << ' ' << c[2] << '\n';
}

void server_init() {
    server_sock.sck = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Internet�� Stream ������� ���� ����
   // SOCKET_INFO�� ���� ��ü�� socket �Լ� ��ȯ��(��ũ���� ����)
   // ���ͳ� �ּ�ü��, ��������, TCP �������� �� ��. 

    SOCKADDR_IN server_addr = {}; // ���� �ּ� ���� ����
    // ���ͳ� ���� �ּ�ü�� server_addr

    server_addr.sin_family = AF_INET; // ������ Internet Ÿ�� 
    server_addr.sin_port = htons(6666); // ���� ��Ʈ ����
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // �����̱� ������ local �����Ѵ�. 
    //Any�� ���� ȣ��Ʈ�� 127.0.0.1�� ��Ƶ� �ǰ� localhost�� ��Ƶ� �ǰ� ���� �� ����ϰ� �� �� �ֵ�. �װ��� INADDR_ANY�̴�.
    //ip �ּҸ� ������ �� server_addr.sin_addr.s_addr -- ������ ���?

    bind(server_sock.sck, (sockaddr*)&server_addr, sizeof(server_addr)); // ������ ���� ������ ���Ͽ� ���ε��Ѵ�.
    listen(server_sock.sck, SOMAXCONN); // ������ ��� ���·� ��ٸ���.
    server_sock.user = "server";

    cout << "game Server On" << endl;
}

void add_client() {
    while (true) {
        SOCKADDR_IN addr = {};
        int addrsize = sizeof(addr);
        char buf[MAX_SIZE] = { };
        string msg;

        ZeroMemory(&addr, addrsize); // addr�� �޸� ������ 0���� �ʱ�ȭ

        SOCKET_INFO new_client = {};

        new_client.sck = accept(server_sock.sck, (sockaddr*)&addr, &addrsize);
        if (new_client.sck == -1) {
            closesocket(new_client.sck);
            continue;
        }
        recv(new_client.sck, buf, MAX_SIZE, 0);
        // Winsock2�� recv �Լ�. client�� ���� �г����� ����.
        new_client.user = string(buf);

        msg = "[����] " + new_client.user + " ���� �����߽��ϴ�.";
        cout << msg << endl;
        sck_list.push_back(new_client); // client ������ ��� sck_list �迭�� ���ο� client �߰�

        //std::thread th(recv_msg, client_count);
        // �ٸ� �����κ��� ���� �޽����� ����ؼ� ���� �� �ִ� ���·� ����� �α�.
        client_count++; // client �� ����.
        cout << "[����] ���� ������ �� : " << client_count << "��" << endl;
        send_msg(msg.c_str()); // c_str : string Ÿ���� const chqr* Ÿ������ �ٲ���.

        if (client_count == 2) {
            system("cls");
            msg = "[����] �߱������� �����ϰڽ��ϴ�!";
            cout << msg << endl;
            send_msg(msg.c_str());
            Sleep(1000);
            baseball_init();

            while (true) {
                msg = "������Դϴ�! ��ٷ��ּ���.";
                send(sck_list[1].sck, msg.c_str(), msg.size(), 0);
                Sleep(1000);
                game_manager(0);
                if (game_end == 1) {
                    msg = "******** �����մϴ� [" + sck_list[0].user + "] ���� �¸��ϼ̽��ϴ�! *********";
                    send_msg(msg.c_str());
                    pstmt = con->prepareStatement("update win_lose set win = win+1 where user_id = (select user_id from user where nickname = ?)");
                    pstmt->setString(1, sck_list[0].user);
                    pstmt->execute();
                    pstmt = con->prepareStatement("update win_lose set lose = lose+1 where user_id = (select user_id from user where nickname = ?)");
                    pstmt->setString(1, sck_list[1].user);
                    pstmt->execute();
                    Sleep(3000);
                    ZeroMemory(buf, MAX_SIZE);
                    if (recv(new_client.sck, buf, MAX_SIZE, 0) > 0) {
                        game_end = 0;
                        system("cls");
                        pstmt = con->prepareStatement("select nickname, win, lose from user join win_lose on user.user_id = win_lose.user_id order by win desc limit 3;");
                        result = pstmt->executeQuery();
                        cout << "\t\t\t@@@@@@@@@@@@@@@@@@@@!�߱����� ��ŷ!@@@@@@@@@@@@@@@@@@@@\n";
                        int cnt = 0;
                        while (result->next()) {
                            cout << "\t\t\t\t\t*" << ++cnt << "��*:" << result->getString(1) << " ��:" << result->getString(2) << " ��:" << result->getString(3) << '\n';
                        }
                        cout << "\t\t\t@@@@@@@@@@@@@@@@@@@@!�߱����� ��ŷ!@@@@@@@@@@@@@@@@@@@@\n";
                        del_client(1);
                        del_client(0);
                        break;
                    }
                }
                send(sck_list[0].sck, msg.c_str(), msg.size(), 0);
                Sleep(1000);
                game_manager(1);
                if (game_end == 1) {
                    msg = "******** �����մϴ� [" + sck_list[1].user + "] ���� �¸��ϼ̽��ϴ�! ********";
                    send_msg(msg.c_str());
                    // �߱����� ��ŷ
                    pstmt = con->prepareStatement("update win_lose set win = win+1 where user_id = (select user_id from user where nickname = ?)");
                    pstmt->setString(1, sck_list[1].user);
                    pstmt->execute();
                    pstmt = con->prepareStatement("update win_lose set lose = lose+1 where user_id = (select user_id from user where nickname = ?)");
                    pstmt->setString(1, sck_list[0].user);
                    pstmt->execute();
                    Sleep(3000);
                    ZeroMemory(buf, MAX_SIZE);
                    if (recv(new_client.sck, buf, MAX_SIZE, 0) > 0) {
                        game_end = 0;
                        system("cls");
                        // �߱����� ��ŷ
                        pstmt = con->prepareStatement("select nickname, win, lose from user join win_lose on user.user_id = win_lose.user_id order by win desc limit 3;");
                        result = pstmt->executeQuery();
                        cout << "\t\t\t@@@@@@@@@@@@@@@@@@@@!�߱����� ��ŷ!@@@@@@@@@@@@@@@@@@@@\n";
                        int cnt = 0;
                        while (result->next()) {
                            cout << "\t\t\t\t\t*" << ++cnt << "��*:" << result->getString(1) << " ��:" << result->getString(2) << " ��:" << result->getString(3) << '\n';
                        }
                        cout << "\t\t\t@@@@@@@@@@@@@@@@@@@@!�߱����� ��ŷ!@@@@@@@@@@@@@@@@@@@@\n";
                        del_client(1);
                        del_client(0);
                        break;
                    }
                }
            }
        }
    }
}

void send_msg(const char* msg) { // ������ �ִ� ��� client���� �޽��� ����
    for (int i = 0; i < client_count; i++) {
        send(sck_list[i].sck, msg, MAX_SIZE, 0);
    }
}

bool is_digit(string str) {
    return atoi(str.c_str()) != 0 || str.compare("0") == 0;
}

bool checkThreeNumber(string& msg, int* memo) {
    std::stringstream stream(msg);
    string sub;

    while (stream >> sub) {
        int x = atoi(sub.c_str());

        if (!is_digit(sub)) {
            msg = "���ڰ� �ƴѰ��� �����ֽ��ϴ�. �ٽ� �Է����ּ���.***";
            return false;
        }
        else if (x < 1 || x > 9) {
            msg = "������ �Ѿ ���ڰ� �ֽ��ϴ�. �ٽ� �Է����ּ���.***";
            return false;
        }
        else if (memo[x] == 1) {
            msg = "���ڰ� �ߺ��Ǿ��ֽ��ϴ�. �ٽ� �Է����ּ���.***";
            return false;
        }
        p.push_back(x);
        memo[x] = 1;
    }
    return true;
}

void game_manager(int turn) { // ������� Ŭ���̾�Ʈ���� ���� ������ �޾ƿ��� �Լ�
    char buf[MAX_SIZE] = { };
    string msg = "";

    while (1) {
        msg = "1~9 ������ ���� 3���� �Է� �ϼ���. \n<Number><Spacebar><Number><Spacebar><Number>�������� �Է�";
        send(sck_list[turn].sck, msg.c_str(), msg.size(), 0); //�������� ������� ������.
        ZeroMemory(buf, MAX_SIZE);
        if (recv(sck_list[turn].sck, buf, MAX_SIZE, 0) > 0) {

            int memo[10] = { 0 }; // Ŭ���̾�Ʈ�� �� ���� 3�� �޸�

            msg = buf;

            if (!checkThreeNumber(msg, memo)) {
                send(sck_list[turn].sck, msg.c_str(), msg.size(), 0);
                memo[10] = { 0, }; p.clear();
                continue;
            }

            msg = sck_list[turn].user + " : ";
            for (int i : p) msg += std::to_string(i) + " ";
            send_msg(msg.c_str()); //Ŭ���̾�Ʈ�� �θ� ���ڸ� ��ü������ ������.
            msg = "";

            int strike = 0, ball = 0; // ��Ʈ����ũ, �� ���� �˰���.

            for (int i = 0; i < 3; i++) {
                if (memo[c[i]] == 1 && p[i] == c[i]) strike++; // p �� c ������ �ְ�, p ������ ��ġ�� c ������ ��ġ�� ������ strike;
                else if (memo[c[i]] == 1 && p[i] != c[i]) ball++; // p �� 
            }

            if (strike == 3) {          // ���а� �������� �Ǵ� �ڵ�
                game_end = 1;
                return;
            }

            msg = "[" + sck_list[turn].user + "] : Strike : " + std::to_string(strike) + "\tBall : " + std::to_string(ball);
            send_msg(msg.c_str());
            memo[10] = { 0, }; p.clear();
            return;
        }
    }
}

void del_client(int idx) {
    closesocket(sck_list[idx].sck);
    sck_list.erase(sck_list.begin() + idx);
    client_count--;
}

int main() {
    WSADATA wsa;
  
    int code = WSAStartup(MAKEWORD(2, 2), &wsa);

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(server, username, db_password);
    }
    catch (sql::SQLException& e) {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        exit(1);
    }
    con->setSchema("chat");
    stmt = con->createStatement();
    stmt->execute("set names euckr");
    if (stmt) { delete stmt; stmt = nullptr; }

    if (!code) {
        server_init();

        std::thread th1[MAX_CLIENT];
        for (int i = 0; i < MAX_CLIENT; i++) {
            // �ο� �� ��ŭ thread �����ؼ� ������ Ŭ���̾�Ʈ�� ���ÿ� ������ �� �ֵ��� ��.
            th1[i] = std::thread(add_client);
        }
        while (1) { // ���� �ݺ����� ����Ͽ� ������ ����ؼ� ä�� ���� �� �ִ� ���¸� ����� ��. �ݺ����� ������� ������ �� ���� ���� �� ����.
            string text, msg = "";

            std::getline(cin, text);

            const char* buf = text.c_str();
            msg = server_sock.user + " : " + buf;
            send_msg(msg.c_str());
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