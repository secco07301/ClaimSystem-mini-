/*
*   此文件用于构建报销系统的存储结点
*/

#include <iostream>
#include <limits>   //引入输入审查，防止用户的错误输入导致程序崩溃
using namespace std;

/* 节点 */
struct Node {   
    string date;    // 日期
    string claim_sort;  // 报销类型
    double per_money;   // 单次报销类型的金额
    Node *last; //下一结点
    Node *right;    //日期相同的结点

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

/* 链表 */
struct Link {
    Node *head;
    Node *pt;   // pt指向当前操作节点
    Node *pt_same;  // 该指针指向枝干节点

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

void input_message(Link *message) {  // 该函数用于实现读取用户输入信息构建链表
    claim_input:    // 回溯标签
    cout << "请输入日期：";
    if(message->pt->date == "0") {  // 当前节点数据为空则正常输入
        cin >> message->pt->date;
    }
    else {  // 若数据不为空则新建节点，根据日期比对决定新建主节点还是枝干节点
        string new_input_date;
        cin >> new_input_date;
        if(message->pt->date == new_input_date) {   // 新建枝干节点
            message->pt->right = new Node();
            message->pt->right->date = message->pt->date;
            message->pt = message->pt->right;
        }
        else {  // 新建主节点
            message->pt_same->last = new Node();
            message->pt = message->pt_same->last;
            message->pt_same = message->pt_same->last;
            message->pt->date = new_input_date;
        }
    }

    claim_sort_choice:
    cout << "1.停车费" <<endl;
    cout << "2.通行费" <<endl;
    cout << "3.加油费" <<endl;
    cout << "4.货拉拉" <<endl;
    cout << "5.五金" <<endl;
    cout << "6.其他" <<endl;
    cout << "请选择报销类型（输入数字）：";
    int select_num; // 用户选择的类型
    cin >> select_num;
    if (cin.fail()) {  // 输入类型不匹配
            cin.clear();  // 清除错误状态
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 清空缓冲区
        } 

    switch(select_num) {
        case 1: message->pt->claim_sort = "停车费"; break;
        case 2: message->pt->claim_sort = "通行费"; break;
        case 3: message->pt->claim_sort = "加油费"; break;
        case 4: message->pt->claim_sort = "货拉拉"; break;
        case 5: message->pt->claim_sort = "五金"; break;
        case 6:
        cout << "请输入报销类型:";
        cin >> message->pt->claim_sort; break;
        default:
            cout <<endl << "无此业务类型！" <<endl <<endl;
            goto claim_sort_choice; break;
    }

    input_money:
    cout << "请输入金额：";
    cin >> message->pt->per_money;
    if (cin.fail()) {  // 输入类型不匹配
            cin.clear();  // 清除错误状态
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 清空缓冲区
            cout <<endl << "输入错误！" <<endl <<endl;
            goto input_money;
        } 

    /* 输出录入的内容 */
    cout <<endl;
    cout << "日期：" << message->pt->date <<endl;
    cout << "报销类型：" << message->pt->claim_sort <<endl;
    cout << "金额:" << message->pt->per_money <<endl;

    if_continue: cout << "录入成功，是否继续录入？（输入数字即可）" <<endl;
    cout << "1.是           2.否" <<endl;
    cin >> select_num;  // select_num重用，判断用户的选择
    if (cin.fail()) {  // 输入类型不匹配
            cin.clear();  // 清除错误状态
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 清空缓冲区
        } 

    /* 判断用户是否需要继续输入 */
    if(select_num == 1) {

        select_date:
        cout << "1." << message->pt->date <<endl;
        cout << "2.其他" <<endl;
        cout << "请选择日期：" <<endl;
        int choice_date;    // 记录选择的日期
        cin >> choice_date;
        if (cin.fail()) {  // 输入类型不匹配
            cin.clear();  // 清除错误状态
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 清空缓冲区
            cout <<endl << "输入错误！" <<endl <<endl;
            goto select_date;
        } 
        
        /* 若用户选择了之前输入的日期，则创建枝干节点，否则创建主节点 */
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
                cout <<endl << "没有此选项，请重新输入！" <<endl <<endl;
                goto if_continue;
                break;
        }
    }
    else if(select_num != 2) {
        cout <<endl << "输入错误！" <<endl <<endl;
        goto if_continue;
    }
}