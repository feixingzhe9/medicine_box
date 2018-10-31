/**************************************************************************************************
								凌承芯电子LC12S使用例程
		发射数据：只需要给数据a赋初值，调用UartSendBuf(a,lenth)函数变可以
		接收数据：发射端的数据格式为 AA5A [.......]  BB6B    
		 [.......]  为客户自己需要的数据，接收程序会正确接收后用uart_handle（）处理
		 
		 例程中 可以发射放数据应该为 AA5A 11[...] BB6B    //[...]可以写任意字节或不写
		 uart_handle（）判断了有效数据的第一个数据11，如果是11，则LED等反转
		 
																june
																2018.7.28
**************************************************************************************************/
#define DATA_START0 0xAA   	//数据头1
#define DATA_START1 0x5A	//数据头2
#define DATA_END0 0xBB		//数据结尾1
#define DATA_END1 0x6B		//数据结尾1

#define UART_LENGTH 128		//串口数据缓存区
#define LED P11
unsigned char UartBuffer[UART_LENGTH],write=0,data_start_true=0;

//串口初始化设置
//晶振11.0592MHZ 波特率9600
void UartConfig()				
{
	//串口方式1
	TMOD=0X20;    //T1定时器，工作方式2
	TH1=0XFD;    //T1定时器装初值   //253=256-(1*11.0592*10^6)/(384*9600)
	TL1=0XFD;    //T1定时器装初值
	TR1=1;        //启动T1定时器
	REN=1;        //允许串口接收
	SM0=0;        //设定串口工作方式1
	SM1=1;        //设定串口工作方式1
	EA=1;        //开总中断
	ES=1;        //开串口中断                         
}



void UartSendByte(uint8 ch){					//串口发送单字符函数
	 
    SBUF= ch;
	while(!TI);
    TI = 0;
}

void UartSendBuf(char*buf,uint8 sz){			//串口发射字符串函数
    uint8 i;
    for(i=0;i<sz;i++){
        UartSendByte(buf[i]);
    }
}

//接收到正确数据头和数据结尾的处理函数
//数据头AA5A 数据结尾BB6B
//发射数据AA5A 11 BB6B 则LED灯会取反
void uart_handle(void)							
{
		if(UartBuffer[2]==0x11)
		LED=!LED;
	
	
}
//串口中断函数
//用于识别数据头和数据结尾，如果正确失败，则会调用uart_handle（）处理函数
//用户只需要修改uart_handle（）函数完成响应的操作

void uart_inter() interrupt 4				
{
	uint8 ch,i;
	if(RI)
	{
		RI=0;
		ch=SBUF;
		UartBuffer[write++] = ch;
		
		if(UartBuffer[0]!=DATA_START0)//判断数据头1是否正确，如果不正确，数据指针还是0，用于下一次判断
		{
			write=0;
		}
		else						//如果数据头1正确，判断数据头2是否正确
		{
			if((write==2))			//一定要，因为数据1正确了，但是 UartBuffer[1] 里面永远都是有数据的，一定会进行下面的判断
			{
					if((UartBuffer[1]!=DATA_START1))//如果数据头1正确，数据头2错误，给UartBuffer[0，1]赋初值
					{
						UartBuffer[0]=1;
						UartBuffer[1]=1;
						write=0;
					}
					else							//数据头正确			
					{
						data_start_true=1;	
					}
			}	
		}
		if(data_start_true)    						//如果 数据头正确	接下来判断数据结尾
		{
			if((write>3))							//一定要的，因为write-2可能小于0，数组出错
			{
				if((UartBuffer[write-2]==DATA_END0)&&(UartBuffer[write-1]==DATA_END1))//判断数据结尾是否正确
				{
					uart_handle();					//数据处理函数
					for(i=0;i<write;i++)			//给数组赋初值，不能跟数据头和结尾相同的数据
						UartBuffer[i]=1;
					data_start_true=0;				//数据出来完，应该让数据头正确标志清零
					write=0;						//指针清零
				}
			}
			
		}			
	}
}

//延时函数
void Delay_ms(int ms)
{
    int i, j;

    for(i=0; i<ms; i++)
        for(j=0; j<1400; j++);
			
}



#define lenth 10					//发射数据长度，可以根据单片机内存随意修改
void main(void){

	uint8 i,a[lenth];
	UartConfig();  				//串口初始化
	for(i=0;i<lenth;i++)
	a[i]=i;
	LED=0;
	while(1)
	{
		UartSendBuf(a,lenth);
//		UartSendBuf(a,lenth);
//		UartSendBuf(a,lenth);
//		UartSendBuf(a,lenth);
//		UartSendBuf(a,lenth);
//		UartSendBuf(a,lenth);
//		UartSendBuf(a,lenth);
//		UartSendBuf(a,lenth);
//		UartSendBuf(a,lenth);
		Delay_ms(500);				//延时可以没有
	}
		
}



