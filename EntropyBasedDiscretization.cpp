#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;
//����һ�����ݼ���entropy
double entropy( double S[][2],int from,int to,int class_types ){
    //ֻ��һ��Ԫ�أ�entropy=0
    if( from>=to )
        return 0.0;
    double ent=0.0;
    //����ÿ����ĸ���
    int N=to-from+1;
    int* c_count=new int[class_types];
    memset(c_count,0,class_types*sizeof(int));
    for( int i=from;i<=to;i++ ){
        c_count[(int)S[i][1]]+=1;
    }
    double pro_i=0.0;
    //����entropy, ��2Ϊ��
    for( int i=0;i<class_types;i++ ){
        if( c_count[i]>0 ){
            pro_i=(double)c_count[i]/N;
            ent-=(pro_i*log2(pro_i));
        }
    }
    delete[] c_count;
    return ent;
}
//����һ�����ֵ�entropy
double entropyWithT( double S[][2],int from,int to,int T,int class_types){
    double ent1=entropy(S,from,T,class_types);
    double ent2=entropy(S,T+1,to,class_types);
    int N=to-from+1;
    double entST=(double)(T-from+1)/N*ent1+(double)(to-(T+1)+1)/N*ent2;
    //���T==to�����ص���entropy(S)
    return entST;
}
//������ֻ��ֵ���ѻ��ֵ�
int bestCutPoint( double S[][2],int from,int to,int class_types ){
    int T=-1;double min_entST;double entST;
    for( int i=from;i<=to;i++ ){
        //�ҵ���ֵͬ���ұ߽�
        while( i<to && S[i]==S[i+1] )
            i++;
        entST=entropyWithT(S,from,to,i,class_types);
        //minimize Entropy(S,T), means maximize information gain
        if( T==-1 ){
            T=i;min_entST=entST;
        }else if( entST<min_entST ){
            T=i;min_entST=entST;
        }
    }
    return T;
}
//���㲻ͬ�������Ŀ
int uniqueClass( double S[][2],int from,int to,int class_types ){
    if( from==to )
        return 1;
    int* c_count=new int[class_types];
    memset(c_count,0,class_types*sizeof(int));
    for( int i=from;i<=to;i++ ){
        c_count[(int)S[i][1]]+=1;
    }
    int uniqueClass=0;
    for( int i=0;i<class_types;i++ ){
        if( c_count[i]!=0 )
            uniqueClass+=1;
    }
    return uniqueClass;
}
//Recursive Minimal Entropy Partioning
void partition(double S[][2],int from,int to,vector<int> &Ts,int class_types){
    //ֻ��һ��Ԫ�أ�û�л��ֵı�Ҫ
    if( from>=to )
        return ;
    int N=to-from+1;
    int T=bestCutPoint(S,from,to,class_types);
    if( T==to )//��ѻ��ֵ���ĩβԪ�أ��൱��û�л���
        return ;
    //���Minimal Description Length Principle
    double gainST=entropy(S,from,to,class_types)-entropyWithT(S,from,to,T,class_types);
    //ͳ��ÿ������������Ŀ,��entropy
    int k=uniqueClass(S,from,to,class_types);
    int k1=uniqueClass(S,from,T,class_types);
    int k2=uniqueClass(S,T+1,to,class_types);
    double entS=entropy(S,from,to,class_types);
    double entS1=entropy(S,from,T,class_types);
    double entS2=entropy(S,T+1,to,class_types);
    //�����½�
    double delta=log2(pow(3,k)-2)-(k*entS-k1*entS1-k2*entS2);
    double threshold=log2(N-1)/N+delta/N;
    if( gainST<= threshold ){
        return ;
    }else{
        //�ָ���¼
        Ts.push_back(T);
        //�ݹ鴦������������
        partition(S,from,T,Ts,class_types);
        partition(S,T+1,to,Ts,class_types);
    }
}
//����Entropy��Minimal Description Length����ɢ������
int discretization(double S[][2],int from,int to,int class_types){
    vector<int> Ts;
    //���û���
    partition(S,from,to,Ts,class_types);
    //���ֵ�����
    sort(Ts.begin(),Ts.end());
    cout<< "-----���ֵ���" << Ts.size() << "��" <<endl;
    for( int i=0;i<Ts.size();i++ ){
        cout<< Ts[i] <<endl;
    }
    cout<< "-----��ɢ�������"<<endl;
    for( int i=0;i<Ts.size();i++ ){
        int po_left=(i==0)?from:Ts[i-1]+1;
        int po_right=Ts[i];
        for( int j=po_left;j<=po_right;j++ ){
            //��ɢ�����
            cout<< i <<endl;
        }
    }
    for( int i=Ts[Ts.size()-1]+1;i<=to;i++ ){
        //��ɢ�����
        cout<< Ts.size() <<endl;
    }
}
int main()
{
    //double S[7][2]={{1,0},{2,0},{3,1},{4,1},{5,2},{6,2},{7,2}};
    //double S[7][2]={{1,0},{2,0},{3,1},{4,1},{5,2},{6,2},{7,2}};
    double S[15][2]={{1,0},{2,0},{3,0},{4,0},{5,1},{6,1},{7,1},{8,1},{9,1},{10,2},{11,2},{12,2},{13,1},{14,2},{15,2}};
    //double entS=entropy(S,0,6,2);
    //cout<< entS <<endl;
    //cout<< infoGain(S,0,6,2,2,entS) <<endl;
    //cout<< bestCutPoint(S,0,6,2) <<endl;

    //vector<int> Ts;
    //partition(S,0,6,Ts,3);
    //cout<< "-----���ֵ���" << Ts.size() << "��" <<endl;
    //for( int i=0;i<Ts.size();i++ ){
    //    cout<< Ts[i] <<endl;
    //}

    discretization(S,0,14,3);
    return 0;
}
