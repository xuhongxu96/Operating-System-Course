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
	cout << "将第" << mem[ri] << "页 换出" << endl;
	if (pages[mem[ri]].w_flag) {
		cout << "该页已修改，写入硬盘第" << pages[mem[ri]].disk_pos << "块" << endl;
	}
	cout << "将第" << pages[p].disk_pos << "块硬盘块 换入 到第" << ri << "块内存块" << endl;

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
	cout << "请输入逻辑地址：" << endl;
	cin >> logicAddr;
	unsigned int p = logicAddr >> 10;
	unsigned int v = logicAddr & 0x3FF;

	cout << endl;

	if (p >= n) {
		cout << "越界中断！" << endl;
		return;
	}

	if (pages[p].flag == 0) {
		cout << "第" << p << "页 缺页中断！" << endl;
		swapPage(p, v);
	}

	unsigned int addr = (pages[p].mem_pos << 10) + v;
	cout << "物理地址为：" << addr << endl;

	if (write) {
		pages[p].w_flag = 1;
	}
}

void displayPageTable() {
	cout << "-------------------" << endl;
	cout << setw(10) << "页号" << setw(10) << "磁盘块" << setw(10) << "内存块"
		<< setw(10) << "可用" << setw(10) << "已写" << endl;
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
	cout << "请输入页数：" << endl;
	while (n == 0) {
		cin >> n;
		if (n == 0) {
			cout << "页数不能为0！" << endl;
		}
	}
	for (int i = 0; i < n; ++i) {
		pages[i].page_id = i;
		cout << "请输入第" << i << "页的硬盘块号：" << endl;
		cin >> pages[i].disk_pos;
	}
	cout << "请输入内存块数：" << endl;
	while (m == 0) {
		cin >> m;
		if (m == 0) {
			cout << "内存块数不能为0！" << endl;
		}
	}
	for (int i = 0; i < m; ++i) {
		cout << "请输入第" << i << "块内存中装入页的页号：" << endl;
		cin >> mem[i];
		pages[mem[i]].mem_pos = i;
		pages[mem[i]].flag = 1;
	}
	while (1) {
		displayPageTable();
		int o;
		cout << "请输入指令（1：写；2：取址；3：退出）：" << endl;
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
			cout << "无效的指令！" << endl;
			break;
		}
	}
	return 0;
}