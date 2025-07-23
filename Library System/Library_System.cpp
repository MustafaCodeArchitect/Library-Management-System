#include <iostream>
#include <algorithm>
#include <assert.h>
using namespace std;

const int MAX_BOOKS = 100;
const int MAX_USERS = 100;
bool flag = true;

    struct Book {
        int id;
        string name;
        int total_quantity;
        int total_borrow;

        Book() {
            id = -1;
            name = " ";
            total_borrow = total_quantity = 0;
        }

        void read() {
            cout << "Enter book info: id & name & total quantity: ";
            cin >> id >> name >> total_quantity;
            total_borrow = 0;
        }

        bool borrow() {
            if (total_quantity - total_borrow == 0) {
                return false;
            }

            total_borrow ++;
            return true;
        }

        void return_copy() {
            assert (total_borrow > 0);
                total_borrow --;
        }

        bool has_prefix(string prefix) {
            if (prefix.size() > name.size())
                return false;

            for (int i = 0; i < (int)prefix.size(); ++i) {
                if(name[i] != prefix[i])
                    return false;
            }
            return true;
        }

        void print() {
            cout << "id = " << id << " name = " << name << " total quantity " << total_quantity
            << " total borrow " << total_borrow << "\n";
        }

    };

    bool compare_id(Book &a, Book& b) {
        return a.id < b.id;
    }

    bool compare_name(Book &a, Book& b) {
        return a.name < b.name;
    }

    struct User {
        int id;
        string name;
        int borrowed_books_ids[MAX_BOOKS];
        int len;

        User() {
            id = -1;
            name = " ";
            len = 0;
        }

        void read() {
            cout << "Enter user name & national id: ";
            cin >> name >> id;
        }

        void borrow(int book_id) {
            borrowed_books_ids[len ++] = book_id;
        }

        void return_copy(int book_id) {
            bool removed = false;
            for (int i = 0; i < len; ++i) {
                if (borrowed_books_ids[i] == book_id) {
                    // Let's shift the array to the left to remove this entry

                    for (int j = i + 1; j < len; ++i) {
                        borrowed_books_ids[j - 1] = borrowed_books_ids[j];
                    }

                    removed = true;
                    len --;
                    break;
                }
            }

            if (!removed) {
                cout << "User " << name << " never borrowed book id " << book_id << "\n";
            }
        }

        bool is_borrowed(int book_id) {
            for (int i = 0; i < len; ++i) {
                if (book_id == borrowed_books_ids[i]) {
                    return true;
                }
            }
            return false;
        }

        void print() {
            sort (borrowed_books_ids, borrowed_books_ids + len);

            cout << "user " << name << " id " << id << " borrowed books ids: ";
            for (int i = 0; i < len; ++i) {
                cout << borrowed_books_ids[i] << " ";
            }
            cout << "\n";
        }

    };

    struct Library_system {
        int total_books;
        Book books[MAX_BOOKS];
        int total_users;
        User users[MAX_USERS];

        Library_system() {
            total_books = total_users = 0;
        }

        void run() {
            while (true) {
                int choice = menu();

                if (choice == 1)
                    add_book();
                else if (choice == 2)
                    search_books_by_prefix();
                else if (choice == 3)
                    print_who_borrowed_book();
                else if (choice == 4)
                    print_library_by_id();
                else if (choice == 5)
                    print_library_by_name();
                else if (choice == 6)
                    add_user();
                else if (choice == 7)
                    user_borrow_book();
                else if (choice == 8)
                    user_return_book();
                else if (choice == 9)
                    print_users();
                else
                    break;
            }
        }

        int menu() {
            int choice = -1;
            while (choice == -1) {
                if (flag) {
                    cout << "\nLibrary menu\n";
                    cout << "1) Add book\n";
                    cout << "2) Search books by prefix\n";
                    cout << "3) Print who borrowed book by name\n";
                    cout << "4) Print library by id\n";
                    cout << "5) Print library by name\n";
                    cout << "6) Add user\n";
                    cout << "7) User borrow book\n";
                    cout << "8) User return book\n";
                    cout << "9) Print users\n";
                    cout << "10) Exit\n";
                }
                cout << "\nEnter your menu choice [ 1 - 10 ]: ";
                cin >> choice;

                if (!(choice >= 1 && choice <= 10)) {
                    cout << "Invalid choice, Try again\n";
                    choice = -1; // keep loop working
                }

                flag = false;
            }
            return choice;
        }

        void add_book() {
            books[total_books ++].read();
        }

        void add_user() {
            users[total_users ++].read();
        }

        void search_books_by_prefix() {
            cout << "Enter book name prefix: ";
            string prefix;
            cin >> prefix;

            int count = 0;
            for (int i = 0; i < total_books; ++i) {
                if (books[i].has_prefix(prefix)) {
                    cout << books[i].name << " ";
                    count ++;
                }
            }

            if (!count)
                cout << "No books with such prefix\n";
            cout << "\n";
        }

        int find_user_idx_by_name(string user_name) {
            for (int i = 0; i < total_users; ++i) {
                if (users[i].name == user_name) {
                    return i;
                }
            }
            return -1;
        }

        int find_book_idx_by_name(string book_name) {
            for (int i = 0; i < total_books; ++i) {
                if (books[i].name == book_name) {
                    return i;
                }
            }
            return -1;
        }

        bool read_user_name_and_book_name(int &user_idx, int &book_idx, int trials = 3) {
            string user_name, book_name;

            while (trials --) {
                cout << "Enter user name and book name: ";
                cin >> user_name >> book_name;

                user_idx = find_user_idx_by_name(user_name);

                if (user_idx == -1) {
                    cout << "Invalid user name, Try name\n";
                    continue;
                }

                book_idx = find_book_idx_by_name(book_name);

                if (book_idx == -1) {
                    cout << "Invalid book name, Try again\n";
                    continue;
                }
                return true;
            }
            cout << "You did several trials! Try again\n";
            return false;
        }

        void user_borrow_book() {
            int user_idx, book_idx;

            if (!read_user_name_and_book_name(user_idx, book_idx)) {
                return;
            }

            int user_id = users[user_idx].id;
            int book_id = books[book_idx].id;

            if (!books[book_idx].borrow()) {
                cout << "No more copies available right now\n";
            }

            else {
                users[user_idx].borrow(book_id);
            }

        }

        void user_return_book() {
            int user_idx, book_idx;

            if(!read_user_name_and_book_name(user_idx, book_idx)) {
                return;
            }

            int book_id = books[book_idx].id;
            books[book_idx].return_copy();
            users[user_idx].return_copy(book_id);

        }

        void print_library_by_id() {
            sort (books, books + total_books, compare_id);

            cout << "\n";
            for (int i = 0; i < total_books; ++i) {
                books[i].print();
            }
        }

        void print_library_by_name() {
            sort (books, books + total_books, compare_name);

            cout << "\n";
            for (int i = 0; i < total_books; ++i) {
                books[i].print();
            }
        }

        void print_users() {
            cout << "\n";
            for (int i = 0; i < total_users; ++i) {
                users[i].print();
            }
        }

        void print_who_borrowed_book() {
            string book_name;
            cout << "Enter book name: ";
            cin >> book_name;

            int book_idx = find_book_idx_by_name(book_name);

            if (book_idx == -1) {
                cout << "Invalid book name.\n";
                return;
            }

            int book_id = books[book_idx].id;

            if (books[book_id].total_borrow == 0) {
                cout << "No borrowed copies\n";
                return;
            }

            for (int i = 0; i < total_users; ++i) {
                if (users[i].is_borrowed(book_id)) {
                    cout << users[i].name << "\n";
                }
            }

        }

    };

int main() {
    Library_system Library;
    Library.run();
    return 0;
}