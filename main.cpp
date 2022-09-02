#include "SkipList.h"

int main()
{
    SkipList<int,int> sl(16);
    for(int i=0;i<40;i++)
        sl.insertElement(i,i);
    cout<<"*********************after insert**************"<<endl;
    sl.displayList();
    cout<<"size:"<<sl.size()<<endl;
    cout<<"height:"<<sl.height()<<endl;

    for(int i=5;i<=15;i++)
        sl.deleteElement(i);
    cout<<"********************after delete****************"<<endl;
    sl.displayList();
    cout<<"size:"<<sl.size()<<endl;
    cout<<"height:"<<sl.height()<<endl;

    for(int i=1;i<=20;i++)
    {
        if(nullptr!=sl.searchElement(i))
            cout<<"\""<<i<<"\"find!"<<endl;
        else   
            cout<<"\""<<i<<"\"not find!"<<endl;
    }
    
    sl.dumpFile();
    return 0;
}