#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS //Ȥ�� localtime_s�� ���

#include <WinSock2.h> //Winsock ������� include. WSADATA �������
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <mysql/jdbc.h>
#include <ctime>
#include <windows.h>
#include <cstdio> 
#include <vector>

#define MAX_SIZE 1024

WSADATA wsa;
using std::cout;
using std::cin;
using std::endl;
using std::string;

sql::mysql::MySQL_Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

const string server = "tcp://127.0.0.1:3306"; //�����ͺ��̽� �ּ�
const string username = "root"; //�����ͺ��̽� �����
const string db_password = "Aa103201579!"; //�����ͺ��̽� ���� ��й�ȣ

int all_color_num = 16;
int delete_return = 0;
const int special_word = 3; //��й�ȣ ��Ģ
SOCKET client_sock;
string id, password, real_nickname, user;
SOCKADDR_IN client_addr = {};
SOCKADDR_IN new_client = {};
std::vector <string> bad_word = { "�ٺ�", "��û��" };
int turn_flag = 0; //���� ���� Ȯ���ϱ� ���� �÷��� ����

time_t timer;
struct tm* t;

bool password_check(string);
int chat_recv();
void modify_user_info(string*, string*);
void user_delete(string*, string*);
void chat_history();
void MenuList(SOCKADDR_IN&, string*, string*);
void first_screen();
void current_user(string);
bool duplicate_current_user(string);
void delete_current_user(string);
void re_chat();
void output_chat(char buf[MAX_SIZE]);
void gameRoomFunction(string);
void go_chatting(SOCKADDR_IN&, int);
void my_info(string, string);

namespace my_to_str { //�����Ϸ��� ǥ�� ������ �� �¾Ƽ� to_string ��� �� �����߻�
    template < typename T > std::string to_string(const T& n)
    {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}
enum ConsolColor {
    CC_BLACK,
    CC_DARKBLUE,
    CC_DARKGREEN,
    CC_DARKCYAN,
    CC_DARKRED,
    CC_DARKMAGENTA,
    CC_DARKYELLOW,
    CC_LIGHTGRAY,
    CC_DARKGRAY,
    CC_BLUE,
    CC_GREEN,
    CC_CYAN,
    CC_RED,
    CC_MAGENTA,
    CC_YELLOW,
    CC_WHITE,
};
const char* enum_str[] = {
    "BLACK",
    "DARKBLUE",
    "DARKGREEN",
    "DARKCYAN",
    "DARKRED",
    "DARKMAGENTA",
    "DARKYELLOW",
    "LIGHTGRAY",
    "DARKGRAY",
    "BLUE",
    "GREEN",
    "CYAN",
    "RED",
    "MAGENTA",
    "YELLOW",
    "WHITE",
};

typedef struct _Color {
    int num = 1;
    int color[10] = { CC_BLACK };
    string user_nickname[10] = { "." };
}Color;

Color c;

enum Modify {
    MODIFY_ID,
    MODIFY_PASSWORD,
    MODIFY_NICKNAME,
    MODIFY_EMAIL,
    MODIFY_BACK,
    MEMBER_INFORMATION,
};
enum Delete {
    _DELETE,
    YES_DELETE,
    NO_DELETE,
};
enum Menu {
    _MENU,
    MENU_MODIFY,
    MENU_CHAT,
    MENU_DELETE,
    MENU_MAIN,
    MENU_GAME,
};
enum Main {
    _MAIN,
    MAIN_LOGIN,
    MAIN_MEMBERSHIP,
};

void to_clean() {
    system("cls");
}

void setFontColor(int color) { //�ؽ�Ʈ ���� ����
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (info.wAttributes & 0xf0) | (color & 0xf));
}

void setColor(int color, int bgcolor) { //�ؽ�Ʈ & ��� ���� ����
    color &= 0xf;
    bgcolor &= 0xf;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
        (bgcolor << 4) | color);
}

bool email_check(string email) { //�̸��� ��Ģ �˻�
    if (email.find("@") == string::npos) {
        return false;
    }
    else if (email.find(".com") == string::npos) {
        return false;
    }
    else if (email.find(".com") < email.find("@")) {
        return false;
    }
    else if (email.find("@") == 0) {
        return false;
    }
    else
        return true;
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

bool login_possible(string possible_id, string possible_password) { //�α���
    string check_id, check_pw;

    pstmt = con->prepareStatement("SELECT * FROM user where user_id=? and password=?;");
    pstmt->setString(1, possible_id);
    pstmt->setString(2, possible_password);
    pstmt->execute();
    result = pstmt->executeQuery(); //�����ͺ��̽����� id�� password�� ������
    while (result->next()) {
        check_id = result->getString(1).c_str();
        check_pw = result->getString(2).c_str();
        real_nickname = result->getString(3).c_str();
    }
    if (check_id == possible_id && check_pw == possible_password) {
        if (duplicate_current_user(possible_id)) {
            cout << "�α��� �Ǿ����ϴ�." << endl;
            current_user(possible_id);
            Sleep(2000);
            to_clean();
            return true;
        }
        else {
            cout << "���� ���� ���� �����Դϴ�." << endl;
            Sleep(2000);
            to_clean();
        }
    }
    else {
        cout << "�α��ο� �����߽��ϴ�." << endl;
        Sleep(2000);
        to_clean();
        return false;
    }
}

void chat_history() { //���� ���� �� SERVER�� ���� 1�� ������ ä�� ������ �����ͺ��̽��κ��� �޾ƿ�
    pstmt = con->prepareStatement("select * from chatting");
    pstmt->execute();
    result = pstmt->executeQuery();
    while (result->next()) {
        cout << result->getString(2) << " [" + result->getString(3) + "] :" << result->getString(1) << '\n';
    }
}

void re_chat() { //socket�� ���� ��, ���ο� socket ����
    closesocket(client_sock);
    WSACleanup();

    int code = WSAStartup(MAKEWORD(2, 2), &wsa);

    if (!code) {
        client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(7777);
        InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);
    }
}

void go_chatting(SOCKADDR_IN& client_addr, int port_num) { //ä�ù� ���� �Լ�
    client_addr.sin_port = htons(port_num);

    while (1) {
        if (!connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) {
            if (port_num == 7777) cout << "Server Connect" << endl; //sin_port = 7777 -> ä�� ���� ����
            if (port_num == 6666) cout << "gameServer Connect" << endl; //sin_port = 6666 -> ���� ���� ����
            send(client_sock, real_nickname.c_str(), real_nickname.length(), 0);
            break;
        }
        cout << "Connecting..." << endl;
    }
    if (port_num == 6666) {
        char buf[MAX_SIZE] = { };
        string msg = "", text = "";
        while (1) {
            ZeroMemory(&buf, MAX_SIZE);
            if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
                msg = buf;
                cout << msg << endl;
                if (msg.find("1~9 ������ ���� 3���� �Է� �ϼ���.") != string::npos) {
                    std::getline(cin, text);
                    send(client_sock, text.c_str(), text.size(), 0);
                    continue;
                }
                else if (msg.find("******** �����մϴ�") != string::npos) {

                    text = "end_game";
                    send(client_sock, text.c_str(), text.size(), 0);
                    Sleep(2000);
                    to_clean();
                    closesocket(client_sock);
                    re_chat();
                    MenuList(client_addr, &id, &password); //���� ���� -> �޴��� �̵�
                    return;
                }
            }
        }
    }
    std::thread th_recv(chat_recv);

    while (1) {
        string text;
        std::getline(cin, text);
        const char* buffer = text.c_str(); //string���� char* Ÿ������ ��ȯ

        if (text == "") { continue; } //enter -> db ���� x
        if (text == "/back") { //�ڷΰ��� ���
            send(client_sock, buffer, strlen(buffer), 0);
            re_chat();
            MenuList(client_addr, &id, &password);
            break;
        }
        else {
            if (text != "/help" && text.find("/color") == string::npos && text.substr(0, 3) != "/dm") { 
                pstmt = con->prepareStatement("insert into chatting(message_content, send_time, user_nickname) values(?, ?, ?)");
                pstmt->setString(1, text);
                pstmt->setString(2, check_timestamp());
                pstmt->setString(3, real_nickname);
                pstmt->execute();
            } //��ɾ� ä�ó��� db�� ���� x
        }
        if (text == "/help") { //����� ���� ��ɾ�
            setFontColor(CC_YELLOW);
            cout << "/dm <�г���> <�޽���>  : Ư�� �������Ը� �޽����� �����ϰ�, �ٸ� �������Դ� ǥ�õ��� �ʽ��ϴ�.\n";
            cout << "/dm_msg                : ������ ���� DM���� ǥ���մϴ�.\n";
            cout << "/color <����> <�г���> : Ư�� ������ �޽��� ������ �����ŵ�ϴ�.(/color ��ɾ� : ���� ����)\n";
            cout << "/back                  : �޴�ȭ������ ���ư��� ��ɾ��Դϴ�.\n";
            setColor(CC_LIGHTGRAY, CC_BLACK);
            continue;
        }
        if (text == "/color") {
            for (int color = 0; color < all_color_num; color++) {
                setFontColor(CC_YELLOW);
                cout << color << " : " << enum_str[color] << endl;
            }setColor(CC_LIGHTGRAY, CC_BLACK);
            continue;
        }

        for (int i = 0; i < 2; i++) {
            if (text.find(bad_word[i]) != string::npos) { //�弳 �� ����. socket�� �ݰ�, �ٽ� socket ����
                send(client_sock, buffer, strlen(buffer), 0);
                re_chat();
                MenuList(client_addr, &id, &password);
                break;
            }
        }
        send(client_sock, buffer, strlen(buffer), 0);
    }
    th_recv.join();
    closesocket(client_sock);
}

int chat_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {

            msg = buf;
            std::stringstream ss(msg); //���ڿ��� ��Ʈ��ȭ
            ss >> user; //��Ʈ���� ����, ���ڿ��� ���� �и��� ������ �Ҵ�. ���� ����� �̸��� user�� �����.
            if (msg == "1") { //server�� ���� 1�� ������ db�� �ҷ����� �Լ� ����
                chat_history(); continue;
            }
            if (msg.substr(0, 6) == "/color") {
                std::stringstream s(msg);
                string nick, color;
                while (s >> nick >> color) {
                    if (nick != "/color") {
                        break;
                    }
                }
                c.color[c.num] = stoi(color); //���ϴ� ����� Ư�� ������ �г����� ����ü�� ����
                c.user_nickname[c.num] = nick;
                c.num++;
                continue;
            }

            std::thread th_color(output_chat, buf);

            th_color.join();
        }
        else {
            cout << "Server Off" << endl;
            return -1;
        }
    }
}

void output_chat(char buf[MAX_SIZE]) { //���� ���� ��� ������ ê -> �ߺ� ��� ����

    for (int i = 0; i < c.num; i++) {
        if (user == c.user_nickname[i]) {
            setFontColor(c.color[i]);

            cout << buf << endl;

            setColor(CC_LIGHTGRAY, CC_BLACK); //���� ����.
            return;
        }
    }
    if (user != real_nickname) {
        setColor(CC_LIGHTGRAY, CC_BLACK);
        cout << buf << endl;
        return;
    }//���� ���� �� �ƴ� ��쿡�� ����ϵ���
}

void MenuList(SOCKADDR_IN& client_addr, string* menu_id, string* menu_password) {
    int num = 0;

    to_clean();
    cout << "�١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ�\n";
    cout << "��                                                  ��\n";
    cout << "��                                                  ��\n";
    cout << "��                                                  ��\n";
    cout << "��                  1. ȸ������                     ��\n";
    cout << "��                                                  ��\n";
    cout << "��                                                  ��\n";
    cout << "��                  2. ä�ù� ����                  ��\n";
    cout << "��                                                  ��\n";
    cout << "��                                                  ��\n";
    cout << "��                  3. ȸ��Ż��                     ��\n";
    cout << "��                                                  ��\n";
    cout << "��                                                  ��\n";
    cout << "��                  4. �α׾ƿ�                     ��\n";
    cout << "��                                                  ��\n";
    cout << "��                                                  ��\n";
    cout << "��                  5. ���ӹ� ����                  ��\n";
    cout << "��                                                  ��\n";
    cout << "��                                                  ��\n";
    cout << "��                                                  ��\n";
    cout << "��                                                  ��\n";
    cout << "�١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ�\n";

    cin >> num;
    string temp;
    getline(cin, temp);
    to_clean();
    switch (num) {
    case MENU_MODIFY:
        modify_user_info(menu_id, menu_password);
        MenuList(client_addr, menu_id, menu_password); //ȸ������ ������ �Ϸ�� �� �ٽ� �޴��� �̵�
        break;
    case MENU_CHAT:
        setFontColor(CC_YELLOW);
        cout << "\n\n\n\n\n\n\n\t\t/help ��ɾ�� ����ڿ��� ��ɾ� ����� �����մϴ�. ��";
        setColor(CC_LIGHTGRAY, CC_BLACK); //���� ����
        Sleep(3000);
        to_clean();
        go_chatting(client_addr, 7777); //ä�� ���� ����
        break;
    case MENU_DELETE:
        user_delete(menu_id, menu_password);
        if (delete_return) {
            break;
        }
        else {
            MenuList(client_addr, menu_id, menu_password);
            break;
        }
    case MENU_MAIN:
        delete_current_user(*menu_id); //�α׾ƿ� �� ���� �������� ���� ���̺��� ����
        first_screen();
        break;
    case MENU_GAME:
        to_clean();
        go_chatting(client_addr, 6666); //���� ���� ����
        break;

    }
}

void modify_user_info(string* modify_id, string* password) {
    string new_info;
    int what_modify = 0;

        cout << "1.��й�ȣ  2.�г���  3.�̸���  4.�ڷΰ���  5.ȸ������Ȯ��" << endl;
        cout << "���Ͻô� �׸��� �������ּ���. >> ";
        cin >> what_modify;

        switch (what_modify) {
        case MODIFY_PASSWORD:
            cout << "���ο� ��й�ȣ�� �Է����ּ���. (!, ?, # �� 1�� �̻� ����) >> ";
            while (1) {
                cin >> new_info;
                if (password_check(new_info)) {
                    pstmt = con->prepareStatement("UPDATE user set password = ? where user_id = ?");
                    pstmt->setString(1, new_info);
                    pstmt->setString(2, *modify_id);
                    pstmt->execute();
                    *password = new_info;
                    cout << "���������� ������ �����Ǿ����ϴ�." << endl;
                    Sleep(1000);
                    break;
                }
                else {
                    cout << "��й�ȣ ���Ŀ� ���缭 �Է����ּ���. >> ";
                    continue;
                }
            }break;
        case MODIFY_NICKNAME:
            cout << "���ο� �г����� �Է����ּ���. >> ";
            cin >> new_info;
            pstmt = con->prepareStatement("UPDATE user set nickname = ? where user_id = ?");
            pstmt->setString(1, new_info);
            pstmt->setString(2, *modify_id);
            pstmt->execute();
            cout << "���������� ������ �����Ǿ����ϴ�." << endl;
            real_nickname = new_info;
            Sleep(1000);
            break;
        case MODIFY_EMAIL:
            cout << "���ο� �̸����� �Է����ּ���. >> ";
            while (1) {
                cin >> new_info;
                if (email_check(new_info)) {
                    pstmt = con->prepareStatement("UPDATE user set email = ? where user_id = ?");
                    pstmt->setString(1, new_info);
                    pstmt->setString(2, *modify_id);
                    pstmt->execute();
                    cout << "���������� ������ �����Ǿ����ϴ�." << endl;
                    Sleep(2000);
                    break;
                }
                else {
                    cout << "�̸��� ���Ŀ� ���缭 �Է����ּ���. >> ";
                    continue;
                }
            }
            break;
        case MODIFY_BACK:
            break;
        case MEMBER_INFORMATION:
            my_info(*modify_id, *password);
            break;
    }
}

void my_info(string id, string pwd) {
    pstmt = con->prepareStatement("SELECT * FROM user where user_id = ? and password = ?");
    pstmt->setString(1, id);
    pstmt->setString(2, pwd);
    pstmt->execute();
    result = pstmt->executeQuery();

    cout << "������-------------------------\n";
    while (result->next()) {
        cout << "ID:" << result->getString(1) << "\nPW:" << result->getString(2) << "\n�г���:" << result->getString(3) << "\nemail:" << result->getString(4) << "\n------------------------------";
    }
    Sleep(5000);
    to_clean();
}

void user_delete(string* id, string* password) {
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
            delete_current_user(*id);
            delete_return = 1;
            Sleep(2000);
            to_clean();
            break;
        }
        else {
            cout << "�ùٸ��� ���� ��й�ȣ�� �Է��ϼ̽��ϴ�." << endl;
            delete_return = 0;
            Sleep(2000);
            break;
        }
    case NO_DELETE:
        delete_return = 0;
        break;
    }
}

void current_user(string current_id) { //���� ���� ���� ���� ���̺�
    pstmt = con->prepareStatement("INSERT INTO connecting_user(user_id) values(?)");
    pstmt->setString(1, current_id);
    pstmt->execute();
}

void delete_current_user(string delete_id) { //�α׾ƿ� �� �ش� ������ ���̺��� ����
    pstmt = con->prepareStatement("DELETE FROM connecting_user where user_id = ?");
    pstmt->setString(1, delete_id);
    pstmt->execute();
}

bool duplicate_current_user(string duplicate_current_id) { //�ش� ������ �̹� ���� ���̸� �α��� �Ұ�
    string current_id;

    pstmt = con->prepareStatement("SELECT * FROM connecting_user");
    pstmt->execute();
    result = pstmt->executeQuery();

    while (result->next()) {
        current_id = result->getString(1).c_str();
    }
    if (current_id == duplicate_current_id) {
        return false;
    }
    else return true;
}

bool password_check(string a) {
    char special_char[special_word] = { '!', '?', '#' }; //!, ?, # �� �ϳ��� �ݵ�� �����Ͽ� ��й�ȣ ����
    for (char c : special_char) {
        if (a.find(c) != string::npos) {
            return true;
        }
    }
    return false;
}

void join_membership() {
    string membership_id, membership_password, nickname, email;
    string correct_id, correct_password;
    to_clean();
    while (1) {
        cout << "����� ���̵� �Է����ּ���. >> ";
        cin >> membership_id;

        pstmt = con->prepareStatement("SELECT * FROM user where user_id=? ;"); //���� id�� �ߺ� üũ�� ���� select��
        pstmt->setString(1, membership_id);
        pstmt->execute();
        result = pstmt->executeQuery();

        while (result->next()) {
            correct_id = result->getString(1).c_str();
        }
        if (correct_id == membership_id) {
            cout << "�̹� ������� ���̵��Դϴ�." << endl;
        }
        else if (correct_id != membership_id) {
            id = membership_id;
            while (1) {
                cout << "����� ��й�ȣ�� �Է����ּ���. (!, ?, # �� 1�� �̻� ����) >> ";
                cin >> membership_password;
                if (password_check(membership_password)) {
                    password = membership_password;
                    break;
                }
                continue;
            }
            cout << "�г����� �Է����ּ��� >> ";
            cin >> nickname;
            real_nickname = nickname;
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
    //���̵� �ߺ� üũ�� ������ ��й�ȣ �̸��� user���̺� ����
    pstmt = con->prepareStatement("insert into user(user_id, password, nickname, email) values(?,?,?,?)");
    pstmt->setString(1, membership_id);
    pstmt->setString(2, membership_password);
    pstmt->setString(3, nickname);
    pstmt->setString(4, email);
    pstmt->execute();

    //���ÿ� win_lose ���̺� ȸ�������� client�� id ����(���� ��ŷ)
    pstmt = con->prepareStatement("insert into win_lose(win, lose, user_id) values(?, ?, ?)");
    pstmt->setInt(1, 0);
    pstmt->setInt(2, 0);
    pstmt->setString(3, membership_id);
    pstmt->execute();

    cout << "ȸ�������� �Ϸ�Ǿ����ϴ�.��" << endl;
    Sleep(2000);
    to_clean();
}

void first_screen() { //���� ȭ��
    int client_choose = 0;
    while (1) {
        cout << "�١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ�\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��     �ڡڡڡڡ�      ��      ��       ��  ��      ��\n";
        cout << "��         ��        �� ��     ��       �ڡ�        ��\n";
        cout << "��         ��       �ڡڡڡ�   ��       �� ��       ��\n";
        cout << "��         ��     ��       ��  �ڡڡڡ� ��   ��     ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��              1. �α���   2. ȸ������             ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "��                                                  ��\n";
        cout << "�١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ�\n";

        cin >> client_choose;

        if (client_choose == MAIN_LOGIN) { //�α���
            cout << "ID :";
            cin >> id;
            cout << "PW :";
            cin >> password;
            bool check = login_possible(id, password);
            if (!check) continue; //�α��� ���� �� �ٽ� main����
            MenuList(client_addr, &id, &password); //�α��� ���� �� �޴���
            continue; //ȸ��Ż�� �� main���� ������
        }
        if (client_choose == MAIN_MEMBERSHIP) { //ȸ������
            join_membership();
            continue;
        }
    }
}

int main() {
    bool login = true;

    int code = WSAStartup(MAKEWORD(2, 2), &wsa);
    //Winsock�� �ʱ�ȭ�ϴ� �Լ�. MAKEWORD(2, 2)�� Winsock�� 2.2 ������ ����ϰڴٴ� �ǹ�
    //���࿡ �����ϸ� 0��, �����ϸ� �� �̿��� ���� ��ȯ
    //0�� ��ȯ�ߴٴ� ���� Winsock�� ����� �غ� �Ǿ��ٴ� �ǹ�

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
    stmt->execute("set names euckr");//�ѱ�� �ޱ����� ������

    if (stmt) { delete stmt; stmt = nullptr; }
    stmt = con->createStatement();
    delete stmt;

    if (!code) {
        client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  //������ ���� ���� ���� �κ�

        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(7777);
        InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);

        first_screen();
    }
    WSACleanup();
    return 0;
}