#include <bits/stdc++.h>

using namespace std;

typedef vector<vector<double> > vvd;
typedef vector<vector<bool> > vvb;
typedef vector<pair<int, double> > vpid;
typedef pair<int, int> ii;

void get_dataset(int remove_id, vvd *ds){
	string str;
	int len, num_attr, first_ln = 1;
	double cur_val;
	vector<double> cur_instance;
	
	while(getline(cin, str)){
		len = int(str.size());
		num_attr = 0;
		
		for(int i = 0; i < len; ++i){
			if(str[i] != ' '){
				if(str[i] == ',' || i == len - 1){
					if(first_ln){
						if((remove_id == 1 && num_attr > 0) || remove_id == 0){
							cur_instance.push_back(cur_val);
						}
					} else
						cur_instance[num_attr] = cur_val;
				
					num_attr++;
					cur_val = 0;
				} else {
					cur_val = cur_val * 10 + (str[i] - '0');
					
					if(i - 1 >= 0 && str[i-1] == '-') cur_val *= -1;
				}
			}
		}
		
		ds->push_back(cur_instance);
		
		if(first_ln) first_ln = 0;
	}
}

void normalize_ds(vvd *ds){
	ii ds_sz = make_pair(int(ds->size()), int((*ds)[0].size()));
	double mx, mn;
	
	for(int j = 0; j < ds_sz.second; ++j){
		mx = 0, mn = 1000000000;
	
		for(int i = 0; i < ds_sz.first; ++i){
			if((*ds)[i][j] > mx) mx = (*ds)[i][j];
			if((*ds)[i][j] < mn) mn = (*ds)[i][j];
		}
		
		for(int i = 0; i < ds_sz.first; i++)
			((*ds)[i][j]) = ((*ds)[i][j] - mn) / (mx - mn);
	}
}

double distance(vvd ds, vvd Z, int x, int y){
	int obj_sz = (int) ds[x].size();
	double dist = 0;
	
	for(int i = 0; i < obj_sz; ++i)
		dist += pow(ds[x][i] - Z[y][i], 2);
		
	return dist;
}

void init_U(vvd ds, vvd Z, ii ds_sz, int k, vector<int> *U){
	int idx;
	double min_dist, cur_dist;

	for(int i = 0; i < ds_sz.first; ++i){
		min_dist = 1000000000;
	
		for(int j = 0; j < k; ++j){
			cur_dist = distance(ds, Z, i, j);
		
			if(cur_dist < min_dist)
				min_dist = cur_dist, (*U)[i] = j;
		}
	}
}

/*
double get_average_dist(vvd ds, vector<double> Z, ii ds_sz, vpid *U){
	for(int i = 0; i < ds_sz.first; ++i){
		
	}
}

void kmor(vvb ds, int k, int gama, int no, int sigma, int nmax, ii ds_sz) {
	int cnt = 0, r;
	double prev_p = 0, cur_p;
	set<int> s;
	vvd Z;
	vector<int> U;
	vpid close_cluster(ds_sz.first);

	while(cnt < k){
		r = rand() % (ds_sz.first + 1);
		
		if(s.find(r) == s.end()){
			s.insert(r);
			cnt++;
		}
	}
	
	for(set<int>::iterator iter = s.begin(); iter != s.end(); iter++)
		Z.push_back(ds[*iter]);
		
	while(true){
		
	}
}
*/

int main(){
	ios_base::sync_with_stdio(0);
	
	int class_idx, remove_id;
	pair<int, int> ds_sz;
	vvd ds;

	// Optional flag if dataset's instances contain IDs.
	remove_id = 0;
	
	get_dataset(remove_id, &ds);
	ds_sz = make_pair(int(ds.size()), int(ds[0].size()));
	
	printf("%d %d\n", ds_sz.first, ds_sz.second);
	
	return 0;
}
