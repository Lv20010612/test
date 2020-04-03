#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
using namespace std;

const int base=10000;//每个单元存一个万进制数0-9999
const int power=4;   //位数

class big
{
    vector<int> v;
    bool flag;   //符号位，为1是正数，0是负数
public:

    //无参构造
    big(){
        v.push_back(0);
        flag=1;//默认0为正数
    }

    //有参构造
    big(std::string& s){
        int len,i,w,t;
        len=s.size();
        if(s[0]=='-'){//负数的赋值
            flag=0;
            reverse(s.begin()+1,s.end());//逆序存储，v[0]为低四位
            for(i=1,t=-1;i<len;i++){
                if((i-1)%power==0){//录入四个数字后开始录入下一位
                    v.push_back(0);
                    w=1;
                    t++;
                }
                v[t]+=w*(s[i]-'0');
                w*=10;//w为每一位权重
            }
        }
        else{//正数的赋值
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

    //输出函数
    void print(){
        if(v[0]==base) printf("ERROR");
        else{
            if(flag==0) cout<<"-";//若答案符号位为负，则先输出一个负号
            int n=v.size()-1;//逆序输出，从尾部（最高位）开始
            cout<<v[n];
            for(n--;n>=0;n--){
                printf("%0*d",power,v[n]);//除最高位外，每一位确保位数与power吻合，不足用0补
            }
        }
        cout<<endl;
    }

    //>=判断
    bool operator >= (const big &x){
        int i=v.size(),j=x.v.size(),n;
        if(flag==1&&x.flag==0) return 1;//符号位不同可直接判断
        if(flag==0&&x.flag==1) return 0;
        if(flag==1&&x.flag==1){//均为正数时
            if(i<j) return 0;
            if(i>j) return 1;
            for(n=i-1;n>=0;n--){
                if(v[n]<x.v[n]) return 0;
                if(v[n]>x.v[n]) return 1;
            }
            return 1;//最后还没比较出结果，则为相等
        }
        if(i<j) return 0;//均为负数时
        if(i>j) return 1;
        for(n=i-1;n>=0;n--){
            if(v[n]>x.v[n]) return 0;
            if(v[n]<x.v[n]) return 1;
        }
        return 1;
    }

    //比较绝对值大小的函数
    bool absolute_larger (const big &y,const big &x)
    {//忽略符号位，只比较vector
        int i=y.v.size(),j=x.v.size(),n;
        if(i<j) return 0;
        if(i>j) return 1;
        for(n=i-1;n>=0;n--){
            if(y.v[n]<x.v[n]) return 0;
            if(y.v[n]>x.v[n]) return 1;
        }
        return 1;
    }

    //自增运算
    big & operator ++ (int k){
        int i,n=v.size(),m;
        //负数时
        if(flag==0){
            v[0]--;
            for(m=0;m<n-1;m++){
                if(v[m]<0){//借位操作
                    v[m]+=base;
                    v[m+1]--;
                }
            }
            while(v[m]==0&&m>=0){//将高位的零删去
                v.erase(v.end()-1);
                m--;
            }
            if(m==-1){//若最后结果为零，则全部被删除，再赋值0
                flag=1;
                v.push_back(0);
            }
            return *this;
        }
        //正数时
        v[0]++;
        for(i=0;v[i]==base&&i<n-1;i++){//进位操作
            v[i+1]++;
            v[i]=0;
        }
        if(v[n-1]==base){//最后一位的进位需要重新分配空间，单独讨论
            v[n-1]=0;
            v.push_back(1);
        }
        return *this;
    }

    //加法运算
    big operator + (const big &x){
        int i=v.size(),j=x.v.size(),n,m;
        big ans;
        //异号时
        if(flag!=x.flag){
            if(absolute_larger(*this,x)){//当this绝对值>=x大时
                ans=*this;//用this-x计算，答案符号与绝对值较大的数（this）一致，不用管flag
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
                if(ans.v.size()==1&&ans.v[0]==0) ans.flag=1;//若结果为0，则重新赋值符号位
                return ans;
            }
            else{//当this绝对值比x小时
                ans=x;//用x-this计算，答案符号与绝对值较大的数（x）一致，不用管flag
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
                return ans;//这里两数绝对值是大于，而不是大于等于关系，不必考虑0的情况
            }
        }
        //同号时
        ans.flag=flag;
        if(i>=j){//绝对值较大的数作为基数，符号位不变
            ans=*this;
            for(n=0;n<j;n++){
                ans.v[n]+=x.v[n];
            }
            for(m=0;m<i-1;m++){//进位
                ans.v[m+1]+=ans.v[m]/base;
                ans.v[m]%=base;
            }
            while(ans.v[i-1]>=base){//最后一位的进位需要重新分配空间，单独讨论
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
            for(m=0;m<j-1;m++){//进位
                ans.v[m+1]+=ans.v[m]/base;
                ans.v[m]%=base;
            }
            while(ans.v[j-1]>=base){//最后一位的进位需要重新分配空间，单独讨论
                ans.v.push_back(ans.v[j-1]/base);
                ans.v[j-1]%=base;
                j++;
            }
        }
        return ans;
    }

    //减法运算
    big operator - (const big &x){
        big ans;
        int i=v.size(),j=x.v.size(),n,m;
        //异号时
        if(flag!=x.flag){//符号不一致时，两数绝对值相加，符号位与被减数一致
            if(absolute_larger(*this,x)){
                ans=*this;//绝对值较大的数（this）为基数，符号位最后统一修改
                for(n=0;n<j;n++){
                ans.v[n]+=x.v[n];
                }
                for(m=0;m<i-1;m++){//进位
                    ans.v[m+1]+=ans.v[m]/base;
                    ans.v[m]%=base;
                }
                while(ans.v[i-1]>=base){//最后一位的进位需要重新分配空间，单独讨论
                    ans.v.push_back(ans.v[i-1]/base);
                    ans.v[i-1]%=base;
                    i++;
                }
            }
            else{
                ans=x;//绝对值较大的数（x）为基数，符号位最后统一修改
                for(n=0;n<i;n++){
                    ans.v[n]+=v[n];
                }
                for(m=0;m<j-1;m++){//进位
                    ans.v[m+1]+=ans.v[m]/base;
                    ans.v[m]%=base;
                }
                while(ans.v[j-1]>=base){//最后一位的进位需要重新分配空间，单独讨论
                    ans.v.push_back(ans.v[j-1]/base);
                    ans.v[j-1]%=base;
                    j++;
                }
            }
            ans.flag=flag;//无论绝对值大小，答案符号位与被减数一致
            return ans;
        }
        //同号时
        if(absolute_larger(*this,x)){//选取绝对值较大的为被减数
            ans=*this;//符号与新的被减数一致
            for(n=0;n<j;n++){
                ans.v[n]-=x.v[n];
            }
            for(m=0;m<i-1;m++){
                if(ans.v[m]<0){//借位
                    ans.v[m]+=base;
                    ans.v[m+1]--;
                }
            }
            while(ans.v[m]==0&&m>0){//从最后一位开始检查是否为零，将高位零删去（不检查v[0]是否为0）
                ans.v.erase(ans.v.end()-1);
                m--;
            }
        }
        else{
            ans=x;//暂定符号与新的被减数一致，但由于顺序与原题相反，最后会改变
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
            ans.flag=!ans.flag;//与原数相减顺序不同，改变符号位
        }
        return ans;
    }

    //除法运算
    big operator / (const big &x){
        big ans,beichushu=*this,chushu=x;
        if(x.v.size()==1&&x.v[0]==0){
            ans.v[0]=base;//除数为零，输入不合法，设定为base后输出函数会输出error
            return ans;
        }
        beichushu.flag=1;//将两数绝对值，算出答案后再添加符号位
        chushu.flag=1;
        while(beichushu>=chushu){
            ans++;//记录相减的次数
            beichushu=beichushu-chushu;
        }
        if(flag!=x.flag) ans.flag=0;//符号不同结果为负
        else ans.flag=1;//符号相同结果为正
        if(ans.v.size()==1&&ans.v[0]==0) ans.flag=1;//若结果为0，符号位置1
        return ans;
    }

    //取模运算
    big operator % (const big &x){
        big ans,chushu=x;
        if(x.v.size()==1&&x.v[0]==0){
            ans.v[0]=base;//除数为零，输入不合法，设定为base后输出函数会输出error
            return ans;
        }
        ans=*this;
        ans.flag=1;//将两数绝对值，求出答案后再添加符号
        chushu.flag=1;
        while(ans>=chushu) ans=ans-chushu;
        ans.flag=flag;//符号与被除数一致
        if(ans.v.size()==1&&ans.v[0]==0) ans.flag=1;//若结果为0，符号位置1
        return ans;
    }

    //乘法运算
    big operator * (const big &x){
        big ans;
        int m=v.size(),n=x.v.size(),i,j;
        ans.v.insert(ans.v.end(),m+n,0);//先使答案的位数为两个乘数位数和+1，全部置零
        for(j=0;j<n;j++){
            for(i=0;i<m;i++){//用第二个数的每一位与第一个数相乘，在加到ans的对应位上
                ans.v[j+i]+=v[i]*x.v[j];
                ans.v[j+i+1]+=ans.v[j+i]/base;//若位数太高，积累可能超出int范围，故每次计算后均进位
                ans.v[j+i]%=base;
            }
        }
        n=ans.v.size()-1;
        while(ans.v[n]==0&&n>0){//从最后一位开始检查是否为零，将高位零删去（不检查v[0]是否为0）
            ans.v.erase(ans.v.end()-1);
            n--;
        }
        if(flag!=x.flag) ans.flag=0;//符号不同结果为负
        else ans.flag=1;//符号相同结果为正
        if(ans.v.size()==1&&ans.v[0]==0) ans.flag=1;//若结果为0，符号位置1
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

