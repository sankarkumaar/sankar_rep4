
void decimalcopyhex_date(char *msg);
void decimalcopyhex_month(char *msg);
void decimalcopyhex_Year(char *msg);
void decimalcopyhex(char *msg);
char * weekdays[7] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
extern unsigned char time[20];


extern unsigned char Hour,Min,Sec;
extern unsigned char Date,Month,Year;
char Check;

//unsigned char RTC_bit=0;////
unsigned char rx_time[50];
char change_bit=0;

void decimalcopyhex(char *msg)
{
    unsigned char  temp,i=0,cnt=0;


        Hour  = ((msg[11]-48)*10)+(msg[12]-48);
        Min   = ((msg[14]-48)*10)+(msg[15]-48);
        Sec   = ((msg[17]-48)*10)+(msg[18]-48);

        for(i=0;i<3;i++)
        {
                if(i==0)
                {
                    Check  = Hour;

                }
                if(i==1)
                {
                    Min = Min;
                    Check  = Min;
                }
                if(i==2)
                    Check  = Sec;

                if(Check<10)
                {
                    Check =  Check;
                }
                else if((Check>=10)&&(Check<20))
                {
                    temp =   Check%10;
                    Check =  Check+6;
                }
                else if((Check>=20)&&(Check<30))
                {
                    temp =   Check%10;
                    Check =  Check+12;
                }
                else if((Check>=30)&&(Check<40))
                {
                    temp =   Check%10;
                    Check =  Check+18;
                }
                else if((Check>=40)&&(Check<50))
                {
                    temp =   Check%10;
                    Check =  Check+24;
                }
                else if((Check>=50)&&(Check<60))
                {
                    temp =   Check%10;
                    Check =  Check+30;
                }
                else if(Check>=60)
                {
                    temp =   Check%10;
                    Check =  Check+36;
                }
                if(i==0)
                {

                    Hour = Check;

//                    ei_printf("Hour  <%d",Hour);
//                    ei_printf("> ");
                }
                if(i==1)
                {
                    Min  = Check;

//                    ei_printf("Min  <%d",Min);
//                    ei_printf(" >");
                }
                if(i==2)
                {
                    Sec  = Check;

//                    ei_printf("Sec  <%d",Sec);
//                    ei_printf('>');
                }
        }

}

void decimalcopyhex_date(char *msg)
{
            unsigned char  Checkval=0;
            Date  = ((msg[8]-48)*10)+(msg[9]-48);

            Check  = Date;

            if((Check>=10)&&(Check<20))
            {
                Checkval =  Check+6;
            }
            else if(Check<10)
            {
                Checkval =  Check;
            }
            else if((Check>=20)&&(Check<30))
            {
                Checkval =  Check+12;
            }
            else if((Check>=30)&&(Check<40))
            {
                Checkval =  Check+18;
            }

            Date  = Checkval;



//            ei_printf("Date< %X",Date);
//            ei_printf('>');

    }

void decimalcopyhex_month(char *msg)
{
            unsigned char  Checkval=0;
            Month  = ((msg[5]-48)*10)+(msg[6]-48);

            Check  = Month;

            if((Check>=10)&&(Check<20))
            {
                Checkval =  Check+6;
            }
            else if(Check<10)
            {
                Checkval =  Check;
            }
            else if((Check>=20)&&(Check<30))
            {
                Checkval =  Check+12;
            }
            else if((Check>=30)&&(Check<40))
            {
                Checkval =  Check+18;
            }

            Month  = Checkval;



//            ei_printf("Month < %d",Month);
//            ei_printf('>');

    }



void decimalcopyhex_Year(char *msg)
{

  unsigned char  Checkval,i=0,cnt=0;


    Year  = ((msg[2]-48)*10)+((msg[3]-48));
    Check  = Year;

        if((Check>=10)&&(Check<20))
        {
            Checkval =  Check+6;
        }
        else if((Check>=20)&&(Check<30))
        {
            Checkval =  Check+12;
        }
        else if((Check>=30)&&(Check<40))
        {
            Checkval =  Check+18;
        }
        else if((Check>=40)&&(Check<50))
        {
            Checkval =  Check+24;
        }
        else if((Check>=50)&&(Check<60))
        {
            Checkval =  Check+30;
        }
        else if(Check>=60)
        {
            Checkval =  Check+36;
        }

        Year  = Checkval;


}




void decimalcopyhex_Month(char *msg)
{

  unsigned char  Checkval=0,i=0,j=0,k=0,flag=0;
  char *month[12]={"Jan","Feb","Mar","Apr","May","June","July","Aug","Sept","Oct","Nov","Dec"};


     Month  = ((msg[5]-48)*10)+(msg[6]-48);
     //Month=i;

      /*  if((Check>=10)&&(Check<20))
        {
            Checkval =  Check+6;
        }
        else if((Check>=20)&&(Check<30))
        {
            Checkval =  Check+12;
        }
        else if((Check>=30)&&(Check<40))
        {
            Checkval =  Check+18;
        }
        else if((Check>=40)&&(Check<50))
        {
            Checkval =  Check+24;
        }
        else if((Check>=50)&&(Check<60))
        {
            Checkval =  Check+30;
        }
        else if(Check>=60)
        {
            Checkval =  Check+36;
        }

        Month  = Checkval;*/


          // Check  = Month;

            if((Check>=10)&&(Check<20))
            {
                Checkval =  Check+6;
            }
            else if(Check<10)
            {
                Checkval =  Check;
            }
            else if((Check>=20)&&(Check<30))
            {
                Checkval =  Check+12;
            }
            else if((Check>=30)&&(Check<40))
            {
                Checkval =  Check+18;
            }

          //  Month  = Checkval;




}

void update_time(char *rx_time)
{
    decimalcopyhex_date(rx_time);
    decimalcopyhex_month(rx_time);
    decimalcopyhex_Year(rx_time);
    decimalcopyhex(rx_time);
}


void converthex(char *data,unsigned int a)
{
    unsigned int temp=a;
    int digits=0,set;
    int i=0;
    unsigned int x;
    unsigned int mask;
    mask=0x0f;
    while(temp!=0){
        temp=temp/16;
        digits++;
        if(digits>1)mask=mask<<4;
    }
    set = digits;
    set--;

    //data[i]='0';
    //i++;
    //data[i]='x';
    //i++;
    temp=a;
    while(digits!=0)
    {
        x=(temp&mask);
        x=x>>(set*4);
        if(x<=9)data[i]=0x30+x;
        else
        {
            data[i]=0x41+(x-10);

        }
        digits--;
        i++;
        temp=temp<<4;
    }
    data[i]='\0';
    ei_printf("data %s",data);
}
