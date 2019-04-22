#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include "split.h"
#include "GFChip.h"
#include "stdlib.h"
#include <ctype.h>
using namespace std;

char buffer[102400];
// chip save code rules by GFTool
const string rules[2] = { "InfinityFrost", "FatalChapters" };

// helper functions
char helperMain();
void helperExcel2Web();
void helperWeb2Excel();
void excel2Web(const char* filename, int color);

int main()
{
    start:
    if(auto t = helperMain(); t == '1')
    {
        helperExcel2Web();
    }
    else if(t == '2')
    {
        helperWeb2Excel();
    }
    else if(t == '0')
    {
        return 0;
    }
    else
    {
        // input error, restart.
        system("cls");
        goto start;
    }
    system("pause");
}

char helperMain()
{
    cout << "欢迎使用《少女前线 芯片代码转换器》" << endl;
    cout << "有问题请邮件至zaxs0130@gmail.com" << endl;
    cout << "或https://github.com/xxzl0130/GFChipConverter" << endl;
    cout << "===========================================================" << endl;
    cout << "请按键选择要使用的功能：" << endl;
    cout << "1：Excel（杯具）版 ====> 网页（乐章）版" << endl;
    cout << "2：网页（乐章）版  ====> Excel（杯具）版" << endl;
    cout << "0：退出" << endl;
    return getchar();
}

void helperExcel2Web()
{
    system("cls");
    gets_s(buffer, sizeof(buffer));//clear input
    cout << "功能：Excel（杯具）版 ====> 网页（乐章）版" << endl;
    cout << "由于Excel不区分颜色，请先按键选择芯片颜色，r为红色，其他为蓝色。" << endl;
    char color = getchar(); gets_s(buffer, sizeof(buffer));//clear input
    cout << "你选择的颜色是：" << (color == 'r' ? "红色" : "蓝色") << endl;
    cout << "请将excel表格保存为csv格式，保存时选择“芯片总表”工作表。" << endl;
    cout << "然后在此处输入csv文件名或直接将文件拖至此处。" << endl;
    cout << "文件名：";
    cin.getline(buffer, sizeof(buffer));
    excel2Web(buffer,color == 'r' ? 2 : 1);
    cout << "代码已保存到SaveCode.txt中，请查看" << endl;
}

void helperWeb2Excel()
{
    start2:
    system("cls");
    gets_s(buffer, sizeof(buffer));//clear input
    cout << "功能：网页（乐章）版  ====> Excel（杯具）版" << endl;
    cout << "请直接将储存代码粘贴在此处，按回车键确认。" << endl;

    memset(buffer, 0, sizeof(buffer));
    int i = 0;
    for(i = 0;(buffer[i] = getchar()) != ']';++i)
    {
        if(!isalpha(buffer[i]) && !isdigit(buffer[i]) && buffer[i] != ',' && buffer[i] != '[' && buffer[i] != '&')
        {
            //skip this char
            --i;
        }
    }

    if(i < 10)
    {
        //error, restart
        goto start2;
    }
    auto list = split(string(buffer + 2, strlen(buffer) - 3 - 2),//skip begin and end
            '&');

    std::vector<GFChip> redChips,blueChips;
    for(const auto& it : list)
    {
        // create chips
        auto chip = GFChip::createFromSaveCode(it);
        if(chip.chipType == 9 || chip.chipType == 10 || chip.chipType > 110)
        {//5-block-2-type not used in excel
            continue;
        }
        if (chip.chipColor == 2)
        {
            chip.chipNum = redChips.size() + 1;
            redChips.push_back(GFChip::createFromSaveCode(it));
        }
        else
        {
            chip.chipNum = blueChips.size() + 1;
            blueChips.push_back(chip);
        }
    }

    fstream fout;
    fout.open("RedChips.csv", ios::out);
    for(const auto& it : redChips)
    {
        fout << it.toExcelLine() << endl;
    }
    fout.close();
    fout.open("BlueChips.csv", ios::out);
    for (const auto& it : blueChips)
    {
        fout << it.toExcelLine() << endl;
    }
    fout.close();

    cout << "芯片已分颜色保存在程序目录中，请打开查看。" << endl;
    cout << "将代码复制到Excel表格中的相应位置，可正常计算." << endl;
}

void excel2Web(const char* filename,int color)
{
    fstream fin;
    fstream fout;
    fin.open(filename, ios::in);
    fout.open("SaveCode.txt", ios::out);
    std::vector<GFChip> chips;

    // skip useless line
    for(auto i = 0;i < 11;++i)
    {
        fin.getline(buffer, sizeof(buffer));
    }

    //read in
    while(true)
    {
        fin.getline(buffer, sizeof(buffer));
        if(strlen(buffer) < 10 || !isdigit(buffer[0]))
        {
            break;
        }
        auto chip = GFChip::createFromExcelLine(buffer);
        chip.chipColor = color;
        chips.push_back(chip);
    }

    auto chipNum = (*chips.rbegin()).chipNum;
    fout << "[" << rules[0][chipNum - 13 * int(chipNum / 13)] << "!";
    for(auto& it : chips)
    {
        fout << it.toSaveCode() << "&";
    }
    fout << "?" << rules[1][chipNum - 13 * int(chipNum / 13)] << "]";
    fin.close();
    fout.close();
}
