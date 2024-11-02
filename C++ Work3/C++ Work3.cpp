#include <iostream>
#include <string>
#include <list>
#include <forward_list>
#include <vector>
#include <algorithm>

using namespace std;

// 圖書類別，儲存每本圖書的基本資訊
struct Book {
    string title;     // 書名
    string author;    // 作者
    string bookID;    // 圖書編號

    // 構造函數，初始化圖書資訊
    Book(string t, string a, string id) : title(t), author(a), bookID(id) {}
};

// 借閱者類別，儲存每位借閱者的基本資訊
struct Borrower {
    string name;                 // 借閱者姓名
    vector<string> borrowedBooks; // 借閱的圖書編號列表

    // 構造函數，初始化借閱者姓名
    Borrower(string n) : name(n) {}
};

// 圖書館管理系統類別，管理所有圖書和借閱者
class LibrarySystem {
private:
    list<Book> books;                // 使用 list 容器管理圖書
    forward_list<Borrower> borrowers; // 使用 forward_list 容器管理借閱者

public:
    // 構造函數，初始化並載入測試資料
    LibrarySystem() {
        loadTestData();
    }

    // 驗證圖書編號格式，須符合5個字元且格式為字母+4位數字
    bool isValidBookID(const string& id) {
        return (id.size() == 5 && isalpha(id[0]) && isdigit(id[1]) && isdigit(id[2]) && isdigit(id[3]) && isdigit(id[4]));
    }

    // 添加新圖書到系統
    void addBook() {
        string title, author, id;
        cout << "輸入書名: ";
        cin >> title;
        cout << "輸入作者: ";
        cin >> author;
        cout << "輸入圖書編號 (5位，首字為字母後面為4位數字): ";
        cin >> id;

        // 檢查圖書編號格式是否正確，不正確時提示重新輸入
        while (!isValidBookID(id)) {
            cout << "圖書編號無效，請重新輸入: ";
            cin >> id;
        }

        books.emplace_back(title, author, id);  // 添加圖書到 list 容器
        cout << "圖書添加成功。\n";
    }

    // 刪除指定圖書編號的圖書
    void deleteBook() {
        string id;
        cout << "輸入要刪除的圖書編號: ";
        cin >> id;

        // 使用 find_if 查找指定編號的圖書
        auto it = find_if(books.begin(), books.end(), [&id](const Book& book) { return book.bookID == id; });

        if (it != books.end()) {
            books.erase(it);  // 找到圖書並刪除
            cout << "圖書刪除成功。\n";
        }
        else {
            cout << "找不到該圖書。\n";
        }
    }

    // 搜索指定圖書編號的圖書
    void searchBook() {
        string id;
        cout << "輸入要查詢的圖書編號: ";
        cin >> id;

        // 使用 find_if 查找指定編號的圖書
        auto it = find_if(books.begin(), books.end(), [&id](const Book& book) { return book.bookID == id; });

        if (it != books.end()) {
            // 找到圖書並顯示資訊
            cout << "找到圖書 - 書名: " << it->title << ", 作者: " << it->author << ", 圖書編號: " << it->bookID << endl;
        }
        else {
            cout << "找不到該圖書。\n";
        }
    }

    // 列出所有圖書並依圖書編號排序
    void listBooks() {
        // 對 list 容器中的圖書依照 bookID 進行排序
        books.sort([](const Book& a, const Book& b) { return a.bookID < b.bookID; });

        cout << "圖書列表:\n";
        for (const auto& book : books) {
            cout << "書名: " << book.title << ", 作者: " << book.author << ", 圖書編號: " << book.bookID << endl;
        }
    }

    // 添加新借閱者到系統，並輸入借閱的圖書編號
    void addBorrower() {
        string name;
        cout << "輸入借閱者姓名: ";
        cin >> name;

        Borrower newBorrower(name);

        string bookID;
        char addMore;
        do {
            cout << "輸入借閱的圖書編號 (5位，首字為字母後面為4位數字): ";
            cin >> bookID;

            // 檢查圖書編號格式是否正確
            while (!isValidBookID(bookID)) {
                cout << "圖書編號無效，請重新輸入: ";
                cin >> bookID;
            }

            // 檢查是否已經借閱過該書籍
            if (find(newBorrower.borrowedBooks.begin(), newBorrower.borrowedBooks.end(), bookID) != newBorrower.borrowedBooks.end()) {
                cout << "該圖書已經借閱過，無法重複借閱。\n";
            }
            else {
                newBorrower.borrowedBooks.push_back(bookID); // 添加圖書編號至借閱者的借閱列表
                cout << "圖書已添加至借閱清單。\n";
            }

            cout << "是否添加更多圖書編號？(y/n): ";
            cin >> addMore;
        } while (addMore == 'y' || addMore == 'Y');

        borrowers.push_front(newBorrower);  // 新增借閱者至 forward_list 容器
        cout << "借閱者添加成功。\n";
    }

    // 刪除指定姓名的借閱者
    void deleteBorrower() {
        string name;
        cout << "輸入要刪除的借閱者姓名: ";
        cin >> name;

        // 使用 remove_if 刪除符合條件的借閱者
        borrowers.remove_if([&name](const Borrower& borrower) { return borrower.name == name; });
        cout << "借閱者刪除成功（若存在）。\n";
    }

    // 搜索指定姓名的借閱者
    void searchBorrower() {
        string name;
        cout << "輸入要查詢的借閱者姓名: ";
        cin >> name;

        // 使用 find_if 查找指定姓名的借閱者
        auto it = find_if(borrowers.begin(), borrowers.end(), [&name](const Borrower& borrower) { return borrower.name == name; });

        if (it != borrowers.end()) {
            // 找到借閱者並顯示其借閱的圖書編號
            cout << "找到借閱者 - 姓名: " << it->name << ", 借閱的圖書編號: ";
            for (const auto& bookID : it->borrowedBooks) {
                cout << bookID << " ";
            }
            cout << endl;
        }
        else {
            cout << "找不到該借閱者。\n";
        }
    }

    // 列出所有借閱者及其借閱的圖書
    void listBorrowers() {
        cout << "借閱者列表:\n";
        for (const auto& borrower : borrowers) {
            cout << "姓名: " << borrower.name << ", 借閱的圖書編號: ";
            for (const auto& bookID : borrower.borrowedBooks) {
                cout << bookID << " ";
            }
            cout << endl;
        }
    }

    // 載入測試資料，包含預設圖書和借閱者
    void loadTestData() {
        // 添加圖書測試資料
        books.emplace_back("紅樓夢", "曹雪芹", "A1234");
        books.emplace_back("西遊記", "吳承恩", "B2345");
        books.emplace_back("水滸傳", "施耐庵", "B3456");
        books.emplace_back("三國演義", "羅貫中", "C4567");
        books.emplace_back("金瓶梅", "蘭陵笑笑生", "C5678");
        books.emplace_back("聊齋志異", "蒲松齡", "D6789");
        books.emplace_back("儒林外史", "吳敬梓", "D7890");
        books.emplace_back("封神演義", "許仲琳", "E8901");
        books.emplace_back("鏡花緣", "李汝珍", "E9012");
        books.emplace_back("老殘遊記", "劉鶚", "F0123");

        // 添加借閱者測試資料
        Borrower borrower1("小明");
        borrower1.borrowedBooks = { "A1234", "B2345" };
        borrowers.push_front(borrower1);

        Borrower borrower2("小華");
        borrower2.borrowedBooks = { "C4567" };
        borrowers.push_front(borrower2);

        Borrower borrower3("小美");
        borrower3.borrowedBooks = { "D6789", "E8901" };
        borrowers.push_front(borrower3);

        Borrower borrower4("小強");
        borrower4.borrowedBooks = { "F0123" };
        borrowers.push_front(borrower4);

        Borrower borrower5("小麗");
        borrower5.borrowedBooks = { "B3456", "C5678" };
        borrowers.push_front(borrower5);
    }
};

// 主程式
int main() {
    LibrarySystem library;
    int choice;

    do {
        cout << "\n--- 圖書館管理系統 ---\n";
        cout << "1. 添加新圖書\n";
        cout << "2. 刪除圖書\n";
        cout << "3. 搜尋圖書\n";
        cout << "4. 列出所有圖書\n";
        cout << "5. 添加新借閱者\n";
        cout << "6. 刪除借閱者\n";
        cout << "7. 搜尋借閱者\n";
        cout << "8. 列出所有借閱者\n";
        cout << "0. 退出\n";
        cout << "請輸入您的選擇: ";
        cin >> choice;

        // 根據選擇呼叫相應功能
        switch (choice) {
        case 1: library.addBook(); break;
        case 2: library.deleteBook(); break;
        case 3: library.searchBook(); break;
        case 4: library.listBooks(); break;
        case 5: library.addBorrower(); break;
        case 6: library.deleteBorrower(); break;
        case 7: library.searchBorrower(); break;
        case 8: library.listBorrowers(); break;
        case 0: cout << "退出中...\n"; break;
        default: cout << "無效的選擇，請重新輸入。\n";
        }
    } while (choice != 0);

    return 0;
}
