/*
    *此文件用于测试Excel相关功能
*/
#include "xlsxwriter.h" //这里包含了libxlsxwriter库的头文件，主要用于处理Excel文件
#include <iostream>
using namespace std;

int main() {
    cout << "Program started\n";    // 输出程序开始的提示信息

    lxw_workbook  *workbook  = workbook_new("test.xlsx");   // 创建一个新的工作簿，文件名为test.xlsx
    if (!workbook) {    // 检查工作簿是否创建成功
        // 如果工作簿创建失败，输出错误信息并结束程序
        cout << "Failed to create workbook\n";
        system("pause");
        return 1;
    }

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);  // 添加一个新的工作表到工作簿中
    // 如果工作表添加失败，输出错误信息并结束程序
    if (!worksheet) {
        cout << "Failed to add worksheet\n";
        workbook_close(workbook);
        system("pause");
        return 1;
    }

    lxw_error err1 = worksheet_write_string(worksheet, 0, 0, "Hello", NULL);    // 在工作表的第0行第0列写入字符串"Hello"
    // 检查写入字符串是否成功，如果失败则输出错误信息
    if (err1 != LXW_NO_ERROR)
        cout << "Error writing string\n";

    lxw_error err2 = worksheet_write_number(worksheet, 1, 0, 123, NULL);    // 在工作表的第1行第0列写入数字123
    // 检查写入数字是否成功，如果失败则输出错误信息
    if (err2 != LXW_NO_ERROR)
        cout << "Error writing number\n";

    lxw_error err3 = workbook_close(workbook);  // 关闭工作簿并保存到文件
    // 检查关闭工作簿是否成功，如果失败则输出错误信息
    if (err3 != LXW_NO_ERROR)
        cout << "Error closing workbook\n";

    cout << "Program finished\n";   // 输出程序结束的提示信息
    // 等待用户输入以便查看输出结果
    system("pause"); 
    return 0;
}
