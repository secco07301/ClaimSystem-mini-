#include <iostream>
#include "xlsxwriter.h" //���������libxlsxwriter���ͷ�ļ�����Ҫ���ڴ���Excel�ļ�
#include <windows.h>    //���ڱ���ת��
#include <string>
#include "store.cpp"
#include <limits>   //����������飬��ֹ�û��Ĵ������뵼�³������
using namespace std;

// GBK ת UTF-8 �ĸ�������
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

void write_data(lxw_worksheet *worksheet, Link* message, lxw_format *cn_format) {  // �˺������ڽ������д洢������д������
    Node *pt = message->head;   // ptָ�����ڶ�ȡ�Ľ��
    Node *pt_same = message->head;  // pt_sameָ�����ɽ��
    int row = 2, col = 0;   // rowΪ�У�colΪ��
    double sum_money = 0;   // ���㱨���ܽ��

    if(pt->date == "0") return; // ��û�б�������ʱֱ�ӷ���

    while(pt_same != NULL) {    // �������ɽڵ�
        string date = GBKToUTF8(pt->date);  // �� GBK תΪ UTF-8 ����ֹ��������
        // ������д����
        worksheet_write_string(worksheet, row, col, date.c_str(), cn_format);
        string str_sum; // ����֧�ڵ��е��������ϳ�һ��������
        double sum_day_money = 0;   // ͳ��ÿ�ձ���

        while(pt != NULL) { // ������֧�ڵ�
            string str; // ���ϱ������͸����

            /* ����pt�Ƿ�ָ�����ɽڵ��ж�strǰ���Ƿ�Ӷٺ� */
            if(pt != pt_same) { 
                str = "��" + pt->claim_sort + to_string(pt->per_money);
            }
            else {
                str = pt->claim_sort + to_string(pt->per_money);
            }

            sum_day_money += pt->per_money; // ͳ��ÿ�ձ���
            pt = pt->right; // ����ָ��
            str_sum += str; // ����ÿ���ַ�
        }

        sum_money += sum_day_money; // ͳ�Ʊ����ܽ��

        /* �����ַ�ת��������д�룬�������кź�ָ�� */
        str_sum = GBKToUTF8(str_sum);
        worksheet_write_string(worksheet, row, col+1, str_sum.c_str(), cn_format);
        worksheet_write_number(worksheet, row, col+2,sum_day_money, cn_format);
        row++;
        pt_same = pt_same->last;
        pt = pt_same;
    }

    string sum_money_label = GBKToUTF8("�ϼ�");
    worksheet_write_string(worksheet, row+2, col+1, sum_money_label.c_str(), cn_format);
    worksheet_write_number(worksheet, row+2, col+2, sum_money, cn_format);
    cout <<endl << "Excel������ɣ���Ϣ�ѵ��룬�ɰ�ȫ�˳���" <<endl <<endl;
}

void create_excel_file(string name, Link *message) {  //�ú�������ʵ�ִ���Excel�ļ�����д�����ݵĹ���

    cout << "��ʼ����Excel�ļ�......" << endl;
    lxw_workbook *workbook = workbook_new("excel.xlsx") ;   //  ����һ��Excel�ļ�
    if(!workbook) {     // �ж��ļ������Ƿ�ɹ�
        cout <<endl << "Excel�ļ�����ʧ�ܣ�" <<endl <<endl;
        return;
    }
    cout << "Excel�ļ������ɹ���" <<endl;

    cout << "��ʼ�������......" << endl;
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL) ; //  ��Excel�ļ��д���һ�����
    if(!worksheet) {    // �жϱ�񴴽��Ƿ�ɹ�
        cout <<endl << "��񴴽�ʧ��!" << endl <<endl;
        workbook_close(workbook);   //  �ر�Excel�ļ�
        return;
    }
    cout << "��񴴽��ɹ���" <<endl;

    lxw_format *cn_format = workbook_add_format(workbook);  //  ��������Ϊ����
    format_set_font_name(cn_format, "SimSun");

    /*���ַ��� GBK ת���� UTF-8 */
    string name_label_utf8 = GBKToUTF8("��Ա��");
    string name_utf8 = GBKToUTF8(name);
    string date_label = GBKToUTF8("����");
    string catalog = GBKToUTF8("����Ŀ¼");
    string money_label = GBKToUTF8("���");

    /* �ڱ����д������ */
    worksheet_write_string(worksheet, 0, 0, name_label_utf8.c_str(), cn_format);
    worksheet_write_string(worksheet, 0, 1, name_utf8.c_str(), cn_format);
    worksheet_write_string(worksheet, 1, 0, date_label.c_str(), cn_format);
    worksheet_write_string(worksheet, 1, 1, catalog.c_str(), cn_format);
    worksheet_write_string(worksheet, 1, 2, money_label.c_str(), cn_format);

    write_data(worksheet, message, cn_format);  // д�������д洢�ı�����Ϣ

    /* �ر�Excel�ļ� */
    lxw_error err_close = workbook_close(workbook);
    if(err_close != LXW_NO_ERROR) {
        cout <<endl << "Excel�ļ��ر�ʧ�ܣ�" <<endl <<endl;
    }
    cout << "�����ɹ���" <<endl;
}

int main() {
    string name;
    name_question: cout << "�������������: ";  // �˴�������һ��name_question��ǩ�����ڻ���
    cin >> name;

    Link *message = new Link(); // �����������ڴ洢������Ϣ

    int select_num; // �û�ѡ���ҵ��
    business_question:  // ���ݱ�ǩ
    cout << "1.¼�뱨����Ϣ" <<endl;
    cout << "2.¼������ɣ�����Excel��" <<endl;
    cout << "3.����" <<endl;
    cout << "4.�˳�" <<endl;
    cout << "��ѡ�����ҵ���������ּ��ɣ���";

    cin >> select_num;
    if (cin.fail()) {  // �������Ͳ�ƥ��
            cin.clear();  // �������״̬
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // ��ջ�����
        } 

    switch(select_num) {
        case 1: input_message(message); goto business_question; break;
        case 2: create_excel_file(name, message); goto business_question; break;
        case 3: goto name_question; break;
        case 4: break;
        default: 
        cout <<endl << "��������޴�ҵ��" <<endl;
        cout <<endl;
        goto business_question;
    }
    system("pause");
    return 0;
}