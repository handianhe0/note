#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <stack>
#include <fstream>

using namespace std;

class Solution
{
public:
    void solveSudoku(vector<vector<char>>& board)
    {
        // ofstream logTxt("./log.txt");
        // cout.rdbuf(logTxt.rdbuf() );
        vector<vector<unordered_set<char>>> vvus;
        vvus.resize(board.size(),vector<unordered_set<char> >(board.size() ) );
        fillVVSet(board,vvus);
        int spacenum = 81;
        stack<vector<vector<char>>> stvv;
        stack<vector<vector<unordered_set<char>>>> stvvus;
        stack<pair<int,int>> stiloc;
        stack<int> sti;
        int i = 0,j = 0,ordinal = 0;
        while(spacenum != 0)
        {
            if(ministSize(board,vvus) == 1 )
            {
                // printvvus(vvus);
                spacenum = FillOneEleSet(board,vvus);
                // cout << "spacenum1:" << spacenum << endl;
                // printvvus(vvus);
                // printBoard(board);
            }
            else
            {
                // printvvus(vvus);
                pair<int,int> ijpair = minisizeLoc(board,vvus);
                i = ijpair.first;
                j = ijpair.second;
                // cout << "ministLoc_i:" << ijpair.first << "ministLoc_j:" << ijpair.second << endl;
                stvv.push(board);
                stvvus.push(vvus);
                stiloc.push(ijpair);
                sti.push(0);
                // cout << "sti.size(): " << sti.size() << " ";
                spacenum = FillMoreEleSet(0,i,j,board,vvus);
                // cout << "spacenum2:" << spacenum << "   ordinal:" << ordinal << endl;
                // cout << "---------------------1-----------------------" << endl;
                // printBoard(board);
            }
            while(-1 == spacenum)
            {
                do
                {
                    // cout << "test" << endl;
                    // cout << stvv.size() << stvvus.size() << stiloc.size() << sti.size() << endl;
                    board = stvv.top();
                    vvus = stvvus.top();
                    i = stiloc.top().first;
                    j = stiloc.top().second;
                    // cout << "sti.top():" << sti.top() << " ";
                    ordinal = sti.top();
                    stvv.pop();
                    stvvus.pop();
                    stiloc.pop();
                    sti.pop();
                    // if( !sti.empty() ) cout << "sti.top():" << sti.top() << " ";
                    // cout << "sti.size():" << sti.size();
                    // cout << "one pop:" << ordinal << endl;
                    // cout << " vvus.at(i).at(j).size at:" << i << " " << j << " is "<< vvus.at(i).at(j).size() << endl;
                }
                while(ordinal+1 == vvus.at(i).at(j).size() );
                if(ordinal+1 < vvus.at(i).at(j).size() )
                {
                    stvv.push(board);
                    stvvus.push(vvus);
                    stiloc.push(make_pair(i,j) );
                    sti.push(ordinal+1);
                    // cout << "sti.top():" << sti.top() << endl; 
                    spacenum = FillMoreEleSet(ordinal+1,i,j,board,vvus);
                    // cout << "spacenum3:" << spacenum << "   ordinal:" << ordinal+1 << " sti.size():" << sti.size() << endl;
                    // cout << "---------------------2-----------------------" << endl;
                    // cout << " i:" << i << " j:" << j << endl;
                    // printBoard(board);
                }
            }
        }
    }

    void fillVVSet(vector<vector<char>>& board,vector<vector<unordered_set<char>>>& vvus)
    {
        for(int i = 0; i < board.size(); i++)
        {
            for(int j = 0; j < board.size(); j++)
            {
                if(board.at(i).at(j) == '.')
                {
                    for(int k = 0; k < board.size(); k++)
                    {
                        bool flag = true;
                        for(int m = 0; m < board.size(); m++ )
                        {
                            if( ( (k+1+'0') == board.at(m).at(j) ) || ( (k+1+'0') == board.at(i).at(m) ) || 
                                    ( (k+1+'0') == board.at(m/3+i/3*3).at(m%3+j/3*3) ) )
                                flag = false;
                        }
                        if(flag) vvus.at(i).at(j).insert(k+1+'0');
                    }
                }
            }
        }
    }

    int FillOneEleSet(vector<vector<char>>& board,vector<vector<unordered_set<char>>>& vvus)
    {
        int num = 0;
        for(int i = 0; i < board.size(); i++)
        {
            for(int j = 0; j < board.size(); j++)
            {
                if(board.at(i).at(j) == '.')
                {
                    if(vvus.at(i).at(j).size() == 0 )
                    {
                        // cout << " happen to be -1 at: " << i << " " << j << endl;
                        return -1;
                    }
                    else if(vvus.at(i).at(j).size() == 1)
                    {
                        board.at(i).at(j) = *vvus.at(i).at(j).cbegin();
                        for(int m = 0; m < board.size(); m++)
                        {
                            vvus.at(m).at(j).erase(board.at(i).at(j) );
                            vvus.at(i).at(m).erase(board.at(i).at(j) );
                            vvus.at(i/3*3+m/3).at(j/3*3+m%3).erase(board.at(i).at(j) );
                        }
                    }
                    else num++;
                }
            }
        }
        return num;
    }

    int ministSize(vector<vector<char>>& board,vector<vector<unordered_set<char>>>& vvus)
    {
        int ministSetSize = 9;
        for(int i = 0; i < vvus.size(); i++)
        {
            for(int j = 0; j < vvus.size(); j++)
            {
                if(board.at(i).at(j) == '.')
                {
                    ministSetSize = ministSetSize < vvus.at(i).at(j).size() ? ministSetSize : vvus.at(i).at(j).size();
                }
            }
        }
        return ministSetSize;
    }

    int FillMoreEleSet(int ordinal,int i,int j,vector<vector<char>>& board,vector<vector<unordered_set<char>>>& vvus)
    {
        int num = 0;
        bool flag = false;
        for(auto it = vvus.at(i).at(j).begin(); it != vvus.at(i).at(j).end(); it++ )
        {
            if(num == ordinal)
            {
                board.at(i).at(j) = *it;
                flag = true;
                for(int m = 0; m < board.size(); m++)
                {
                    vvus.at(m).at(j).erase(board.at(i).at(j) );
                    vvus.at(i).at(m).erase(board.at(i).at(j) );
                    vvus.at(i/3*3+m/3).at(j/3*3+m%3).erase(board.at(i).at(j) );
                }
                break;
            }
            num++;
        }
        if( !flag ) return -1;
        num = 0;
        for(int k = 0; k < board.size(); k++)
        {
            for(int l = 0; l < board.size(); l++)
            {
                if(board.at(k).at(l) == '.') num++;
            }
        }
        return num;
    }

    pair<int,int> minisizeLoc(vector<vector<char>>& board,vector<vector<unordered_set<char>>>& vvus)
    {
        int minisize = 9;
        pair<int,int> ijpair;
        for(int i = 0; i < vvus.size(); i++)
        {
            for(int j = 0; j < vvus.size(); j++)
            {
                if(board.at(i).at(j) != '.')  continue;
                if(vvus.at(i).at(j).size() < minisize)
                {
                    minisize = vvus.at(i).at(j).size();
                    ijpair.first = i;
                    ijpair.second = j;
                }
            }
        }
        return ijpair;
    }
    
    // void printBoard(vector<vector<char>> board)
    // {
    //     for(auto e:board)
    //     {
    //         for(auto ee:e)
    //         {
    //             cout << ee << ' ';
    //         }
    //         cout << endl;
    //     }
    //     cout << endl;
    // }

    // void printvvus(vector<vector<unordered_set<char>>> vvus)
    // {
    //     for(int i = 0; i < vvus.size(); i++)
    //     {
    //         for(int j = 0; j < vvus.size(); j++)
    //         {
    //             cout << "i:" << i << "j:" << j << " :";
    //             for(auto it = vvus.at(i).at(j).cbegin(); it != vvus.at(i).at(j).cend(); it++ )
    //             {
    //                 cout << *it << " ";
    //             }
    //             cout << "  |  ";
    //         }
    //         cout << endl;
    //     }
    //     cout << endl;
    // }
};

int main()
{
    Solution so;
    vector<vector<char>> v = {
            {'.','.','.','2','.','.','.','6','3'},
            {'3','.','.','.','.','5','4','.','1'},
            {'.','.','1','.','.','3','9','8','.'},
            {'.','.','.','.','.','.','.','9','.'},
            {'.','.','.','5','3','8','.','.','.'},
            {'.','3','.','.','.','.','.','.','.'},
            {'.','2','6','3','.','.','5','.','.'},
            {'5','.','3','7','.','.','.','.','8'},
            {'4','7','.','.','.','1','.','.','.'}
        };
    so.solveSudoku(v);
    for(auto e:v)
    {
        for(auto ee:e)
        {
            cout << ee << ' ';
        }
        cout << endl;
    }
    return 0;
}