#include <iostream>
#include <iomanip>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

struct Partition {
	int start;
	int len;
	int flag;
	int job;

	bool operator < (Partition &f) {
		return start < f.start;
	}
};

int m = -1;
int o;
vector<Partition> mem;

void splitPartition(vector<Partition>::iterator it, int name, int len) {
	Partition f = *it;
	mem.erase(it);
	if (len) {
		mem.push_back({ f.start, len, 1, name });
	}
	if (f.len - len) {
		mem.push_back({ f.start + len, f.len - len, 0 });
	}
}

void firstFit(int name, int len) {
	auto it = find_if(mem.begin(), mem.end(), [len](const Partition &f) {
		return f.flag == 0 && f.len >= len;
	});
	if (it == mem.end()) {
		// No available space
		cout << "�ռ䲻�㣡" << endl;
		return;
	}
	splitPartition(it, name, len);
}

void nextFit(int name, int len) {
	static int start = 0;
	auto it = find_if(mem.begin(), mem.end(), [len](const Partition &f) {
		return f.start >= start && f.flag == 0 && f.len >= len;
	});
	if (it == mem.end()) {
		it = find_if(mem.begin(), mem.end(), [len](const Partition &f) {
			return f.flag == 0 && f.len >= len;
		});
		if (it == mem.end()) {
			// No available space
			cout << "�ռ䲻�㣡" << endl;
			return;
		}
	}
	start = (*it).start;
	splitPartition(it, name, len);
}

void bestFit(int name, int len) {
	sort(mem.begin(), mem.end(), [](const Partition &a, const Partition &b) {
		if (a.flag < b.flag) return true;
		else if (a.flag > b.flag) return false;
		else return a.len < b.len;
	});
	auto it = find_if(mem.begin(), mem.end(), [len](const Partition &f) {
		return f.flag == 0 && f.len >= len;
	});
	if (it == mem.end()) {
		// No available space
		cout << "�ռ䲻�㣡" << endl;
		return;
	}
	splitPartition(it, name, len);
}

vector<function<void(int, int)>> methods = { firstFit, nextFit, bestFit };

void allocateMemory() {
	int name, len;
	cout << "��������ҵ�������֣���" << endl;
	cin >> name;
	cout << "��������ҵ���賤�ȣ�KB����" << endl;
	cin >> len;
	cout << "�ڴ��ѷ���" << endl;
	(methods[m])(name, len * 1024);
	sort(mem.begin(), mem.end());
}

void freeMemory() {
	int name;
	cout << "�ͷ���ҵ����" << endl;
	cin >> name;
	
	auto it = find_if(mem.begin(), mem.end(), [name](Partition &f) {
		return f.job == name;
	});
	if (it == mem.end()) {
		// No such job
		cout << "δ�ҵ�����ҵ��" << endl;
		return;
	}

	Partition space = *it;
	space.flag = 0;
	auto begin = it;
	auto end = it + 1;

	if (it != mem.begin() && (*(it-1)).flag == 0) {
		// Previous partition can be merged
		space.start = (*(it - 1)).start;
		space.len += (*(it - 1)).len;
		--begin;
	}

	if (it + 1 != mem.end() && (*(it + 1)).flag == 0) {
		// Next partition can be merged
		space.len += (*(it + 1)).len;
		++end;
	}

	mem.erase(begin, end);
	mem.push_back(space);
	sort(mem.begin(), mem.end());
}

void displayMemory() {
	cout << endl << "���з�����" << endl;
	cout << setw(10) << "��ʼ��ַ" << setw(10) << "��������" << setw(10) << "�ѷ���" << setw(10) << "������ҵ" << endl;
	for (Partition f : mem) {
		cout << setw(10) << f.start << setw(10) << f.len << setw(10) << f.flag;
		if (f.flag) {
			cout << setw(10) << f.job;
		}
		cout << endl;
	}
}

void init() {
	mem.push_back({ 0, 102400, 0 });
}

int main() {
	init();
	while (m > 2 || m < 0) {
		cout << "ѡ������㷨��0���״���Ӧ�㷨��1��ѭ���״���Ӧ�㷨��2�������Ӧ�㷨����" << endl;
		cin >> m;
	}
	while (1) {
		cout << "ѡ�����0���˳���1���������棻2���������棻3����ʾ���棩" << endl;
		cin >> o;
		switch (o) {
		case 1:
			allocateMemory();
			break;
		case 2:
			freeMemory();
			break;
		case 3:
			displayMemory();
			break;
		case 0:
			return 0;
		default:
			cout << "�޴˹��ܣ�" << endl;
		}
	}
    return 0;
}

