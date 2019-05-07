#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
#include <cstdlib>
#include <stdio.h>
#include <cctype>
#include <ctime>
#include <time.h>
#include <sys/stat.h>

#include "split.h"
#include "GFChip.h"
#include <ctype.h>
using namespace std;

char *buffer;
// chip save code rules by GFTool
const string rules[2] = { "InfinityFrost", "FatalChapters" };
string timeStr;

// helper functions
void helperMain();
void excel2Web(int color);
void web2Excel();
int fileSize(const char* filename);
//读取文件并判断文件类型，1为网页代码，2为Excel，0为错误，文件保存在全局buffer中
int readFile(const char* filename);

int main(int argc,char** argv)
{
    //获取时间
    auto timer = time(NULL);
    struct tm timeS;
    localtime_s(&timeS,&timer);
    stringstream ss;
    ss << timeS.tm_year + 1900 << "-" << timeS.tm_mon << "-" << timeS.tm_mday << " " << timeS.tm_hour << "-" << timeS.tm_min << "-" << timeS.tm_sec;
    timeStr = ss.str();

    const char* filename;
    string tmpStr;
    int color = -1;
    if(argc >= 2)
    {
        filename = argv[1];
        if(argc >= 3)
        {
            color = argv[2][0] == 'r' ? 2 : 1;
        }
    }
    else
    {
        helperMain();
        cin >> tmpStr;
        getchar();
        filename = tmpStr.c_str();
    }
    switch (readFile(filename))
    {
    case 1:
        web2Excel();
        break;
    case 2:
        if(color <= 0)
        {
            //颜色未初始化
            cout << "由于Excel不区分颜色，请按键选择芯片颜色，r为红色，b为蓝色:";
            char c;
            while(true)
            {
                c = tolower(getchar());
                if (c == 'r' || c == 'b')
                    break;
                else
                    while (getchar() != '\n');
                cout << "由于Excel不区分颜色，请按键选择芯片颜色，r为红色，b为蓝色:";
            }
            color = c == 'r' ? 2 : 1;
        }
        excel2Web(color);
        break;
    default:
        cout << "请输入正确的文件！" << endl;
        break;
    }
    cout << "转换完毕，请在程序目录下寻找相应文件。\n谢谢使用。" << endl;
    system("pause");
}

void helperMain()
{
    cout << "欢迎使用《少女前线 芯片代码转换器》" << endl;
    cout << "有问题请邮件至zaxs0130@gmail.com" << endl;
    cout << "或https://github.com/xxzl0130/GFChipConverter" << endl;
    cout << "========================================================\n" << endl;
    cout << "使用说明：" << endl;
    cout << "命令行：GFChipConverter <filename> [color(r/b)]" << endl;
    cout << "Example：GFChipConverter 重装芯片计算器-红.csv r" << endl;
    cout << "或从此处输入文件名。" << endl << endl;
    cout << "请将excel表格保存为csv格式，保存时选择“芯片总表”工作表。" << endl;
    cout << "或将网页版存储代码保存为Txt文件。" << endl;
    cout << "请输入要转换的文件名，或直接将文件拖入此窗口，然后按回车。" << endl << endl;
    cout << "文件名：";
}

void web2Excel()
{
    auto list = split(string(buffer + 2, strlen(buffer) - 3 - 2),//skip begin and end
        '&');

    std::vector<GFChip> redChips, blueChips;
    for (const auto& it : list)
    {
        // create chips
        auto chip = GFChip::createFromSaveCode(it);
        if (chip.chipType == 9 || chip.chipType == 10 || chip.chipType > 110 || chip.chipType == 81 || chip.chipType == 82)
        {//5-block-2-type not used in excel
            continue;
        }
        if (chip.chipColor == 2)
        {
            chip.chipNum = redChips.size() + 1;
            redChips.push_back(chip);
        }
        else
        {
            chip.chipNum = blueChips.size() + 1;
            blueChips.push_back(chip);
        }
    }

    fstream fout;
    fout.open("RedChips " + timeStr + ".csv", ios::out);
    fout << "芯片编号,形状代号,杀伤,破防,精度,装填,强化等级,杀伤,破防,精度,装填," << endl;
    for (const auto& it : redChips)
    {
        fout << it.toExcelLine() << endl;
    }
    fout.close();
    fout.open("BlueChips " + timeStr + ".csv", ios::out);
    fout << "芯片编号,形状代号,杀伤,破防,精度,装填,强化等级,杀伤,破防,精度,装填," << endl;
    for (const auto& it : blueChips)
    {
        fout << it.toExcelLine() << endl;
    }
    fout.close();
}

void excel2Web(int color)
{
    std::vector<GFChip> chips;
    //read in
    auto lines = split(buffer, '\n');
    lines.erase(lines.begin(), lines.begin() + 11);
    for(const auto& it : lines)
    {
        if(it.size() < 10 || !isdigit(it[0]))
        {
            break;
        }
        auto chip = GFChip::createFromExcelLine(it);
        chip.chipColor = color;
        chips.push_back(chip);
    }
    if(chips.empty())
    {
        return;
    }
    auto chipNum = (*chips.rbegin()).chipNum;
    fstream fout;
    fout.open("SaveCode" + timeStr + ".txt", ios::out);
    fout << "[" << rules[0][chipNum - 13 * int(chipNum / 13)] << "!";
    for(auto& it : chips)
    {
        fout << it.toSaveCode() << "&";
    }
    fout << "?" << rules[1][chipNum - 13 * int(chipNum / 13)] << "]";
    fout.close();
}

int fileSize(const char* filename)
{
    struct stat statbuf;
    if (stat(filename, &statbuf) != 0) 
        return 0;//获取失败。
    return statbuf.st_size;//返回文件大小。
}

int readFile(const char* filename)
{
    auto size = fileSize(filename);
    if (size <= 10)//too short
    {
        return 0;
    }
    size += 10;
    buffer = new char[size];
    memset(buffer, 0, size);
    FILE *fin;
    fopen_s(&fin, filename, "r");
    if(fin == NULL)
    {
        return -1;
    }
    fread_s(buffer, size, sizeof(char), size, fin);
    fclose(fin);

    auto len = strlen(buffer);
    while(buffer[len - 1] == '\n' || buffer[len - 1] == '\r' || buffer[len - 1] == ' ' || buffer[len - 1] < 0)
    {
        //排除末尾空白
        buffer[len - 1] = '\0';
        --len;
    }
    auto t = &buffer[len - 10];
    if(buffer[0] == '[' && isalpha(buffer[1]) && buffer[2] == '!' && buffer[len - 1] == ']')
    {
        //简单校验网页版代码
        return 1;
    }
    else if(strstr(buffer,"0,1,5,13,8,6"))
    {
        //以excel里的一段固定文本为校验
        return 2;
    }
    else
    {
        return 0;
    }
}
