#include <windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

// 全局变量
bool clicking = false;
int clickInterval = 10; // 默认点击间隔(毫秒)
int clickCount = 0;

// 鼠标点击函数
void PerformClick()
{
    INPUT input[2] = {0};
    
    // 鼠标按下
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    
    // 鼠标释放
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    
    SendInput(2, input, sizeof(INPUT));
    clickCount++;
}

// 连点器线程函数
DWORD WINAPI ClickerThread(LPVOID lpParam)
{
    while(clicking)
    {
        PerformClick();
        Sleep(clickInterval);
    }
    return 0;
}

int main()
{
    cout << "=== 鼠标连点器 ===" << endl;
    cout << "按F6开始/停止连点" << endl;
    cout << "按F7增加点击间隔" << endl;
    cout << "按F8减少点击间隔" << endl;
    cout << "当前点击间隔: " << clickInterval << "ms" << endl;
    cout << "------------------" << endl;

    HANDLE hThread = NULL;
    
    while(true)
    {
        if(GetAsyncKeyState(VK_F6) & 0x8000)
        {
            clicking = !clicking;
            
            if(clicking)
            {
                cout << "连点已启动 (间隔: " << clickInterval << "ms)" << endl;
                hThread = CreateThread(NULL, 0, ClickerThread, NULL, 0, NULL);
            }
            else
            {
                cout << "连点已停止" << endl;
                cout << "总点击次数: " << clickCount << endl;
                clickCount = 0;
                if(hThread) CloseHandle(hThread);
            }
            Sleep(200); // 防抖
        }
        
        if(GetAsyncKeyState(VK_F7) & 0x8000)
        {
            clickInterval += 10;
            cout << "点击间隔增加至: " << clickInterval << "ms" << endl;
            Sleep(200);
        }
        
        if(GetAsyncKeyState(VK_F8) & 0x8000)
        {
            if(clickInterval > 10)
            {
                clickInterval -= 10;
                cout << "点击间隔减少至: " << clickInterval << "ms" << endl;
            }
            else if(clickInterval > 0)
			{
				clickInterval -= 1;
				cout << "点击间隔减少至: " << clickInterval << "ms" << endl;
			} 
            Sleep(200);
        }
        
        if(GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            clicking = false;
            if(hThread) CloseHandle(hThread);
            break;
        }
    }
    
    return 0;
}

