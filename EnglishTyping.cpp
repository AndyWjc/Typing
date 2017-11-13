#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <string>
#include<time.h>
#pragma comment(lib,"Imm32.lib") 
using namespace std;
#define WINDOWS_X 1280	//窗口大小X
#define WINDOWS_Y 800   //窗口大小Y
#define BBLACK 80		//空格大小
#define ESC 27
#define ESCEXIT (_kbhit()&&_getch()==ESC)
#define	CMD_UP			1
#define	CMD_DOWN		2
#define	CMD_LEFT		4
#define	CMD_RIGHT		8
#define	CMD_LSHIFT		16
#define	CMD_ZOOMOUT		32
#define	CMD_QUIT		64
#define STACK_INIT_SIZE 100
#define STACKINCREMENT  10
typedef int SElemType;
typedef char QElemType;
/*栈的存储结构*/
typedef struct{
	SElemType *base; /*栈低指针*/
	SElemType *top;  /*栈顶指针*/
	int stacksize;   /*栈当前已分配的所有空间，不是已使用的空间长度*/
}SqStack;
/*start***************队列的存储结构*********************/
typedef struct QNode{
	QElemType data; 
	struct QNode *next;
}QNode,*QueuePtr;
typedef struct{
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;
/****************队列的存储结构********************end*/

/*函数声明*/
void GetIMEString(HWND hWnd,string& str);
SElemType GetTop(SqStack *S);
void Push(SqStack *S,SElemType e);
bool SEmpty(SqStack *S);
SElemType Pop(SqStack *S);
void main();
LinkQueue * InitQ()
{
	LinkQueue *Q;
	Q=(LinkQueue *)malloc(sizeof(LinkQueue));
	Q->front=Q->rear=(QueuePtr)malloc(sizeof(QNode));
	if(!Q->front) exit(1);
	Q->front->next=NULL;
	printf("Successful Q!");
	return Q;
}

void EnQ(LinkQueue *Q,QElemType e) //入队
{
	QueuePtr p;
	p=(QueuePtr)malloc(sizeof(QNode));
	if(!p) exit(1);
	p->data=e;
	p->next=NULL;
	Q->rear->next=p;
	Q->rear=p;
}

char DeQ(LinkQueue *Q) //出队
{
	int e;
	QueuePtr p;
	if(Q->front==Q->rear) return 0;
	else
	{
		p=Q->front->next;
		e=p->data;
		Q->front->next=p->next;
		if(Q->rear==p) Q->rear=Q->front;
		free(p);
		return e;
	}
}

SqStack * InitStack() /*1.初始化栈*/
{
	SqStack *S;
	S=(SqStack *)malloc(sizeof(SqStack));
	S->base=(SElemType *)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if(!S || !S->base) exit(1);
	S->top=S->base;
	S->stacksize=STACK_INIT_SIZE;
	return S;
}

bool SEmpty(SqStack *S) /*2.判栈空*/
{
	if(S->top==S->base)	return 0;
	else return 1;  /*1表示栈不空*/
}

void Push(SqStack *S,SElemType e) /*3.入栈*/
{
	if(S->top-S->base >= S->stacksize )
	{
		S->base=(SElemType *)realloc(S->base,(S->stacksize+STACKINCREMENT)*sizeof(SElemType));
		if(!S->base) exit(1);
		printf("\n空间不够，开辟新空间成功！\n");
		S->top=S->base+S->stacksize;
		S->stacksize +=STACKINCREMENT;
	}
	*S->top++=e;
}

SElemType Pop(SqStack *S) /*4.出栈*/
{
	if(S->top==S->base)
	{
		return 0;
		printf("\nThe sequence stack is empty!");
	}
	else return *(--S->top);
}

void display(SqStack *S) /*5.打印*/
{
	int e;

	SqStack *T;
	T=InitStack(); /*1.新构建一个栈T*/
	
	while(SEmpty(S))
	{
		e=Pop(S);
		Push(T,e);/*2.将出栈打印的元素入栈T*/
		printf("%5d",e); /*当栈不空依次打印*/
	}
	while(SEmpty(T)) Push(S,Pop(T));/*3.将栈T中元素出栈并入栈到栈S中*/
	free(T);
	printf("\n");
}

int StackLength(SqStack *S) /*6.统计栈长度*/
{
	printf("\n统计得栈的长度为：%d\n",(S->top-S->base));
	return (S->top-S->base);
}

void search(SqStack *S,SElemType e) /*7.查找*/
{
	int count=0,flag=0;
	SqStack *T;
	T=InitStack();
	while(SEmpty(S))
	{
		if(e==GetTop(S))
		{
			count++;
			flag=1;
			printf("\n%d 已找到元素：%d\n",count,e);
		}
		Push(T,Pop(S));
	}
	while(SEmpty(T)) Push(S,Pop(T));
	free(T);
	if(flag==0) printf("\n没有找到元素：%d\n",e);
}

void modify(SqStack *S,SElemType e) /*8.修改*/
{
	int e1,count=0,flag=0,a;
	SqStack *T;
	T=InitStack();
	while(SEmpty(S))
	{
		if(e==GetTop(S)) 
		{
			count++;
			flag=1;
			printf("\n%d 已找到元素：%d\n",count,e);
			printf("\n1.修改   2.不修改：");
			scanf("%d",&a);
			switch(a)
			{
			case 1:
				e1=Pop(S);
				printf("\n将元素%d修改为：",e);   
				scanf("%d",&e1);
				Push(S,e1);
				printf("\n修改成功！\n");
			//除输入1之外的情况都不做任何操作
			}
		}
		Push(T,Pop(S));
	}
	while(SEmpty(T)) Push(S,Pop(T));
	free(T);
	if(flag==0) printf("\n没有找到元素：%d\n",e);
}

void ClearStack(SqStack *S) /*9.清空栈*/
{
	S->top=S->base;
	printf("\n清空栈成功！\n");
}

void DestoryStatck(SqStack *S) /*10.销毁栈*/ 
{  
    S->top=S->base;  
    free(S->base);  
    S->base=NULL;  
    S->top=NULL;
	S->stacksize=0;
	free(S);
    printf("\n销毁栈成功！\n");    
}

SElemType GetTop(SqStack *S) /*11.取栈顶元素*/
{
	if(S->top==S->base)
	{
		printf("\nThe sequence stack is empty!\n");
		return 0;
	}
	else return *(S->top-1); /*区别于Pop中的*--S->top,*(S->top-1)不改变S->top指针的指向*/	
}
//删除指定位置开始指定字符串长度函数。
void DeleteString(char* array,char*result,int begin,int len)//begin：开始删除的位置，len：删除的长度。
{ int i=0,j=0; while(array[i]!='\0') {  if(i<(begin-1)||i>=(begin+len-1))   result[j++]=array[i++];  else   i++; } result[j]='\0';}

void main()//main()
{
	LinkQueue *Q;
	Q=InitQ();
	SqStack *S;
	S=InitStack();
	FILE *p;
	char ag[1000],ai[1000],b[100],c[100],d[100],e[100],ff[100],g[100],h[100],ii[100],jj[100];
	int i=0;
    int j=0,k=0;
	int num=0;
	static int temp=0,temp1=0;
	static int ch = 0;//保存换行信息
	char result[100];

	initgraph(WINDOWS_X, WINDOWS_Y); // 创建绘图窗口 HWND hWnd=GetHWnd();
/*---------------------------定义各个操作区间-----------------------------------------------------------------------*/
	RECT r = { 0, 0, WINDOWS_X, WINDOWS_Y / 10};//顶部框架
	RECT r2 = { 150, WINDOWS_Y/10, WINDOWS_X-150, WINDOWS_Y /10+(BBLACK-40)+2 };//正文1
	RECT r3 = { 150, WINDOWS_Y /10+(BBLACK-40)+3, WINDOWS_X-150-2, WINDOWS_Y /10+2*(BBLACK-40)+2};//输入1
	RECT r4 = { 150, WINDOWS_Y /10+2*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+3*(BBLACK-40)+2 };//正文2
	RECT r5 = { 150, WINDOWS_Y /10+3*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+4*(BBLACK-40)+2 };//输入2
	RECT r6 = { 150, WINDOWS_Y /10+4*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+5*(BBLACK-40)+2 };//正文3
	RECT r7 = { 150, WINDOWS_Y /10+5*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+6*(BBLACK-40)+2 };//输入3
	RECT r8 = { 150, WINDOWS_Y /10+6*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+7*(BBLACK-40)+2 };//正文4
	RECT r9 = { 150, WINDOWS_Y /10+7*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+8*(BBLACK-40)+2 };//输入4
	RECT r10 = { 150, WINDOWS_Y /10+8*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+9*(BBLACK-40)+2 };//正文5
	RECT r11 = { 150, WINDOWS_Y /10+9*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+10*(BBLACK-40)+2 };//输入5
	RECT r12 = { 150, WINDOWS_Y /10+10*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+11*(BBLACK-40)+2 };//正文6
	RECT r13 = { 150, WINDOWS_Y /10+11*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+12*(BBLACK-40)+2 };//输入6
	RECT r14 = { 150, WINDOWS_Y /10+12*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+13*(BBLACK-40)+2 };//正文7
	RECT r15 = { 150, WINDOWS_Y /10+13*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+14*(BBLACK-40)+2 };//输入7
	RECT r16 = { 150, WINDOWS_Y /10+14*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+15*(BBLACK-40)+2 };//正文8
	RECT r17 = { 150, WINDOWS_Y /10+15*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+16*(BBLACK-40)+2 };//输入8
	RECT r18 = { 0+2, WINDOWS_Y /10+16*(BBLACK-40)+3, WINDOWS_X-2, WINDOWS_Y+2 };//底部提示区
	
	string str;
	HWND hWnd=GetForegroundWindow();
	LOGFONT f;
	setbkmode(TRANSPARENT);					//透明字体
	gettextstyle(&f);						// 获取字体样式
	f.lfHeight = (BBLACK-40)-10;					// 设置字体高度
	strcpy(f.lfFaceName, _T("方正姚体"));	// 设置字体
	f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	settextstyle(&f);						// 设置字体样式
	setlinecolor(RED);
	if((p=fopen("testenglish.txt","r"))==NULL)
	{
	   drawtext("文件打开失败!", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	   exit(0);
	}
	//fgets(ag,1000,p);
	while(true)
	{
		BeginBatchDraw(); // 设置批绘图模式
		long start=clock();
		if (kbhit()) //如果是ASCII输入
		{
			char c=getch();
			if (c=='\b')
			{
				if (str.length()>0)
				{
					if (str.at(str.length()-1)&0x8000)
						str.erase(str.end()-1);
					str.erase(str.end()-1);
				}
			}
			else if (c==27) {break; }
			else
			{
				str+=c;
			}
		}
		else //除此之外，检测是否有IME输入，如果有，则将输入结果添加到string中
		{
			GetIMEString(hWnd,str);
		}
		strcpy(ai,str.c_str());
		strcpy(b,str.c_str());
		Sleep(15);
		cleardevice();
		loadimage(NULL,"bkAndy2english.jpg");
/*--------------------------------------批量画线--------------------------------------------*/
		rectangle(0, 0, WINDOWS_X, WINDOWS_Y / 10);
		for(i=0;i<10;i++)
		{
			line(150, WINDOWS_Y/10+80*i, WINDOWS_X-150, WINDOWS_Y / 10+80*i);
		}
		rectangle(0+2, WINDOWS_Y /10+16*(BBLACK-40), WINDOWS_X-2, WINDOWS_Y-2);
		settextcolor(BLUE);
		drawtext("英文打字", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("提示：\t 1.退出练习按Esc键\n\t2.可以练习自己的文档，把自己的文档复制粘贴到本程序test.txt文档中，再返回练习即可。", &r18,DT_VCENTER);
		settextcolor(WHITE);
/*--------------------------------------读取文件内容并打印---------------------------------*/
		rewind(p);
		if (str.length()>89*8-8 || temp1!=0)
		{
			fseek(p,89*8-8,SEEK_SET);
			++temp1;
		}
		fgets(ag,89*8,p);
		for(i=0;ag[i]!='\0';i++)
			EnQ(Q,ag[i]);//入队保存
		rewind(p);
		if (str.length()>89*8-8 || temp!=0)
		{
			fseek(p,89*8-8,SEEK_SET);
			++temp;
		}
		fgets(c,89,p);
		drawtext(c, &r2,DT_VCENTER);
		fgets(d,89,p);
		drawtext(d, &r4,DT_VCENTER);
		fgets(e,89,p);
		drawtext(e, &r6,DT_VCENTER);
		fgets(ff,89,p);
		drawtext(ff, &r8,DT_VCENTER);
		fgets(g,89,p);
		drawtext(g, &r10,DT_VCENTER);
		fgets(h,89,p);
		drawtext(h, &r12,DT_VCENTER);
		fgets(ii,89,p);
		drawtext(ii, &r14,DT_VCENTER);
		fgets(jj,89,p);
		drawtext(jj, &r16,DT_VCENTER);
/*-------------------------------------字符串比较并分析输出--------------------------------------------*/
		if (str.length()>89*8-8)
		{
			str="";
		}
		char c=DeQ(Q);//出队比较
		for(i=0;i<8*89&&ag[i]!='\0'&&ai[i]!='\0';i++)
		{
			if(ag[i]!=ai[i])
			{
				settextcolor(RED);
			}
			else
			{
				settextcolor(GREEN);
			}
		}
		if (str.length()<=89-1) drawtext(b, &r3,NULL);
		else if (str.length()<=89*2-2)
		{
			drawtext(b, &r3,NULL);
			DeleteString(b,result,0,89);
			drawtext(result, &r5,NULL);
		}
		else if (str.length()<=89*3-3)
		{
			drawtext(b, &r3,NULL);
			DeleteString(b,result,0,89);
			drawtext(result, &r5,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r7,NULL);
		}
		else if (str.length()<=89*4-4)
		{
			drawtext(b, &r3,NULL);
			DeleteString(b,result,0,89);
			drawtext(result, &r5,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r7,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r9,NULL);
		}
		else if (str.length()<=89*5-5)
		{
			drawtext(b, &r3,NULL);
			DeleteString(b,result,0,89);
			drawtext(result, &r5,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r7,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r9,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r11,NULL);
		}
		else if (str.length()<=89*6-6)
		{
			drawtext(b, &r3,NULL);
			DeleteString(b,result,0,89);
			drawtext(result, &r5,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r7,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r9,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r11,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r13,NULL);
		}
		else if (str.length()<=89*7-7)
		{
			drawtext(b, &r3,NULL);
			DeleteString(b,result,0,89);
			drawtext(result, &r5,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r7,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r9,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r11,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r13,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r15,NULL);
		}
		else if (str.length()<=89*8-8)
		{
			drawtext(b, &r3,NULL);
			DeleteString(b,result,0,89);
			drawtext(result, &r5,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r7,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r9,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r11,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r13,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r15,NULL);
			DeleteString(result,result,0,89);
			drawtext(result, &r17,NULL);
		}
		settextcolor(WHITE);
		FlushBatchDraw();
		Sleep(10);
	//	EndBatchDraw();
	}
	closegraph();
} 

void GetIMEString(HWND hWnd,string& str)
{
	HIMC hIMC=ImmGetContext(hWnd);//获取HIMC
	if(hIMC)
	{
		//这里先说明一下，以输入“中国”为例 
		//切换到中文输入法后，输入“zhongguo”，这个字符串称作IME组成字符串 
		//而在输入法列表中选择的字符串“中国”则称作IME结果字符串
		static bool flag=false;//输入完成标记：在输入中时，IME组成字符串不为空，置true；输入完成后，IME组成字符串为空，置false
		DWORD dwSize=ImmGetCompositionStringW(hIMC,GCS_COMPSTR,NULL,0); //获取IME组成输入的字符串的长度
		if (dwSize>0)//如果IME组成字符串不为空，且没有错误（此时dwSize为负值），则置输入完成标记为true 
		{
			if (flag==false)
			{
				flag=true;
			}
		}
		else if (dwSize==0&&flag) //如果IME组成字符串为空，并且标记为true，则获取IME结果字符串 
		{
			int iSize; //IME结果字符串的大小
			LPSTR pszMultiByte=NULL;//IME结果字符串指针
			int ChineseSimpleAcp=936;//宽字节转换时中文的编码
			WCHAR *lpWideStr=NULL;//宽字节字符数组
			dwSize=ImmGetCompositionStringW(hIMC,GCS_RESULTSTR,NULL,0);//获取IME结果字符串的大小
			if (dwSize>0) //如果IME结果字符串不为空，且没有错误
			{
				dwSize+=sizeof(WCHAR);//大小要加上NULL结束符
				//为获取IME结果字符串分配空间
				if (lpWideStr)
				{
					delete []lpWideStr;
					lpWideStr=NULL;
				}
				lpWideStr=new WCHAR[dwSize];
				memset(lpWideStr,0,dwSize); //清空结果空间
				ImmGetCompositionStringW(hIMC,GCS_RESULTSTR,lpWideStr,dwSize);//获取IME结果字符串，这里获取的是宽字节
				iSize=WideCharToMultiByte(ChineseSimpleAcp,0,lpWideStr,-1,NULL,0,NULL,NULL);//计算将IME结果字符串转换为ASCII标准字节后的大小
				//为转换分配空间
				if (pszMultiByte)
				{
					delete [] pszMultiByte;
					pszMultiByte=NULL;
				}
				pszMultiByte=new char[iSize+1];
				WideCharToMultiByte(ChineseSimpleAcp,0,lpWideStr,-1,pszMultiByte,iSize,NULL,NULL);//宽字节转换
				pszMultiByte[iSize]='\0';
				str+=pszMultiByte;//添加到string中
				//释放空间
				if (lpWideStr)
				{
					delete []lpWideStr;
					lpWideStr=NULL;
				}
				if (pszMultiByte)
				{
					delete [] pszMultiByte;
					pszMultiByte=NULL;
				}
			}
			flag=false;
		}
		ImmReleaseContext(hWnd,hIMC);//释放HIMC 
	}
}