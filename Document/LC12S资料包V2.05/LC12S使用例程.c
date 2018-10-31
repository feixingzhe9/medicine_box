/**************************************************************************************************
								���о����LC12Sʹ������
		�������ݣ�ֻ��Ҫ������a����ֵ������UartSendBuf(a,lenth)���������
		�������ݣ�����˵����ݸ�ʽΪ AA5A [.......]  BB6B    
		 [.......]  Ϊ�ͻ��Լ���Ҫ�����ݣ����ճ������ȷ���պ���uart_handle��������
		 
		 ������ ���Է��������Ӧ��Ϊ AA5A 11[...] BB6B    //[...]����д�����ֽڻ�д
		 uart_handle�����ж�����Ч���ݵĵ�һ������11�������11����LED�ȷ�ת
		 
																june
																2018.7.28
**************************************************************************************************/
#define DATA_START0 0xAA   	//����ͷ1
#define DATA_START1 0x5A	//����ͷ2
#define DATA_END0 0xBB		//���ݽ�β1
#define DATA_END1 0x6B		//���ݽ�β1

#define UART_LENGTH 128		//�������ݻ�����
#define LED P11
unsigned char UartBuffer[UART_LENGTH],write=0,data_start_true=0;

//���ڳ�ʼ������
//����11.0592MHZ ������9600
void UartConfig()				
{
	//���ڷ�ʽ1
	TMOD=0X20;    //T1��ʱ����������ʽ2
	TH1=0XFD;    //T1��ʱ��װ��ֵ   //253=256-(1*11.0592*10^6)/(384*9600)
	TL1=0XFD;    //T1��ʱ��װ��ֵ
	TR1=1;        //����T1��ʱ��
	REN=1;        //�����ڽ���
	SM0=0;        //�趨���ڹ�����ʽ1
	SM1=1;        //�趨���ڹ�����ʽ1
	EA=1;        //�����ж�
	ES=1;        //�������ж�                         
}



void UartSendByte(uint8 ch){					//���ڷ��͵��ַ�����
	 
    SBUF= ch;
	while(!TI);
    TI = 0;
}

void UartSendBuf(char*buf,uint8 sz){			//���ڷ����ַ�������
    uint8 i;
    for(i=0;i<sz;i++){
        UartSendByte(buf[i]);
    }
}

//���յ���ȷ����ͷ�����ݽ�β�Ĵ�����
//����ͷAA5A ���ݽ�βBB6B
//��������AA5A 11 BB6B ��LED�ƻ�ȡ��
void uart_handle(void)							
{
		if(UartBuffer[2]==0x11)
		LED=!LED;
	
	
}
//�����жϺ���
//����ʶ������ͷ�����ݽ�β�������ȷʧ�ܣ�������uart_handle����������
//�û�ֻ��Ҫ�޸�uart_handle�������������Ӧ�Ĳ���

void uart_inter() interrupt 4				
{
	uint8 ch,i;
	if(RI)
	{
		RI=0;
		ch=SBUF;
		UartBuffer[write++] = ch;
		
		if(UartBuffer[0]!=DATA_START0)//�ж�����ͷ1�Ƿ���ȷ���������ȷ������ָ�뻹��0��������һ���ж�
		{
			write=0;
		}
		else						//�������ͷ1��ȷ���ж�����ͷ2�Ƿ���ȷ
		{
			if((write==2))			//һ��Ҫ����Ϊ����1��ȷ�ˣ����� UartBuffer[1] ������Զ���������ݵģ�һ�������������ж�
			{
					if((UartBuffer[1]!=DATA_START1))//�������ͷ1��ȷ������ͷ2���󣬸�UartBuffer[0��1]����ֵ
					{
						UartBuffer[0]=1;
						UartBuffer[1]=1;
						write=0;
					}
					else							//����ͷ��ȷ			
					{
						data_start_true=1;	
					}
			}	
		}
		if(data_start_true)    						//��� ����ͷ��ȷ	�������ж����ݽ�β
		{
			if((write>3))							//һ��Ҫ�ģ���Ϊwrite-2����С��0���������
			{
				if((UartBuffer[write-2]==DATA_END0)&&(UartBuffer[write-1]==DATA_END1))//�ж����ݽ�β�Ƿ���ȷ
				{
					uart_handle();					//���ݴ�����
					for(i=0;i<write;i++)			//�����鸳��ֵ�����ܸ�����ͷ�ͽ�β��ͬ������
						UartBuffer[i]=1;
					data_start_true=0;				//���ݳ����꣬Ӧ��������ͷ��ȷ��־����
					write=0;						//ָ������
				}
			}
			
		}			
	}
}

//��ʱ����
void Delay_ms(int ms)
{
    int i, j;

    for(i=0; i<ms; i++)
        for(j=0; j<1400; j++);
			
}



#define lenth 10					//�������ݳ��ȣ����Ը��ݵ�Ƭ���ڴ������޸�
void main(void){

	uint8 i,a[lenth];
	UartConfig();  				//���ڳ�ʼ��
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
		Delay_ms(500);				//��ʱ����û��
	}
		
}



