#define UNICODE
#include<iostream>
#include<windows.h>
#include<cstdio>
#include<chrono>
#include<vector>
#include<stdio.h>
#include<utility>
#include<algorithm>
#include<stdio.h>
#include<string>
#include<math.h>

using namespace std;

int nscreenwidth=120; //screen columns
int nscreenheight=40; //screen rows

float fplayerX=14.0f;
float fplayerY=5.0f;
float playerA=0.0f;
float fov=3.14159f/4.0f;
float depth=16.0f;
float speed=5.0f;

int mapheight=16;
int mapwidth=16;


int main()
{
    //creating screen buffer

    wchar_t *screen= new wchar_t[nscreenwidth*nscreenheight];
    HANDLE hconsole=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    SetConsoleActiveScreenBuffer(hconsole);
    DWORD byteswritten=0;

    //creating a map #=Wall , . = Space
    wstring map;
    map += L"#########.......";
	map += L"#...............";
	map += L"#.......########";
	map += L"#..............#";
	map += L"#......##......#";
	map += L"#......##......#";
	map += L"#..............#";
	map += L"###............#";
	map += L"##.............#";
	map += L"#......####..###";
	map += L"#......#.......#";
	map += L"#......#.......#";
	map += L"#..............#";
	map += L"#......#########";
	map += L"#..............#";
	map += L"################";

	auto tp1=chrono::system_clock::now();
	auto tp2=chrono::system_clock::now();
while(1)
{
        tp2=chrono::system_clock::now();
        chrono::duration<float>elapsedtime=tp2-tp1;
        tp1=tp2;
        float felapsedtime=elapsedtime.count();

    if(GetAsyncKeyState('A'))
        playerA-=(speed*0.75)*felapsedtime;

    if(GetAsyncKeyState('D'))
        playerA+=(speed*0.75)*felapsedtime;

    if(GetAsyncKeyState('W'))
    {

        fplayerX+=sinf(playerA)*speed*felapsedtime;
        fplayerY+=cosf(playerA)*speed*felapsedtime;
        if(map.c_str()[(int)fplayerX*mapwidth+(int)fplayerY]=='#')
        {

             fplayerX-=sinf(playerA)*speed*felapsedtime;
             fplayerY-=cosf(playerA)*speed*felapsedtime;
        }
    }
    if(GetAsyncKeyState('S'))
    {

        fplayerX-=sinf(playerA)*speed*felapsedtime;
        fplayerY-=cosf(playerA)*speed*felapsedtime;
        if(map.c_str()[(int)fplayerX*mapwidth+(int)fplayerY]=='#')
        {

             fplayerX+=sinf(playerA)*speed*felapsedtime;
             fplayerY+=cosf(playerA)*speed*felapsedtime;
        }
    }

    for(int x=0;x<nscreenwidth;x++)
    {
        //int rayangle=0;
        float rayangle=(playerA-fov/2.0f)+((float)x/(float)nscreenwidth);
        float stepsize=0.1f;
        float distancetowall=0; //set to 0

        bool hitwall=false; //set when ray hit the wall
        bool boundary=false;
        float eyeX=sinf(rayangle);
        float eyeY=cosf(rayangle);

        while(!hitwall && distancetowall<depth)
        {
            //int testX=0;
            //int testY=0;

            distancetowall+=stepsize;
            int testX= (int)(fplayerX+eyeX*distancetowall);
            int testY= (int)(fplayerY+eyeY*distancetowall);

            if(testX<0||testX>=mapwidth||testY<0||testY>=mapheight)
            {

                hitwall=true;
                distancetowall=depth;
            }
            else
            {

                if(map.c_str()[testX*mapwidth+testY]=='#')
                {
                    hitwall=true;

                }
            }
        }
        //calculate distance to ceiling and floor
        int ceiling=(float)(nscreenheight/2.0)-nscreenheight/((float)distancetowall);
        int floor=nscreenheight-ceiling;

        short shade=' ';
        if(distancetowall<=depth/4.0f)       shade=0x2588;
        else if(distancetowall<depth/3.0f)   shade=0x2593;
        else if(distancetowall<depth/2.0f)   shade=0x2592;
        else if(distancetowall<depth)        shade=0x2591;
        else
            if(boundary)     shade=' ';

        for(int y=0;y<nscreenheight;y++)
        {

            if(y<=ceiling)
                screen[y*nscreenwidth+x]=' ';
            else if(y>ceiling && y <=floor)
                screen[y*nscreenwidth+x]=shade;
            else
            {

                float b=1.0f-(((float)y-nscreenheight/2.0f)/((float)nscreenheight/2.0f));
                if(b<0.25)       shade='#';
                else if(b<0.5)   shade='x';
                else if(b<0.75)  shade='.';
                else if(b<0.0)   shade='-';
                else shade=' ';
                screen[y*nscreenwidth+x]=shade;
            }
        }


    }
    snwprintf(screen,40,L"X=%3.2f,Y=%3.2f,A=3.2f,FPS=%3.2",fplayerX,fplayerY,playerA,1.0f/felapsedtime);
    for(int nx=0;nx<mapwidth;nx++)
        for(int ny=0;ny<mapwidth;ny++)
        {

            screen[(ny+1)*nscreenwidth+nx]=map[ny*mapwidth+nx];
        }
        screen[((int)fplayerX+1)*nscreenwidth+(int)fplayerY]='P';



    screen[nscreenwidth*nscreenheight-1] ='\0';
    WriteConsoleOutputCharacter(hconsole,screen,nscreenwidth*nscreenheight,{0,0},&byteswritten);
}


    return 0;
}


