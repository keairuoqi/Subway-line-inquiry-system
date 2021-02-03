#include<bits/stdc++.h>
using namespace std;

#define INF 9999999
#define NUM 14
#define MAX 100

map<string,int> M1;
map<int,string> M2;
//vector<pair<int,int> > V;//地铁站所在地铁线路(换乘站可能在2条地铁线路上) 
vector<vector<int> > V;
struct{
	int Lnum;//线路编号 
	int stationNum; //站台数 
	vector<string> v;//站名 
	vector<int> w;//weight 
}Line[22];
//0 3号线北延段 
//15 4号线 
struct Graph_b{
	vector<vector<int> > adj;//邻接表存图 
	int vexnum;
	Graph_b(int n):vexnum(n){
		vexnum=n;
		vector<int> v;
		adj.resize(n,v);
	}
}; 

struct vertex{
	bool known;//记录顶点是否访问过 
	int dist; //在Dijkstra中记录路径的长度 
	int path;//记录上一个顶点 
};

struct Graph_a{
	int vexnum;
	int arcnum;
	vector<vertex> v; 
	vector<vector<pair<int,int> > > adj;//int 编号 int 权重; 
	 Graph_a(int n):vexnum(n){
	 	vector<pair<int,int> > V;
	 	vertex ver;
	 	v.resize(n+1,ver); 
	 	adj.resize(n+1,V);
	 	for(int i=0;i<n;i++){
	 		v[i]={false,INT_MAX,-1};//对顶点的信息进行初始化 
		 }
	 }
};

void read(int i,string fileName,int &cnt){//i 地铁线路Line的下标  fileName 存放地铁线路信息的txt文件的文件名  cnt 顶点编号 
	FILE *fp;
	int j, p;
	fp=fopen(fileName.c_str(),"r");
	if(fp==NULL){
		cout<<"Can not open the file!\n";
		exit(0);
	}
	p=fscanf(fp,"%d %d",&(Line[i].Lnum),&(Line[i].stationNum));
	p++;
	for(j=0;j<Line[i].stationNum;j++){
		char Station[50];
		p=fscanf(fp,"%s",Station);
		string station=Station;
		Line[i].v.push_back(station);
		p++;
		//给地铁站编号 
		map<string, int>::iterator iter=M1.find(station);
		if(iter==M1.end()){
			M1.insert(make_pair(station,cnt));
			M2.insert(make_pair(cnt,station));
			cnt++;
			vector<int> vec;
			vec.push_back(Line[i].Lnum);
			V.push_back(vec);
		}
		else{
			V[M1[station]].push_back(Line[i].Lnum);
			//如果地铁站已经记录过，说明该地铁站是换乘站 
		} 
	}
	//地铁线路相邻2站所需时间 
	for(j=0;j<Line[i].stationNum-1;j++){
		int weight;
		p=fscanf(fp,"%d",&weight);
		Line[i].w.push_back(weight);
		p++;
	}
	fclose(fp);
}


//建无向带权图 
Graph_a mkGraph_a(){
	Graph_a g(V.size());//vector<pair<int,int> > V; 
	for(int i=0;i<NUM;i++){//#define NUM 14   14条地铁线路 
		for(int j=0;j<Line[i].v.size()-1;j++){
			int weight=Line[i].w[j];
			int c=M1[Line[i].v[j]];
			int d=M1[Line[i].v[j+1]];
			g.adj[c].push_back(make_pair(d,weight));
			g.adj[d].push_back(make_pair(c,weight));
		}
	}
	return g;
}

void print_min_d(int i,Graph_a g){
	int j;
	if(0==g.v[i].dist) cout<<M2[i];
	else{
		print_min_d(g.v[i].path,g);
		int temp=g.v[i].path;
		
		if(V[temp].size()>1&&V[g.v[temp].path][0]!=V[temp][0]){
			int a=V[temp][0];
			V[temp][0]=V[temp][1];
			V[temp][1]=a;
		}
		if(V[temp].size()>1){
			int mark=-1;
			for(int m=0;m<V[i].size();m++){
				for(int n=0;n<V[g.v[temp].path].size();n++){
					if(V[i][m]==V[g.v[temp].path][n]){
						mark=1;
						break;
					}
				}
			}
			if(mark==-1){
			int b;
			cout<<"(换乘 ";
			for(int m=0;m<V[i].size();m++){
				for(int n=0;n<V[temp].size();n++){
					if(V[i][m]==V[temp][n]){
						b=V[i][m];
						cout<<V[i][m];
					}
				}
			}
			cout<<" 号线 "; 
			for(j=0;j<NUM;j++){
				if(Line[j].Lnum==b) break;
			} 
			for(int m=0;m<Line[j].v.size();m++){
				if(Line[j].v[m]==M2[i]){
					if(m+1<Line[j].v.size()&&Line[j].v[m+1]==M2[temp])
						cout<<"往 "<<Line[j].v[0]<<" 方向列车)";
					else if(m-1>=0&&Line[j].v[m-1]==M2[temp]){
						int a=Line[j].v.size()-1;
						cout<<"往 "<<Line[j].v[a]<<" 方向列车)";
					}
				}
				
			}
			}
		}
		cout<<"->"<<M2[i];
	}
}

void MinDist(string v1,string v2,Graph_a g){
	int a=M1[v1];
	int b=M1[v2];
	g.v[a]={true,0,a};
	
	int i,j,k;
	for(i=0;i<g.adj[a].size();i++){
		g.v[g.adj[a][i].first].dist=g.adj[a][i].second;
	}
	for(i=1;i<g.vexnum;i++){
		int min=INT_MAX;
		for(j=0;j<g.vexnum;j++){
			if(!g.v[j].known&&g.v[j].dist<min){
				k=j;
				min=g.v[j].dist;
			}
		}
		g.v[k].known=true;
		if(g.v[k].path==-1) g.v[k].path=a;
		for(j=0;j<g.adj[k].size();j++){
			if(!g.v[g.adj[k][j].first].known&&(min+g.adj[k][j].second<g.v[g.adj[k][j].first].dist)){
				g.v[g.adj[k][j].first].dist=min+g.adj[k][j].second;
				g.v[g.adj[k][j].first].path=k;
			}
		}
	}
	cout<<endl;
	cout<<"最短距离的乘车方案有："<<endl; 
	print_min_d(M1[v2],g);
	cout<<endl;
	cout<<"=================================================================================="<<endl<<endl;
}



Graph_b mkGraph_b_c(){ //最小换乘图
	Graph_b g(V.size());
	for(int i=0;i<NUM;i++){
		for(int j=0;j<Line[i].v.size();j++){
			int temp1=M1[Line[i].v[j]];
			for(int k=j+1;k<Line[i].v.size();k++){
				int temp2=M1[Line[i].v[k]];
				g.adj[temp1].push_back(temp2);
				g.adj[temp2].push_back(temp1);
			}
		}
	}
	return g;
}

Graph_b mkGraph_b_s(){//最小站数图 
	Graph_b g(V.size());
	for(int i=0;i<NUM;i++){
		for(int j=0;j<Line[i].v.size()-1;j++){
			int temp1=M1[Line[i].v[j]];
			int temp2=M1[Line[i].v[j+1]];
			g.adj[temp1].push_back(temp2);
			g.adj[temp2].push_back(temp1);
		}
	}
	return g;
}

void print_min_s(vector<int>from, int dest, int start){
	cout<<endl;
	cout<<"最少站台数的乘车方案有："<<endl; 
	int temp=dest;
	while(from[temp]!=-1){
		cout<<M2[temp];
//		if(V[temp].first==V[temp].second) cout<<"[line"<<V[temp].first<<"]"<<"->";
//		else cout<<"[line"<<V[temp].first<<"&"<<V[temp].second<<"]"<<"->"; 
		cout<<"->";
		temp=from[temp];
	}
	cout<<M2[start];
//	if(V[start].first==V[start].second) cout<<"[line "<<V[start].first<<"]"<<endl;
//	else cout<<"[line "<<V[start].first<<"&"<<V[start].second<<"]"<<endl; 
	cout<<endl<<endl;
	cout<<"=================================================================================="<<endl<<endl;
}

void print_min_c(vector<int>from, int dest, int start){
	cout<<endl;
	cout<<"最少转乘数的乘车方案有："<<endl; 
	int temp=dest;
	int n=0; 
	while(from[temp]!=-1){
		n++;
		for(int a=0;a<V[temp].size();a++){
			for(int b=0;b<V[from[temp]].size();b++){
				if(V[temp][a]==V[from[temp]][b]){
					cout<<M2[temp]<<"-[沿"<<V[temp][a]<<"号线一直到]->";
				}
			}
		}
//		if(V[temp].first==V[from[temp]].first||V[temp].first==V[from[temp]].second)
//			cout<<M2[temp]<<"-[沿"<<V[temp].first<<"号线一直到]->";
//		else
//			cout<<M2[temp]<<"-[沿"<<V[temp].second<<"号线一直到]->";
//		if(V[temp][0]==V[from[temp]][0]||V[temp][0]==V[from[temp]][1]||V[temp][0]==V[from[temp]][2])
		temp=from[temp];
	}
	cout<<M2[start]<<endl;
	cout<<"共换乘"<<n-1<<"次"<<endl; 
	cout<<endl;
	cout<<"=================================================================================="<<endl<<endl;
}



void find_min(Graph_b g, int start, int dest, bool ifChange){
	vector<int> dist;
	queue<int> q;
	vector<int> from(g.vexnum,-1);
	bool ifFind=false;
	for(int i=0;i<g.vexnum;i++){
		dist.push_back(INF);
	}
	dist[start]=0;
	q.push(start);
	while(!q.empty()){
		int v=q.front();
		q.pop();
		for(int i=0;i<g.adj[v].size();i++){
			if(dist[g.adj[v][i]]==INF){
				dist[g.adj[v][i]]=dist[v]+1;
				from[g.adj[v][i]]=v;
				q.push(g.adj[v][i]);
				if(g.adj[v][i]==dest){
					ifFind=true;
					if(ifChange) print_min_c(from,dest,start);
					else print_min_s(from,dest,start);
				}
			}
			if(ifFind&&i==g.adj[v].size()-1)break;
		}
	}
}

bool judge(string &name1, string &name2){
	cout<<endl;
	cout<<"请输入出发地：";
	cin>>name1;
	if(M1.find(name1)==M1.end()){
		cout<<"查无此站，请重新操作"<<endl;
		return false; 
	}
	cout<<"请输入目的地：";
	cin>>name2; 
	if(M1.find(name2)==M1.end()){
		cout<<"查无此站，请重新操作"<<endl;
		return false; 
	}
	return true;
}

void MinChange_(Graph_b g){
	string name1,name2;
	if(!judge(name1,name2)) return;
	int start=M1[name1];
	int dest=M1[name2];
	find_min(g,dest,start,true);
}

void MinDist_(Graph_a g){
	string name1,name2;
	if(!judge(name1,name2)) return;
	int start=M1[name1];
	int dest=M1[name2];
	MinDist(name1,name2,g);
}

void MinStation_(Graph_b g){
	string name1,name2;
	if(!judge(name1,name2)) return;
	int start=M1[name1];
	int dest=M1[name2];
	find_min(g,dest,start,false);
}
//输出全部地铁线路信息 
void print_whole(){
	cout<<endl;
	for(int i=0;i<22;i++){
		if(!Line[i].v.empty()){
			cout<<endl<<Line[i].Lnum<<"号线："; 
			if(Line[i].Lnum==0)cout<<"（这里0号线表示3号线北延段）"<<endl;
			if(Line[i].Lnum==15)cout<<"（这里15号线表示14号线支线（知识城线））"<<endl;
			cout<<endl;
			for(int j=0;j<Line[i].v.size();j++){
				cout<<Line[i].v[j]<<" ";
			}
			cout<<endl;
		}
	}
	cout<<endl;
}

int main(){
	system("color f4");//color f4
	//读文件函数 
	int n=0;
	for(int i=0;i<NUM;i++){
		string filename="line"+to_string(i)+".txt";
		read(i,filename,n);
	}
	Graph_a ga=mkGraph_a();
	Graph_b gc=mkGraph_b_c();
	Graph_b gs=mkGraph_b_s();
	
	
	while(1){
		cout<<"*************欢迎来到广州地铁*************"<<endl
			<<"    1 ---------------查看每条线路及站点"<<endl 
			<<"    2 ---------------按最短距离查询路线"<<endl 
			<<"    3 ---------------按最小换乘查询路线"<<endl 
			<<"    4 ---------------按最少站数查询路线"<<endl
			<<"******************************************"<<endl<<endl
			<<"【【请注意：这里0号线表示3号线北延段，15号线表示14号线支线（知识城线）】】"<<endl
			<<"【【并且此地图不包括广佛线和APM线】】"<<endl<<endl
			<<"请输入1~4进行选择，其他退出:";
		char choice;
		cin>>choice;
		switch(choice){
			case '1':
				print_whole();
				cout<<endl;
				break;
			case '2':
				MinDist_(ga);
				cout<<endl;
				break;
			case '3':
				MinChange_(gc);
				cout<<endl;
				break;
			case '4':
				MinStation_(gs);
				cout<<endl;
				break;
			default:
				return 0;
		}
			
	}
	return 0;
}
