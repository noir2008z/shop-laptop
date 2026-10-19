#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LAPTOPS 200
#define MAX_CODES 100
#define MAX_FAVORITES 200
#define MAX_HISTORY 200
#define MAX_CRITERIA 20
#define NAME_LEN 80
#define TEXT_LEN 160
#define FILE_LAPTOPS "laptops.txt"
#define FILE_CODES "discount_codes.txt"
#define FILE_FAVORITES "favorites.txt"
#define FILE_HISTORY "history.txt"
#define FILE_CRITERIA "criteria.txt"

typedef struct {
    int id;
    char name[NAME_LEN], brand[NAME_LEN], cpu[TEXT_LEN], gpu[TEXT_LEN];
    int ramGB, ssdGB;
    long long price;
    char screen[TEXT_LEN], battery[TEXT_LEN], weight[TEXT_LEN], url[TEXT_LEN];
    int active;
} Laptop;

typedef struct {
    char code[NAME_LEN];
    int type; /* 1 = phần trăm, 2 = số tiền cố định */
    double percent;
    long long amount, minPurchase;
    char brand[NAME_LEN]; /* "ALL" nghĩa là áp dụng cho mọi hãng */
    int active;
    char startDate[11], endDate[11]; /* YYYY-MM-DD; để trống nghĩa là không giới hạn */
    int usageLimit, usageCount;
} 

typedef struct {
    int laptopId;
    char field[NAME_LEN], usage[NAME_LEN], reason[TEXT_LEN];
    long long budget, finalPrice;
    char dateTime[32];
} History;

typedef struct {
    char key[NAME_LEN];
    char value[TEXT_LEN];
    int mandatory;
    int weight;
} Criterion;
void mainMenu(void) {
    int ch;
    do {
        puts("\n========== CHƯƠNG TRÌNH TƯ VẤN LAPTOP ==========");
        puts("1. Tư vấn laptop (4 bước)");
        puts("2. Tìm kiếm laptop");
        puts("3. Lọc và sắp xếp laptop");
        puts("4. Thêm laptop");
        puts("5. Sửa thông tin laptop");
        puts("6. Ẩn/xóa laptop");
        puts("7. Xem danh sách laptop");
        puts("8. So sánh 2 laptop");
        puts("9. Quản lý tiêu chí cá nhân");
        puts("10. Quản lý danh sách yêu thích");
        puts("11. Xem lịch sử tư vấn");
        puts("13. Lưu/tải dữ liệu");
        puts("0. Thoát");
        ch=readInt("Chọn chức năng: ",0,13);
        switch(ch) {
            case 1: consult(); break;
            case 2: searchLaptops(); break;
            case 3: filterSort(); break;
            case 4: addLaptop(); break;
            case 5: editLaptop(); break;
            case 6: deleteLaptop(); break;
            case 7: listLaptops(); break;
            case 8: compareLaptops(); break;
            case 9: manageCriteria(); break;
            case 10: manageFavorites(); break;
            case 11: viewHistory(); break;
            case 12: manageCodes(); break;
            case 13: {
                int sub;
                puts("1. Lưu ngay\n2. Tải lại từ tệp (các thay đổi chưa lưu có thể bị mất)\n0. Quay lại");
                sub=readInt("Chọn: ",0,2);
                if(sub==1) saveAll();
else if(sub==2) { loadAll(); puts("Đã tải dữ liệu."); }
                break;
            }
        }
        if(ch!=0) pauseEnter();
    } while(ch!=0);
    saveAll();
    puts("Cảm ơn bạn đã sử dụng chương trình tư vấn laptop!");
}
int main(void)  {
    loadAll();
    if(laptopCount==0)  {
        seedData();
        saveAll();
    }
    mainMenu();
    return 0;
}