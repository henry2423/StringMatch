//
//  main.cpp
//  StringMatch
//
//  Created by Henry on 14/11/2016.
//  Copyright © 2016 henry. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sys/time.h>

using namespace std;

class success_set //比對成功的紀錄
{
public:
    friend class success_set_chain;
    success_set()
    {
        
    }
    success_set(int line, int pos, char *pat, char *text, success_set *next=0)
    {
        this->line = line;
        this->pos = pos;
        for(int i=0; pat[i]!='\0'; i++)
        {
            this->pat[i] = pat[i];
        }
        for(int i=0; text[i]!='\0'; i++)
        {
            this->text[i] = text[i];
        }
        this->link = next;
    }
    friend ostream& operator<< (ostream& os, success_set *a);     //最後結果輸出用此
    
private:
    int line;   //第幾行
    int pos;    //第幾個字
    char pat[2000];     //keywords
    char text[2000];    //serach words
    
public:
    success_set *link;
};


ostream& operator<< (ostream& os, success_set *a)
{
    if(a!=NULL)
    {
        while(a)
        {
            os << a->line << " " << a->pos << " " << "\"";
            for(int i=0; a->pat[i]!='\0'; i++)
            {
                os << a->pat[i];
            }
            os << "\"" << " " << "\"";
            
            for(int i=0; a->text[i]!='\0'; i++)
            {
                os << a->text[i];
            }
            os << "\"" << endl;
            a = a->link;
        }
    }
    else
    {
        os << "No match string in this test." << endl;
    }
    
    
    return os;
}




int main(int argc, const char * argv[]) {
    
    struct timeval t_val;
    gettimeofday(&t_val, NULL);
    /*---要計算的程式效率區域---*/

    
    char text[2000] = {0};    //text = search words
    char pat[2000] = {0};    //pat = keywords
    success_set *first = NULL;           //link-list first
    success_set *current = first;         //link-list control
    int line = 1;   //第幾行
    int sizeof_pat = 0;     //運算patter的長度
    int sizeof_text = 0;    //運算text的長度
    

    if(argc<=2)     //確定會參數設定正確
    {
        cout<<"File Setting is not complete."<<endl;
        return 1;
    }
    
    fstream pat_file;
    fstream text_file;
    pat_file.open(argv[1],ios::in);
    text_file.open(argv[2],ios::in);
    
    //確定有讀到檔案
    if(!pat_file)
    {
        cout<<"Can't open the file"<<endl;
        return 1;
    }
    if(!text_file)
    {
        cout<<"Can't open the file"<<endl;
        return 1;
    }
    
    
    //先從第一個pat開始search 把所以組text搜完再換下一組pat
    while(!pat_file.eof())
    {
        pat_file.getline(pat, sizeof(pat));
        for(int i=0; pat[i]!='\0'; i++) //計算pat的長度
        {
            sizeof_pat = i+1;
        }
        
        while(!text_file.eof())
        {
            text_file.getline(text, sizeof(text));
            for(int i=0; text[i]!='\0'; i++) //計算text的長度
            {
                sizeof_text = i+1;
            }
            

            //實作Brute-Force
            for(int i=0; i<=(sizeof_text-sizeof_pat); i++)
            {
                int j=0;
                int k=i;
                while(tolower(text[k]) == tolower(pat[j]))
                {
                    k++;
                    j++;
                    if( j == sizeof_pat ) //當pat比到最後一個一樣代表成功找到
                    {
                        if(!first)  //沒有first先放進去
                        {
                            first = new success_set(line, i+1, pat, text);
                            current = first;
                        }
                        else
                        {
                            current->link = new success_set(line, i+1, pat, text); //從current直接插入
                            current = current->link;
                        }
                        break;
                    }
                }
                
            }

            line++;
        }
        
        
        text_file.clear();  // 重置 eof
        text_file.seekg(0);  // get 指標移至檔案首
        line = 1;
    }
    


    
    
    cout << first;
    
    pat_file.close();
    text_file.close();
    
    
    struct timeval t_val_end;
    gettimeofday(&t_val_end, NULL);
    struct timeval t_result;
    timersub(&t_val_end, &t_val, &t_result);
    double consume = t_result.tv_sec*1000 + (1.0 * t_result.tv_usec)/1000;
    printf("%f ms \n", consume);
    
    return 0;
}
