#include<iostream>
#include<fstream>
#include<stdio.h>//sscanf
#include<string>
#include<sstream>
#include<iomanip>//setprecision
#include<dirent.h>
#include<vector>
#include<stdlib.h>//strtod
#include<windows.h>//CreateDirectory
#include<algorithm>//sort
#include<regex>
#include<ctime>
using namespace std;
////////////////////////////////////////
//dxpcon log parser
///////////////////////////////////////
ofstream log_file;

class target_par{
	private:
	int member_number;
	public:
	string time;
	string plot_mode;
	string job_name;
	string lot_number;
	string sequence_number;
	string light_position;
	string align_ring_light_0;
	string align_ring_light_1;
	string align_ring_light_2;
	string align_vert_light_0;
	string align_vert_light_1;
	string align_vert_light_2;
	string align_ring_light2_0;
	string align_ring_light2_1;
	string align_ring_light2_2;
	string mark_type;
	string mark_setting;
	string meas_in_file;
	string result;
	string cam_path;
	
	//The out_file_path was not manage by member_list and also would not be erased in method clear().
	string out_file_path;
	
	//Define a vector to manage the variables in target_par object except out_file_path.
	vector<string*> member_list;
	
	target_par():member_number(20){ //member_number did not include out_file_path
		member_list.resize(member_number);
			member_list[0] = &time;
			member_list[1] = &plot_mode;
			member_list[2] = &job_name;
			member_list[3] = &lot_number;
			member_list[4] = &sequence_number;
			member_list[5] = &light_position;
			member_list[6] = &align_ring_light_0;
			member_list[7] = &align_ring_light_1;
			member_list[8] = &align_ring_light_2;
			member_list[9] = &align_vert_light_0;
			member_list[10] = &align_vert_light_1;
			member_list[11] = &align_vert_light_2;
			member_list[12] = &align_ring_light2_0;
			member_list[13] = &align_ring_light2_1;
			member_list[14] = &align_ring_light2_2;
			member_list[15] = &mark_type;
			member_list[16] = &mark_setting;
			member_list[17] = &meas_in_file;
			member_list[18] = &result;
			member_list[19] = &cam_path;
	}
	
	//Copy constructor
	target_par(const target_par &t):target_par()
	{
		for(int i=0; i<member_number; i++)
			*(member_list.at(i)) =  *((t.member_list).at(i));
	}
	
	target_par operator = (const target_par &t)
	{
		if(this == &t)
			return *this;
		for(int i=0; i<member_number; i++)
			*(member_list.at(i)) =  *((t.member_list).at(i));
		return *this;
	}
	
	//Get the private member: member_number.
	int get_member_number(){
		return member_number;
	}
	
	//Erase all members except out_file_path.
	void clear(){
		for(vector<string*>::iterator iter=member_list.begin(); iter!= member_list.end(); iter++)
			(*iter)->clear();
	}
	
	void output(){	
		ifstream check_file_size;
		check_file_size.open(out_file_path.c_str(), ios_base::binary|ios_base::ate); //ate: if file doesn't exist, create an empty file
		int file_size = check_file_size.tellg();
		check_file_size.close();
		
		ofstream out_file;
		out_file.open(out_file_path.c_str(), ios_base::out|ios_base::app);
		
		if(file_size <= 0)//If file is empty, output the head column to file. The output file may be analyzed manually, so the head column is necessary.  
			out_file << "time,plot_mode,job_name,lot_number,sequence_number,light_position,align_ring_light_0,align_ring_light_1,align_ring_light_2,align_vert_light_0,align_vert_light_1,align_vert_light_2,align_ring_light2_0,align_ring_light2_1,align_ring_light2_2,mark_type,mark_setting,meas_in_file,result,cam_path" << endl; 
		
		//Since the output file is csv form, remove the comma in mark_setting variable to prevent being devided into several columns when opening in csv-supported software. 
		//mark_setting.erase(remove_if(mark_setting.begin(), mark_setting.end(), [](char s){return s==',';}), mark_setting.end());
		
		for(vector<string*>::iterator iter=member_list.begin(); iter!= member_list.end(); iter++)
			iter+1 == member_list.end()? out_file << *(*iter) << endl: out_file << *(*iter) << ',';
	};
};

class key{
	public:
	string key_name;
	string binary_key;	
	key(string x, string y):key_name(x), binary_key(y){};
};
//keys define
const key scale_meas("scale_meas", "111000101000000010111011111001111011100010101110111001101001010010111110111010001010100010001000111001101011100010101100111010011001110010000000111010001010011010000001111001011001110010101000111001001011100010001011111001011000100010010111111001101010001010011101111001001011101110110110111001001011100010001011111001011010111110100110111001101001011010111101111011111011110010011010");
const key job_name("job_name", "00100000001000000010000000100000111001001011110110011100111001101010010110101101111001011001000010001101111001111010100010110001001000000011101000100000");
const key right_recipe("right_recipe", "0011110100111101001111010011110100111101001000000100010001001001001011010111001001101001011001110110100001110100001010000011000000101001001000000101001001100101011000110110100101110000011001010010000001010000011000010111001001100001011011010110010101110100011001010111001001110011001000000011110100111101001111010011110100111101");
const key left_recipe ("left_recipe", "00111101001111010011110100111101001111010010000001000100010010010010110101101100011001010110011001110100001010000011000100101001001000000101001001100101011000110110100101110000011001010010000001010000011000010111001001100001011011010110010101110100011001010111001001110011001000000011110100111101001111010011110100111101");
const key right_counter("right_counter", "0101001101100001011101100110010101100100001000000101101000111010010111000100010001011000010100000011001100110011001100000011000101011100010001100101001101011111011100100110100101100111011010000111010001011100010000110110111101101110011001100110100101100111010111000111001101100101011100010110001101101111011101010110111001110100011001010111001000101110011101000111100001110100");
const key left_counter("left_counter", "01010011011000010111011001100101011001000010000001011010001110100101110001000100010110000101000000110011001100110011000000110001010111000100011001010011010111110110110001100101011001100111010001011100010000110110111101101110011001100110100101100111010111000111001101100101011100010110001101101111011101010110111001110100011001010111001000101110011101000111100001110100");
const key lot_number("lot_number", "001001000100110001001111010101000010000000100000001000000010000000100000001000000011110100100000");
const key sequence("sequence", "001001000101001101000101010100010101010101000101010011100100001101000101001000000011110100100000");
const key light_position("light_postion", "01001100011010010110011101101000011101000101000001101111011100110110100101110100011010010110111101101110001000000010000000100000001000000011110100100000");
const key meas_in_file("meas_in_file", "0100110101100101011000010111001101001001011011100110100101000110011010010110110001100101001000000010000000100000001000000010000000100000001111010010000000100000");
const key right_enter_alignevent("right_enter_alignevent", "0100010101101110011101000110010101110010001000000111000001101100011000110100000101101100011010010110011101101110010100110110111101100011011010110101111101001111011011100100000101101100011010010110011101101110010011010110010101100001011100110100010101110110011001010110111001110100001000000110010001101001010111110110111001101111001111010111001001101001011001110110100001110100001010000011000000101001");
const key left_enter_alignevent("left_enter_alignevent", "01000101011011100111010001100101011100100010000001110000011011000110001101000001011011000110100101100111011011100101001101101111011000110110101101011111010011110110111001000001011011000110100101100111011011100100110101100101011000010111001101000101011101100110010101101110011101000010000001100100011010010101111101101110011011110011110101101100011001010110011001110100001010000011000100101001");
const key exit_alignevent("exit_alignevent", "0100010101111000011010010111010000100000001000000111000001101100011000110100000101101100011010010110011101101110010100110110111101100011011010110101111101001111011011100100000101101100011010010110011101101110010011010110010101100001011100110100010101110110011001010110111001110100001000000110010001101001010111110110111001101111001000000011110100100000");
const key mark("mark", "0100110101100001011100100110101101011011");
const key cam_saving("cam_saving", "11100011100000111001111011100011100000111011110011100011100000101010111111100101100001011000001111100111100101001011101111100101100000111000111111100011100000101001001011100100101111111001110111100101101011011001100011100011100000011001011111100011100000011011111011100011100000011001011111100011100000011001111111100011100000001000001000100000");
const key right_mark_info("right_mark_info", "0011110100111101001111010010000001011011011001000110100101011111011011100110111100111101011100100110100101100111011010000111010000101000001100000010100101011101001000000100110101100001011100100110101100100000011010010110111001100110011011110010111000100000001111010011110100111101");
const key left_mark_info("left_mark_info", "00111101001111010011110100100000010110110110010001101001010111110110111001101111001111010110110001100101011001100111010000101000001100010010100101011101001000000100110101100001011100100110101100100000011010010110111001100110011011110010111000100000001111010011110100111101");
const key decide_mark("decide_mark", "010001000110010101100011011010010110010001100101010011010110000101110010011010110101000001101111011100110110100101110100011010010110111101101110");
const key align_ring_light_0("align_ring_light_0","01000001011011000110100101100111011011100101001001101001011011100110011101001100011010010110011101101000011101000010000000100000001000000011110100100000001100000010110000100000");
const key align_ring_light_1("align_ring_light_1","01000001011011000110100101100111011011100101001001101001011011100110011101001100011010010110011101101000011101000010000000100000001000000011110100100000001100010010110000100000");
const key align_ring_light_2("align_ring_light_2","01000001011011000110100101100111011011100101001001101001011011100110011101001100011010010110011101101000011101000010000000100000001000000011110100100000001100100010110000100000");
const key align_vert_light_0("align_vert_light_0","01000001011011000110100101100111011011100101011001100101011100100111010001001100011010010110011101101000011101000010000000100000001000000011110100100000001100000010110000100000");
const key align_vert_light_1("align_vert_light_1","01000001011011000110100101100111011011100101011001100101011100100111010001001100011010010110011101101000011101000010000000100000001000000011110100100000001100010010110000100000");
const key align_vert_light_2("align_vert_light_2","01000001011011000110100101100111011011100101011001100101011100100111010001001100011010010110011101101000011101000010000000100000001000000011110100100000001100100010110000100000");
const key align_ring_light2_0("align_ring_light2_0","01000001011011000110100101100111011011100101001001101001011011100110011101001100011010010110011101101000011101000011001000100000001000000011110100100000001100000010110000100000");
const key align_ring_light2_1("align_ring_light2_1","01000001011011000110100101100111011011100101001001101001011011100110011101001100011010010110011101101000011101000011001000100000001000000011110100100000001100010010110000100000");
const key align_ring_light2_2("align_ring_light2_2","01000001011011000110100101100111011011100101001001101001011011100110011101001100011010010110011101101000011101000011001000100000001000000011110100100000001100100010110000100000");
const key mark_not_found("mark_not_found","01001101011000010111001001101011001000000100111001101111011101000100011001101111011101010110111001100100");
//Define a vecotr to manage the const keys
//vector<key> keys was initialized by a list, which is only available after c++11.
vector<key> keys = {scale_meas, job_name, right_recipe, left_recipe, right_counter, left_counter, lot_number, sequence, light_position, meas_in_file, right_enter_alignevent, left_enter_alignevent, exit_alignevent, mark, cam_saving, right_mark_info, left_mark_info, decide_mark, align_ring_light_0, align_ring_light_1, align_ring_light_2, align_vert_light_0, align_vert_light_1, align_vert_light_2, align_ring_light2_0, align_ring_light2_1, align_ring_light2_2, mark_not_found};

void key_push_back(vector<key> &search_keys,const key &x)
{
	if(find_if(search_keys.begin(), search_keys.end(), [&](key y){return(x.key_name == y.key_name);}) == search_keys.end())
		search_keys.push_back(x);//push_back "copy" the value from x, so it will no be affected if the x assignment a new value after it		
}

void key_initialize(vector<key> &search_keys, string initial_list)
{
	if(initial_list == "")
	{
		key_push_back(search_keys, scale_meas);
		key_push_back(search_keys, job_name);
	}
	else
	{
		istringstream iss(initial_list);
		string str;
		while(((iss >> str).rdstate() & istream::failbit) == 0)//Can simplely write while(iss>>str). The (iss>>str) returns an is object and is.rdstate() returns it stateflag. (iss >> str).rdstate() & istream::failbit is a bitmask operation to see whether the failbit has been set on (iss >> str).rdstate().
		{
			for(vector<key>::iterator iter=keys.begin(); iter!=keys.end(); iter++)
			{
				if(str == iter->key_name)
					key_push_back(search_keys, *iter);	
			}		
		}
	}
	
}

bool key_erase(vector<key> &search_keys, const key& x)
{
	bool i = false;
	search_keys.erase(remove_if(search_keys.begin(), search_keys.end(), [&](key y){ i = i || y.key_name == x.key_name; return (y.key_name == x.key_name);}), search_keys.end());
	return i;//return true if find and remove. Otherwise, return false if dosen't exist
}

void key_clear(vector<key> &search_keys)
{
	//remain "scale_meas" & "job_name"
	search_keys.erase(remove_if(search_keys.begin(), search_keys.end(), [](key y){ return (y.key_name!="scale_meas"&&y.key_name!="job_name");}), search_keys.end());
}

void key_display(vector<key> search_keys)
{
	for_each(search_keys.begin(), search_keys.end(), [](key x){log_file << x.key_name << endl;});
}

key key_cmp(vector<key> &search_keys, string &row)
{
	string binary_row;
	for(size_t i=0; i<row.length(); i++)
		binary_row = binary_row + bitset<8>(row.c_str()[i]).to_string();
		
	vector<key>::iterator iter;
	size_t i;
	if((iter = find_if(search_keys.begin(), search_keys.end(), [&](key x){ return( (i=binary_row.find(x.binary_key)) != string::npos); })) == search_keys.end())
	{
		key empty("", "");//If can't match any keys in search_keys, create a key object with empty string member and return. 
		return empty;
	}
	else
	{
		binary_row = binary_row.substr(i + iter->binary_key.length());
		i = binary_row.length() / 8;
		row = row.substr(row.length() - i);//If match one key in search_keys, substract the binary_key in binary_row to leave only the setting value in row which was called by reference.
		return *iter;//Return the matched key object if matched.	
	}
}



////////////////////////////////////////
//Sorting dxpcon log
///////////////////////////////////////

class dxpcon{
private:
	int member_number;
	public:
	string s_time;
	string f_time;
	string file_name;
	string readed;
	string search_keys_list;
	string read_time;
	string scale_mode;
	string seq_ptr;
	string par_ptr;
	target_par left;
	target_par right;
	
	//Define a vector to manage the variables in dxpcon object
	vector<string*> member_list;	
	
	// default constructor
	dxpcon():member_number(49){ //Total variables are 49.
		member_list.resize(member_number);
		//The first 9 variable are string member of dxpcon object
		member_list[0] = &s_time;
		member_list[1] = &f_time;
		member_list[2] = &file_name;
		member_list[3] = &readed;
		member_list[4] = &search_keys_list;
		member_list[5] = &read_time;
		member_list[6] = &scale_mode;
		member_list[7] = &seq_ptr;
		member_list[8] = &par_ptr;
		
		//The following "target_par.member_number" variables which is 20 when writing this note are string members of target_par obeject: left.
		//Note that the index of left.member_list is from 0 to left.member_number-1.
		int j = 0;
		for(int i=member_number-(left.get_member_number())-(right.get_member_number()); i<member_number-(right.get_member_number()); i++)
		{
			member_list[i] = left.member_list[j];
			j++;
		}
		
		//The last "target_par.member_number" variables which is 19 when writing this note are string members of target_par obeject: right.
		j = 0;
		for(int i=member_number-(right.get_member_number()); i<member_number; i++)
		{
			member_list[i] = right.member_list[j];
			j++;
		} 

	}
	
	//Construct by rows in dxpcon list.
	dxpcon(string row):dxpcon(){//Callinf default constructor in another constructor is only available after c++11. Call the default construct to initialize the member_list vector.
		//Use a string to initialize the regular expression.
		string str;
		//Since the dxpcon list are csv form, use the [,] as seperator and (.*) to retrieve the values of each variable.
		for(int i=1; i<=member_number; i++)
		{
			if(i==1)
				str = "(^.*)[,]";
			else if(i==member_number)
				str = str + "(.*)";
			else
				str = str + "(.*)[,]";
		}
		regex reg(str);
		smatch sm;
		regex_search(row, sm, reg);
		for(int i=0; i<member_number; i++)
			*member_list[i]	= sm[i+1];//The benefit of making a member_list vector.
	}
	
	//Construct which only initialize start time, finish time, file_name and readed flag when a dxpcon has never parsed.
	dxpcon(string x, string y, string z, string w):dxpcon(){
		*member_list[0] = x;
		*member_list[1] = y;
		*member_list[2] = z;
		*member_list[3] = w;
	}
	
	//Copy constructor
	dxpcon(const dxpcon &d):dxpcon()
	{
		log_file << "use copy constructor" << endl;
		for(int i=0; i<member_number; i++)
			*(member_list.at(i)) =  *((d.member_list).at(i));
	}
	
	
	int get_member_number(){
		return member_number;
	} 
	
	//Print all variables and its index. 
	void display(){
		int i = 0;
		for(vector<string*>::iterator iter=member_list.begin(); iter!=member_list.end(); iter++)
		{			
			i++;
			log_file << i <<':' << *(*iter) << endl;
		}
	}
	
	//Define operator = for sort algorithm.
	dxpcon operator = (const dxpcon &d)
	{
		log_file << "use assignment operator" << endl;
		if(this == &d)
			return *this;
		for(int i=0; i<member_number; i++)
			*(member_list.at(i)) =  *((d.member_list).at(i));
		return *this;
	}
	
	//Define operator < for sort algorithm.
	bool operator < (const dxpcon &d) const
	{
		log_file << "compare" << endl;
		return (stod(s_time.c_str(), NULL) < stod(d.s_time.c_str(), NULL));	
	}
	
};

void make_list(string file_path, vector<dxpcon>& check_list)//Read each row from file_path and use the row content to initialize a dxpcon element in check_list.
{
	ifstream in_file;
	in_file.open(file_path.c_str());

	if(!in_file)
		return;
	else
	{
		string row;
		getline(in_file, row);//Remove the head column
		while(getline(in_file, row))
		{
			//use the constructor dxpcon(string row)
			dxpcon element(row);
			check_list.push_back(element);	
		}
	}
	in_file.close();
	return;
}

void save_list(string file_path, vector<dxpcon>& check_list)
{
	remove(file_path.c_str());
	ofstream out_file;
	out_file.open(file_path.c_str());
	//Output the head column
	out_file << "start,finish,file_name,readed,search_keys,read_time,scale_mode,seq-ptr,par-ptr,time(left),plot-mode(left),job-name(left),lot-number(left),sequence-number(left),light-position(left),align-ring-light-0(left),align-ring-light-1(left),align-ring-light-2(left),align-vert-light-0(left),align-vert-light-1(left),align-vert-light-2(left),align-ring-light2-0(left),align-ring-light2-1(left),align-ring-light2-2(left),mark-type(left),mark-setting(left),meas-in-file(left),result(left),cam-path(left),time(right),plot-mode(right),job-name(right),lot-number(right),sequence-number(right),light-position(right),align-ring-light-0(right),align-ring-light-1(right),align-ring-light-2(right),align-vert-light-0(right),align-vert-light-1(right),align-vert-light-2(right),align-ring-light2-0(right),align-ring-light2-1(right),align-ring-light2-2(right),mark-type(right),mark-setting(right),meas-in-file(right),result(right),cam-path(right)" << endl;
	//record start time and finish time in a form of 3 decimal digits float number.
	for(vector<dxpcon>::iterator iter=check_list.begin(); iter!=check_list.end(); ++iter)
	{
		for(int i=0; i<iter->get_member_number(); i++)//In fact, the member_number should equals iter->member_list.size().
		{
			(i + 1) == iter->get_member_number()? log_file << *((iter->member_list).at(i)) << endl: log_file << *((iter->member_list).at(i)) << endl;
			(i + 1) == iter->get_member_number()? out_file << *((iter->member_list).at(i)) << endl: out_file << *((iter->member_list).at(i)) << ',';
		}
	}
	
	out_file.close();
	return;
}

bool check_readed(string target_file, vector<dxpcon>& check_list)
{
	for(vector<dxpcon>::iterator iter=check_list.begin(); iter!=check_list.end(); iter++)
	{
		if((iter->file_name == target_file)&&(iter->readed == "Y"))
			return true;
		else if((iter->file_name == target_file)&&(iter->readed == "N"))//If a file is unreaded, renew it for any possible modification after last checking.
		{
			check_list.erase(iter);	
			return false;
		}
	}
	return false;
	
}


//////////////////////////////////////////
//Find the correct backup file name of meas_in_file
//////////////////////////////////////////	

long long file_name_parser(string &file_name) //return 0 if file_name match reg_ini, date if file_name match reg_backup, and -1 if all fails.
{
	regex reg_ini("(.*)([.]ini)$"); //(.*) means a string of any length larger than 1 and with any character.
	regex reg_backup("(.*)-([0-9]{12})([.]backup)$");
	smatch match_result;
	if(regex_search(file_name, match_result, reg_ini))
	{
		file_name = match_result[1].str();
		return 0;
	}
	else if(regex_search(file_name, match_result, reg_backup))
	{
		file_name = match_result[1].str();
		return stoll(match_result[2].str());
	}
	return -1;
}

class hash_chain{
	public:
	hash_chain* next;
	string key;
	vector<long long> backup_history;
	hash_chain():next(NULL), key("head"){}
	hash_chain(int n):next(NULL), key("head"), backup_history(n){}
	~hash_chain(){delete next;}
	
	//If return false, the end_node will point to the last node of chain. If return true, the end_node will point to the node which has the target.
	bool search_key(string target, hash_chain* &end_node)
	{
		hash_chain* ptr = next;
		while(ptr != NULL)
		{
			end_node = end_node->next;
			if(ptr->key == target)
				return true;
			else
				ptr = ptr->next;

		}
		return false;
	}
	
	//Must insert from head
	void insert_key(string file_name)
	{
		size_t pos;
		hash_chain* ptr = this;
		long long date = file_name_parser(file_name);
		if(date == 0)
		{
			if(!this->search_key(file_name, ptr))
			{
				ptr->next = new hash_chain;
				ptr->next->key = file_name;
			}
			return;
		}
		else if(date > 0)
		{
				
			if(!this->search_key(file_name, ptr))//It still have to check whether the chain exist because backup file may insert earlier than its ini file.
			{	
				ptr->next = new hash_chain;
				ptr->next->key = file_name;
				ptr = ptr->next;
				log_file << ptr->key << endl; 
			}
			//ptr now points to the chain with target key
			auto iter=ptr->backup_history.begin();
			for(; iter!=ptr->backup_history.end(); iter++)
			{
				if(*iter == date)
					return;
				else if(*iter < date)
					break;
			}
			ptr->backup_history.insert(iter, date);
			return;

		}
		else
		{
			log_file << "Exception file_name: " << file_name << endl;
			system("pause"); 
			if(!this->search_key(file_name, ptr))
			{
				ptr->next = new hash_chain;
				ptr->next->key = file_name;
			}
			return;
		}
		
	}
};

long long GetCreateTime(string file)
{
	HANDLE hfile1;
	FILETIME ftCreate;
	SYSTEMTIME stUTC, stLocal;
	hfile1 = CreateFile(file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hfile1 == INVALID_HANDLE_VALUE)
	{
		log_file << "GetFileTime error," << file << endl;
		system("pause");
		log_file << "Could not open file," << file << ',' << GetLastError << endl;
		system("pause");
		return 0;	
	}
	if(!GetFileTime(hfile1, &ftCreate, NULL, NULL))
	{
		return 0;	
	}
	FileTimeToSystemTime(&ftCreate, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);//Convert system time from UTC to local time zone.
	long long date = stLocal.wYear*10000000000 + stLocal.wMonth*100000000 + stLocal.wDay*1000000 + stLocal.wHour*10000 + stLocal.wMinute*100 + stLocal.wSecond;
	CloseHandle(hfile1);
	return date;
}


int measure_hash(string file_name)//There is already some standard hash function in std-lib <functional> but I thought the date and number in the file name has better indication for sortting.
{
	file_name_parser(file_name);
	
	regex reg_exp("-[0-9]{8}([0-9]{4})$");
	smatch match_result;
		
	if(regex_search(file_name, match_result, reg_exp))
	{
		int min, sec;
		sscanf((match_result[1].str()).c_str(), "%2d%2d", &min, &sec);
		return (min + 1) * (sec + 1) % 3600;//The biggest possible result of (min+1) * (sec+1) equals 3600. After mod 3600, the index would be in range from 0-3599
	}
	else
	{
		int hash_index = 0;
		for(int i=0; i<file_name.size(); ++i)
		{
			if(isdigit(file_name[i]))
			{
				hash_index = hash_index * 10 + (int)(file_name[i]-'0');
				hash_index = hash_index % 1000;//Leave the last 3 digits only.
			}
		}
		return (hash_index + 3600);//The max index would be 3600+999 and if there is no digit, the index would be 3600.
	}
}

void output_hash_table(hash_chain hash_table[], int size, long long last_read_date, string file_path)//c++ alway pass array by passing pointer, so it always called by reference
{
	ofstream out_file;
	out_file.open(file_path.c_str());
	for(int i=0; i<size; i++)
	{
		out_file << "Index:" << i << endl;
		hash_chain *ptr = hash_table[i].next;
		while(ptr != NULL)
		{
			out_file << "Key:" << ptr->key;
			for(auto iter=ptr->backup_history.begin(); iter!=ptr->backup_history.end(); iter++)
				out_file << ',' << *iter; 
			out_file << endl; 
			ptr = ptr->next;
		}
	}
	out_file << "Record_date:" << last_read_date << endl;
	out_file.close();	
}

void input_hash_table(hash_chain hash_table[], int size, long long &record_date, string file_path)
{
	ifstream in_file;
	in_file.open(file_path.c_str());
	if(in_file)// for fstream, () operator has been overload to check the state of fstream.
	{
		string row;
		int hash_index;
		string key, date;
		long long backup_date;
		hash_chain* ptr;
		while(getline(in_file, row))
		{
			if(sscanf(row.c_str(), "Index:%d", &hash_index))
				ptr = &hash_table[hash_index];
			else if(row.find("Key:") != string::npos)
			{
				row = row.substr(4); // 4 is the length of "Key:"
				int backup_number = 0;
				transform(row.begin(), row.end(), row.begin(), [&](char s){
				if(s == ',')
				{
					backup_number++;
					return ' ';
				}
				else
					return s;	
				});
				//*************************************************************//
				//A file cantained ',' in its name will crash this input function.
				//A better way may be to choose an alternative seperator which can't be included in file_name rather than ','
				//or transfrom the ',' into anothor character which can't be included in file_name when processing and storage. 
				//*************************************************************//
				istringstream iss(row);
				iss >> key;
				ptr->next = new hash_chain(backup_number); //Avoid to use push_back(backup_date)
				ptr = ptr->next;
				ptr->key = key;
				for(vector<long long>::iterator iter=ptr->backup_history.begin(); iter!=ptr->backup_history.end(); iter++)
				{
					iss >> backup_date;
					*iter = backup_date;
				}
				
			}
			else if(sscanf(row.c_str(), "Record_date:%I64d", &record_date));
		}
	}
	/*else
	{
		log_file << "Fail to open:" << file_path << endl;
		system("pause"); 
	}*/ 
	
}

class date_to_int{
	
	public:
	int date[15];
	date_to_int(string str){
		int j = 0;
		for(int i=0; i<15; i++)
		{	
			date[i] = 0;
			for(; j<str.length(); j++)
			{
				if(isdigit(str[j]))
				{
					date[i] = (int)(str[j]-'0');
					j++;
					break;
				}
			}

		}
		
	}
	date_to_int(long long n):date_to_int(to_string(n)){
	}
	bool operator >= (const date_to_int d2){
		for(int i=0; i<15; i++){
			if(date[i]>d2.date[i])
				break;
			if(date[i]<d2.date[i])
				return false;	
		}
		return true;
		
	}
	void display(){
		for(int i=0; i<15; i++)
			log_file << date[i];
		log_file << endl;
	}
	
};

//////////////////////////////////////////
//Meas_in_file parser
//////////////////////////////////////////
string meas_parser(string file_path){
	ifstream meas_file;
	meas_file.open(file_path.c_str());
	string setting_value;
	string parameters[] = {
		"GrayFilter = Equalizing, ",
		"GrayFilter = GaussSampler, ",
		"GrayFilter = Dilating, ",
		"GrayFilter = Eroding, ", 
		"GrayFilter = AddConstant, ",
		"GrayFilter = MedianFilter, ",
		"Brightness = 0, ",
		"Contrast = 0, ",
		"Exposure = 0, ",
		"Brightness = 1, ",
		"Contrast = 1, ",
		"Exposure = 1, ",
		"Brightness = 2, ",
		"Contrast = 2, ",
		"Exposure = 2, ",
		"RunAlgorithm = 0, ",
		"AcceptThreshold = 0, ",
		"ContrastThreshold = 0, ",
		"RunAlgorithm = 1, ",
		"AcceptThreshold = 1, ",
		"ContrastThreshold = 1, ",
		"RunAlgorithm = 2, ",
		"AcceptThreshold = 2, ",
		"ContrastThreshold = 2, ",
		"CaliperEnable = ",
		"CaliperSearchDirection = ",
		"CaliperLength = ",
		"CaliperNum = ",
		"CaliperThreshold = ",
		"CaliperFilterSize = ",
		"CaliperPolarity = ",
		"CaliperSearchMethod = ",
		"ExtractContrastThreshold = ",
		"ExtractConnectTolerance = ",
		"ExtractPerimeterMin = ",
		"ExtractPerimeterMax = ",
		"ExtractApproximationTolerance = ",
		"ExtractPolarity = "
		};
	vector<string> parameter_list(parameters, end(parameters)); //In std::end, constexpr T* end( T (&array)[N] ) returns the pointer to the end of array

	if(meas_file)
	{
		vector<int>	search_number(parameter_list.size()); //Only the parameter_list elements which index were included in search_number would be searched. After a parameter_list's element has matched, its index would be removed from search_number for preventing redundant matching.
		for(int i=0; i<search_number.size(); ++i)
			search_number.at(i) = i;
		
		string row;
		while(getline(meas_file, row))
		{
			for(vector<int>::iterator iter=search_number.begin(); iter!=search_number.end(); ++iter)
			{
				size_t pos;
				if((pos = row.find(parameter_list.at(*iter))) != string::npos)
				{
					parameter_list.at(*iter) = row.substr(pos + parameter_list.at(*iter).length());
					search_number.erase(iter);
					break;
				}
				
			}
		
			if(search_number.size() <= 0)
				break;	
		}
		meas_file.close();
	
		for(vector<string>::iterator iter=parameter_list.begin(); iter!=parameter_list.end(); ++iter)
			(iter + 1) == parameter_list.end()? setting_value = setting_value + *iter: setting_value = setting_value + *iter +',';
	}
	else
	{
		log_file << "Can't find file:" << file_path << endl;
		//system("pause");
		for(vector<string>::iterator iter=parameter_list.begin(); iter!=parameter_list.end(); ++iter)
			(iter + 1) == parameter_list.end()? setting_value = setting_value + "Can't find file": setting_value = setting_value + "Can't find file" +',';
	}

	return setting_value;
}





int main(){
	log_file.open("C:\\Users\\lpp1\\Desktop\\ORC parser\\log.txt", ios_base::app);
//////////////////////////////////////////
//Copy all dxpon log from source directory to dest directory
//////////////////////////////////////////	
	DIR *dir;
	struct dirent* ent;
	string source_dir = "Z:\\DXP3301\\Log";
	string dest_dir = "C:\\Users\\lpp1\\Desktop\\ORC parser";
	string file_name;
	int file_number = 0;
	ifstream source_dxp;
	ofstream dest_dxp;
	//Read files in source directory
	ShowWindow( GetConsoleWindow(), SW_HIDE );
	if((dir = opendir(source_dir.c_str())) != NULL)
	{
		//If the source directory is not empty.
		while((ent = readdir(dir)) != NULL)
		{
			file_name = ent->d_name;
			if(file_name.find(".log")!=string::npos && file_name.find("dxpcon-")!=string::npos)
			{
				++file_number;
				//If the file name contains ".log" and "dxpcon-", copy file to destination directory.
				source_dxp.open((source_dir+"\\"+file_name).c_str(), ios_base::binary);
				//Create the destination directory if not exist.
				CreateDirectory(dest_dir.c_str(), NULL);
				dest_dxp.open((dest_dir+"\\"+file_name).c_str(), ios_base::binary);
				dest_dxp << source_dxp.rdbuf();
				//If the file with the same name already in the destination directory, it would be replaced by new one
				source_dxp.close();
				dest_dxp.close();
			}
		}
	}
	
//////////////////////////////////////////
//Sort the dxpcon log by the starting time it recorded.
//////////////////////////////////////////	
	
	//Make a list which records whether a dxpcon log is readed or not and the last alignment parameters after reading for initializing variables when reading the next dxpcon log.
	//The alignment parameters should contain target_par for left and right side and whether the *seq_ptr, *par_ptr point to right or left and search_keys, scale_mode, and read_time.
	string list_name = "dxpcon_list.txt";
	
	//Use a vector the record the informations(ex:start time) for each dxpcon  
	vector<dxpcon> check_list;
	check_list.reserve(file_number);
	if((dir = opendir(dest_dir.c_str())) != NULL)
	{
		//Read each row from list and use the row content to initialize a dxpcon element in check_list.
		make_list(list_name, check_list);
		while((ent = readdir(dir)) != NULL)
		{
			file_name = ent->d_name;
			if(file_name.find(".log")!=string::npos && file_name.find("dxpcon")!=string::npos)
			{	
				//Use the check_list to comfirm whether a dxpcon log has readed or not.
				if(!check_readed(file_name, check_list))
				{
					string s_time, f_time;
					ifstream in_file;
					in_file.open((dest_dir+"\\"+file_name).c_str());
					string row;
					while(getline(in_file, row))
					{
						int year, month, day, hour, min; double sec;//May use the tm struct
						stringstream ss;
						if(sscanf(row.c_str(), "%d%*[/]%d%*[/]%d %d%*[:]%d%*[:]%lf",&year, &month, &day, &hour, &min, &sec) == 6)
						{
							ss.fill('0');ss.width(2);ss << year ;
							ss.fill('0');ss.width(2); ss << month;
							ss.fill('0');ss.width(2); ss << day;
							ss.fill('0');ss.width(2); ss << hour;
							ss.fill('0');ss.width(2); ss << min;
							ss.fill('0');ss.width(6); ss << fixed << setprecision(3) << sec; //ss.with(6): set the minimun number of char on next input
																						 //fixed : control the number of decimal digits
							if(s_time.empty())
								s_time = ss.str();
							else
								f_time = ss.str();
						}
						ss.str("");
						ss.clear();

					}
					log_file << s_time << ',' << f_time << ',' << file_name << ",N" << endl;
					//If the file did not readed, get its start time and finish time and initialize a dxpcon object and then add into the check_list.
					dxpcon element(s_time, f_time, file_name, "N");
					log_file << "push back" << endl;
					//The push_back() will call the copy constructor to initialize a object than add to the vecotr
					//To prevent the default copy constructor from copying the address record in the pointer array: member_list, which will result the member_list doesn't point to its member
					//we need to overload the copy constructor 
					check_list.push_back(element);					 
					in_file.close();	
				}
			}
		}

		//Sort vector element by s_time (use operator <)
		sort(check_list.begin(), check_list.end());
		//The std::sort will sort by using operator <, so it need to be overloaded.
		//If the sequence need to be switched, it will call copy constructor to initialize a temp object and use operator = to perform assignment
		//To prevent copying the value  of pointer, we also need to overload copy constructor and operator =.
	}


//////////////////////////////////////////
//Parse the dxpcon
//////////////////////////////////////////	

	//Variables for dxpcon parser.
	target_par right_par, left_par, *seq_ptr, *par_ptr, *temp_ptr = NULL;
	vector<key> search_keys;
	string read_time;
	int scale_mode = 0;
	
	//Define the output directory.
	string left_dest_dir = dest_dir + "\\left";
	CreateDirectory(left_dest_dir.c_str(), NULL);
	
	string right_dest_dir = dest_dir + "\\right";
	CreateDirectory(right_dest_dir.c_str(), NULL);
	
	//Find the first unreaded file in list.
	vector<dxpcon>::iterator check_list_iter;
	if((check_list_iter = find_if(check_list.begin(), check_list.end(), [](dxpcon x){return(x.readed == "N");})) + 1 == check_list.end())
		return 0;//If only the last files is unreaded, end the program. We would not parse the last file because it still update by machine.
	else if(check_list_iter != check_list.begin())//If the unreaded file is not the first one, initialize variables for dxpcon parser by the members in precedent dxpcon object.
	{
		//initialize search_keys
		key_initialize(search_keys, (check_list_iter-1)->search_keys_list);
		
		//initialize read_time
		read_time = (check_list_iter-1)->read_time;
		
		//initialize scale_mode
		scale_mode = stoi((check_list_iter-1)->scale_mode);
		
		//initialize target_par
		left_par = (check_list_iter-1)->left;
		right_par = (check_list_iter-1)->right;
		
		//initialize par_ptr
		if((check_list_iter-1)->par_ptr == "left")
			par_ptr = &left_par;
		else if((check_list_iter-1)->par_ptr == "right")
			par_ptr = &right_par;	
		else
		{
			log_file << "The par_ptr in dxpcon list is neither left nor right." << endl;
			par_ptr = NULL;
		}
			
		//initialize seq_ptr
		if((check_list_iter-1)->seq_ptr == "left")
			seq_ptr = &left_par;
		else if((check_list_iter-1)->seq_ptr == "right")
			seq_ptr = &right_par;	
		else
		{
			log_file << "The seq_ptr in dxpcon list is neither left nor right." << endl;
			seq_ptr = NULL;
		}
	}
	else if(check_list_iter == check_list.begin())//If the unreaded file is the first one, initialize search_keys only.
	{
		//initialize search_keys
		key_initialize(search_keys, "");
	}
	else
	{
		log_file << "Check_list_iter points to check_list.end()" << endl;
		system("pause");
	}
	
	//From check_list_iter, parse the dxpcon log iteratively except the last one.
	for(; (check_list_iter+1)!=check_list.end(); check_list_iter++)
	{
		ifstream in_file;
		in_file.open((dest_dir+"\\"+check_list_iter->file_name).c_str());
		
		left_par.out_file_path = left_dest_dir + "\\" + (check_list_iter->file_name).substr(0, (check_list_iter->file_name).find(".log")) + "-left(dxpcon_parsed).log";
		right_par.out_file_path = right_dest_dir + "\\" + (check_list_iter->file_name).substr(0, (check_list_iter->file_name).find(".log")) + "-right(dxpcon_parsed).log";
		//Delete parsed out_file if exist.
		remove(left_par.out_file_path.c_str());
		remove(right_par.out_file_path.c_str());
		
		string row;
		while(getline(in_file, row))
		{
			int year, month, day, hour, min; double sec;
			if(sscanf(row.c_str(), "%d%*[/]%d%*[/]%d %d%*[:]%d%*[:]%lf", &year, &month, &day, &hour, &min, &sec) == 6)
			{
				//get time
				stringstream ss;
				ss.fill('0');ss.width(2);ss << year ;
				ss.fill('0');ss.width(2); ss << month;
				ss.fill('0');ss.width(2); ss << day;
				ss.fill('0');ss.width(2); ss << hour;
				ss.fill('0');ss.width(2); ss << min;
				ss.fill('0');ss.width(6); ss << fixed << setprecision(3) << sec;
			
				read_time = ss.str();
			
				ss.str("");
				ss.clear();
			}
		
			key match_key = key_cmp(search_keys, row);
			//log_file << match_key.key_name << endl; 
			if(match_key.key_name == ""){}// An empty key object mean doesn't match any keys in search_keys.
			else if(match_key.key_name == "scale_meas")
				scale_mode = 1;
			else if(match_key.key_name == "job_name")
			{
				//Clear search_key, right/left_par 
				key_clear(search_keys);
				right_par.clear();
				left_par.clear();
				//Check the plot mode
				if(scale_mode == 1)
					right_par.plot_mode = left_par.plot_mode = "Scale measure";
				else
					right_par.plot_mode = left_par.plot_mode = "Plot";
				scale_mode = 0;
			
				right_par.job_name = left_par.job_name = row;
		
				key_push_back(search_keys, right_recipe);
				key_push_back(search_keys, left_recipe);
				key_push_back(search_keys, right_counter);
				key_push_back(search_keys, left_counter);
				key_push_back(search_keys, right_mark_info);
				key_push_back(search_keys, left_mark_info);
			}
			else if((match_key.key_name == "right_recipe" && (par_ptr = &right_par)) || (match_key.key_name == "left_recipe" && (par_ptr = &left_par)))//(par_ptr = &right/left_par) returns true if par_ptr is not a null pointer. 
			{
				key_erase(search_keys, match_key);
			
				key_push_back(search_keys, align_ring_light_0);
				key_push_back(search_keys, align_ring_light_1);
				key_push_back(search_keys, align_ring_light_2);
				key_push_back(search_keys, align_vert_light_0);
				key_push_back(search_keys, align_vert_light_1);
				key_push_back(search_keys, align_vert_light_2);
				key_push_back(search_keys, align_ring_light2_0);
				key_push_back(search_keys, align_ring_light2_1);
				key_push_back(search_keys, align_ring_light2_2);
			
				key_push_back(search_keys, light_position);
				key_push_back(search_keys, meas_in_file);
				key_push_back(search_keys, right_enter_alignevent);
				key_push_back(search_keys, left_enter_alignevent);			
			}
			else if(match_key.key_name == "align_ring_light_0")
			{
				par_ptr->align_ring_light_0 = row;
			
				key_erase(search_keys, match_key);
			}
			else if(match_key.key_name == "align_ring_light_1")
			{
				par_ptr->align_ring_light_1 = row;
			
				key_erase(search_keys, match_key);
			}
			else if(match_key.key_name == "align_ring_light_2")
			{
				par_ptr->align_ring_light_2 = row;
			
				key_erase(search_keys, match_key);
			}
			else if(match_key.key_name == "align_vert_light_0")
			{
				par_ptr->align_vert_light_0 = row;
			
				key_erase(search_keys, match_key);
			}
			else if(match_key.key_name == "align_vert_light_1")
			{
				par_ptr->align_vert_light_1 = row;
			
				key_erase(search_keys, match_key);
			}
			else if(match_key.key_name == "align_vert_light_2")
			{
				par_ptr->align_vert_light_2 = row;
			
				key_erase(search_keys, match_key);
			}
			else if(match_key.key_name == "align_ring_light2_0")
			{
				par_ptr->align_ring_light2_0 = row;
			
				key_erase(search_keys, match_key);
			}
			else if(match_key.key_name == "align_ring_light2_1")
			{
				par_ptr->align_ring_light2_1 = row;
			
				key_erase(search_keys, match_key);
			}
			else if(match_key.key_name == "align_ring_light2_2")
			{
				par_ptr->align_ring_light2_2 = row;
			
				key_erase(search_keys, match_key);
			}
			else if(match_key.key_name == "light_postion")
			{
				par_ptr->light_position = row;
			
				key_erase(search_keys, match_key);
			}
			else if(match_key.key_name == "meas_in_file")
			{
				par_ptr->meas_in_file = row;
		
				key_erase(search_keys, match_key);
			}
			else if((match_key.key_name == "right_mark_info" && (par_ptr = &right_par)) || (match_key.key_name == "left_mark_info" && (par_ptr = &left_par)))
			{
				key_push_back(search_keys, decide_mark);
			}
			else if(match_key.key_name == "decide_mark")
			{
				string str;
				getline(in_file, str);
				char ch1[10], ch2[30];
				if(sscanf(str.c_str(), "%*[^=]%*[= ]%s%*[= ]%[0-9,. ]", ch1, ch2) == 2)
				{
					par_ptr->mark_type = ch1;
					par_ptr->mark_setting = ch2;
					par_ptr->mark_setting.erase(remove_if(par_ptr->mark_setting.begin(), par_ptr->mark_setting.end(), [](char s){return s==',';}), par_ptr->mark_setting.end());
				}
				else
					par_ptr->mark_type = par_ptr->mark_setting = "sscanf fail.";
			
				key_erase(search_keys, match_key);
			}
			else if((match_key.key_name == "right_enter_alignevent" && (temp_ptr = &right_par)) || (match_key.key_name == "left_enter_alignevent" && (temp_ptr = &left_par)))
			{
				if(key_erase(search_keys, exit_alignevent) && key_erase(search_keys, cam_saving) && par_ptr)
					par_ptr->output();
				par_ptr = temp_ptr;
				key_erase(search_keys, mark_not_found);
				par_ptr->cam_path.clear();
				par_ptr->time.clear();
				key_push_back(search_keys, mark);
				key_push_back(search_keys, exit_alignevent);
			}
			else if(match_key.key_name == "mark")
			{
				par_ptr->time = read_time;
				par_ptr->result = "OK";
				key_erase(search_keys, match_key);			
				key_push_back(search_keys, cam_saving);
				key_push_back(search_keys, mark_not_found);
			}
			else if(match_key.key_name == "mark_not_found")
			{
				par_ptr->result = "NG";
				key_erase(search_keys, match_key);
			}
			else if(match_key.key_name == "cam_saving")
			{
				par_ptr->cam_path = row;
				par_ptr->output(); 
			}
			else if(match_key.key_name == "exit_alignevent")
			{
				key_erase(search_keys, match_key);
				key_erase(search_keys, mark);
				key_erase(search_keys, mark_not_found);
				if(key_erase(search_keys, cam_saving) && par_ptr->cam_path == "")//The "key_erase(search_keys, cam_saving)" must put in the first condition to prevent from unexecuting if the fist condition is false already
				{
					par_ptr->output();
				}
	
			}
			else if((match_key.key_name == "right_counter" && (seq_ptr = &right_par)) || (match_key.key_name == "left_counter" && (seq_ptr = &left_par)))
			{
				key_push_back(search_keys, lot_number);
				key_push_back(search_keys, sequence);		
			}
			else if(match_key.key_name == "lot_number")
			{
				key_erase(search_keys, match_key);
				char ch[10];
				sscanf(row.c_str(),"\"%[^\"]",ch);
				seq_ptr->lot_number.assign(ch);
			}
			else if(match_key.key_name == "sequence")
			{
				key_erase(search_keys, match_key);
				char ch[10];
				sscanf(row.c_str(),"\"%[^\"]",ch);
				seq_ptr->sequence_number.assign(ch);
			}

		}
		
		//Record the last settings of dxpcon object
		check_list_iter->readed = 'Y';
		
		check_list_iter->search_keys_list.clear();
		for(vector<key>::iterator iter=search_keys.begin(); iter!=search_keys.end(); iter++)
			(iter + 1) != search_keys.end()? check_list_iter->search_keys_list = check_list_iter->search_keys_list + iter->key_name + ' ': check_list_iter->search_keys_list = check_list_iter->search_keys_list + iter->key_name;
		
		check_list_iter->read_time = read_time;
		check_list_iter->scale_mode = to_string(scale_mode);
		
		if(seq_ptr == &left_par)
			check_list_iter->seq_ptr = "left";
		else if(seq_ptr == &right_par)
			check_list_iter->seq_ptr = "right";
		else
			check_list_iter->seq_ptr = "";
			
		if(par_ptr == &left_par)
			check_list_iter->par_ptr = "left";
		else if(par_ptr == &right_par)
			check_list_iter->par_ptr = "right";
		else
			check_list_iter->par_ptr = "";
		
		check_list_iter->left = left_par;
		check_list_iter->right = right_par;
		

		//set the check_list_iter->dxpcon element 
		in_file.close();
	}
	
	//Save the sorted dxpcon log list to a file named by list_name.
	save_list(list_name, check_list);
	//system("pause");
//////////////////////////////////////////
//Find the correct backup file name of meas_in_file
//////////////////////////////////////////	
	
	//Each time we want to add meas_in_files into hash table, we only include those create time larger than record_date in hash table.
	long long record_date = 20160101000000;//64 bit integer
	long long last_read_date;
	
	int hash_index;
	hash_chain left_hash_table[4600];//Index from 0-4599
	hash_chain right_hash_table[4600];
	
	string left_meas_dir = "Z:\\DXP3301\\Settings\\left\\Alignment";
	string right_meas_dir = "Z:\\DXP3301\\Settings\\right\\Alignment";	
	
	string left_hash_file = "hash_table(left).csv";
	string right_hash_file = "hash_table(right).csv";
	
	//Make the hash table of left side
	input_hash_table(left_hash_table, 4600, record_date, left_hash_file); 
	last_read_date = record_date;
		
	//Sorting Meas_in_files and storage in a hash chain table 
	if((dir = opendir(left_meas_dir.c_str())) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			string file_name = ent->d_name;	
			if(file_name.find(".ini") != string::npos || file_name.find(".backup") != string::npos)
			{
				if(file_name.find(',') != string::npos)
				{
					log_file << "The file name:<" << file_name << "> contains ',' which will crush this program. Please rename the file name and restart." << endl;
					return 1;
				}
				//Get create time of each meas_in_file 
				long long read_date = GetCreateTime(left_meas_dir+"\\"+file_name);
				if(read_date >  last_read_date)
					last_read_date = read_date;
					
				if(read_date > record_date)
				{
					hash_index = measure_hash(file_name);
					left_hash_table[hash_index].insert_key(file_name);
				}

			}
		}			
	}
	output_hash_table(left_hash_table, 4600, last_read_date, left_hash_file);
	
	//Make the hash table of right side
	record_date = 20160101000000;
	
	input_hash_table(right_hash_table, 4600, record_date, right_hash_file);
	last_read_date = record_date;
		
	//Sorting Meas_in_files and storage in a hash chain table 
	if((dir = opendir(right_meas_dir.c_str())) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			string file_name = ent->d_name;	
			if(file_name.find(".ini") != string::npos || file_name.find(".backup") != string::npos)
			{
				if(file_name.find(',') != string::npos)
				{
					log_file << "The file name:<" << file_name << "> contains ',' which will crush this program. Please rename the file name and restart." << endl;
					return 1;
				}
				//Get create time of each meas_in_file 
				long long read_date = GetCreateTime(right_meas_dir+"\\"+file_name);
				if(read_date >  last_read_date)
					last_read_date = read_date;
					
				if(read_date > record_date)
				{
					hash_index = measure_hash(file_name);
					right_hash_table[hash_index].insert_key(file_name);
				}

			}
		}			
	}
	output_hash_table(right_hash_table, 4600, last_read_date, right_hash_file);
	
	//Substitute the correct meas_in_file name of left side dxpcon log
	if((dir = opendir(left_dest_dir.c_str())) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			file_name = ent->d_name;
			size_t pos;
			if((pos = file_name.find("-left(dxpcon_parsed).log"))!=string::npos && file_name.find("dxpcon-")!=string::npos)
			{
				ifstream in_file;
				in_file.open((left_dest_dir+"\\"+file_name).c_str());
				ofstream out_file;
				out_file.open((left_dest_dir+"\\"+file_name.substr(0, pos)+"-left(correct_meas_in_file).log").c_str());
				
				string row;
				getline(in_file, row);//parse the header line
	
				int time_column = 0, meas_column = 17, i = 0;
				do{
					pos = row.find(',');
					string column_name = row.substr(0, pos);//If pos = npos, indicates all character until the end of the string.
					column_name == "time"? time_column = i : (column_name == "meas_in_file"? meas_column = i : true);
					//equal if(column_name == "time"){time_column = i;) else if(column_name == "meas_in_file"){meas_column = i;) else {true;}
					pos != string::npos? out_file << column_name << ',' : out_file << column_name << endl;
					i++;
					row.erase(0,pos+1);//If pos = npos, pos+1 = -1+1 = 0, which means row.erase(0,pos+1) equals row.erase(0,0) 		
				}while(pos != string::npos);
	
	
				while(getline(in_file, row))
				{
					i = 0;
					string time, meas_in_file; 
					do{
						pos = row.find(',');
						string column_content = row.substr(0, pos);
						if(i == time_column)
							time = column_content; //assume the time_column is always less than meas_column
						if(i == meas_column)
						{
							meas_in_file = column_content;
							hash_index = measure_hash(meas_in_file);
							if(file_name_parser(meas_in_file) < 0)
							{
								log_file << "Exception file_name:" << meas_in_file << endl;
								system("pause");	
							}		
							hash_chain* ptr = &left_hash_table[hash_index];
							if(left_hash_table[hash_index].search_key(meas_in_file, ptr))
							{
								string closest_backup = "";
								//Since time is a 15 digits number, compare time with backup date one by one digit to prevent overflow. 
								date_to_int date(time);
								for(vector<long long>::iterator iter=ptr->backup_history.begin(); iter!=ptr->backup_history.end(); iter++)
								{
									date_to_int backup(*iter);
									if(backup >= date)
										closest_backup = to_string(*iter);
									else
										break;	
								}
				
								if(closest_backup != "")
									column_content = meas_in_file + '-' + closest_backup + ".backup";
							}
							else
								column_content = "Can't find file.";
						}
		
						pos != string::npos? out_file << column_content << ',' : out_file << column_content << endl;
						i++;
						row.erase(0,pos+1);//If pos = npos, pos+1 = -1+1 = 0, which means row.erase(0,pos+1) equals row.erase(0,0) 		
					}while(pos != string::npos);
				
				}				
				in_file.close();
				out_file.close();
			}
		}
	}
	
	//Substitute the correct meas_in_file name of right side dxpcon log
	if((dir = opendir(right_dest_dir.c_str())) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			file_name = ent->d_name;
			size_t pos;
			if((pos = file_name.find("-right(dxpcon_parsed).log"))!=string::npos && file_name.find("dxpcon-")!=string::npos)
			{
				ifstream in_file;
				in_file.open((right_dest_dir+"\\"+file_name).c_str());
				ofstream out_file;
				out_file.open((right_dest_dir+"\\"+file_name.substr(0, pos)+"-right(correct_meas_in_file).log").c_str());
				
				string row;
				getline(in_file, row);//parse the header line
	
				int time_column = 0, meas_column = 17, i = 0;
				do{
					pos = row.find(',');
					string column_name = row.substr(0, pos);//If pos = npos, indicates all character until the end of the string.
					column_name == "time"? time_column = i : (column_name == "meas_in_file"? meas_column = i : true);
					//equal if(column_name == "time"){time_column = i;) else if(column_name == "meas_in_file"){meas_column = i;) else {true;}
					pos != string::npos? out_file << column_name << ',' : out_file << column_name << endl;
					i++;
					row.erase(0,pos+1);//If pos = npos, pos+1 = -1+1 = 0, which means row.erase(0,pos+1) equals row.erase(0,0) 		
				}while(pos != string::npos);
	
	
				while(getline(in_file, row))
				{
					i = 0;
					string time, meas_in_file; 
					do{
						pos = row.find(',');
						string column_content = row.substr(0, pos);
						if(i == time_column)
							time = column_content; //assume the time_column is always less than meas_column
						if(i == meas_column)
						{
							meas_in_file = column_content;
							hash_index = measure_hash(meas_in_file);
							if(file_name_parser(meas_in_file) < 0)
							{
								log_file << "Exception file_name:" << meas_in_file << endl;
								system("pause");	
							}		
							hash_chain* ptr = &right_hash_table[hash_index];
							if(right_hash_table[hash_index].search_key(meas_in_file, ptr))
							{
								string closest_backup = "";
								//Since time is a 15 digits number, compare time with backup date one by one digit to prevent overflow. 
								date_to_int date(time);
								for(vector<long long>::iterator iter=ptr->backup_history.begin(); iter!=ptr->backup_history.end(); iter++)
								{
									date_to_int backup(*iter);
									if(backup >= date)
										closest_backup = to_string(*iter);
									else
										break;	
								}
				
								if(closest_backup != "")
									column_content = meas_in_file + '-' + closest_backup + ".backup";
							}
							else
								column_content = "Can't find file.";
						}
		
						pos != string::npos? out_file << column_content << ',' : out_file << column_content << endl;
						i++;
						row.erase(0,pos+1);//If pos = npos, pos+1 = -1+1 = 0, which means row.erase(0,pos+1) equals row.erase(0,0) 		
					}while(pos != string::npos);
				
				}				
				in_file.close();
				out_file.close();
			}
		}
	}
	
	
	
	
	
	
//////////////////////////////////////////
//Mark setting parser
//////////////////////////////////////////
	
	//Parse the left side log	
	if((dir = opendir(left_dest_dir.c_str())) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			file_name = ent->d_name;
			size_t pos;
			if((pos = file_name.find("-left(correct_meas_in_file).log"))!=string::npos && file_name.find("dxpcon-")!=string::npos)
			{
				ifstream in_file;
				in_file.open((left_dest_dir+"\\"+file_name).c_str());
				ofstream out_file;
				out_file.open((left_dest_dir+"\\"+file_name.substr(0, pos)+"-left(mark_setting_parsed).log").c_str());
				
				string row;
				getline(in_file, row);//parse the header line
	
				int mark_type_column = 15, mark_setting_column = 16, i = 0;
				do{
					pos = row.find(',');
					string column_name = row.substr(0, pos);
					if(column_name == "mark_type")
						mark_type_column = i;
			
					if(column_name == "mark_setting")
					{
						mark_setting_column = i;
						column_name = "RingMark-Circle_Radius,RingMark-Number_of_Circle,RingMark-Collections_Radius,RingMark-Polarity,FreeMark-Circle_Radius,FreeMark-Polarity";
					}
		
					pos != string::npos? out_file << column_name << ',' : out_file << column_name << endl;
					i++;
					row.erase(0,pos+1);//If pos = npos, pos+1 = -1+1 = 0, which means row.erase(0,pos+1) equals row.erase(0,0) 		
				}while(pos != string::npos);
	
				while(getline(in_file, row))
				{
					i = 0;
					int Ring_or_Free = 0;
					do{
						pos = row.find(',');
						string column_content = row.substr(0, pos);
						if(i == mark_type_column)
							column_content == "RingMark"? Ring_or_Free = -1: column_content == "FreeMark"? Ring_or_Free =1: Ring_or_Free = 0; 
						if(i == mark_setting_column)
						{
							istringstream iss(column_content);
							if(Ring_or_Free == -1)//-1 : RingMark
							{
								double value1 = 0.0, value4 = 0.0;
								int value2 = 0, value3 = 0, value5  = 0;
								iss >> value1;
								iss >> value2;
								iss >> value3;
								iss >> value4;
								iss >> value5;
					
								out_file << value1 << ',' << value3 << ',' << value4/2 << ',' << value5 << ",NA,NA,";
							}
							else if(Ring_or_Free == 1)//1 : FreeMark
							{
								int value1 = 0, value2 = 0, value4 = 0;
								double value3 = 0.0;
								iss >> value1;
								iss >> value2;
								iss >> value3;
								iss >> value4;
					
								out_file << "NA,NA,NA,NA," << value3 << ',' << value4 << ',';
							}
							else//If sscanf in dxpcon fail, it may contain "sscanf fail"
								out_file << "NA,NA,NA,NA,NA,NA,";
							iss.str("");
							iss.clear();	
						}
						else
							pos != string::npos? out_file << column_content << ',' : out_file << column_content << endl;
						i++;
						row.erase(0,pos+1);//If pos = npos, pos+1 = -1+1 = 0, which means row.erase(0,pos+1) equals row.erase(0,0) 		
					}while(pos != string::npos);		
				}
				in_file.close();
				out_file.close();
			}
		}
	}
	
	//Parse the right side log
	if((dir = opendir(right_dest_dir.c_str())) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			file_name = ent->d_name;
			size_t pos;
			if((pos = file_name.find("-right(correct_meas_in_file).log"))!=string::npos && file_name.find("dxpcon-")!=string::npos)
			{
				ifstream in_file;
				in_file.open((right_dest_dir+"\\"+file_name).c_str());
				ofstream out_file;
				out_file.open((right_dest_dir+"\\"+file_name.substr(0, pos)+"-right(mark_setting_parsed).log").c_str());
				
				string row;
				getline(in_file, row);//parse the header line
	
				int mark_type_column = 15, mark_setting_column = 16, i = 0;
				do{
					pos = row.find(',');
					string column_name = row.substr(0, pos);
					if(column_name == "mark_type")
						mark_type_column = i;
			
					if(column_name == "mark_setting")
					{
						mark_setting_column = i;
						column_name = "RingMark-Circle_Radius,RingMark-Number_of_Circle,RingMark-Collections_Radius,RingMark-Polarity,FreeMark-Circle_Radius,FreeMark-Polarity";
					}
		
					pos != string::npos? out_file << column_name << ',' : out_file << column_name << endl;
					i++;
					row.erase(0,pos+1);//If pos = npos, pos+1 = -1+1 = 0, which means row.erase(0,pos+1) equals row.erase(0,0) 		
				}while(pos != string::npos);
	
				while(getline(in_file, row))
				{
					i = 0;
					int Ring_or_Free = 0;
					do{
						pos = row.find(',');
						string column_content = row.substr(0, pos);
						if(i == mark_type_column)
							column_content == "RingMark"? Ring_or_Free = -1: column_content == "FreeMark"? Ring_or_Free =1: Ring_or_Free = 0; 
						if(i == mark_setting_column)
						{
							istringstream iss(column_content);
							if(Ring_or_Free == -1)//-1 : RingMark
							{
								double value1 = 0.0, value4 = 0.0;
								int value2 = 0, value3 = 0, value5  = 0;
								iss >> value1;
								iss >> value2;
								iss >> value3;
								iss >> value4;
								iss >> value5;
					
								out_file << value1 << ',' << value3 << ',' << value4/2 << ',' << value5 << ",NA,NA,";
							}
							else if(Ring_or_Free == 1)//1 : FreeMark
							{
								int value1 = 0, value2 = 0, value4 = 0;
								double value3 = 0.0;
								iss >> value1;
								iss >> value2;
								iss >> value3;
								iss >> value4;
					
								out_file << "NA,NA,NA,NA," << value3 << ',' << value4 << ',';
							}
							else//If sscanf in dxpcon fail, it may contain "sscanf fail"
								out_file << "NA,NA,NA,NA,NA,NA,";
							iss.str("");
							iss.clear();	
						}
						else
							pos != string::npos? out_file << column_content << ',' : out_file << column_content << endl;
						i++;
						row.erase(0,pos+1);//If pos = npos, pos+1 = -1+1 = 0, which means row.erase(0,pos+1) equals row.erase(0,0) 		
					}while(pos != string::npos);		
				}
				in_file.close();
				out_file.close();
			}
		}
	}
	
//////////////////////////////////////////
//Meas_in_file parser
//////////////////////////////////////////	
	
	//Parse the left side log	
	if((dir = opendir(left_dest_dir.c_str())) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			file_name = ent->d_name;
			size_t pos;
			if((pos = file_name.find("-left(mark_setting_parsed).log"))!=string::npos && file_name.find("dxpcon-")!=string::npos)
			{
				ifstream in_file;
				in_file.open((left_dest_dir+"\\"+file_name).c_str());
				ofstream out_file;
				out_file.open((left_dest_dir+"\\"+file_name.substr(0, pos)+"-left(meas_in_file_parsed).log").c_str());
				
				string row;
				getline(in_file, row);//parse the header line
				int mark_type_column = 22, i = 0;
				do{
					pos = row.find(',');
					string column_name = row.substr(0, pos);
					if(column_name == "meas_in_file")
					{
						mark_type_column = i;
						column_name = "GrayFilter_Equalizing,GrayFilter_GaussSampler,GrayFilter_Dilating,GrayFilter_Eroding,GrayFilter_AddConstant,GrayFilter_MedianFilter,";
						column_name = column_name + "Brightness_0,Contrast_0,Exposure_0,Brightness_1,Contrast_1,Exposure_1,Brightness_2,Contrast_2,Exposure_2,";
						column_name = column_name + "RunAlgorithm_0,AcceptThreshold_0,ContrastThreshold_0,RunAlgorithm_1,AcceptThreshold_1,ContrastThreshold_1,RunAlgorithm_2,AcceptThreshold_2,ContrastThreshold_2,";
						column_name = column_name + "CaliperEnable,CaliperSearchDirection,CaliperLength,CaliperNum,CaliperThreshold,CaliperFilterSize,CaliperPolarity,CaliperSearchMethod,";
						column_name = column_name + "ExtractContrastThreshold,ExtractConnectTolerance,ExtractPerimeterMin,ExtractPerimeterMax,ExtractApproximationTolerance,ExtractPolarity";
					}
					pos != string::npos? out_file << column_name << ',' : out_file << column_name << endl;
					i++;
					row.erase(0,pos+1);//If pos = npos, pos+1 = -1+1 = 0, which means row.erase(0,pos+1) equals row.erase(0,0) 		
				 }while(pos != string::npos);
				
				while(getline(in_file, row))
				{
					i = 0;		
					do{
					pos = row.find(',');
					string column_content = row.substr(0, pos);
					if(i == mark_type_column)
					{
						string file_path = left_meas_dir + "\\" + column_content;
						column_content = meas_parser(file_path);			
					}	
					pos != string::npos? out_file << column_content << ',' : out_file << column_content << endl;
					i++;
					row.erase(0,pos+1);//If pos = npos, pos+1 = -1+1 = 0, which means row.erase(0,pos+1) equals row.erase(0,0) 		
					}while(pos != string::npos);	
				}
				
				
				in_file.close();
				out_file.close();
			}
		}
	}
	
	//Parse the right side log	
	if((dir = opendir(right_dest_dir.c_str())) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			file_name = ent->d_name;
			size_t pos;
			if((pos = file_name.find("-right(mark_setting_parsed).log"))!=string::npos && file_name.find("dxpcon-")!=string::npos)
			{
				ifstream in_file;
				in_file.open((right_dest_dir+"\\"+file_name).c_str());
				ofstream out_file;
				out_file.open((right_dest_dir+"\\"+file_name.substr(0, pos)+"-left(meas_in_file_parsed).log").c_str());
				
				string row;
				getline(in_file, row);//parse the header line
				int mark_type_column = 22, i = 0;
				do{
					pos = row.find(',');
					string column_name = row.substr(0, pos);
					if(column_name == "meas_in_file")
					{
						mark_type_column = i;
						column_name = "GrayFilter_Equalizing,GrayFilter_GaussSampler,GrayFilter_Dilating,GrayFilter_Eroding,GrayFilter_AddConstant,GrayFilter_MedianFilter,";
						column_name = column_name + "Brightness_0,Contrast_0,Exposure_0,Brightness_1,Contrast_1,Exposure_1,Brightness_2,Contrast_2,Exposure_2,";
						column_name = column_name + "RunAlgorithm_0,AcceptThreshold_0,ContrastThreshold_0,RunAlgorithm_1,AcceptThreshold_1,ContrastThreshold_1,RunAlgorithm_2,AcceptThreshold_2,ContrastThreshold_2,";
						column_name = column_name + "CaliperEnable,CaliperSearchDirection,CaliperLength,CaliperNum,CaliperThreshold,CaliperFilterSize,CaliperPolarity,CaliperSearchMethod,";
						column_name = column_name + "ExtractContrastThreshold,ExtractConnectTolerance,ExtractPerimeterMin,ExtractPerimeterMax,ExtractApproximationTolerance,ExtractPolarity";
					}
					pos != string::npos? out_file << column_name << ',' : out_file << column_name << endl;
					i++;
					row.erase(0,pos+1);//If pos = npos, pos+1 = -1+1 = 0, which means row.erase(0,pos+1) equals row.erase(0,0) 		
				 }while(pos != string::npos);
				
				while(getline(in_file, row))
				{
					i = 0;		
					do{
					pos = row.find(',');
					string column_content = row.substr(0, pos);
					if(i == mark_type_column)
					{
						string file_path = right_meas_dir + "\\" + column_content;
						column_content = meas_parser(file_path);			
					}	
					pos != string::npos? out_file << column_content << ',' : out_file << column_content << endl;
					i++;
					row.erase(0,pos+1);//If pos = npos, pos+1 = -1+1 = 0, which means row.erase(0,pos+1) equals row.erase(0,0) 		
					}while(pos != string::npos);	
				}
				
				
				in_file.close();
				out_file.close();
			}
		}
	}		
}

