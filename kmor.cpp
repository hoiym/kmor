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
				if(str[i] == ','){
					if(first_ln && ((remove_id == 1 && num_attr > 0) || !remove_id))
						cur_instance.push_back(cur_val);
					else if(!first_ln){
						if(remove_id && num_attr > 0) cur_instance[num_attr-1] = cur_val;
						else if(remove_id == 0) cur_instance[num_attr] = cur_val;
					}
				
					num_attr++;
					cur_val = 0;
				} else {
					cur_val = cur_val * 10 + (str[i] - '0');
					
					if(i - 1 >= 0 && str[i-1] == '-') cur_val *= -1;
				}
			}
		}
		
		if(first_ln) cur_instance.push_back(cur_val), first_ln = 0;
		else{
			if(remove_id) num_attr--;
			cur_instance[num_attr] = cur_val;
		}
		
		ds->push_back(cur_instance);
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
		
		for(int i = 0; i < ds_sz.first; ++i)
			(*ds)[i][j] = ((*ds)[i][j] - mn) / (mx - mn);
	}
}

double distance(vvd ds, vvd Z, int x, int y){
	int obj_sz = (int) ds[x].size();
	double dist = 0;
	
	for(int i = 0; i < obj_sz; ++i)
		dist += pow(ds[x][i] - Z[y][i], 2);
		
	return dist;
}

void update_U(vvd ds, vvd Z, ii ds_sz, int k, double d_avg, vpid *U){
	int idx, outlier = 0;
	double cur_dist;

	for(int i = 0; i < ds_sz.first; ++i){
		(*U)[i].second = 1000000000;
	
		for(int j = 0; j < k; ++j){
			cur_dist = distance(ds, Z, i, j);
		
			if(cur_dist < (*U)[i].second)
				(*U)[i].second = cur_dist, (*U)[i].first = j;
		}
		
		if((*U)[i].second > d_avg){
			outlier++;
			(*U)[i] = ii(k, d_avg);
		}
	}
	
	// printf("partial: %d\n", outlier);
}

void update_Z(vvd ds, ii ds_sz, int k, vpid U, vvd *Z){
	vector<int> occur(k);
	
	for(int i = 0; i < k; ++i)
		for(int j = 0; j < ds_sz.second; ++j) (*Z)[i][j] = 0;

	for(int i = 0; i < ds_sz.first; ++i){
		if(U[i].first != k){
			occur[U[i].first]++;
			
			for(int j = 0; j < ds_sz.second; ++j)
				(*Z)[U[i].first][j] += ds[i][j];
		}
	}
	
	for(int i = 0; i < k; ++i)
		for(int j = 0; j < ds_sz.second; ++j) (*Z)[i][j] /= occur[i];
		
	occur.clear();
}

double get_avg_dist(ii ds_sz, int k, vpid *U){
	int cnt = 0;
	double sum_dist = 0;

	for(int i = 0; i < ds_sz.first; ++i){
		if((*U)[i].first != k) sum_dist += (*U)[i].second, cnt++;
	}
	
	return sum_dist/cnt;
}

double get_p(vpid U, ii ds_sz){
	double p = 0;
	
	for(int i = 0; i < ds_sz.first; ++i)
		p += U[i].second;
	
	return p;
}

void kmor(vvd ds, int k, int gama, int no, int sigma, int nmax, ii ds_sz) {
	int cnt = 0, r;
	double prev_p = 0, cur_p, d_avg;
	set<int> s;
	vvd Z;
	vpid U(ds_sz.first);

	while(cnt < k){
		r = rand() % (ds_sz.first + 1);
		
		if(s.find(r) == s.end()){
			s.insert(r);
			cnt++;
		}
	}
	
	for(set<int>::iterator iter = s.begin(); iter != s.end(); iter++)
		Z.push_back(ds[*iter]);
		
	update_U(ds, Z, ds_sz, k, 1000000000, &U);
		
	for(int i = 0; i < nmax; ++i){
		d_avg = gama * get_avg_dist(ds_sz, k, &U);
		update_U(ds, Z, ds_sz, k, d_avg, &U);
		update_Z(ds, ds_sz, k, U, &Z);
		cur_p = get_p(U, ds_sz);
		
		if(fabs(cur_p - prev_p) < sigma)
			break;
		
		prev_p = cur_p;
	}
	
	int outliers = 0;
	
	for(int i = 0; i < ds_sz.first; ++i)
		if(U[i].first == k) outliers++;
		
	printf("Number of outiers: %d\n", outliers);
}

int main(){
	ios_base::sync_with_stdio(0);
	srand( time( NULL ) );
	
	int class_idx, remove_id;
	pair<int, int> ds_sz;
	vvd ds;

	// Optional flag if dataset's instances contain IDs.
	remove_id = 0;
	
	get_dataset(remove_id, &ds);
	normalize_ds(&ds);
	ds_sz = make_pair(int(ds.size()), int(ds[0].size()));
	
	/*
	for(int i = 0; i < ds_sz.first; ++i){
		for(int j = 0; j < ds_sz.second; ++j){
			printf("%.4lf ", ds[i][j]);
		}
		printf("\n");
	}
	*/
	
	int k = 1, gama = 3, no = int(ds_sz.first * 0.1), nmax = 100;
	double sigma = 1e-6;
	
	printf("%d %d\n", ds_sz.first, ds_sz.second);
	
	kmor(ds, k, gama, no, sigma, nmax, ds_sz);
	
	return 0;
}
