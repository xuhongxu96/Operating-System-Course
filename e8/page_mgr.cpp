#include <iostream>
#include <iomanip>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

const int N = 64;

struct Page{
	int page_id;
	int disk_pos;
	int mem_pos = -1;
	int flag = 0;
	int w_flag = 0;
}pages[N];

int mem[N];
int ri = 0; // the earliest swapped page id
int m = 0, n = 0; // memory block count, page count

void swapPage(int p, int v) {
	cout << "����" << mem[ri] << "ҳ ����" << endl;
	if (pages[mem[ri]].w_flag) {
		cout << "��ҳ���޸ģ�д��Ӳ�̵�" << pages[mem[ri]].disk_pos << "��" << endl;
	}
	cout << "����" << pages[p].disk_pos << "��Ӳ�̿� ���� ����" << ri << "���ڴ��" << endl;

	pages[mem[ri]].w_flag = 0;
	pages[mem[ri]].flag = 0;
	pages[mem[ri]].mem_pos = -1;

	mem[ri] = p;

	pages[p].mem_pos = ri;
	ri = (ri + 1) % m;
	pages[p].flag = 1;
}

void getAddr(bool write = false) {
	unsigned int logicAddr;
	cout << "�������߼���ַ��" << endl;
	cin >> logicAddr;
	unsigned int p = logicAddr >> 10;
	unsigned int v = logicAddr & 0x3FF;

	cout << endl;

	if (p >= n) {
		cout << "Խ���жϣ�" << endl;
		return;
	}

	if (pages[p].flag == 0) {
		cout << "��" << p << "ҳ ȱҳ�жϣ�" << endl;
		swapPage(p, v);
	}

	unsigned int addr = (pages[p].mem_pos << 10) + v;
	cout << "�����ַΪ��" << addr << endl;

	if (write) {
		pages[p].w_flag = 1;
	}
}

void displayPageTable() {
	cout << "-------------------" << endl;
	cout << setw(10) << "ҳ��" << setw(10) << "���̿�" << setw(10) << "�ڴ��"
		<< setw(10) << "����" << setw(10) << "��д" << endl;
	for (int i = 0; i < n; ++i) {
		cout << setw(10) << pages[i].page_id << setw(10) << pages[i].disk_pos << setw(10);
		if (pages[i].flag) {
			cout << pages[i].mem_pos;
		} else {
			cout << " ";
		}
		cout << setw(10) << pages[i].flag << setw(10) << pages[i].w_flag << endl;
	}
	cout << "-------------------" << endl;
}

int main() {
	cout << "������ҳ����" << endl;
	while (n == 0) {
		cin >> n;
		if (n == 0) {
			cout << "ҳ������Ϊ0��" << endl;
		}
	}
	for (int i = 0; i < n; ++i) {
		pages[i].page_id = i;
		cout << "�������" << i << "ҳ��Ӳ�̿�ţ�" << endl;
		cin >> pages[i].disk_pos;
	}
	cout << "�������ڴ������" << endl;
	while (m == 0) {
		cin >> m;
		if (m == 0) {
			cout << "�ڴ��������Ϊ0��" << endl;
		}
	}
	for (int i = 0; i < m; ++i) {
		cout << "�������" << i << "���ڴ���װ��ҳ��ҳ�ţ�" << endl;
		cin >> mem[i];
		pages[mem[i]].mem_pos = i;
		pages[mem[i]].flag = 1;
	}
	while (1) {
		displayPageTable();
		int o;
		cout << "������ָ�1��д��2��ȡַ��3���˳�����" << endl;
		cin >> o;
		switch (o) {
		case 1:
			getAddr(true);
			break;
		case 2:
			getAddr();
			break;
		case 3:
			return 0;
		default:
			cout << "��Ч��ָ�" << endl;
			break;
		}
	}
	return 0;
}