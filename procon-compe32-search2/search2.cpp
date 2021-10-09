#include "search2.hpp"

int targetX = 0;
int targetY = 0;

bool Check(State state)
{
    //正しい座標までの距離がすべて0か調べる
    for (int i = 0; i <= NumOfDiv::Horizontal; i++)
    {
        for (int k = 0; k <= NumOfDiv::Vertical; k++)
        {
            if(state.distance[k][i] != 0) return false;
        }
    }
    return true;
}

bool Check2(int target, const board& goal)
{
    //選択しているピースが正しい座標にあるか調べる
    if (goal[targetX][targetY] == target) return true;
    else return false;
}

//Pairの比較用関数
bool Comp(std::pair<int, State> lhs, std::pair<int, State> rhs)
{
    return lhs.first < rhs.first;
}

std::string BeamSearch2(State initialState, board goal, int selectCostRate, int swapCostRate)
{
    const int BeamWidth = 425 - (NumOfDiv::Horizontal * NumOfDiv::Vertical);
    int itr = 0;

    int target = 0;

    //ビームサーチのキュー
    std::deque<std::pair<int, State>> beam;
    std::deque<std::pair<int, State>> nexts;

    Select(initialState);

    //評価値と盤面のPair
    std::pair<int, State> state(Eval2(initialState), initialState);

    beam.push_front(state);

    //キューがからになるまでループ
    while (!beam.empty())
    {
        itr++;
        state = beam.front();
        beam.pop_front();
        //すべて正しい座標にあれば終了
        if (Check(state.second)) break;
        //選択しているピースが正しい座標にあれば選択を変える
        if (Check2(target, goal)) 
        {
            NextTarget();
        }

        std::pair<int, State> newState(state);
        //それぞれの方向と交換させる
        SwapWithU(newState.second.status, newState.second.selectPieceX, newState.second.selectPieceY);
        newState.second.cost += swapCostRate;
        newState.second.result += 'U';
        newState.second.count++;
        newState.first = Eval2(newState.second);
        nexts.push_back(newState);
        newState = state;

        SwapWithD(newState.second.status, newState.second.selectPieceX, newState.second.selectPieceY);
        newState.second.cost += swapCostRate;
        newState.second.result += 'D';
        newState.second.count++;
        newState.first = Eval2(newState.second);
        nexts.push_back(newState);
        newState = state;

        SwapWithR(newState.second.status, newState.second.selectPieceX, newState.second.selectPieceY);
        newState.second.cost += swapCostRate;
        newState.second.result += 'R';
        newState.second.count++;
        newState.first = Eval2(newState.second);
        nexts.push_back(newState);
        newState = state;

        SwapWithL(newState.second.status, newState.second.selectPieceX, newState.second.selectPieceY);
        newState.second.cost += swapCostRate;
        newState.second.result += 'L';
        newState.second.count++;
        newState.first = Eval2(newState.second);
        nexts.push_back(newState);
        newState = state;
        //キューが空になったら次のイテレーション用のキューからビーム幅取り出す
        if (beam.empty())
        {
            std::sort(nexts.begin(), nexts.end(), Comp);
            for (int i = 0; i < BeamWidth; i++)
            {
                beam.push_back(nexts.front());
                nexts.pop_front();
                if (nexts.empty()) break;
            }
            nexts.clear();
        }

        //定期的に経過を表示する
        if (itr == 2000)
        {
            std::cout << "Select: " << state.second.status[state.second.selectPieceX][state.second.selectPieceY];
            std::cout << " Score: " << state.first << std::endl;
            for (int i = 0; i <= NumOfDiv::Horizontal; i++) {
                for (int k = 0; k <= NumOfDiv::Vertical; k++) {
                    if (i == state.second.selectPieceY && k == state.second.selectPieceX)
                    {
                        std::cout << "\33[41m" << state.second.status[k][i] << "\33[m" << " ";
                        continue;
                    }
                    if (state.second.distance[k][i] == 0)
                    {
                        std::cout << "\33[44m" << state.second.status[k][i] << "\33[m" << " ";
                        continue;
                    }
                    if (state.second.status[k][i] == target)
                    {
                        std::cout << "\33[42m" << state.second.status[k][i] << "\33[m" << " ";
                        continue;
                    }
                    std::cout << state.second.status[k][i] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << state.second.result << std::endl;
            std::cout << "-----------------------" << std::endl;
            itr = 0;
        }
    }

    //結果の表示
    int correct = 0;
    int correctPercent = 0;
    for (int i = 0; i <= NumOfDiv::Horizontal; i++) {
        for (int k = 0; k <= NumOfDiv::Vertical; k++) {
            if (state.second.distance[k][i] == 0)
            {
                std::cout << "\33[44m" << state.second.status[k][i] << "\33[m" << " ";
                correct++;
                continue;
            }
            std::cout << state.second.status[k][i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    correctPercent = (correct * 100) / ((NumOfDiv::Horizontal+1) * (NumOfDiv::Vertical+1));
    std::cout << "一致率: " << correctPercent << "%" << std::endl;
    state.second.result += '\n';
    return state.second.result;
}

void Select(State& state)
{
    state.selectPieceX = NumOfDiv::Vertical;
    state.selectPieceY = NumOfDiv::Horizontal;
    state.numOfselect--;
    state.count == 0;
    state.result += std::to_string(NumOfDiv::Vertical) + " " + std::to_string(NumOfDiv::Horizontal) + '\n';
}

void NextTarget()
{
    if (targetY >= (NumOfDiv::Horizontal-1))
    {
        if (targetY == NumOfDiv::Horizontal)
        {
            targetY = NumOfDiv::Horizontal-1;
            targetX++;
        }
    }
    else if (targetX == NumOfDiv::Vertical)
    {
        targetX = 0;
        targetY++;
    }
    else 
    {
        targetX++;
    }
}