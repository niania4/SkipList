#pragma once
#include <iostream>
#include <mutex>
#include <fstream>
#include "math.h"
#define STORE_FILE "../store/dumpFile.txt"
#define newNode(n) (Node<K,V>*)malloc(sizeof(Node<K,V>)+(n-1)*sizeof(Node<K,V>*))

using namespace std;

mutex mtx;

template<typename K,typename V>
class Node//跳表结点
{
    
public:
    K key;
    V value;
    Node *next[1];//此处采用柔性数组
};

template<typename K,typename V>
class SkipList
{
private:
    int curHeight,maxHeight,len;
    Node<K,V> *head;

    ofstream fileWriter;
    ifstream fileReader;


public:
    SkipList(int maxHeight);//跳表创建，参数指定最大高度
    ~SkipList();
    bool insertElement(K key,V value);//插入元素
    bool deleteElement(K key);//删除元素
    Node<K,V>* searchElement(K key);//查找元素
    void displayList();//打印跳表
    void destoryList();//跳表销毁
    int randomHeight();//抛硬币，产生插入的随机高度
    Node<K,V>* createNode(K key,V value,int n);//创建结点
    int height();//跳表高度
    int size();//结点个数
    void dumpFile();
    void loadFile();
};




template<typename K,typename V>
SkipList<K,V>::SkipList(int maxHeight):curHeight(0),len(0),maxHeight(maxHeight)
{
    this->head=createNode(0,0,maxHeight);
    srand(time(NULL));    
}

template<typename K,typename V>
SkipList<K,V>::~SkipList()
{
    destoryList();
}

template<typename K,typename V>
Node<K,V>* SkipList<K,V>::searchElement(K key)
{
    //从高层向底层遍历，每一层都可能搜到，若搜不到，则该元素不在跳表中
    Node<K,V> *p,*q;
    p=this->head;
    for(int i=this->curHeight;i>=0;i--)
    {
        for(q=p->next[i];nullptr!=q && q->key<key;p=q,q=p->next[i])
            continue;
        if(nullptr!=q && q->key==key)
            return q;
    }
    return nullptr;   
}

template<typename K,typename V>
void SkipList<K,V>::destoryList()
{
    if(nullptr!=head)
    {
        Node<K,V> *p,*q;
        p=this->head;
        Node<K,V> *backup;
        for(q=p->next[0];nullptr!=q;q=backup)//先释放非头结点，再释放头结点
        {
            backup=q->next[0];
            free(q);
        }
        free(this->head);
        this->head=nullptr;
        this->curHeight=0;
        this->len=0;
    }
}

template<typename K,typename V>
int SkipList<K,V>::randomHeight()
{
    int i;
    for(i=1;rand()%2;i++)
        continue;
    return min(i,maxHeight);//随机生成的高度不能超过最大高度
}

template<typename K,typename V>
bool SkipList<K,V>::insertElement(K key,V value)
{
    //1.从高层向底层遍历找到待插入的位置（前继），备份到backup
    //2.产生插入的随机高度，若高度大于当前高度，则补充backup
    //3.根据backup执行插入
    mtx.lock();//信号量P操作
    Node<K,V>* backup[maxHeight];
    Node<K,V> *p,*q;
    int insertHeight;
    p=this->head;
    for(int i=this->curHeight-1;i>=0;i--)
    {
        for(q=p->next[i];nullptr!=q && q->key<key;p=q,q=p->next[i])
            continue;
        backup[i]=p;
    }

    if(nullptr!=q && q->key==key)//该元素已经存在，不需要插入
    {
        mtx.unlock();//信号量V操作
        return false;
    }
        
    insertHeight=randomHeight();
    for(int i=insertHeight-1;i>=this->curHeight;i--)
        backup[i]=this->head;
    this->curHeight=max(this->curHeight,insertHeight);//高度更新

    q=createNode(key,value,this->curHeight);
    if(nullptr==q)
        return false;
    for(int i=insertHeight-1;i>=0;i--)
    {
        q->next[i]=backup[i]->next[i];
        backup[i]->next[i]=q;
    }
    this->len++;
    mtx.unlock();
    return true;
}


template<typename K,typename V>
bool SkipList<K,V>::deleteElement(K key)
{
    //1.从高层向底层遍历，找到待删除结点的前继，备份到buckup
    //2.根据backup执行删除，注意在每一层删除之后要判断当前层是否为空，若是，height--
    mtx.lock();
    Node<K,V>* backup[maxHeight];
    Node<K,V> *p,*q;
    p=this->head;
    for(int i=this->curHeight-1;i>=0;i--)
    {
        for(q=p->next[i];nullptr!=q && q->key<key;p=q,q=p->next[i])
            continue;
        backup[i]=p;
    }
    if(nullptr==q || q->key!=key)//没有找到该元素
    {
        mtx.unlock();
        return false;
    }
        
    for(int i=this->curHeight-1;i>=0;i--)
    {
        if(backup[i]->next[i]==q)
            backup[i]->next[i]=q->next[i];
        if(head->next[i]==nullptr)
            this->curHeight--;
    }
    free(q);
    this->len--;
    mtx.unlock();
    return true;
}

template<typename K,typename V>
Node<K,V>* SkipList<K,V>::createNode(K key,V value,int n)
{
    Node<K,V> *node=newNode(n);
    if(nullptr==node)
        return nullptr;
    node->key=key;
    node->value=value;
    for(int i=0;i<n;i++)//指针一定要初始化，很重要！！！
        node->next[i]=nullptr;
    return node;
}

template<typename K,typename V>
void SkipList<K,V>::displayList()
{
    for(int i=this->curHeight;i>=0;i--)
    {
        for(Node<K,V>* p=this->head->next[i];nullptr!=p;p=p->next[i])
            cout<<p->key<<" ";
        cout<<endl;
    }
}

template<typename K,typename V>
int SkipList<K,V>::height()
{
    return this->curHeight;
}

template<typename K,typename V>
int SkipList<K,V>::size()
{
    return this->len;
}

template<typename K,typename V>
void SkipList<K,V>::dumpFile()
{
    cout << "dump_file-----------------" << endl;
    fileWriter.open(STORE_FILE);
    if(!fileWriter.is_open())
    {
        cout<<"error when open file!"<<endl;
        return;
    }
    for(Node<K,V> *node=head->next[0];nullptr!=node;node=node->next[0])
    {
        fileWriter<<node->key<<":"<<node->value<<endl;
        cout<<node->key<<":"<<node->value<<endl;
    }
    fileWriter.flush();
    fileWriter.close();
}
