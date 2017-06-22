#include <bits/stdc++.h>

using namespace std;

int class_idx, remove_id;
pair<int, int> ds_sz;
vector<vector<double> > ds;

void getDataset(){
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
					} else {
						cur_instance[num_attr] = cur_val;
					}
				
					num_attr++;
					cur_val = 0;
				} else {
					cur_val = cur_val * 10 + (str[i] - '0');
					
					if(i - 1 >= 0 && str[i-1] == '-') cur_val *= -1;
				}
			}
		}
		
		ds.push_back(cur_instance);
		
		if(first_ln) first_ln = 0;
	}
}

void kmor() {

}

int main(){
	ios_base::sync_with_stdio(0);

	// Optional flag if dataset's instances contains IDs.
	remove_id = 0;
	
	getDataset();
	ds_sz = make_pair(int(ds.size()), int(ds[0].size()));
	
	printf("%d %d\n", ds_sz.first, ds_sz.second);
	
	return 0;
}
