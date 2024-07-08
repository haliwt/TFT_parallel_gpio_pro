#include "bsp.h"


#define Zero_Degree           5828    
#define ADC_Sample_Times      10

#define COMPENSATION_VALUE    0

#define NTC_RES_F3425         1

#define DISP_VALUE            10






uint16_t *pArray[23];
uint8_t find_out_temperature_value;
uint8_t temp_uint16_t_vlue;
uint8_t length_simple;

uint16_t temp_variable_value_1;

uint8_t  temp_degree;
uint8_t temp_decimal_point;
uint16_t ntc_voltage_value;
uint8_t array_subscript;

uint8_t disp_temp_degree;

uint8_t read_input_times;

uint8_t disp_ntc_value[DISP_VALUE];




//static uint16_t Read_NTC_Temperature_Voltage(void);

static int8_t  Binary_Search(const uint8_t *array ,uint8_t key,uint8_t length);





//static void Read_Ntc_Decimal_Point_Numbers(void);

//static uint8_t Calculate_Display_Temperature_Value(const uint16_t *pt,uint16_t ntc_res,uint8_t length);

static uint8_t Calculate_Display_Temperature_Value(const uint16_t *pt,uint8_t key,uint16_t ntc_res_value,uint8_t length);




//static void Calculate_Speicial_Temperature_Value(uint8_t temp);

//static uint8_t  ntc_res_linear_value(uint8_t ntc_value);


uint8_t search_key;

typedef enum{

   degree_zero = 0,
   degree_one = 1,
   degree_two = 2,
   degree_three = 3,
   degree_four = 4,
   degree_five =5,
   degree_six = 6,
   degree_seven = 7,
   degree_eight = 8,
   degree_nine = 9,
   degree_ten = 10,
   degree_eleven = 11,
   degree_twelve = 12,
   degree_thirteen =13,
   degree_fourteen = 14,
   degree_fiveteen =15,
   degree_sixteen =16,
   degree_seventeen =17,
   degree_eighteen = 18,
   degree_nineteen = 19,
   degree_twenty = 20,
   degree_twenty_one = 21,
   degree_twenty_two =22
   
   


}degree_state;

typedef struct _ntc_t{

   uint8_t temperature_value;
   uint8_t ntc_voltage_value;
   uint8_t temperature_rectify_value;
  
   
}ntc_res_t;


ntc_res_t ntc_t;



int8_t left_point =0;
int8_t right_point ;
////int8_t length = sizeof(R10K_Init_20_96_simple)/(sizeof(R10K_Init_20_96_simple[0]));
int8_t mid_value;
//拆分成23个数组，mid =11
//static const uint16_t R10K_0_0[2]={2558,2528};
//static const uint16_t R10K_1_4[4]={2497,2466,2434,2402};


//static const uint16_t R10K_5_7[3]={2382,2348,2314};
//static const uint16_t R10K_8_10[3]={2280,2245,2210,};
//static const uint16_t R10K_11_13[3]={2174,2138,2102};

//static const uint16_t R10K_14_15[3]={2065,2028};
//static const uint16_t R10K_16_18[3]={1991,1954,1917};

//NTC RES F3950  new calculation 

static const uint16_t R10K_19_20[2]={1880,1816}; //array[0] = error range = 32
static const uint16_t R10K_21_23[3]={1784,1752,1721};//array[1] error range = 32
static const uint16_t R10K_24_26[3]={1689,1658,1627}; //array[2] error range = 32

static const uint16_t R10K_27_30[4]={1597,1567,1537,1507}; //array[3] =error range = 30
static const uint16_t R10K_31_33[3]={1477,1448,1419};       //array[4] =error range = 29
static const uint16_t R10K_34_37[4]={1391,1362,1334,1307}; //array[5]=error range= 27
static const uint16_t R10K_38_41[4]={1279,1253,1226,1200};  //array[6]=error range = 27
static const uint16_t R10K_42_44[3]={1175,1149,1124};  //array[7]=error range = 25

static const uint16_t R10K_45_49[5]={1099,1075,1051,1028,1005}; //array[8]=error range = 24

static const uint16_t R10K_50_53[4]={982,960,938,917};  //array[9]： error range = 21
static const uint16_t R10K_54_58[5]={896,875,855,835,816}; //array[10]：error range = 19

static const uint16_t R10K_59_64[6]={797,779,760,743,725,708};  //array[11]：error range = 16
static const uint16_t R10K_65_71[7]={692,675,660,644,629,614,600};  //array[12]： error range = 14 

static const uint16_t R10K_72_78[7]={585,572,558,545,532,520,507}; //array[13]:error range = 12

static const uint16_t R10K_79_88[10]={495,484,472,461,450,440,430,419,410,400}; ////array[14]:error range = 10

static const uint16_t R10K_89_100[12] ={391,382,373,364,356,348,340,332,324,317,309,302}; //array[15]:error range = 8

static const uint16_t R10K_101_118[18] ={295,289,282,276,270,264,258,252,246,241,235,230,225,220,215,211,206,201}; //array[16]:error range = 4

static const uint16_t R10K_119_120[2]={197,193}; //array[17] error range = 4



///


static const uint8_t R10K_Init_20_120_simple[18]={

	18,17,16,15,14,
	13,12,11,10,9,
	8,7,6,5,4,
	3,2,1
};



#if 0
static const uint8_t R10K_Init_0_81_simple[23]={

    25,24,23,22,21,
	20,19,18,17,16,
	15,14,13,12,11,
	10,9,8,7,6,
	5,4,3
};
#endif 




static uint16_t Get_Adc_Channel(uint32_t ch) ;

static uint16_t Get_Adc_Average(uint32_t ch,uint8_t times);

//static void Judge_PTC_Temperature_Value(uint16_t adc_ptc);

static void display_ntc_temp_value(uint8_t disp);

static void Calculate_Speicial_Temperature_Value(uint8_t temp);

uint16_t ptc_res_temp_voltage;

uint8_t disp_value;

/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
*****************************************************************/
static uint16_t Get_Adc_Channel(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;

	ADC1_ChanConf.Channel=ch;                                   //Í¨µÀ
    ADC1_ChanConf.Rank=  ADC_REGULAR_RANK_1;                                    //第一个序列
    ADC1_ChanConf.SamplingTime= ADC_SAMPLETIME_1CYCLE_5;//ADC_SAMPLETIME_239CYCLES_5;      //²ÉÑùÊ±¼ä               


	HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //Í¨µÀÅäÖÃ
	
    HAL_ADC_Start(&hadc1);                               //start ADC transmit
	
    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
 
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	        	//·µ»Ø×î½üÒ»´ÎADC1¹æÔò×éµÄ×ª»»½á¹û 
   
 
	
}
/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
*****************************************************************/
static uint16_t Get_Adc_Average(uint32_t ch,uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc_Channel(ch);
		//delay_ms(5);
	}
	return temp_val/times;
} 

/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
*****************************************************************/
void Get_Ntc_Resistance_Temperature_Voltage(uint32_t channel,uint8_t times)
{
    static uint8_t the_first_cunter,read_adc_value;
	uint16_t adcx,temp_vlue;
	
	adcx = Get_Adc_Average(channel,times);

    ptc_res_temp_voltage  =(uint16_t)((adcx * 3300)/4096); //amplification 100 ,3.11V -> 311

   // gctl_t.ptc_read_adc_value = ptc_res_temp_voltage;

	if(adcx ==0 && the_first_cunter < 1){

        the_first_cunter ++;

	   ptc_res_temp_voltage =1880;

	   // display_ntc_temp_value(ntc_t.temperature_value);

	}
    else{
   
    gctl_t.ntc_res_read_adc_value = ptc_res_temp_voltage ;

    temp_vlue= ptc_res_temp_voltage /100;
	
	length_simple =  sizeof(R10K_Init_20_120_simple)/sizeof(R10K_Init_20_120_simple[0]);
    
   	 disp_temp_degree = Binary_Search(R10K_Init_20_120_simple,temp_vlue,length_simple);
    // disp_conver_end=1;
	 

	Calculate_Speicial_Temperature_Value(disp_temp_degree);
    
   

     if(read_adc_value ==0){
        read_adc_value++;
        disp_value = ntc_t.temperature_value; //disp_value= disp_ntc_value[0];
        disp_ntc_value[0]= ntc_t.temperature_value;

     }
     else{

        disp_value = ntc_res_linear_value(ntc_t.temperature_value);

     }
	 
	 display_ntc_temp_value(disp_value);

    }

   //  Judge_NTC_Temperature_Value(ntc_t.temperature_value);

}


/*****************************************************************
	*
	*Function Name: void Judge_PTC_Temperature_Value(void)
	*Function: PTC adc read voltage
	*Input Ref: NO
	*Return Ref: No
	*
	*
*****************************************************************/
void Judge_NTC_Temperature_Value(uint16_t adc_ptc)
{
  
	
  if(adc_ptc < 373 || adc_ptc == 373){   //91 degree 

         gctl_t.ptc_warning =1;

		 gctl_t.ptc_flag=0 ;

        Ptc_Off();
		LED_PTC_ICON_OFF();
      

		Publish_Data_Warning(ptc_temp_warning,warning);
		osDelay(200);//HAL_Delay(200);  
        
		MqttData_Publish_SetPtc(0);
		osDelay(200);//HAL_Delay(200);  
		
		Buzzer_Ptc_Error_Sound();
		
			  
				
	   	}
}

/*************************************************************************
	*
	*Functin Name: static int8_t  Binary_Search(uint8_t *R10K_NTC_81 ,uint8_t key)
	*Funtion: binary search arithmetic
	*
	*
	*
*************************************************************************/
static int8_t  Binary_Search(const uint8_t *array ,uint8_t key,uint8_t length)
{
    left_point =0 ;
	right_point = length -1;
   while(left_point <= right_point){

        mid_value = (left_point + right_point)/2;

        if(key == array[mid_value]){

		    return   mid_value;

		}
        else if(array[mid_value] >   key ){ //move right  -> right small numbers

             left_point = mid_value +1;
			

		}
		else if(array[mid_value] < key){ //move left -> left big numbers

          right_point = mid_value -1;
		   

       }
	} 
   
	return -1;
} 

/******************************************************************************
	*
	*Function Name: static void Calculate_Speicial_Temperature_Value(uint8_t temp)
	*Function :calculate special value
	*Input Ref: speical transmit display temperature value 
	*Return Ref:NO
	*
******************************************************************************/
static void Calculate_Speicial_Temperature_Value(uint8_t temp)
{
  // static uint8_t array_subscript;

   switch(temp){

    case degree_zero: //zero degree 19 `` 20 degree

         // zero_d =2;
        array_subscript =  Calculate_Display_Temperature_Value(R10K_19_20,0,gctl_t.ntc_res_read_adc_value,2);
      
       switch(array_subscript){

		 case 0:
		 	ntc_t.temperature_value = 19 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 1:
		 	ntc_t.temperature_value = 20 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;



        }

   break;


   case degree_one: //zero degree  21~23 degree

         // zero_d =2;
        array_subscript =  Calculate_Display_Temperature_Value(R10K_21_23,1,gctl_t.ntc_res_read_adc_value,3);
      
       switch(array_subscript){

		 case 0:
		 	ntc_t.temperature_value = 21 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 1:
		 	ntc_t.temperature_value = 22 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 2:
		 	ntc_t.temperature_value = 23+ ntc_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;


        }

   break;

   case degree_two: //24~26 degree
   	   array_subscript =  Calculate_Display_Temperature_Value(R10K_24_26,2,gctl_t.ntc_res_read_adc_value,3);
	   
	   switch(array_subscript){

		 case 0:
		 	ntc_t.temperature_value = 24 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;

		 break;

		 case 1:
		 	ntc_t.temperature_value = 25 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 2:
		 	ntc_t.temperature_value = 26 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		

        }

   break;

   
   case degree_three : //27~~30 degree

         array_subscript =  Calculate_Display_Temperature_Value(R10K_27_30,3,gctl_t.ntc_res_read_adc_value,4);
		 // HAL_Delay(5);
  		
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 27 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 28 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 2:
			  ntc_t.temperature_value = 29 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 3:
			  ntc_t.temperature_value = 30 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  
   
		  }

   break;
   
   case degree_four: //31~~33 degree

         array_subscript =  Calculate_Display_Temperature_Value(R10K_31_33,4,gctl_t.ntc_res_read_adc_value,3);
			// HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 31 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 32 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ntc_t.temperature_value = 33 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }


   break;

   case degree_five : //34 À~~37 degree
		 array_subscript =  Calculate_Display_Temperature_Value(R10K_34_37,5,gctl_t.ntc_res_read_adc_value,4);
		
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 34 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 35 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ntc_t.temperature_value = 36 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ntc_t.temperature_value = 37 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }
   break;
   
   case degree_six: //38 ```41 degree
   	     array_subscript =  Calculate_Display_Temperature_Value(R10K_38_41,6,gctl_t.ntc_res_read_adc_value,4);
		 // HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 38 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 39 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ntc_t.temperature_value = 40 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ntc_t.temperature_value = 41 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case degree_seven ://42 ````44 degree
   	     array_subscript =  Calculate_Display_Temperature_Value(R10K_42_44,7,gctl_t.ntc_res_read_adc_value,3);
		  

		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 42 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 43 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ntc_t.temperature_value = 44 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
   
		  }

   break;

  
   case degree_eight : //45````49 degree
   	     array_subscript =  Calculate_Display_Temperature_Value(R10K_45_49,8,gctl_t.ntc_res_read_adc_value,5);
		  //HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 45 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 46+ ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ntc_t.temperature_value = 47 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ntc_t.temperature_value = 48 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ntc_t.temperature_value = 49 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;

   
   case degree_nine :  //50````53 degree
   	    array_subscript =  Calculate_Display_Temperature_Value(R10K_50_53,9,gctl_t.ntc_res_read_adc_value,4);
		  //HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 50 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 51 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ntc_t.temperature_value = 52 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 3:
			  ntc_t.temperature_value = 53 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case degree_ten : //54````58 degree
   	 array_subscript =  Calculate_Display_Temperature_Value(R10K_54_58,10,gctl_t.ntc_res_read_adc_value,5);
	     // HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 54 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 55 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ntc_t.temperature_value = 56 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ntc_t.temperature_value = 57 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ntc_t.temperature_value = 58 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case  degree_eleven ://59````64 degree
   		 array_subscript =  Calculate_Display_Temperature_Value(R10K_59_64,11,gctl_t.ntc_res_read_adc_value,6);
		 // HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 59 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 60 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ntc_t.temperature_value = 61 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 3:
			  ntc_t.temperature_value = 62 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ntc_t.temperature_value = 63 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 5:
			  ntc_t.temperature_value = 64 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case degree_twelve : //65````71 degree
   	   	 array_subscript =  Calculate_Display_Temperature_Value(R10K_65_71,12,gctl_t.ntc_res_read_adc_value,7);
	
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 65 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 66 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ntc_t.temperature_value = 67 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 3:
			  ntc_t.temperature_value = 68 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ntc_t.temperature_value = 69 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 5:
			  ntc_t.temperature_value = 70 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 6:
			  ntc_t.temperature_value = 71 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   
   
		  }

   break;

   case degree_thirteen : //72````78 degree
   		 array_subscript =  Calculate_Display_Temperature_Value(R10K_72_78,13,gctl_t.ntc_res_read_adc_value,7);
		//  HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 72 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 73 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ntc_t.temperature_value = 74 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 3:
			  ntc_t.temperature_value = 75 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ntc_t.temperature_value = 76 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 5:
			  ntc_t.temperature_value = 77 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 6:
			  ntc_t.temperature_value = 78 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case degree_fourteen : //79~85 degree
       array_subscript =  Calculate_Display_Temperature_Value(R10K_79_88,14,gctl_t.ntc_res_read_adc_value,10);
	     //  HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 79 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 80 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ntc_t.temperature_value = 81 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ntc_t.temperature_value = 82 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ntc_t.temperature_value = 83 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 5:
			  ntc_t.temperature_value = 84 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 6:
			  ntc_t.temperature_value = 85 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 7:
			  ntc_t.temperature_value = 86 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 8:
			  ntc_t.temperature_value = 87 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 9:
			  ntc_t.temperature_value = 88 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }


   break;

   case degree_fiveteen : //89~100 degree
       array_subscript =  Calculate_Display_Temperature_Value(R10K_89_100,15,gctl_t.ntc_res_read_adc_value,12);
	     //  HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 89 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 90 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ntc_t.temperature_value = 91 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ntc_t.temperature_value = 92 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ntc_t.temperature_value = 93 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 5:
			  ntc_t.temperature_value = 94 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 6:
			  ntc_t.temperature_value = 95 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 7:
			  ntc_t.temperature_value = 96 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 8:
			  ntc_t.temperature_value = 97 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 9:
			  ntc_t.temperature_value = 98 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 10:
			  ntc_t.temperature_value = 99 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;

		   break;

		    case 11:
			  ntc_t.temperature_value = 100 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }


   break;

   case degree_sixteen : //101~118 degree
       array_subscript =  Calculate_Display_Temperature_Value(R10K_101_118,16,gctl_t.ntc_res_read_adc_value,18);
	     //  HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 101+ ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 102 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ntc_t.temperature_value = 103 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ntc_t.temperature_value = 104 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ntc_t.temperature_value = 105 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 5:
			  ntc_t.temperature_value = 106 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 6:
			  ntc_t.temperature_value = 107 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 7:
			  ntc_t.temperature_value = 108 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 8:
			  ntc_t.temperature_value = 109 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 9:
			  ntc_t.temperature_value = 110 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 10:
			  ntc_t.temperature_value = 111 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 11:
			  ntc_t.temperature_value = 112 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 12:
			  ntc_t.temperature_value = 113 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 13:
			  ntc_t.temperature_value = 114 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 14:
			  ntc_t.temperature_value = 115 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 15:
			  ntc_t.temperature_value = 116 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 16:
			  ntc_t.temperature_value = 117 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 17:
			  ntc_t.temperature_value = 118 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		 
   
		  }


   break;

    case degree_seventeen: //119````120 degree
   	   	 array_subscript =  Calculate_Display_Temperature_Value(R10K_119_120,17,gctl_t.ntc_res_read_adc_value,2);
	
		 switch(array_subscript){
   
		   case 0:
			  ntc_t.temperature_value = 119 + ntc_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ntc_t.temperature_value = 120 + ntc_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		 }

   }
}
/******************************************************************************
	*
	*Function Name: static uint8_t Calculate_Display_Temperature_Value(const uin16_t *pt,uint8_t length)
	*Function :calculate special value
	*Input Ref: speical transmit display temperature value 
	*Return Ref:NO
	*
******************************************************************************/
static uint8_t Calculate_Display_Temperature_Value(const uint16_t *pt,uint8_t key,uint16_t ntc_res_value,uint8_t length)
{
      uint8_t i;
	  

      for(i=0;i<length;i++){

      if(*(pt+i) ==  ntc_res_value ){

             return i;


      }
     else if(*(pt+i) >  ntc_res_value && (*(pt+i+1) <ntc_res_value)){//high temperature degree is number is smaller

                // temp_decimal_point = ntc_res_value - *(pt+i +1) ;

			   // ntc_t.temperature_decimal_point_value =  temp_decimal_point %10;  //10  ;
                 
                  
                   return i;
				 
          
          }

      }
      return i;

}






  #if 0
		    else if(i==(length-1)){

               if(*(pt+i) >  ntc_res_value){



               if(key <= 4){
				   if((*(pt+i) - ntc_res_value) >=30){
	                 ntc_t.temperature_rectify_value =1;

                     temp_decimal_point =*(pt+i+1) - *(pt+i+2);
                         
                     temp_decimal_point = temp_decimal_point     %10 ;
					 
	               }
                   else{

                          temp_decimal_point =*(pt+i) - *(pt+i+1)  ;
                                            
                          temp_decimal_point = temp_decimal_point     %10 ;
                        }
                   ntc_t.temperature_decimal_point_value =  temp_decimal_point;///10  ;
               }
			   else if(key > 4 && key <= 7){

				   if((*(pt+i) - ntc_res_value) >=25){
						ntc_t.temperature_rectify_value =1;
                         temp_decimal_point =*(pt+i+1) - *(pt+i+2);
                         
                         temp_decimal_point = temp_decimal_point     %10 ;
										
					}
                    else{

                          temp_decimal_point =*(pt+i) - *(pt+i+1)  ;
                                            
                          temp_decimal_point = temp_decimal_point     %10 ;
                        }
                   ntc_t.temperature_decimal_point_value =  temp_decimal_point;///10  ;
               }
			   else if(key ==8){

				   if((*(pt+i) - ntc_res_value) >=23){
						ntc_t.temperature_rectify_value =1;
                        temp_decimal_point =*(pt+i+1) - *(pt+i+2);
                         
                     temp_decimal_point = temp_decimal_point     %10 ;
										
					}
                    else{

                          temp_decimal_point =*(pt+i) - *(pt+i+1)  ;
                                            
                          temp_decimal_point = temp_decimal_point     %10 ;
                        }
                       ntc_t.temperature_decimal_point_value =  temp_decimal_point;///10  ;
                   
               }
			   else if(key ==9){

				   if((*(pt+i) - ntc_res_value) >=21){
						ntc_t.temperature_rectify_value =1;
                        temp_decimal_point =*(pt+i+1) - *(pt+i+2);
                         
                     temp_decimal_point = temp_decimal_point     %10 ;
										
					}
                    else{

                          temp_decimal_point =*(pt+i) - *(pt+i+1)  ;
                                            
                          temp_decimal_point = temp_decimal_point     %10 ;
                        }
                      ntc_t.temperature_decimal_point_value =  temp_decimal_point;///10  ;
               }
			   else if(key ==10){

				   if((*(pt+i) - ntc_res_value) >=19){
						ntc_t.temperature_rectify_value =1;
                        temp_decimal_point =*(pt+i+1) - *(pt+i+2);
                         
                     temp_decimal_point = temp_decimal_point     %10 ;
										
					}
                    else{

                          temp_decimal_point =*(pt+i) - *(pt+i+1)  ;
                                            
                          temp_decimal_point = temp_decimal_point     %10 ;
                        }
                   ntc_t.temperature_decimal_point_value =  temp_decimal_point;///10  ;
               }
			   else if(key ==11){

				   if((*(pt+i) - ntc_res_value) >=16){
						ntc_t.temperature_rectify_value =1;
                        temp_decimal_point =*(pt+i+1) - *(pt+i+2);
                         
                     temp_decimal_point = temp_decimal_point     %10 ;
										
					}
                    else{

                          temp_decimal_point =*(pt+i) - *(pt+i+1)  ;
                                            
                          temp_decimal_point = temp_decimal_point     %10 ;
                        }
                   ntc_t.temperature_decimal_point_value =  temp_decimal_point;///10  ;
               }
			   else if(key ==12 ){

				   if((*(pt+i) - ntc_res_value) >=14){
						ntc_t.temperature_rectify_value =1;

                      temp_decimal_point =*(pt+i+1) - *(pt+i+2);
                         
                     temp_decimal_point = temp_decimal_point     %10 ;
										
					}
                    else{

                          temp_decimal_point =*(pt+i) - *(pt+i+1)  ;
                                            
                          temp_decimal_point = temp_decimal_point     %10 ;
                        }
                   ntc_t.temperature_decimal_point_value =  temp_decimal_point;///10  ;
               }
			   else if(key ==13 ){

				   if((*(pt+i) - ntc_res_value) >=12){
						ntc_t.temperature_rectify_value =1;

                        temp_decimal_point =*(pt+i+1) - *(pt+i+2);
                         
                     temp_decimal_point = temp_decimal_point     %10 ;
										
					}
                    else{

                          temp_decimal_point =*(pt+i) - *(pt+i+1)  ;
                                            
                          temp_decimal_point = temp_decimal_point     %10 ;
                        }
                   ntc_t.temperature_decimal_point_value =  temp_decimal_point;///10  ;
               }
			   else if(key ==14 ){

				   if((*(pt+i) - ntc_res_value) >=9){
						ntc_t.temperature_rectify_value =1;

                        temp_decimal_point =*(pt+i+1) - *(pt+i+2);
                         
                     temp_decimal_point = temp_decimal_point     %10 ;
										
					}
                    else{

                          temp_decimal_point =*(pt+i) - *(pt+i+1)  ;
                                            
                          temp_decimal_point = temp_decimal_point     %10 ;
                      }
                     ntc_t.temperature_decimal_point_value =  temp_decimal_point;///10  ;
               }
			   else if(key ==15 ){

				   if((*(pt+i) - ntc_res_value) >=7){
						ntc_t.temperature_rectify_value =1;

                      temp_decimal_point =*(pt+i+1) - *(pt+i+2);
                         
                     temp_decimal_point = temp_decimal_point     %10 ;
										
					}
                    else{

                          temp_decimal_point =*(pt+i) - *(pt+i+1)  ;
                                            
                          temp_decimal_point = temp_decimal_point     %10 ;
                        }
                   ntc_t.temperature_decimal_point_value =  temp_decimal_point;///10  ;
               }
			    else if(key ==16 ||  key==17){

				   if((*(pt+i) - ntc_res_value) >=4){
						ntc_t.temperature_rectify_value =1;
                        temp_decimal_point =*(pt+i+1) - *(pt+i+2);
                         
                     temp_decimal_point = temp_decimal_point     %10 ;
										
					}
                    else{

                          temp_decimal_point =*(pt+i) - *(pt+i+1)  ;
                                            
                          temp_decimal_point = temp_decimal_point     %10 ;
                        }
                   ntc_t.temperature_decimal_point_value =  temp_decimal_point;///10  ;
               }
			   else
		      	ntc_t.temperature_rectify_value =0;

			    temp_decimal_point = *(pt+i)-key; //小数点
                temp_decimal_point = temp_decimal_point ;
                ntc_t.temperature_decimal_point_value =  temp_decimal_point;//10 ;

               
			   	

               }
			   temp_temperature_value  = i;

			   return temp_temperature_value ;

		 }
		#endif 
	
/***********************************************************************************************
	*
	*Function Name:void TFT_Display_WorksTime(void)
	*Function : display of works time value 
	*Input: NO
	*Return: NO 
	*
*************************************************************************************************/
static void display_ntc_temp_value(uint8_t disp)
{


static uint8_t timer_hundred_hours,timer_decade_hours,timer_unit_hours;

 timer_hundred_hours =  disp/100;
 timer_decade_hours = (disp% 100)/10;
 timer_unit_hours = (disp% 100) % 10;

 //timer_decade_minutes= gctl_t.gSet_timer_minutes / 10;
 //timer_unit_minutes = gctl_t.gSet_timer_minutes % 10;

    

//      TFT_Disp_Pic_Warnign_Words(100,120,disp,0);//“高”
//	  TFT_Disp_Pic_Warnign_Words(136,120,disp,1);//“温”
//	  TFT_Disp_Pic_Warnign_Words(172,120,disp,2);//“警”
//	  TFT_Disp_Pic_Warnign_Words(208,120,disp,3);//“告”


	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(100,120,0,timer_hundred_hours);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(136,120,0,timer_decade_hours);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(172,120,0,timer_unit_hours);//“警”
}

  
/***********************************************************************************************
	*
	*Function Name:void TFT_Display_WorksTime(void)
	*Function : display of works time value 
	*Input: NO
	*Return: NO 
	*
*************************************************************************************************/
void donot_display_ntc_temp_value(void)
{

    TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(100,120,1,1);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(136,120,1,1);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(172,120,1,1);
}

/***************************************************************************************
****************************************************************************************/

uint8_t ntc_res_linear_value(uint8_t ntc_value)
{

 //  static uint8_t *p ;
   static uint8_t disp_init;

     if(disp_init == 0){
	  	  disp_init++ ;
         disp_ntc_value[0] =  ntc_value;

      }
	  else 
          disp_ntc_value[1] = ntc_value;


			  

    if(disp_ntc_value[1] -  disp_ntc_value[0]  >0){

        if(disp_ntc_value[1] -  disp_ntc_value[0] ==1){

        	// read_input_times =1;

        	display_ntc_temp_value(disp_ntc_value[1]);
        	HAL_Delay(5);
        	 return disp_ntc_value[1];

        }
        else if(disp_ntc_value[1] - disp_ntc_value[0] > 1){

           
        	  //*p = ntc_value + 1 ;
        	  disp_ntc_value[0]= disp_ntc_value[0] + 1 ;

        	 // read_input_times =1;

              display_ntc_temp_value(disp_ntc_value[0]);
              HAL_Delay(5);//osDelay(50);//HAL_Delay(400);

        	  return  disp_ntc_value[0]  ;


        }
    }
    else{

        if(disp_ntc_value[0] - disp_ntc_value[1] ==1){

          // read_input_times =1;

             display_ntc_temp_value(disp_ntc_value[1]);
              HAL_Delay(5);//osDelay(50);//HAL_Delay(400);

           return disp_ntc_value[1];

        }
        else if(disp_ntc_value[0]- disp_ntc_value[1] > 1){

        			 
        	// *p = ntc_value - 1 ;
        	 
        	 disp_ntc_value[0]= disp_ntc_value[0] - 1 ;

             display_ntc_temp_value(disp_ntc_value[0]);
             HAL_Delay(5);//osDelay(50);//HAL_Delay(400);

        //	read_input_times =1;

        	return disp_ntc_value[0] ;


        }

    }
   return disp_ntc_value[0] ;
    
}



