# include "Convolution.h"

using namespace std;

vector<vector<int>> Convolution(const FFS& field)
{
    auto Average = [](const FFS& ffs) {
        int sum = 0;
        for (int y = 0; y < ffs.height; ++y)
        {
            for (int x = 0; x < ffs.width; ++x)
            {
                sum += ffs.points[y][x];
            }
        }
        return sum / (ffs.width * ffs.height);
    };
    int ave = Average(field);

    vector<vector<int>> conv(field.height, vector<int>(field.width, 0));

    for (int y = 0; y < field.height; ++y)
    {
        for (int x = 0; x < field.width; ++x)
        {
            int sum = 0;
            for (int yy = -1; yy <= 1; ++yy)
            {
                for (int xx = -1; xx <= 1; ++xx)
                {
                    int ty = yy+y;
                    int tx = xx+x;
                    int point = 0;
                    if (Point(ty, tx).isOver(field.height, field.width))
                    {
                        point = ave;
                    }
                    else
                    {
                        point = field.points[ty][tx];
                    }
                    sum += point;
                }
            }
            conv[y][x] = sum;
        }
    }

    return conv;
}

