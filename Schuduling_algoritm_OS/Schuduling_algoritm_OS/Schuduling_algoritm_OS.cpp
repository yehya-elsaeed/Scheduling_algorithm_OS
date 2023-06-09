#include <bits/stdc++.h>
using namespace std;
#define   fastIO()   ios_base::sync_with_stdio(false);cin.tie(NULL);

struct process
{
	int at;
	int rt;
	int pn;
};
struct comaparee1 {
	bool operator() (process const& pp1, process const& pp2)
	{
		return pp1.at > pp2.at;
	}
};
struct comparee2 {
	bool operator() (process const& pp1, process const& pp2)
	{
		return pp1.rt > pp2.rt;
	}
};

class SYS {
private:
	int n;
public:
	SYS(int nn)
	{
		n = nn;
	}
	void calW(vector<process>& v, vector<int>& vl, vector<int>& vw)
	{
		if (v.size() > 1)
		{
			if (v[0].pn != v[1].pn)
				vl[v[0].pn] = v[1].at;
			for (int i = 1; i < v.size() - 1; i++)
			{
				if (v[i].pn != v[i - 1].pn)
					vw[v[i].pn] += (v[i].at - vl[v[i].pn]);
				if (v[i].pn != v[i + 1].pn)
					vl[v[i].pn] = v[i + 1].at;
			}
			if (v[v.size() - 1].pn != v[v.size() - 2].pn)
				vw[v[v.size() - 1].pn] += v[v.size() - 1].at - vl[v[v.size() - 1].pn];
		}
	}
	pair <pair<priority_queue<process, vector<process>, comaparee1>, int>, pair<int, int>> ins()
	{
		priority_queue <process, vector<process>, comaparee1 > pq;
		pair<int, pair<int, int >> min;	min.first = min.second.first = min.second.second = 1e9;
		for (int i = 1; i <= n; i++)
		{
			process x1;
			cout << "enter arrival time for P" << i << endl;
			cin >> x1.at;
			cout << "enter running time for P" << i << endl;
			cin >> x1.rt;
			x1.pn = i;
			pq.push(x1);
			if (x1.at < min.first)
			{
				min.first = x1.at;
				min.second.first = x1.rt;
				min.second.second = x1.pn;
			}
			else if (x1.at == min.first && x1.rt < min.second.first)
				min.second.first = x1.rt;
		}
		return { { pq , min.first } , {min.second.first , min.second.second} };
	}
	vector<int> FCFS()
	{
		vector<int>v(n + 1);
		priority_queue <process, vector<process>, comaparee1> pq = ins().first.first;
		int lt = 0;
		while (!pq.empty())
		{
			if (lt - pq.top().at < 0)
			{
				lt = pq.top().at + pq.top().rt;
				v[pq.top().pn] = 0;
				pq.pop();
			}
			else
			{
				v[pq.top().pn] = lt - pq.top().at;
				lt += pq.top().rt;
				pq.pop();
			}
		}
		return v;
	}
	vector<int> SJF()
	{
		vector<int>v(n + 1);
		pair<pair<priority_queue< process, vector<process>, comaparee1>, int>, pair<int, int>>ivk = ins();
		priority_queue< process, vector<process>, comaparee1>pq1 = ivk.first.first;
		priority_queue< process, vector<process>, comparee2>pq2;
		int lt = ivk.first.second + ivk.second.first;
		while (true)
		{
			while (!pq1.empty() && pq1.top().at <= lt)
			{
				if (pq1.top().at == ivk.first.second && pq1.top().rt == ivk.second.first)
				{
					v[pq1.top().pn] = 0;
					pq1.pop();
				}
				else
				{
					pq2.push(pq1.top());
					pq1.pop();
				}
			}
			while (!pq2.empty())
			{
				v[pq2.top().pn] = lt - pq2.top().at;
				lt += pq2.top().rt;
				pq2.pop();
				if (!pq1.empty())
				{
					if (pq1.top().at <= lt)
						break;
					else if (pq2.empty())
					{
						pq2.push(pq1.top());
						lt = pq1.top().at;
						pq1.pop();
					}
				}
			}
			if (pq1.empty() && pq2.empty())
				break;
		}
		return v;
	}
	vector<int> SRT()
	{
		pair <pair<priority_queue<process, vector<process>, comaparee1>, int>, pair <int, int >> ivk = ins();
		priority_queue <process, vector<process>, comaparee1> pq1 = ivk.first.first;
		process curr;
		curr.at = ivk.first.second;	curr.rt = ivk.second.first;		curr.pn = ivk.second.second;
		priority_queue<process, vector<process>, comparee2>pq2;
		vector<process>v;	vector<int>vl(n + 1), vw(n + 1);
		while (!pq1.empty())
		{
			if (pq1.top().at == curr.at && pq1.top().rt == curr.rt && pq1.top().pn == curr.pn)
				pq1.pop();
			else
			{
				if (curr.at == pq1.top().at)
				{
					if (pq1.top().rt >= curr.rt)
					{
						pq2.push(pq1.top());
						vl[pq1.top().pn] = pq1.top().at;
						pq1.pop();
					}
					else
					{
						pq2.push(curr);
						curr = pq1.top();
						vl[pq1.top().pn] = pq1.top().at;
						pq1.pop();
					}
				}
				else
				{
					int exp = curr.rt - (pq1.top().at - curr.at);
					if (exp > pq1.top().rt)
					{
						curr.rt = exp;
						v.push_back(curr);
						pq2.push(curr);
						curr = pq1.top();
						vl[pq1.top().pn] = pq1.top().at;
						pq1.pop();
					}
					else if (exp < 0)
					{
						v.push_back(curr);
						curr = pq2.top();
						curr.at = pq1.top().at + exp;
						pq2.pop();
					}
					else if (exp == 0)
					{
						v.push_back(curr);
						curr = pq2.top();
						pq2.pop();
					}
					else if (exp <= pq1.top().rt)
					{
						v.push_back(curr);
						curr.rt = exp;
						curr.at = pq1.top().at;
						pq2.push(pq1.top());
						vl[pq1.top().pn] = pq1.top().at;
						pq1.pop();
					}
				}
			}
		}
		int lt = curr.at;
		v.push_back(curr);
		while (!pq2.empty())
		{
			lt = curr.rt + lt;
			curr = pq2.top();
			curr.at = lt;
			v.push_back(curr);
			pq2.pop();
		}
		calW(v, vl, vw);
		return vw;
	}
	vector<int> RR()
	{
		priority_queue<process, vector<process>, comaparee1>pq1 = ins().first.first;
		vector<process>v;	vector<int>vl(n + 1), vw(n + 1);
		queue<process>q2;
		cout << "enter Quantum value " << endl;
		int q, lt = pq1.top().at;	cin >> q;
		q2.push(pq1.top());
		vl[pq1.top().pn] = pq1.top().at;
		pq1.pop();
		while (!q2.empty())
		{
			int exp = q2.front().rt - q;
			process val = q2.front();
			if (exp > 0)
			{
				val.rt -= q;
				val.at = lt;
				v.push_back(val);
				lt += q;
				while (!pq1.empty() && pq1.top().at <= lt)
				{
					q2.push(pq1.top());
					vl[pq1.top().pn] = pq1.top().at;
					pq1.pop();
				}
				q2.push(val);
				q2.pop();
			}
			else if (exp == 0)
			{
				val.rt = 0;
				val.at = lt;
				v.push_back(val);
				lt += q;
				q2.pop();
				while (!pq1.empty() && pq1.top().at <= lt)
				{
					q2.push(pq1.top());
					vl[pq1.top().pn] = pq1.top().at;
					pq1.pop();
				}
			}
			else if (exp < 0)
			{
				val.rt = 0;
				val.at = lt;
				v.push_back(val);
				lt += q;
				lt += exp;
				q2.pop();
				while (!pq1.empty() && pq1.top().at <= lt)
				{
					q2.push(pq1.top());
					vl[pq1.top().pn] = pq1.top().at;
					pq1.pop();
				}
			}
			if (!pq1.empty() && q2.empty())
			{
				q2.push(pq1.top());
				lt = pq1.top().at;
				vl[pq1.top().pn] = pq1.top().at;
				pq1.pop();
			}
		}
		calW(v, vl, vw);
		return vw;
	}
};

signed main()
{
	fastIO()
	cout << "enter number of process " << endl;
	int n;
	cin >> n;
	SYS s1(n);
	vector<int>v = s1.FCFS();
	//	vector<int>v = s1.SJF();
	//	vector<int>v = s1.SRT();
	//  vector<int>v = s1.RR();
	cout << endl;
	for (int i = 0; i < n; i++)
		cout << "the waititng time of process p" << i + 1 << " = " << v[i + 1] << endl;
	cout << "the average = " << (double)accumulate(v.begin(), v.end(), 0) / n << endl;
}