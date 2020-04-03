#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
using namespace std;

const int base=10000;//ÿ����Ԫ��һ���������0-9999
const int power=4;   //λ��

class big
{
    vector<int> v;
    bool flag;   //����λ��Ϊ1��������0�Ǹ���
public:

    //�޲ι���
    big(){
        v.push_back(0);
        flag=1;//Ĭ��0Ϊ����
    }

    //�вι���
    big(std::string& s){
        int len,i,w,t;
        len=s.size();
        if(s[0]=='-'){//�����ĸ�ֵ
            flag=0;
            reverse(s.begin()+1,s.end());//����洢��v[0]Ϊ����λ
            for(i=1,t=-1;i<len;i++){
                if((i-1)%power==0){//¼���ĸ����ֺ�ʼ¼����һλ
                    v.push_back(0);
                    w=1;
                    t++;
                }
                v[t]+=w*(s[i]-'0');
                w*=10;//wΪÿһλȨ��
            }
        }
        else{//�����ĸ�ֵ
            flag=1;
            reverse(s.begin(),s.end());
            for(i=0,t=-1;i<len;i++){
                if(i%power==0){
                    v.push_back(0);
                    w=1;
                    t++;
                }
            v[t]+=w*(s[i]-'0');
            w*=10;
            }
        }
    }

    //�������
    void print(){
        if(v[0]==base) printf("ERROR");
        else{
            if(flag==0) cout<<"-";//���𰸷���λΪ�����������һ������
            int n=v.size()-1;//�����������β�������λ����ʼ
            cout<<v[n];
            for(n--;n>=0;n--){
                printf("%0*d",power,v[n]);//�����λ�⣬ÿһλȷ��λ����power�Ǻϣ�������0��
            }
        }
        cout<<endl;
    }

    //>=�ж�
    bool operator >= (const big &x){
        int i=v.size(),j=x.v.size(),n;
        if(flag==1&&x.flag==0) return 1;//����λ��ͬ��ֱ���ж�
        if(flag==0&&x.flag==1) return 0;
        if(flag==1&&x.flag==1){//��Ϊ����ʱ
            if(i<j) return 0;
            if(i>j) return 1;
            for(n=i-1;n>=0;n--){
                if(v[n]<x.v[n]) return 0;
                if(v[n]>x.v[n]) return 1;
            }
            return 1;//���û�Ƚϳ��������Ϊ���
        }
        if(i<j) return 0;//��Ϊ����ʱ
        if(i>j) return 1;
        for(n=i-1;n>=0;n--){
            if(v[n]>x.v[n]) return 0;
            if(v[n]<x.v[n]) return 1;
        }
        return 1;
    }

    //�ȽϾ���ֵ��С�ĺ���
    bool absolute_larger (const big &y,const big &x)
    {//���Է���λ��ֻ�Ƚ�vector
        int i=y.v.size(),j=x.v.size(),n;
        if(i<j) return 0;
        if(i>j) return 1;
        for(n=i-1;n>=0;n--){
            if(y.v[n]<x.v[n]) return 0;
            if(y.v[n]>x.v[n]) return 1;
        }
        return 1;
    }

    //��������
    big & operator ++ (int k){
        int i,n=v.size(),m;
        //����ʱ
        if(flag==0){
            v[0]--;
            for(m=0;m<n-1;m++){
                if(v[m]<0){//��λ����
                    v[m]+=base;
                    v[m+1]--;
                }
            }
            while(v[m]==0&&m>=0){//����λ����ɾȥ
                v.erase(v.end()-1);
                m--;
            }
            if(m==-1){//�������Ϊ�㣬��ȫ����ɾ�����ٸ�ֵ0
                flag=1;
                v.push_back(0);
            }
            return *this;
        }
        //����ʱ
        v[0]++;
        for(i=0;v[i]==base&&i<n-1;i++){//��λ����
            v[i+1]++;
            v[i]=0;
        }
        if(v[n-1]==base){//���һλ�Ľ�λ��Ҫ���·���ռ䣬��������
            v[n-1]=0;
            v.push_back(1);
        }
        return *this;
    }

    //�ӷ�����
    big operator + (const big &x){
        int i=v.size(),j=x.v.size(),n,m;
        big ans;
        //���ʱ
        if(flag!=x.flag){
            if(absolute_larger(*this,x)){//��this����ֵ>=x��ʱ
                ans=*this;//��this-x���㣬�𰸷��������ֵ�ϴ������this��һ�£����ù�flag
                for(n=0;n<j;n++){
                    ans.v[n]-=x.v[n];
                }
                for(m=0;m<i-1;m++){
                    if(ans.v[m]<0){
                        ans.v[m]+=base;
                        ans.v[m+1]--;
                    }
                }
                while(ans.v[m]==0&&m>0){
                    ans.v.erase(ans.v.end()-1);
                    m--;
                }
                if(ans.v.size()==1&&ans.v[0]==0) ans.flag=1;//�����Ϊ0�������¸�ֵ����λ
                return ans;
            }
            else{//��this����ֵ��xСʱ
                ans=x;//��x-this���㣬�𰸷��������ֵ�ϴ������x��һ�£����ù�flag
                for(n=0;n<i;n++){
                    ans.v[n]-=v[n];
                }
                for(m=0;m<j-1;m++){
                    if(ans.v[m]<0){
                        ans.v[m]+=base;
                        ans.v[m+1]--;
                    }
                }
                while(ans.v[m]==0&&m>0){
                    ans.v.erase(ans.v.end()-1);
                    m--;
                }
                return ans;//������������ֵ�Ǵ��ڣ������Ǵ��ڵ��ڹ�ϵ�����ؿ���0�����
            }
        }
        //ͬ��ʱ
        ans.flag=flag;
        if(i>=j){//����ֵ�ϴ������Ϊ����������λ����
            ans=*this;
            for(n=0;n<j;n++){
                ans.v[n]+=x.v[n];
            }
            for(m=0;m<i-1;m++){//��λ
                ans.v[m+1]+=ans.v[m]/base;
                ans.v[m]%=base;
            }
            while(ans.v[i-1]>=base){//���һλ�Ľ�λ��Ҫ���·���ռ䣬��������
                ans.v.push_back(ans.v[i-1]/base);
                ans.v[i-1]%=base;
                i++;
            }
        }
        if(i<j){
            ans=x;
            for(n=0;n<i;n++){
                ans.v[n]+=v[n];
            }
            for(m=0;m<j-1;m++){//��λ
                ans.v[m+1]+=ans.v[m]/base;
                ans.v[m]%=base;
            }
            while(ans.v[j-1]>=base){//���һλ�Ľ�λ��Ҫ���·���ռ䣬��������
                ans.v.push_back(ans.v[j-1]/base);
                ans.v[j-1]%=base;
                j++;
            }
        }
        return ans;
    }

    //��������
    big operator - (const big &x){
        big ans;
        int i=v.size(),j=x.v.size(),n,m;
        //���ʱ
        if(flag!=x.flag){//���Ų�һ��ʱ����������ֵ��ӣ�����λ�뱻����һ��
            if(absolute_larger(*this,x)){
                ans=*this;//����ֵ�ϴ������this��Ϊ����������λ���ͳһ�޸�
                for(n=0;n<j;n++){
                ans.v[n]+=x.v[n];
                }
                for(m=0;m<i-1;m++){//��λ
                    ans.v[m+1]+=ans.v[m]/base;
                    ans.v[m]%=base;
                }
                while(ans.v[i-1]>=base){//���һλ�Ľ�λ��Ҫ���·���ռ䣬��������
                    ans.v.push_back(ans.v[i-1]/base);
                    ans.v[i-1]%=base;
                    i++;
                }
            }
            else{
                ans=x;//����ֵ�ϴ������x��Ϊ����������λ���ͳһ�޸�
                for(n=0;n<i;n++){
                    ans.v[n]+=v[n];
                }
                for(m=0;m<j-1;m++){//��λ
                    ans.v[m+1]+=ans.v[m]/base;
                    ans.v[m]%=base;
                }
                while(ans.v[j-1]>=base){//���һλ�Ľ�λ��Ҫ���·���ռ䣬��������
                    ans.v.push_back(ans.v[j-1]/base);
                    ans.v[j-1]%=base;
                    j++;
                }
            }
            ans.flag=flag;//���۾���ֵ��С���𰸷���λ�뱻����һ��
            return ans;
        }
        //ͬ��ʱ
        if(absolute_larger(*this,x)){//ѡȡ����ֵ�ϴ��Ϊ������
            ans=*this;//�������µı�����һ��
            for(n=0;n<j;n++){
                ans.v[n]-=x.v[n];
            }
            for(m=0;m<i-1;m++){
                if(ans.v[m]<0){//��λ
                    ans.v[m]+=base;
                    ans.v[m+1]--;
                }
            }
            while(ans.v[m]==0&&m>0){//�����һλ��ʼ����Ƿ�Ϊ�㣬����λ��ɾȥ�������v[0]�Ƿ�Ϊ0��
                ans.v.erase(ans.v.end()-1);
                m--;
            }
        }
        else{
            ans=x;//�ݶ��������µı�����һ�£�������˳����ԭ���෴������ı�
            for(n=0;n<i;n++){
                ans.v[n]-=v[n];
            }
            for(m=0;m<j-1;m++){
                if(ans.v[m]<0){
                    ans.v[m]+=base;
                    ans.v[m+1]--;
                }
            }
            while(ans.v[m]==0&&m>0){
                ans.v.erase(ans.v.end()-1);
                m--;
            }
            ans.flag=!ans.flag;//��ԭ�����˳��ͬ���ı����λ
        }
        return ans;
    }

    //��������
    big operator / (const big &x){
        big ans,beichushu=*this,chushu=x;
        if(x.v.size()==1&&x.v[0]==0){
            ans.v[0]=base;//����Ϊ�㣬���벻�Ϸ����趨Ϊbase��������������error
            return ans;
        }
        beichushu.flag=1;//����������ֵ������𰸺�����ӷ���λ
        chushu.flag=1;
        while(beichushu>=chushu){
            ans++;//��¼����Ĵ���
            beichushu=beichushu-chushu;
        }
        if(flag!=x.flag) ans.flag=0;//���Ų�ͬ���Ϊ��
        else ans.flag=1;//������ͬ���Ϊ��
        if(ans.v.size()==1&&ans.v[0]==0) ans.flag=1;//�����Ϊ0������λ��1
        return ans;
    }

    //ȡģ����
    big operator % (const big &x){
        big ans,chushu=x;
        if(x.v.size()==1&&x.v[0]==0){
            ans.v[0]=base;//����Ϊ�㣬���벻�Ϸ����趨Ϊbase��������������error
            return ans;
        }
        ans=*this;
        ans.flag=1;//����������ֵ������𰸺�����ӷ���
        chushu.flag=1;
        while(ans>=chushu) ans=ans-chushu;
        ans.flag=flag;//�����뱻����һ��
        if(ans.v.size()==1&&ans.v[0]==0) ans.flag=1;//�����Ϊ0������λ��1
        return ans;
    }

    //�˷�����
    big operator * (const big &x){
        big ans;
        int m=v.size(),n=x.v.size(),i,j;
        ans.v.insert(ans.v.end(),m+n,0);//��ʹ�𰸵�λ��Ϊ��������λ����+1��ȫ������
        for(j=0;j<n;j++){
            for(i=0;i<m;i++){//�õڶ�������ÿһλ���һ������ˣ��ڼӵ�ans�Ķ�Ӧλ��
                ans.v[j+i]+=v[i]*x.v[j];
                ans.v[j+i+1]+=ans.v[j+i]/base;//��λ��̫�ߣ����ۿ��ܳ���int��Χ����ÿ�μ�������λ
                ans.v[j+i]%=base;
            }
        }
        n=ans.v.size()-1;
        while(ans.v[n]==0&&n>0){//�����һλ��ʼ����Ƿ�Ϊ�㣬����λ��ɾȥ�������v[0]�Ƿ�Ϊ0��
            ans.v.erase(ans.v.end()-1);
            n--;
        }
        if(flag!=x.flag) ans.flag=0;//���Ų�ͬ���Ϊ��
        else ans.flag=1;//������ͬ���Ϊ��
        if(ans.v.size()==1&&ans.v[0]==0) ans.flag=1;//�����Ϊ0������λ��1
        return ans;
    }

};




int main()
{
    big v1,v2,ans;
    string s1,s2;
    cin>>s1;
    cin>>s2;
    v1=big(s1);
    v2=big(s2);
    ans=v1+v2;
    ans.print();
    ans=v1-v2;
    ans.print();
    ans=v1*v2;
    ans.print();
    ans=v1/v2;
    ans.print();
    ans=v1%v2;
    ans.print();
    return 0;
}

