/* Apriori Algorithm */
#include<bits/stdc++.h>
using namespace std;

const int N = (int)1e5+11;

struct NODE{
	bool LF;
	int VAL;
	struct NODE *CHILD=NULL,*NEXT=NULL;
};

// VARIABLES

int NITEMS,SUPPORT,KSIZE=2;
string INPUT_FILE,OUTPUT_FILE,CONFIG="config.csv";
float SUPPORTF,CONFIDENCE;
bool INPUT_FLAG;
NODE* ROOT=NULL;

// CONTAINERS
vector<vector<string> > ITEMS;
vector<vector<int> > HITEMS;
unordered_map<string,int> HASH;
unordered_map<int,int> cnt;
vector<string> REHASH;
vector<vector<int> > SETS,KLIST;
vector<int> UNIQUE;
vector<int> SUPPORTVALS;
vector<vector<int> > LEFT,RIGHT;

// FUNCTIONS

/*
void ptrie(NODE* ptr,vector<int> v){
	if(ptr==NULL){
		for(int i=0;i<v.size();i++)
			cout<<v[i]<<" ";
		cout<<endl;
		return;
	}
	NODE * Psib = ptr->CHILD;
	
	while(Psib){
		vector<int> pp=v;
		pp.push_back(Psib->VAL);
		ptrie(Psib->CHILD,pp);
		Psib=Psib->NEXT;
	}
}
*/
void insert(vector<int> vec){
	int flag=0;
	NODE* path=ROOT;
	NODE *prev,*next,*store=NULL;
	for(int i=0;i<vec.size();i++){
		flag=0;
		store=NULL;
		prev = path;
		next = path->CHILD;
		while(next){
			if(next->VAL==vec[i]){
				flag=1;
				store=next;
				break;
			}
			prev=next;
			next=next->NEXT;
		}
		if(!flag){

			NODE* tmp = (NODE*)malloc(sizeof(NODE));
			tmp->VAL=vec[i];
			tmp->LF=0;
			tmp->CHILD=tmp->NEXT=NULL;
			if(path->CHILD){
				prev->NEXT=tmp;
				store=prev->NEXT;
			}
			else{
				path->CHILD=tmp;
				store = tmp;
			}
		}
		path=store;
	}
	path->LF=1;
}

//check for null condition
//are all elements in the trie in sorted order??
void findK(NODE* ptr,vector<int> str,int len){
	if(len==KSIZE-1){
		NODE* iter1= ptr->CHILD;
		while(iter1){
			NODE* iter2=iter1->NEXT;
			while(iter2){
				vector<int> tmp=str;
				tmp.push_back(iter1->VAL);
				tmp.push_back(iter2->VAL);
				KLIST.push_back(tmp);
				iter2=iter2->NEXT;
			}
			iter1=iter1->NEXT;
		}
	}
	else{
		NODE* sib = ptr->CHILD;
		while(sib){
			vector<int> pass=str;
			pass.push_back(sib->VAL);
			findK(sib,pass,len+1);
			sib=sib->NEXT;
		}
	}
}


bool pruneK(vector<int> vec){
	NODE* ppath = ROOT->CHILD;
	int i=0;
	while(ppath && i<vec.size()){
		if(ppath->VAL==vec[i]){
			ppath=ppath->CHILD;
			i++;
			continue;
		}
		ppath=ppath->NEXT;
	}
	if(i==vec.size())
		return 1;
	else
		return 0;
}

int suppcount(vector<int> v){
	int ct=0,tct=0;
	for(int i=0;i<HITEMS.size();i++){
		int jid=0,vid=0;
		tct=0;
		while(jid<HITEMS[i].size() && vid<v.size()){
			if(HITEMS[i][jid]==v[vid]){
				jid++;
				vid++;
				tct++;
			}
			else if(HITEMS[i][jid]<v[vid])
				jid++;
			else
				vid++;
		}
		if(v.size()==tct)
			ct++;
	}
	return ct;
}

void checker(bool yes,string filename){
	if(!yes){
		cout<<"SORRY !! "<<filename<<" not found"<<endl;
		exit(0);
	}
}

int main()
{
	ROOT = (NODE*)malloc(sizeof(NODE));
	ROOT->VAL=-1;
	ROOT->CHILD=ROOT->NEXT=NULL;

	ifstream file;

	file.open(CONFIG);
	checker(file.is_open(),CONFIG);

	//TAKING INPUT FROM config.csv
	string line;
	int pos,lineno=0;
	while ( !file.eof() )
	{
		getline(file,line);
		pos=line.find(",");
		if(pos==string::npos)
			break;
		if(lineno==0)
			INPUT_FILE = line.substr(pos+1,line.size());
		else if(lineno==1)
			OUTPUT_FILE = line.substr(pos+1,line.size());
		else if(lineno==2)
			SUPPORTF = stof(line.substr(pos+1,line.size()));
		else if(lineno==3)
			CONFIDENCE = stof(line.substr(pos+1,line.size()));
		else
			INPUT_FLAG = stoi(line.substr(pos+1,line.size()));
		lineno++;
	}
	/*
	cout<<INPUT_FILE<<endl;
	cout<<OUTPUT_FILE<<endl;
	cout<<SUPPORTF<<endl;
	cout<<CONFIDENCE<<endl;
	cout<<INPUT_FLAG<<endl;
	*/
	
	
	file.close();
	file.clear();
	//INPUT TAKEN FROM config.csv

	//TAKING INPUT FROM INPUT_FILE

	file.open(INPUT_FILE);
	checker(file.is_open(),INPUT_FILE);

	NITEMS=0;
	while ( !file.eof() ){
		getline(file,line);
		vector<string> b;
		string temp="";
		int i;
			for(i=0;i<=(int)line.length();i++){
				if(line[i]==',' || line[i]=='\0'){
					if(temp.length()!=0)
						b.push_back(temp);
					temp="";
				}
				else temp+=line[i];				
			}
			ITEMS.push_back(b);
		
	/*	getline(file,line);
		if(line=="")
			break;
		vector<string> temp;
		while((pos = line.find(",")) != string::npos){
			string token = line.substr(0, pos);
			line.erase(0,pos+1);
			temp.push_back(token);
			cout<<"1"<<token<<"1"<<endl;
		}
		temp.push_back(line);
		ITEMS.push_back(temp);
		*/
		NITEMS++;
	}
	file.close();
	file.clear();

	// INPUT TAKEN FROM INPUT_FILE

	/*
	for(int i=0;i<ITEMS.size();i++){
		for(int j=0;j<ITEMS[i].size();j++)
			cout<<ITEMS[i][j]<<" ";
		cout<<endl;
	}
	*/
	
	
	// SUPPORT VALUE
	SUPPORT = ceil(SUPPORTF*(float)ITEMS.size());
	
	// COMPUTING HASHES OF THE ITEM STRINGS

	///////////////////////////////////////////////////////////// check 

	int hashvalue=0;
	for(int i=0;i<ITEMS.size();i++){
		vector<int> tmp;
		for(int j=0;j<ITEMS[i].size();j++){
			if(HASH.count(ITEMS[i][j])==0){
				UNIQUE.push_back(hashvalue);
				HASH[ITEMS[i][j]]=hashvalue++;
				REHASH.push_back(ITEMS[i][j]);
			}
			tmp.push_back(HASH[ITEMS[i][j]]);
		}
		HITEMS.push_back(tmp);
	}
	
	/*
	for(int i=0;i<HITEMS.size();i++){
		for(int j=0;j<HITEMS[i].size();j++)
			cout<<HITEMS[i][j]<<" ";
		cout<<endl;
	}
	*/



	// SORTING THE HASHES

	for(int i=0;i<HITEMS.size();i++){
		sort(HITEMS[i].begin(),HITEMS[i].end());
	}

	// FINDING UNIQUE ITEMS 
	

	for(int i=0;i<HITEMS.size();i++){
		int uid=0,jid=0;
		while(uid<UNIQUE.size() && jid<HITEMS[i].size()){
			if(UNIQUE[uid]==HITEMS[i][jid]){
				cnt[UNIQUE[uid]]++;
				uid++;
				jid++;
			}
			else if(UNIQUE[uid]<HITEMS[i][jid])
				uid++;
			else
				jid++;
		}
	}
	
	// PRUNE STEP, K1 GENERATED , INSERTING INTO TRIE

	for(int i=0;i<UNIQUE.size();i++){
		if(cnt[UNIQUE[i]]>=SUPPORT){
			vector<int> one;
			one.push_back(UNIQUE[i]);
			insert(one);
			SETS.push_back(one);
			SUPPORTVALS.push_back(cnt[UNIQUE[i]]);
		}
	}
	vector<int> lola;	
	
	int MARK = SETS.size();
	bool FOUND = true;
	int flag=0;

	// CALCULATING FOR ALL K2..KN
	while(FOUND){
		FOUND=false;
		KLIST.erase(KLIST.begin(),KLIST.end());
		KLIST.clear();
		vector<int> str;
		vector<vector<int> > KPRUNED;
		findK(ROOT,str,1);
		for(int i=0;i<KLIST.size();i++){
			flag=0;
			for(int j=0;j<KLIST[i].size();j++){
				vector<int> realv;
				for(int k=0;k<KLIST[i].size();k++){
					if(j!=k)
						realv.push_back(KLIST[i][k]);
				}
				if(!pruneK(realv))
					flag=1;
			}
			if(flag==0)
				KPRUNED.push_back(KLIST[i]);
		}
		vector<vector<int> > test;
		for(int i=0;i<KPRUNED.size();i++){
			int xx = suppcount(KPRUNED[i]);
			if(xx>=SUPPORT){
				FOUND=true;
				SETS.push_back(KPRUNED[i]);
				SUPPORTVALS.push_back(xx);
				insert(KPRUNED[i]);
			}
		}
		KSIZE++;

		vector<int> lola;
		//ptrie(ROOT,lola);
	}

	ofstream myfile;

	myfile.open(OUTPUT_FILE);

	myfile<<SETS.size()<<"\n";
	for(int i=0;i<SETS.size();i++){
		for(int j=0;j<SETS[i].size();j++)
			myfile<<SETS[i][j]<<",";
		myfile<<"\n";
	}

	if(INPUT_FLAG){
	for(int i=0;i<SETS.size();i++){
		if(SETS[i].size()==1)
			continue;
		int SZ = SETS[i].size();
		int supp = suppcount(SETS[i]);
		//	int supp = SUPPORTVALS[i];
		for(int j=1;j< (1<<SZ)-1;j++){
			int tmp = j,id=0;
			vector<int> vec1,vec2;
			for(int k=0;k<SZ;k++){
				if(j&(1<<k))
					vec1.push_back(SETS[i][k]);
				else
					vec2.push_back(SETS[i][k]);
			}
			int tsupp = suppcount(vec1);
			double CONF = (double)supp/(double)tsupp;
			if(CONF>=CONFIDENCE){
				LEFT.push_back(vec1);
				RIGHT.push_back(vec2);
			}
		}
	}
	myfile<<LEFT.size()<<"\n";
	for(int i=0;i<LEFT.size();i++){
		for(int j=0;j<LEFT[i].size();j++){
			myfile<<REHASH[LEFT[i][j]]<<",";
		}
		myfile<<"=>,";
		for(int j=0;j<RIGHT[i].size();j++){
			myfile<<REHASH[RIGHT[i][j]]<<",";
		}
		myfile<<"\n";
	}
	}
	myfile.close();


	return 0;
}
