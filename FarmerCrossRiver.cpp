#include <set>
#include <queue>
#include <vector>
#include <iostream>
#include <unordered_set>
using namespace std;

#define ObjSet unordered_set <Object>
enum Object { farmer, wolf, sheep, veget };
struct State
{
    ObjSet from, to; int g, t, p;
    bool operator <(const State &a) 
    const { return true; }
    bool operator ==(const State &a)
    const { return from==a.from && to==a.to; }
};
set <State> vis;
vector <State> path;
bool judgeSide(const unordered_set <Object> &side)
{
    if (side.find(farmer) != side.end()) return true;
    if (side.find(wolf)!=side.end() && side.find(sheep)!=side.end()) return false;
    if (side.find(sheep)!=side.end() && side.find(veget)!=side.end()) return false;
    return true;
}
bool judgeState(const State &state)
{ return judgeSide(state.from) && judgeSide(state.to); }
void cross(State oldState, int &id, queue <State> &q)
{
    int parentStep = oldState.g, parentId = oldState.t;
    if (oldState.from.find(farmer)!=oldState.from.end())  // from -> to
    {
        oldState.to.insert(farmer);
        oldState.from.erase(farmer);
        if (vis.find(oldState)==vis.end() && judgeState(oldState))
        {
            oldState.t = id ++;
            oldState.p = parentId;
            oldState.g = parentStep + 1;
            q.push(oldState);
            vis.insert(oldState);
        }
        for (auto i: oldState.from)
        {
            State newState = oldState;
            newState.to.insert(i);
            newState.from.erase(i);
            if (vis.find(newState)==vis.end() && judgeState(newState))
            {
                newState.t = id ++;
                newState.p = parentId;
                newState.g = parentStep + 1;
                q.push(newState);
                vis.insert(newState);
            }
        }
    }
    else  // to -> from
    {
        oldState.from.insert(farmer);
        oldState.to.erase(farmer);
        if (vis.find(oldState)==vis.end() && judgeState(oldState))
        {
            oldState.t = id ++;
            oldState.p = parentId;
            oldState.g = parentStep + 1;
            q.push(oldState);
            vis.insert(oldState);
        }
        for (auto i: oldState.to)
        {
            State newState = oldState;
            newState.from.insert(i);
            newState.to.erase(i);
            if (vis.find(newState)==vis.end() && judgeState(newState))
            {
                newState.t = id ++;
                newState.p = parentId;
                newState.g = parentStep + 1;
                q.push(newState);
                vis.insert(newState);
            }
        }
    }
}
int search(const State &origin, const State &target)
{
    int id = 1;
    queue <State> q;
    q.push(origin);
    vis.insert(origin);
    while (q.size())
    {
        State oldState = q.front(); q.pop();
        path.push_back(oldState);
        if (oldState == target) return oldState.g;
        cross(oldState, id, q);
    }
    return -1;
}
string parse(Object object)
{
    switch (object)
    {
        case farmer: return "人";
        case wolf: return "狼";
        case sheep: return "羊";
        case veget: return "菜";
        default: return "未知";
    }
}
void printState(const State &state)
{
    printf("此岸:");
    for (auto i: state.from) cout << " " << parse(i);
    printf("\n彼岸:");
    for (auto i: state.to) cout << " " << parse(i);
    putchar('\n');
}
void restorePath(int p, int len, int step)
{
	if (step <= 0)
    {
        puts("\n初始状态：");
        printState(path[len]);
		return;
	}
	for (int i=len; i>=0; i--)
		if (path[i].t == p)
		    restorePath(path[i].p, i, step-1);
	printf("第%d步操作：\n", step);
	printState(path[len]);
}
int main()
{
    State origin{ObjSet ({farmer, wolf, sheep, veget}), ObjSet (), 0, 0, 0};
    State target{ObjSet (), ObjSet ({farmer, wolf, sheep, veget}), 0, 0, 0};
    int step = search(origin, target);
    if (step != -1)
    {
        restorePath((path.end()-1)->p, path.size()-1, step);
        puts("已达到目标状态。\n");
    }
    else cout << "无解！" << endl;
    return 0;
}