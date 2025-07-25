#include <iostream>
#include "xlsxwriter.h" //这里包含了libxlsxwriter库的头文件，主要用于处理Excel文件
#include <windows.h>    //用于编码转换
#include <string>
#include "store.cpp"
#include <limits>   //引入输入审查，防止用户的错误输入导致程序崩溃
using namespace std;

// GBK 转 UTF-8 的辅助函数
string GBKToUTF8(const string& strGBK) {
    string strOutUTF8 = "";
    WCHAR* str1;
    int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
    str1 = new WCHAR[n];
    MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
    char* str2 = new char[n];
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
    strOutUTF8 = str2;
    delete[] str1;
    delete[] str2;
    return strOutUTF8;
}

void write_data(lxw_worksheet *worksheet, Link* message, lxw_format *cn_format) {  // 此函数用于将链表中存储的数据写入表格中
    Node *pt = message->head;   // pt指向正在读取的结点
    Node *pt_same = message->head;  // pt_same指向主干结点
    int row = 2, col = 0;   // row为行，col为列
    double sum_money = 0;   // 计算报表总金额

    if(pt->date == "0") return; // 当没有报销数据时直接返回

    while(pt_same != NULL) {    // 遍历主干节点
        string date = GBKToUTF8(pt->date);  // 将 GBK 转为 UTF-8 ，防止出现乱码
        // 将数据写入表格
        worksheet_write_string(worksheet, row, col, date.c_str(), cn_format);
        string str_sum; // 将分支节点中的数据整合成一条输入表格
        double sum_day_money = 0;   // 统计每日报销

        while(pt != NULL) { // 遍历分支节点
            string str; // 整合报销类型跟金额

            /* 根据pt是否指向主干节点判断str前面是否加顿号 */
            if(pt != pt_same) { 
                str = "、" + pt->claim_sort + to_string(pt->per_money);
            }
            else {
                str = pt->claim_sort + to_string(pt->per_money);
            }

            sum_day_money += pt->per_money; // 统计每日报销
            pt = pt->right; // 更新指针
            str_sum += str; // 整合每行字符
        }

        sum_money += sum_day_money; // 统计报表总金额

        /* 进行字符转换和数据写入，并更新行号和指针 */
        str_sum = GBKToUTF8(str_sum);
        worksheet_write_string(worksheet, row, col+1, str_sum.c_str(), cn_format);
        worksheet_write_number(worksheet, row, col+2,sum_day_money, cn_format);
        row++;
        pt_same = pt_same->last;
        pt = pt_same;
    }

    string sum_money_label = GBKToUTF8("合计");
    worksheet_write_string(worksheet, row+2, col+1, sum_money_label.c_str(), cn_format);
    worksheet_write_number(worksheet, row+2, col+2, sum_money, cn_format);
    cout <<endl << "Excel表创建完成，信息已导入，可安全退出。" <<endl <<endl;
}

void create_excel_file(string name, Link *message) {  //该函数用于实现创建Excel文件与表格并写入数据的功能

    cout << "开始创建Excel文件......" << endl;
    lxw_workbook *workbook = workbook_new("excel.xlsx") ;   //  创建一个Excel文件
    if(!workbook) {     // 判断文件创建是否成功
        cout <<endl << "Excel文件创建失败！" <<endl <<endl;
        return;
    }
    cout << "Excel文件创建成功。" <<endl;

    cout << "开始创建表格......" << endl;
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL) ; //  在Excel文件中创建一个表格
    if(!worksheet) {    // 判断表格创建是否成功
        cout <<endl << "表格创建失败!" << endl <<endl;
        workbook_close(workbook);   //  关闭Excel文件
        return;
    }
    cout << "表格创建成功。" <<endl;

    lxw_format *cn_format = workbook_add_format(workbook);  //  设置字体为宋体
    format_set_font_name(cn_format, "SimSun");

    /*将字符从 GBK 转换成 UTF-8 */
    string name_label_utf8 = GBKToUTF8("人员：");
    string name_utf8 = GBKToUTF8(name);
    string date_label = GBKToUTF8("日期");
    string catalog = GBKToUTF8("报销目录");
    string money_label = GBKToUTF8("金额");

    /* 在表格中写入数据 */
    worksheet_write_string(worksheet, 0, 0, name_label_utf8.c_str(), cn_format);
    worksheet_write_string(worksheet, 0, 1, name_utf8.c_str(), cn_format);
    worksheet_write_string(worksheet, 1, 0, date_label.c_str(), cn_format);
    worksheet_write_string(worksheet, 1, 1, catalog.c_str(), cn_format);
    worksheet_write_string(worksheet, 1, 2, money_label.c_str(), cn_format);

    write_data(worksheet, message, cn_format);  // 写入链表中存储的报销信息

    /* 关闭Excel文件 */
    lxw_error err_close = workbook_close(workbook);
    if(err_close != LXW_NO_ERROR) {
        cout <<endl << "Excel文件关闭失败！" <<endl <<endl;
    }
    cout << "操作成功。" <<endl;
}

int main() {
    string name;
    name_question: cout << "请输入你的名字: ";  // 此处安插了一个name_question标签，用于回溯
    cin >> name;

    Link *message = new Link(); // 创建链表，用于存储报销信息

    int select_num; // 用户选择的业务
    business_question:  // 回溯标签
    cout << "1.录入报销信息" <<endl;
    cout << "2.录入已完成，创建Excel表" <<endl;
    cout << "3.返回" <<endl;
    cout << "4.退出" <<endl;
    cout << "请选择你的业务（输入数字即可）：";

    cin >> select_num;
    if (cin.fail()) {  // 输入类型不匹配
            cin.clear();  // 清除错误状态
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 清空缓冲区
        } 

    switch(select_num) {
        case 1: input_message(message); goto business_question; break;
        case 2: create_excel_file(name, message); goto business_question; break;
        case 3: goto name_question; break;
        case 4: break;
        default: 
        cout <<endl << "输入错误，无此业务！" <<endl;
        cout <<endl;
        goto business_question;
    }
    system("pause");
    return 0;
}