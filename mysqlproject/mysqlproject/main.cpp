#include <iostream>
#include <string>
#include <mysql/jdbc.h>
//#pragma comment(lib, "libmySQL.lib")

using namespace std;

const string server = "tcp://127.0.0.1:3306"; // �����ͺ��̽� �ּ�
const string username = "root"; // �����ͺ��̽� �����
const string password = "Aa103201579!"; // �����ͺ��̽� ���� ��й�ȣ


int main()
{

    // MySQL Connector/C++ �ʱ�ȭ
    sql::mysql::MySQL_Driver* driver; // ���� �������� �ʾƵ� Connector/C++�� �ڵ����� ������ ��
    sql::Connection* con;
    sql::Statement* stmt;
    sql::PreparedStatement* pstmt;
    sql::ResultSet* result;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(server, username, password);
    }
    catch (sql::SQLException& e) {
        std::cout << "Could not connect to server. Error message: " << e.what() << endl;
        exit(1);
    }

    // �����ͺ��̽� ����
    con->setSchema("login");

    // db �ѱ� ������ ���� ���� 
    stmt = con->createStatement();
    stmt->execute("set names euckr");
    if (stmt) { delete stmt; stmt = nullptr; }

    // �����ͺ��̽� ���� ����
    //stmt = con->createStatement();
    //stmt->execute("DROP TABLE IF EXISTS client"); // DROP
    //cout << "Finished dropping table (if existed)" << endl;
    //stmt->execute("CREATE TABLE client (id VARCHAR(50) PRIMARY KEY, password VARCHAR(50));"); // CREATE
    //cout << "Finished creating table" << endl;
    //delete stmt;
    string a;
    string check;
    std::cout << "���̵� �Է��Ͻÿ�";
    cin >> a;

    pstmt = con->prepareStatement("SELECT * FROM client where id = ?;"); // INSERT
  
    pstmt->setString(1,a);
    //pstmt->setString(2,"password");
    pstmt->execute();
    result = pstmt->executeQuery();

    while (result->next()) {
        check = result->getString(1).c_str();
    }

    if (check == a) {
        std::cout << "�����ϴ�";
    }
    else if (check != a) {
        std::cout << "������ ���� ����.";
    }
    else {};
    
    
    //stmt->execute("SELECT * FROM client");
   


    // MySQL Connector/C++ ����
    delete pstmt;
    delete con;

    return 0;
}