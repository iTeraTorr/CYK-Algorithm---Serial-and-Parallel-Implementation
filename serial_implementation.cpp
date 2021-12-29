#include<bits/stdc++.h>
#include<omp.h>
using namespace std;
#define rep(i,n) for(int i=0;i<n;i++)
#define pb push_back
void getVarTerm(map<string,vector<vector<string>>> &R,char c,set<string> &se){
    string s="";
    s+=c;
    for(auto rule:R){
        for(auto rhs:rule.second){
            if(rhs.size()==1 && rhs[0]==s)
                se.insert(rule.first);
        }
    }
}
void getVarVar(map<string,vector<vector<string>>> &R,vector<string> s,set<string> &se){
    for(auto rule:R){
        for(auto rhs:rule.second){
            if(rhs==s)
                se.insert(rule.first);
        }
    }
}
pair<int,int> getInterval(int i,int j,int n){
    return {j,n+j-i-1};
}
pair<int,int> getTableIndex(int x,int y,int n){
    x++;
    y++;
    return {n+x-y-1,x-1};
}
bool doExist(set<string> &block, string s){
    for(auto x:block){
        if(x==s)
        return true;
    }
    return false;
}
int main(){
    //This is CYK algorithm that supports only Chomsky Normal Form
    //Start symbol cannot be in rhs of any production rule
    //eps can only be derived by the start symbol
    set<string> term;
    set<string> vars;

    // cout<<"Enter the set of variable symbols followed by END..\n> ";
    while(1){
        string in;
        cin>>in;
        if(in=="END"){
            break;
        }else{
            vars.insert(in);
        }
    }
    string start;
    // cout<<"Specify the start variable: ";
    cin>>start;
    // cout<<"Enter the set of terminal symbols followed by END..\n> ";
    while(1){
        string in;
        cin>>in;
        if(in=="END"){
            break;
        }else{
            term.insert(in);
        }
    }

    map<string,vector<vector<string>>> R;
    int Rcount;
    // cout<<"Format for entering rules..\n";
    // cout<<"\nIf type A->BC\nEnter Rule: A BC\n";
    // cout<<"\nIf type A->c\n\nEnter Rule: A c\n";
    // cout<<"\nIf type A->eps\n\nEnter Rule: A eps\n\n";
    // cout<<"Enter the number of productions: ";
    cin>>Rcount;
    // cout<<"\nStart entering the production rules: \n\n";

    rep(i,Rcount){
        string s1,s2="",s3="",right;
        vector<string> v;
        cin>>s1>>right;
        if(right.length()==1){
            v.pb(right);
        }else if(right.length()==2){
            s2+=right[0];
            s3+=right[1];
            v.pb(s2);
            v.pb(s3);
        }else if(right.length()==3 && right=="eps"){

        }else{
            cout<<"Error!!\n";
            exit(1);
        }
        R[s1].pb(v);
    }
    
    string s;
    // cout<<"Enter a string to check its membership: ";
    cin>>s;
    cout<<"String to check membership is "<<s<<"\n";
    int n=s.length();
    vector<vector<set<string>>> table(n,vector<set<string>>(n));
    double startTime=omp_get_wtime();

    for(int j=0;j<n;j++){
        getVarTerm(R,s[j],table[n-1][j]);
    }
    for(int i=n-2;i>=0;i--){
        for(int j=0;j<=i;j++){
            pair<int,int> interval=getInterval(i,j,n);
            int diff=interval.second-interval.first;
            set<vector<string>> cross;
            rep(k,diff){
                pair<int,int> a= getTableIndex(interval.first,interval.first+k,n);
                pair<int,int> b= getTableIndex(interval.first+k+1,interval.second,n);
                   
                for(auto s1:table[a.first][a.second]){
                    for(auto s2:table[b.first][b.second]){
                        vector<string> temp={s1,s2};
                        cross.insert(temp);
                    }
                }
            }
            for(auto x:cross)
                getVarVar(R,x,table[i][j]);
        }
    }

    double end=omp_get_wtime();
    cout<<"Serial Time taken: "<<end-startTime<<"\n";

    if(doExist(table[0][0],start)){
        cout<<"Passed!!\n";
    }else{
        cout<<"\nFail!!\n";
    }
    // cout<<"\nTable Below: \n";
    // for(int i=0;i<n;i++){
    //     for(int j=0;j<=i;j++){
    //         cout<<"{";
    //         for(auto x:table[i][j]){
    //             cout<<x<<", ";
    //         }
    //         cout<<"}        ";
    //     }
    //     cout<<"\n";
    // }
    return 0;
}