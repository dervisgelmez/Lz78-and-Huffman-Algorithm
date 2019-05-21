// compression.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <vector>
#include <sstream>
#include "windows.h"
#include <unordered_map>
using namespace std;

void karHesapla(string dosya)
{
	cout<<endl;
	long l,m; 
	ifstream file ("dosya.txt", ios::in|ios::binary); 
	l = file.tellg(); 
	file.seekg (0, ios::end); 
	m = file.tellg(); 
	file.close(); 
	cout <<endl<< "     Sýkýþtýrýlmýþ Dosyanýn Boyutu : "<<(m-l)<<"bytedýr."; 
	long l2,m2; 
	ifstream file2 (dosya+".txt", ios::in|ios::binary); 
	l2 = file2.tellg(); 
	file2.seekg (0, ios::end); 
	m2 = file2.tellg(); 
	file2.close(); 
	cout <<endl<< "     Orjinal Dosyanýn Boyutu : "<<(m2-l)<<"bytedýr."<<endl; 
	double kazanc;
	kazanc=(float(m2-1)/float(m-1))*10;
	cout<<"     Sýkýþtýrmadan Kazanýlan Pay : "<<kazanc<<endl;
}

struct NodeHuffman{
	char ch;
	int freq;
	NodeHuffman *left, *right;
};

NodeHuffman* getNode(char ch, int freq, NodeHuffman* left, NodeHuffman* right)
{
	NodeHuffman* node = new NodeHuffman();
	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;
	return node;
}

struct comp
{
	bool operator()(NodeHuffman* l, NodeHuffman* r)
	{
		return l->freq > r->freq;
	}
};

void encode(NodeHuffman* root, string str,
			unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr)
		return;
	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}


void decode(NodeHuffman* root, int &index, string str)
{
	if (root == nullptr) {
		return;
	}

	if (!root->left && !root->right)
	{
		cout << root->ch;
		return;
	}

	index++;

	if (str[index] =='0')
		decode(root->left, index, str);
	else
		decode(root->right, index, str);
}


void buildHuffmanTree(string text,int secim)
{
	

	unordered_map<char, int> freq;
	for (char ch: text) {
		freq[ch]++;
	}

	priority_queue<NodeHuffman*, vector<NodeHuffman*>, comp> pq;

	for (auto pair: freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	while (pq.size() != 1)
	{

		NodeHuffman *left = pq.top(); pq.pop();
		NodeHuffman *right = pq.top();	pq.pop();

		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	NodeHuffman* root = pq.top();

	unordered_map<char, string> huffmanCode;
	encode(root, "", huffmanCode);

	if (secim==1)
	{
		cout <<endl<< "     Huffman Algoritmasý" <<endl<<"     -------------------"<<endl;
		for (auto pair: huffmanCode) {
			cout <<"     "<<pair.first << " " << pair.second << '\n';
		}
	}
	else
	{
		cout <<endl<< "     Huffman Algoritmasý" <<endl<<"     -------------------"<<endl;
		for (auto pair: huffmanCode) {
			cout <<"     "<<pair.first << " " << pair.second << '\n';
		}
		string str = "";
		for (char ch: text) {
			str += huffmanCode[ch];
		}

		
		fstream dosya;
		dosya.open("encodehuffman.txt",ios::out|ios::binary);
		dosya<<str;
		dosya.close();

		int index = -1;
		cout <<endl<<"     Çözümlenen metnin içeriði : ";
		while (index < (int)str.size() - 1) {
			decode(root, index, str);
		}
		karHesapla("encodehuffman");
	}

}


struct NodeLzw{
	int index;
	string data;
	NodeLzw *next;
};

void st_Node(NodeLzw *head, int index, string data){
	head->index = index;
	head->data = data;
	head->next = NULL;
}

void insert_Node(NodeLzw *head, int index, string data){
	NodeLzw *new_Node = new NodeLzw;
	new_Node->index = index;
	new_Node->data = data;
	new_Node->next = NULL;

	NodeLzw *curr = head;
	while (curr != NULL)
	{
		if (curr->next == NULL)
		{
			curr->next = new_Node;
			return;
		}
		curr = curr->next;
	}
}

NodeLzw *search_Node(NodeLzw *head, string data)
{
	NodeLzw *curr = head;
	while (curr != NULL)
	{
		if (data.compare(curr->data) == 0)
			return curr;
		else
			curr = curr->next;
	}
	return NULL;
}

NodeLzw *search_Node(NodeLzw *head, int index)
{
	NodeLzw *curr = head;
	while (curr != NULL)
	{
		if (index == curr->index)
			return curr;
		else
			curr = curr->next;
	}
	return NULL;
}

bool delete_Node(NodeLzw *head, NodeLzw *to_delete){
	if (to_delete == NULL)
		return false;
	else if (to_delete == head)
	{
		head = to_delete->next;
		delete to_delete;
		return true;
	}
	else{
		NodeLzw *curr = head;
		while (curr)
		{
			if (curr->next == to_delete)
			{
				curr->next = to_delete->next;
				delete to_delete;
				return true;
			}
			curr = curr->next;
		}
		return false;
	}
}

vector <string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}
string LZ78(string input, int option);



void lzwCode(string veri, int secim)
{
	string result,input;
	fstream dosya;
	cout <<endl<< "     LZW Algoritmasý" <<endl<<"     -------------------"<<endl;

	if (secim == 1)
	{
		result = LZ78(veri, 1);
		dosya.open("encodeLzw.txt",ios::out|ios::binary);
		dosya<<result;
		dosya.close();
	}
	dosya.open("encodeLzw.txt",ios::in|ios::out|ios::app);
	getline(dosya,input);
	cout<<"     "<<input;
		if(secim==2)
	{
		result = LZ78(input, 2);
		cout <<endl<<"     Çözümlenen metnin içeriði : " << result << endl;
		karHesapla("encodeLzw");
	}

}



int main(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Turkish"); 
	string veri;
	string tempveri;
	fstream dosya;
	dosya.open("dosya.txt",ios::in|ios::app|ios::out);
	while (getline(dosya,tempveri))
	{
		veri+=tempveri;	
	}	
	dosya.close();

	if (veri!="")
	{
		int karar,secim;
		do
		{
			system("cls");
			cout<<"dosya.txt içerisindeki veriyi nasýl sýkýþtýracaðýnýz seçiniz..."<<endl<<"------------------------------------------------------------"<<endl<<endl;
			cout<<"     1 -> Huffman ile sýkýþtýr"<<endl<<"     2 -> Huffman ile çözümle"<<endl<<"     3 -> LZ78 ile sýkýþtýr"<<endl<<"     4 -> LZ78 ile çözümle"<<endl<<"     5 -> Dosyanýn içeriðini göster"<<endl;
			cout<<endl<<"     Seçim : ";cin>>secim;cout<<endl;
			switch (secim)
			{
			case 1:
				buildHuffmanTree(veri,1);
				break;
			case 2:
				buildHuffmanTree(veri,2);
				break;
			case 3:
				lzwCode(veri,1);
				break;
			case 4:
				lzwCode(veri,2);
				break;
			default:
				cout<<"     Dosyanýn içeriði"<<endl<<"     ----------------------------"<<endl<<veri;
				break;
			}

			cout<<endl<<endl<<endl<<"     Devam etmek için 1'e basýnýz -> ";
			cin>>karar;
		} while (karar==1);
	}
	else
	{
		cout<<"Dosyanýzýn içeriði boþtur."<<endl;
		system("pause");
	}
	return 0;
}

string LZ78(string input, int option)
{
	if (option == 1)
	{
		NodeLzw *dictionary = new NodeLzw;
		string word, result;
		int length, last_seen, index = 1;

		length = (int)input.length();
		word = input[0];
		st_Node(dictionary, 1, word);
		result += "0," + word;

		for (int i = 1; i < length; i++)
		{
			string data;
			data = input[i];

		re_check:
			NodeLzw *search = search_Node(dictionary, data);

			if (search)
			{
				i++;
				data += input[i];
				last_seen = search->index;
				goto re_check;
			}
			else
			{
				char zero;
				if (input[i] == ' ')
					zero = '0';
				else
					zero = input[i];

				if ((int)data.length() < 2)
					result += " " + to_string(0) + "," + zero;
				else
					result += " " + to_string(last_seen) + "," + zero;

				index++;
				if (i != length)
					insert_Node(dictionary, index, data);
			}
		}

		return result;
	}
	if (option == 2)
	{
		NodeLzw *dictionary = new NodeLzw;
		string result;

		vector <string> s_input = split(input, ' ');
		int zz = 2;
		for (int i = 0; i < s_input.size(); i++)
		{
			vector <string> ss_input = split(s_input[i], ',');

			if (i == 0)
			{
				st_Node(dictionary, 1, ss_input[1]);
				result += ss_input[1];
			}
			else
			{
				NodeLzw *serched;
				string get_search = ss_input[1];
				serched = search_Node(dictionary, stoi(ss_input[0]));
				if (serched)
				{
					result += serched->data + get_search;
					get_search = serched->data + split(s_input[i], ',')[1];
					insert_Node(dictionary, zz, get_search);
				}
				else
				{
					if (stoi(ss_input[0]) == 0)
						insert_Node(dictionary, zz, get_search);
					else
						insert_Node(dictionary, zz, get_search);

					result += get_search;
				}
				zz++;
			}
		}

		if (result[(int)result.length() - 1] == '0')
			result = result.substr(0, result.size() - 1);
		
		return result;
	}
}

