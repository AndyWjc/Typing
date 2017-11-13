#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <string>
#include<time.h>
#pragma comment(lib,"Imm32.lib") 
using namespace std;
#define WINDOWS_X 1280	//���ڴ�СX
#define WINDOWS_Y 800   //���ڴ�СY
#define BBLACK 80		//�ո��С
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
/*ջ�Ĵ洢�ṹ*/
typedef struct{
	SElemType *base; /*ջ��ָ��*/
	SElemType *top;  /*ջ��ָ��*/
	int stacksize;   /*ջ��ǰ�ѷ�������пռ䣬������ʹ�õĿռ䳤��*/
}SqStack;
/*start***************���еĴ洢�ṹ*********************/
typedef struct QNode{
	QElemType data; 
	struct QNode *next;
}QNode,*QueuePtr;
typedef struct{
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;
/****************���еĴ洢�ṹ********************end*/

/*��������*/
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

void EnQ(LinkQueue *Q,QElemType e) //���
{
	QueuePtr p;
	p=(QueuePtr)malloc(sizeof(QNode));
	if(!p) exit(1);
	p->data=e;
	p->next=NULL;
	Q->rear->next=p;
	Q->rear=p;
}

char DeQ(LinkQueue *Q) //����
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

SqStack * InitStack() /*1.��ʼ��ջ*/
{
	SqStack *S;
	S=(SqStack *)malloc(sizeof(SqStack));
	S->base=(SElemType *)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if(!S || !S->base) exit(1);
	S->top=S->base;
	S->stacksize=STACK_INIT_SIZE;
	return S;
}

bool SEmpty(SqStack *S) /*2.��ջ��*/
{
	if(S->top==S->base)	return 0;
	else return 1;  /*1��ʾջ����*/
}

void Push(SqStack *S,SElemType e) /*3.��ջ*/
{
	if(S->top-S->base >= S->stacksize )
	{
		S->base=(SElemType *)realloc(S->base,(S->stacksize+STACKINCREMENT)*sizeof(SElemType));
		if(!S->base) exit(1);
		printf("\n�ռ䲻���������¿ռ�ɹ���\n");
		S->top=S->base+S->stacksize;
		S->stacksize +=STACKINCREMENT;
	}
	*S->top++=e;
}

SElemType Pop(SqStack *S) /*4.��ջ*/
{
	if(S->top==S->base)
	{
		return 0;
		printf("\nThe sequence stack is empty!");
	}
	else return *(--S->top);
}

void display(SqStack *S) /*5.��ӡ*/
{
	int e;

	SqStack *T;
	T=InitStack(); /*1.�¹���һ��ջT*/
	
	while(SEmpty(S))
	{
		e=Pop(S);
		Push(T,e);/*2.����ջ��ӡ��Ԫ����ջT*/
		printf("%5d",e); /*��ջ�������δ�ӡ*/
	}
	while(SEmpty(T)) Push(S,Pop(T));/*3.��ջT��Ԫ�س�ջ����ջ��ջS��*/
	free(T);
	printf("\n");
}

int StackLength(SqStack *S) /*6.ͳ��ջ����*/
{
	printf("\nͳ�Ƶ�ջ�ĳ���Ϊ��%d\n",(S->top-S->base));
	return (S->top-S->base);
}

void search(SqStack *S,SElemType e) /*7.����*/
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
			printf("\n%d ���ҵ�Ԫ�أ�%d\n",count,e);
		}
		Push(T,Pop(S));
	}
	while(SEmpty(T)) Push(S,Pop(T));
	free(T);
	if(flag==0) printf("\nû���ҵ�Ԫ�أ�%d\n",e);
}

void modify(SqStack *S,SElemType e) /*8.�޸�*/
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
			printf("\n%d ���ҵ�Ԫ�أ�%d\n",count,e);
			printf("\n1.�޸�   2.���޸ģ�");
			scanf("%d",&a);
			switch(a)
			{
			case 1:
				e1=Pop(S);
				printf("\n��Ԫ��%d�޸�Ϊ��",e);   
				scanf("%d",&e1);
				Push(S,e1);
				printf("\n�޸ĳɹ���\n");
			//������1֮�������������κβ���
			}
		}
		Push(T,Pop(S));
	}
	while(SEmpty(T)) Push(S,Pop(T));
	free(T);
	if(flag==0) printf("\nû���ҵ�Ԫ�أ�%d\n",e);
}

void ClearStack(SqStack *S) /*9.���ջ*/
{
	S->top=S->base;
	printf("\n���ջ�ɹ���\n");
}

void DestoryStatck(SqStack *S) /*10.����ջ*/ 
{  
    S->top=S->base;  
    free(S->base);  
    S->base=NULL;  
    S->top=NULL;
	S->stacksize=0;
	free(S);
    printf("\n����ջ�ɹ���\n");    
}

SElemType GetTop(SqStack *S) /*11.ȡջ��Ԫ��*/
{
	if(S->top==S->base)
	{
		printf("\nThe sequence stack is empty!\n");
		return 0;
	}
	else return *(S->top-1); /*������Pop�е�*--S->top,*(S->top-1)���ı�S->topָ���ָ��*/	
}
//ɾ��ָ��λ�ÿ�ʼָ���ַ������Ⱥ�����
void DeleteString(char* array,char*result,int begin,int len)//begin����ʼɾ����λ�ã�len��ɾ���ĳ��ȡ�
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
	static int ch = 0;//���滻����Ϣ
	char result[100];

	initgraph(WINDOWS_X, WINDOWS_Y); // ������ͼ���� HWND hWnd=GetHWnd();
/*---------------------------���������������-----------------------------------------------------------------------*/
	RECT r = { 0, 0, WINDOWS_X, WINDOWS_Y / 10};//�������
	RECT r2 = { 150, WINDOWS_Y/10, WINDOWS_X-150, WINDOWS_Y /10+(BBLACK-40)+2 };//����1
	RECT r3 = { 150, WINDOWS_Y /10+(BBLACK-40)+3, WINDOWS_X-150-2, WINDOWS_Y /10+2*(BBLACK-40)+2};//����1
	RECT r4 = { 150, WINDOWS_Y /10+2*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+3*(BBLACK-40)+2 };//����2
	RECT r5 = { 150, WINDOWS_Y /10+3*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+4*(BBLACK-40)+2 };//����2
	RECT r6 = { 150, WINDOWS_Y /10+4*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+5*(BBLACK-40)+2 };//����3
	RECT r7 = { 150, WINDOWS_Y /10+5*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+6*(BBLACK-40)+2 };//����3
	RECT r8 = { 150, WINDOWS_Y /10+6*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+7*(BBLACK-40)+2 };//����4
	RECT r9 = { 150, WINDOWS_Y /10+7*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+8*(BBLACK-40)+2 };//����4
	RECT r10 = { 150, WINDOWS_Y /10+8*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+9*(BBLACK-40)+2 };//����5
	RECT r11 = { 150, WINDOWS_Y /10+9*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+10*(BBLACK-40)+2 };//����5
	RECT r12 = { 150, WINDOWS_Y /10+10*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+11*(BBLACK-40)+2 };//����6
	RECT r13 = { 150, WINDOWS_Y /10+11*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+12*(BBLACK-40)+2 };//����6
	RECT r14 = { 150, WINDOWS_Y /10+12*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+13*(BBLACK-40)+2 };//����7
	RECT r15 = { 150, WINDOWS_Y /10+13*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+14*(BBLACK-40)+2 };//����7
	RECT r16 = { 150, WINDOWS_Y /10+14*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+15*(BBLACK-40)+2 };//����8
	RECT r17 = { 150, WINDOWS_Y /10+15*(BBLACK-40)+3, WINDOWS_X-150, WINDOWS_Y /10+16*(BBLACK-40)+2 };//����8
	RECT r18 = { 0+2, WINDOWS_Y /10+16*(BBLACK-40)+3, WINDOWS_X-2, WINDOWS_Y+2 };//�ײ���ʾ��
	
	string str;
	HWND hWnd=GetForegroundWindow();
	LOGFONT f;
	setbkmode(TRANSPARENT);					//͸������
	gettextstyle(&f);						// ��ȡ������ʽ
	f.lfHeight = (BBLACK-40)-10;					// ��������߶�
	strcpy(f.lfFaceName, _T("����Ҧ��"));	// ��������
	f.lfQuality = ANTIALIASED_QUALITY;		// �������Ч��Ϊ�����  
	settextstyle(&f);						// ����������ʽ
	setlinecolor(RED);
	if((p=fopen("testenglish.txt","r"))==NULL)
	{
	   drawtext("�ļ���ʧ��!", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	   exit(0);
	}
	//fgets(ag,1000,p);
	while(true)
	{
		BeginBatchDraw(); // ��������ͼģʽ
		long start=clock();
		if (kbhit()) //�����ASCII����
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
		else //����֮�⣬����Ƿ���IME���룬����У�����������ӵ�string��
		{
			GetIMEString(hWnd,str);
		}
		strcpy(ai,str.c_str());
		strcpy(b,str.c_str());
		Sleep(15);
		cleardevice();
		loadimage(NULL,"bkAndy2english.jpg");
/*--------------------------------------��������--------------------------------------------*/
		rectangle(0, 0, WINDOWS_X, WINDOWS_Y / 10);
		for(i=0;i<10;i++)
		{
			line(150, WINDOWS_Y/10+80*i, WINDOWS_X-150, WINDOWS_Y / 10+80*i);
		}
		rectangle(0+2, WINDOWS_Y /10+16*(BBLACK-40), WINDOWS_X-2, WINDOWS_Y-2);
		settextcolor(BLUE);
		drawtext("Ӣ�Ĵ���", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext("��ʾ��\t 1.�˳���ϰ��Esc��\n\t2.������ϰ�Լ����ĵ������Լ����ĵ�����ճ����������test.txt�ĵ��У��ٷ�����ϰ���ɡ�", &r18,DT_VCENTER);
		settextcolor(WHITE);
/*--------------------------------------��ȡ�ļ����ݲ���ӡ---------------------------------*/
		rewind(p);
		if (str.length()>89*8-8 || temp1!=0)
		{
			fseek(p,89*8-8,SEEK_SET);
			++temp1;
		}
		fgets(ag,89*8,p);
		for(i=0;ag[i]!='\0';i++)
			EnQ(Q,ag[i]);//��ӱ���
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
/*-------------------------------------�ַ����Ƚϲ��������--------------------------------------------*/
		if (str.length()>89*8-8)
		{
			str="";
		}
		char c=DeQ(Q);//���ӱȽ�
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
	HIMC hIMC=ImmGetContext(hWnd);//��ȡHIMC
	if(hIMC)
	{
		//������˵��һ�£������롰�й���Ϊ�� 
		//�л����������뷨�����롰zhongguo��������ַ�������IME����ַ��� 
		//�������뷨�б���ѡ����ַ������й��������IME����ַ���
		static bool flag=false;//������ɱ�ǣ���������ʱ��IME����ַ�����Ϊ�գ���true��������ɺ�IME����ַ���Ϊ�գ���false
		DWORD dwSize=ImmGetCompositionStringW(hIMC,GCS_COMPSTR,NULL,0); //��ȡIME���������ַ����ĳ���
		if (dwSize>0)//���IME����ַ�����Ϊ�գ���û�д��󣨴�ʱdwSizeΪ��ֵ��������������ɱ��Ϊtrue 
		{
			if (flag==false)
			{
				flag=true;
			}
		}
		else if (dwSize==0&&flag) //���IME����ַ���Ϊ�գ����ұ��Ϊtrue�����ȡIME����ַ��� 
		{
			int iSize; //IME����ַ����Ĵ�С
			LPSTR pszMultiByte=NULL;//IME����ַ���ָ��
			int ChineseSimpleAcp=936;//���ֽ�ת��ʱ���ĵı���
			WCHAR *lpWideStr=NULL;//���ֽ��ַ�����
			dwSize=ImmGetCompositionStringW(hIMC,GCS_RESULTSTR,NULL,0);//��ȡIME����ַ����Ĵ�С
			if (dwSize>0) //���IME����ַ�����Ϊ�գ���û�д���
			{
				dwSize+=sizeof(WCHAR);//��СҪ����NULL������
				//Ϊ��ȡIME����ַ�������ռ�
				if (lpWideStr)
				{
					delete []lpWideStr;
					lpWideStr=NULL;
				}
				lpWideStr=new WCHAR[dwSize];
				memset(lpWideStr,0,dwSize); //��ս���ռ�
				ImmGetCompositionStringW(hIMC,GCS_RESULTSTR,lpWideStr,dwSize);//��ȡIME����ַ����������ȡ���ǿ��ֽ�
				iSize=WideCharToMultiByte(ChineseSimpleAcp,0,lpWideStr,-1,NULL,0,NULL,NULL);//���㽫IME����ַ���ת��ΪASCII��׼�ֽں�Ĵ�С
				//Ϊת������ռ�
				if (pszMultiByte)
				{
					delete [] pszMultiByte;
					pszMultiByte=NULL;
				}
				pszMultiByte=new char[iSize+1];
				WideCharToMultiByte(ChineseSimpleAcp,0,lpWideStr,-1,pszMultiByte,iSize,NULL,NULL);//���ֽ�ת��
				pszMultiByte[iSize]='\0';
				str+=pszMultiByte;//��ӵ�string��
				//�ͷſռ�
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
		ImmReleaseContext(hWnd,hIMC);//�ͷ�HIMC 
	}
}