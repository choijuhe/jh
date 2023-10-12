#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS // Ȥ�� localtime_s�� ���

#include <WinSock2.h> //Winsock ������� include. WSADATA �������.
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <mysql/jdbc.h>
#include <ctime>

#define MAX_SIZE 1024

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

int delete_return = 0;
const int special_word = 3;
const string new_password;

SOCKET client_sock;
string real_nickname;

time_t timer;
struct tm* t;

int chat_recv();
void modify_user_info(string*, string*);
void user_delete(string*, string*);

namespace my_to_str
{
    template < typename T > std::string to_string(const T& n)
    {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}
enum Modify {
    MODIFY_ID,
    MODIFY_PASSWORD,
    MODIFY_NICKNAME,
    MODIFY_EMAIL,
    MODIFY_BACK,
};
enum Delete {
    _DELETE,
    YES_DELETE,
    NO_DELETE,
};


//void make_room() {
//    string title;
//    string date = check_date();
//    cout << "ä�ù� ������ �������ּ���. >> ";
//    getline(cin, title);
//
//    pstmt = con->prepareStatement("insert into chatting_room(room_id, room_title, date) values(?,?,?)");
//    pstmt->setString(1, title);
//   
//    pstmt->setString(2, date);
//    pstmt->execute();
//    cout << "ä�ù��� �����Ǿ����ϴ�." << endl;
//
//}
bool email_check(string email) {
    if (email.find("@") == string::npos) {
        return false;
    }
    else if (email.find(".com") == string::npos) {
        return false;
    }
    else if (email.find(".com") < email.find("@")) {
        return false;
    }
    else
        return true;
}

string check_date() {
    timer = time(NULL); // 1970�� 1�� 1�� 0�� 0�� 0�ʺ��� �����Ͽ� ��������� ��
    t = localtime(&timer); // �������� ���� ����ü�� �ֱ�
    string to_date;
   
    to_date = my_to_str::to_string(t->tm_year + 1900) + "-" + my_to_str::to_string(t->tm_mon + 1) + "-"
        + my_to_str::to_string(t->tm_mday);

    return to_date;
}

string check_timestamp() {
    timer = time(NULL); // 1970�� 1�� 1�� 0�� 0�� 0�ʺ��� �����Ͽ� ��������� ��
    t = localtime(&timer); // �������� ���� ����ü�� �ֱ�
    string to_timestamp;

    to_timestamp = my_to_str::to_string(t->tm_year + 1900) + "-" + my_to_str::to_string(t->tm_mon + 1) + "-"
        + my_to_str::to_string(t->tm_mday) + "-" + my_to_str::to_string(t->tm_hour)
        + "-" + my_to_str::to_string(t->tm_min) + "-" + my_to_str::to_string(t->tm_sec);

    return to_timestamp;
}
// 2023_10_11 ������Ʈ (�α��� ���ɿ���)
bool login_possible(string id, string password) {

    string check_id, check_pw;

    pstmt = con->prepareStatement("SELECT * FROM user where user_id=? and password=?;");
    pstmt->setString(1, id);
    pstmt->setString(2, password);
    pstmt->execute();
    result = pstmt->executeQuery(); //�����ͺ��̽����� id �� password �� �����´�.
    while (result->next()) {
        check_id = result->getString(1).c_str();
        check_pw = result->getString(2).c_str();
        real_nickname = result->getString(3).c_str();
    }
    if (check_id == id && check_pw == password) {
        cout << "�α��� �Ǿ����ϴ�." << endl;
        return true;
    }
    else {
        cout << "�α��ο� �����߽��ϴ�." << endl;
        return false;
    }
}
// 2023_10_11 ������Ʈ (�޴�����Ʈ - ä�ù� ���� �Լ�)
void go_chatting(SOCKADDR_IN& client_addr) {        //ê�ù� ���� �Լ�
    //�α����� ������ Ŀ��Ʈ �Ѵ�.
    while (1) {
        if (!connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) {
            cout << "Server Connect" << endl;
            send(client_sock, real_nickname.c_str(), real_nickname.length(), 0);
            break;
        }
        cout << "Connecting..." << endl;
    }

    std::thread th2(chat_recv);
    //message_store(check_id);
    while (1) {
        string text;
        std::getline(cin, text);
        const char* buffer = text.c_str(); // string���� char* Ÿ������ ��ȯ
        send(client_sock, buffer, strlen(buffer), 0);
    }
    th2.join();
    closesocket(client_sock);
}

// 2023_10_11 ������Ʈ (�α��� ������ �޴�)
void MenuList(SOCKADDR_IN& client_addr, string* id, string* password) {
    int num;
    
    cout << "1. ȸ���������� 2. ä�ù� ���� 3. ȸ��Ż��\n";
    cin >> num;
    
        switch (num) {
        case 1:
            modify_user_info(id, password);
            MenuList(client_addr, id, password); // ȸ������������ �Ϸ�� �� �ڷΰ��� ����??
            break;
        case 2:
            go_chatting(client_addr);
            break;
        case 3:
            user_delete(id, password);
            if (delete_return) {
               // cout << "check";
                break;
            }
            else {
                MenuList(client_addr, id, password);
                break;
            }
            
        }
   
}
void modify_user_info(string* id, string* password) {
    string check_id, check_password;
    string new_info;
    int what_modify = 0;

    pstmt = con->prepareStatement("SELECT * FROM user where user_id = ? and password = ?");
    pstmt->setString(1, *id);
    pstmt->setString(2, *password);
    pstmt->execute();
    result = pstmt->executeQuery();

    while (result->next()) {
        check_id = result->getString(1).c_str();
        check_password = result->getString(2).c_str();
    }//�� id�� password �� ���ϴ°���? ���� -> ���� �Ѿ�ٰ� �ĵ� 
    //ȸ������ ��й�ȣ �����ϰ� ȸ��Ż���Ϸ��� call by value�� ����.
    if (check_id == *id && check_password == *password) {
        cout << "1.��й�ȣ  2.�г���  3.�̸���" << endl;
        cout << "������ ���Ͻô� �׸��� �������ּ���. >> ";
        cin >> what_modify;

        switch (what_modify) {
        case MODIFY_PASSWORD:
            cout << "���ο� ��й�ȣ�� �Է����ּ���. >> ";
            cin >> new_info;
            pstmt = con->prepareStatement("UPDATE user set password = ? where user_id = ?");
            pstmt->setString(1, new_info);
            pstmt->setString(2, *id);
            pstmt->execute();
            *password = new_info;
            //delete pstmt;
            //delete result;
            cout << "���������� ������ �����Ǿ����ϴ�." << endl;
            break;
        case MODIFY_NICKNAME:
            cout << "���ο� �г����� �Է����ּ���. >> ";
            cin >> new_info;
            pstmt = con->prepareStatement("UPDATE user set nickname = ? where user_id = ?");
            pstmt->setString(1, new_info);
            pstmt->setString(2, *id);
            pstmt->execute();
            cout << "���������� ������ �����Ǿ����ϴ�." << endl;
            break;
        case MODIFY_EMAIL:
            cout << "���ο� �̸����� �Է����ּ���. >> ";
            cin >> new_info;
                pstmt = con->prepareStatement("UPDATE user set email = ? where user_id = ?");
                pstmt->setString(1, new_info);
                pstmt->setString(2, *id);
                pstmt->execute();
                cout << "���������� ������ �����Ǿ����ϴ�." << endl;
            
            break;
        case MODIFY_BACK:
            break;
        }
    }
    else {
        cout << "���̵� �Ǵ� ��й�ȣ�� �ùٸ��� �ʽ��ϴ�." << endl;
    }
}

void user_delete(string* id, string* password) {//bool���� ���� �������� �����ؼ� ���� return �����ü
    int yes_or_no = 0;
    string delete_password;
 
    cout << "ȸ��Ż�� �� ��� ��� ��ȭ ������ ������ϴ�." << endl
        << "��� �����Ͻðڽ��ϱ�? (1. ��  2. �ƴϿ�) >> ";
    cin >> yes_or_no;
    switch (yes_or_no) {
    case YES_DELETE:
        cout << "������ ȸ��Ż�� ����, ��й�ȣ�� �Է����ּ���. >> ";
        cin >> delete_password;
        if (delete_password == *password) {
            pstmt = con->prepareStatement("DELETE FROM user where password = ?");
            pstmt->setString(1, *password);
            pstmt->execute();
            cout << "���������� ȸ��Ż�� �Ǿ����ϴ�." << endl;
            delete_return = 1;
            //return true;
            break;
        }
        else {
            cout << "�ùٸ��� ���� ��й�ȣ�� �Է��ϼ̽��ϴ�." << endl;
            delete_return = 0;
            //return false;
            break;
        }
        //break;
    case NO_DELETE:
        delete_return = 0;
        //return false;
        break;
    }
    //return delete_return;
}



int chat_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
            msg = buf;
            std::stringstream ss(msg);  // ���ڿ��� ��Ʈ��ȭ
            string user;
            ss >> user; // ��Ʈ���� ����, ���ڿ��� ���� �и��� ������ �Ҵ�. ���� ����� �̸��� user�� �����.
            if (user != real_nickname) cout << buf << endl; // ���� ���� �� �ƴ� ��쿡�� ����ϵ���.
        }
        else {
            cout << "Server Off" << endl;
            return -1;
        }
    }
}
bool password_check(string a) {
    char special_char[special_word] = { '!', '?', '#' };
    for (char c : special_char) {
        if (a.find(c) != string::npos) {
            return true;
        }
    }
    return false;
}
void join_membership() {
    string id, password, nickname, email;
    string correct_id, correct_password;

    while (1) {
        cout << "����� ���̵� �Է����ּ���. >> ";
        cin >> id;

        pstmt = con->prepareStatement("SELECT * FROM user where user_id=? ;"); //���� id�� �ߺ� üũ�� ���� select��
        pstmt->setString(1, id);
        
        pstmt->execute();

        result = pstmt->executeQuery();
        while (result->next()) {
            correct_id = result->getString(1).c_str();
        }
        if (correct_id == id) {
            cout << "�̹� ������� ���̵��Դϴ�." << endl;
        }
        else if (correct_id != id) {
            while (1) {
                cout << "����� ��й�ȣ�� �Է����ּ���. (!, ?, # �� 1�� �̻� ����) >> ";
                cin >> password;
                if (password_check(password)) {
                    break;
                }
                continue;
            }
            cout << "�г����� �Է����ּ��� >> ";
            cin >> nickname;
            cout << "�̸����� �Է����ּ���. >> ";
            while (1) {
                cin >> email;
                if (email_check(email)) {
                    break;
                }
                else {
                    cout << "�̸��� ���Ŀ� ���缭 �Է����ּ���. >> ";
                    continue;
                }
            }
            break;
        }
    }
    //���̵� �ߺ� üũ�� ������ ��й�ȣ �̸��� user���̺� �����Ѵ�.
    pstmt = con->prepareStatement("insert into user(user_id, password, nickname, email) values(?,?,?,?)");
    pstmt->setString(1, id);
    pstmt->setString(2, password);
    pstmt->setString(3, nickname);
    pstmt->setString(4, email);
    pstmt->execute();
    cout << "ȸ�������� �Ϸ�Ǿ����ϴ�." << endl;
        
    }
//void message_store(string check_id) {
//
//    string store_id, store_msg; //client���̺��� id��,chatting������ ���� ���� 
//
//    bool id = true;
//    cout << "����� ����" << endl;
//    con->setSchema("login");
//    pstmt = con->prepareStatement("SELECT * FROM chatting_massenger;");
//    result = pstmt->executeQuery();
//    while (result->next()) {
//        store_id = result->getString("id").c_str();
//        store_msg = result->getString("text").c_str();
//        //ó�� �����ؼ� ä�ó����� ������ �� ������ ������ ����
//        if (store_id == check_id)
//        {
//            id = false;
//        }
//        if (id == false)
//        {
//            cout << store_id << " : " << store_msg << endl;
//        }
//    }
//}


int main() {
    WSADATA wsa;
    int client_choose = 0;
    bool login = true;
    string id, password;
    // Winsock�� �ʱ�ȭ�ϴ� �Լ�. MAKEWORD(2, 2)�� Winsock�� 2.2 ������ ����ϰڴٴ� �ǹ�.
     // ���࿡ �����ϸ� 0��, �����ϸ� �� �̿��� ���� ��ȯ.
     // 0�� ��ȯ�ߴٴ� ���� Winsock�� ����� �غ� �Ǿ��ٴ� �ǹ�.
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
    //�ѱ�� �ޱ����� ������
    stmt = con->createStatement();
    stmt->execute("set names euckr");
    if (stmt) { delete stmt; stmt = nullptr; }
    stmt = con->createStatement();
    delete stmt;

    if (!code) {

        client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 

        // ������ ���� ���� ���� �κ�
        SOCKADDR_IN client_addr = {};
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(7777);
        InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);

        while (1) {
            cout << "1: �α����ϱ� 2: ȸ�������ϱ�" << endl;
            cin >> client_choose;
            // �α���
            if (client_choose == 1) {
                //string id, password;
                cout << "ID:";
                cin >> id;
                cout << "PW:";
                cin >> password;
                bool check = login_possible(id, password);
                if (!check) continue;
                MenuList(client_addr, &id, &password);
                continue; //10_12 ȸ��Ż�� �� main����
            }
            if (client_choose == 2)
            {
                join_membership();
                continue;
            }
            
        }
    }
    WSACleanup();
    return 0;
}