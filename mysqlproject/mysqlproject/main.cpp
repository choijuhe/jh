#include <iostream>
#include <string>
#include <mysql/jdbc.h>
//#pragma comment(lib, "libmySQL.lib")

using namespace std;

const string server = "tcp://127.0.0.1:3306"; // 데이터베이스 주소
const string username = "root"; // 데이터베이스 사용자
const string password = "Aa103201579!"; // 데이터베이스 접속 비밀번호


int main()
{

    // MySQL Connector/C++ 초기화
    sql::mysql::MySQL_Driver* driver; // 추후 해제하지 않아도 Connector/C++가 자동으로 해제해 줌
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

    // 데이터베이스 선택
    con->setSchema("login");

    // db 한글 저장을 위한 셋팅 
    stmt = con->createStatement();
    stmt->execute("set names euckr");
    if (stmt) { delete stmt; stmt = nullptr; }

    // 데이터베이스 쿼리 실행
    //stmt = con->createStatement();
    //stmt->execute("DROP TABLE IF EXISTS client"); // DROP
    //cout << "Finished dropping table (if existed)" << endl;
    //stmt->execute("CREATE TABLE client (id VARCHAR(50) PRIMARY KEY, password VARCHAR(50));"); // CREATE
    //cout << "Finished creating table" << endl;
    //delete stmt;
    string a;
    string check;
    std::cout << "아이디를 입력하시오";
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
        std::cout << "동일하다";
    }
    else if (check != a) {
        std::cout << "동일한 것이 없다.";
    }
    else {};
    
    
    //stmt->execute("SELECT * FROM client");
   


    // MySQL Connector/C++ 정리
    delete pstmt;
    delete con;

    return 0;
}