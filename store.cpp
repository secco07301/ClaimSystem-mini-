/*
*   ���ļ����ڹ�������ϵͳ�Ĵ洢���
*/

#include <iostream>
#include <limits>   //����������飬��ֹ�û��Ĵ������뵼�³������
using namespace std;

/* �ڵ� */
struct Node {   
    string date;    // ����
    string claim_sort;  // ��������
    double per_money;   // ���α������͵Ľ��
    Node *last; //��һ���
    Node *right;    //������ͬ�Ľ��

    Node();
    Node(string date, string claim_sort, double per_mony);
    ~Node();
};

Node::Node() {
    this->date = "0";
    this->claim_sort = "0";
    this->per_money = 0;
    this->last = NULL;
    this->right = NULL;
}

Node::Node(string date, string claim_sort, double per_mony) {
    this->date = date;
    this->claim_sort = claim_sort;
    this->per_money = per_money;
    this->last = NULL;
    this->right = NULL;
}

Node::~Node() {
    this->last = NULL;
    this->right = NULL;
}

/* ���� */
struct Link {
    Node *head;
    Node *pt;   // ptָ��ǰ�����ڵ�
    Node *pt_same;  // ��ָ��ָ��֦�ɽڵ�

    Link();
    ~Link();
};

Link::Link() {
    this->head = new Node();
    this->pt = this->head;
    this->pt_same = head;
}

Link::~Link() {
    Node *current_pt = this->head;
    Node *current_pt_same = this->head;
    while(current_pt_same != NULL) {
        while(current_pt != NULL){
            Node *next = current_pt->right;
            delete current_pt;
            current_pt = next;
        }
        current_pt = current_pt_same->last;
        current_pt_same = current_pt_same->last;
    }
    this->head = NULL;
    this->pt = NULL;
    this->pt_same = NULL;
}

void input_message(Link *message) {  // �ú�������ʵ�ֶ�ȡ�û�������Ϣ��������
    claim_input:    // ���ݱ�ǩ
    cout << "���������ڣ�";
    if(message->pt->date == "0") {  // ��ǰ�ڵ�����Ϊ������������
        cin >> message->pt->date;
    }
    else {  // �����ݲ�Ϊ�����½��ڵ㣬�������ڱȶԾ����½����ڵ㻹��֦�ɽڵ�
        string new_input_date;
        cin >> new_input_date;
        if(message->pt->date == new_input_date) {   // �½�֦�ɽڵ�
            message->pt->right = new Node();
            message->pt->right->date = message->pt->date;
            message->pt = message->pt->right;
        }
        else {  // �½����ڵ�
            message->pt_same->last = new Node();
            message->pt = message->pt_same->last;
            message->pt_same = message->pt_same->last;
            message->pt->date = new_input_date;
        }
    }

    claim_sort_choice:
    cout << "1.ͣ����" <<endl;
    cout << "2.ͨ�з�" <<endl;
    cout << "3.���ͷ�" <<endl;
    cout << "4.������" <<endl;
    cout << "5.���" <<endl;
    cout << "6.����" <<endl;
    cout << "��ѡ�������ͣ��������֣���";
    int select_num; // �û�ѡ�������
    cin >> select_num;
    if (cin.fail()) {  // �������Ͳ�ƥ��
            cin.clear();  // �������״̬
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // ��ջ�����
        } 

    switch(select_num) {
        case 1: message->pt->claim_sort = "ͣ����"; break;
        case 2: message->pt->claim_sort = "ͨ�з�"; break;
        case 3: message->pt->claim_sort = "���ͷ�"; break;
        case 4: message->pt->claim_sort = "������"; break;
        case 5: message->pt->claim_sort = "���"; break;
        case 6:
        cout << "�����뱨������:";
        cin >> message->pt->claim_sort; break;
        default:
            cout <<endl << "�޴�ҵ�����ͣ�" <<endl <<endl;
            goto claim_sort_choice; break;
    }

    input_money:
    cout << "�������";
    cin >> message->pt->per_money;
    if (cin.fail()) {  // �������Ͳ�ƥ��
            cin.clear();  // �������״̬
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // ��ջ�����
            cout <<endl << "�������" <<endl <<endl;
            goto input_money;
        } 

    /* ���¼������� */
    cout <<endl;
    cout << "���ڣ�" << message->pt->date <<endl;
    cout << "�������ͣ�" << message->pt->claim_sort <<endl;
    cout << "���:" << message->pt->per_money <<endl;

    if_continue: cout << "¼��ɹ����Ƿ����¼�룿���������ּ��ɣ�" <<endl;
    cout << "1.��           2.��" <<endl;
    cin >> select_num;  // select_num���ã��ж��û���ѡ��
    if (cin.fail()) {  // �������Ͳ�ƥ��
            cin.clear();  // �������״̬
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // ��ջ�����
        } 

    /* �ж��û��Ƿ���Ҫ�������� */
    if(select_num == 1) {

        select_date:
        cout << "1." << message->pt->date <<endl;
        cout << "2.����" <<endl;
        cout << "��ѡ�����ڣ�" <<endl;
        int choice_date;    // ��¼ѡ�������
        cin >> choice_date;
        if (cin.fail()) {  // �������Ͳ�ƥ��
            cin.clear();  // �������״̬
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // ��ջ�����
            cout <<endl << "�������" <<endl <<endl;
            goto select_date;
        } 
        
        /* ���û�ѡ����֮ǰ��������ڣ��򴴽�֦�ɽڵ㣬���򴴽����ڵ� */
        switch(choice_date) {
            case 1:
                message->pt->right = new Node();
                message->pt->right->date = message->pt->date;
                message->pt = message->pt->right;
                goto claim_sort_choice;
                break;
            case 2:
                message->pt_same->last = new Node();
                message->pt = message->pt_same->last;
                message->pt_same = message->pt_same->last;
                goto claim_input;
                break;
            default:
                cout <<endl << "û�д�ѡ����������룡" <<endl <<endl;
                goto if_continue;
                break;
        }
    }
    else if(select_num != 2) {
        cout <<endl << "�������" <<endl <<endl;
        goto if_continue;
    }
}