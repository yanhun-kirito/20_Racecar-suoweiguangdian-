#include "Display.h"
DataInfomationType DataInfo;
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//  @brief      需要调参部分
//  @explain    1、增减页面需要修改PAGE_MAX宏定义
//              2、修改某一页的参数显示行数、列数只需在PageXPara_ROW、PageXPara_COL后修改
//              3、增减页面需要增减PageXPara_ROW、PageXPara_COL宏定义，同时需要将其增减至MaxSelect数组中
//              4、在调参参数结构体中增减相关内容，添加页面需要将以下内容复制到结构体中
//                {{"Para2_0", "Para2_1", "Para2_2", "Para2_3", "Para2_4", "Para2_5", "Para2_6", "Para2_7"},  //参数名
//                 {NULL, NULL, NULL, NULL, NULL, NULL, NULL},                                                //第1列参数地址
//                 {NULL, NULL, NULL, NULL, NULL, NULL, NULL}}                                                //第2列参数地址
//              （注意：参数名数组需要初始化八个，无内容则需初始化为"0"，void指针需要初始化七个，无内容则需初始化为NULL）
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//显示页数

#define PAGE_MAX  10
//每页参数行数（最大行数为7）
#define Page0Para_ROW  3
#define Page1Para_ROW  2
#define Page2Para_ROW  1
#define Page3Para_ROW  6
#define Page4Para_ROW  6
#define Page5Para_ROW  6
#define Page6Para_ROW  1
#define Page7Para_ROW  6
#define Page8Para_ROW  6
#define Page9Para_ROW  6

//每页参数列数（最大列数为2）
#define Page0Para_COL  1
#define Page1Para_COL  1
#define Page2Para_COL  1
#define Page3Para_COL  2
#define Page4Para_COL  1
#define Page5Para_COL  2
#define Page6Para_COL  1
#define Page7Para_COL  2
#define Page8Para_COL  1
#define Page9Para_COL  2

_Bool Start_Flag = ZF_FALSE;

//每页参数可选的最大值
const uint8 MaxSelect[PAGE_MAX][2] =
{
    {Page0Para_ROW, Page0Para_COL},
    {Page1Para_ROW, Page1Para_COL},
    {Page2Para_ROW, Page2Para_COL},
    {Page3Para_ROW, Page3Para_COL},
    {Page4Para_ROW, Page4Para_COL},
    {Page5Para_ROW, Page5Para_COL},
    {Page6Para_ROW, Page6Para_COL},
    {Page7Para_ROW, Page7Para_COL},
    {Page8Para_ROW, Page8Para_COL},
    {Page9Para_ROW, Page9Para_COL},
    
};

//相关调参参数初始化
//注意：参数名数组需要初始化八个，无内容则需初始化为"0"，void指针需要初始化七个，无内容需则初始化为NULL
struct ParameterType Parameter[PAGE_MAX] =
{
    //第一页
    {{ "GO!", "Watch_video", "",  "Save_Data", "","", "", ""},   //参数名
     { NULL,NULL ,NULL, NULL,NULL,NULL, NULL},                                              //第1列参数地址
     {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                                                       //第2列参数地址
       //摄像头页
     {{"",     "",     "", "", "", "", "", ""},      //参数名
     {NULL, NULL, NULL, NULL, NULL, NULL, NULL},  //第1列参数地址
      {NULL,NULL,NULL, NULL, NULL, NULL, NULL}},     
      //标志位页
     {{"", "", "", "", "", "", "", ""},  //参数名
      {NULL, NULL, NULL, NULL, NULL, NULL, NULL},                                                //第1列参数地址
      {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                               //第2列参数地址
      //PID                           
     {{"L_P/R_P", "L_I/R_I","W_P1/P2","W_I","", "", "Save_Data","Save_Data"},   //参数名
      { &CarInfo.vl_kp, &CarInfo.vl_ki,&CarInfo.w_kp1 , &CarInfo.w_ki  ,NULL , NULL,NULL},                                              //第1列参数地址
      { &CarInfo.vr_kp, &CarInfo.vr_ki,&CarInfo.w_kp2 ,NULL,NULL ,NULL ,NULL}},
            //速度页
     {{"base_Speed", "circur_speed", "break_speed", "turn_speed", "", "", "Save_Data", "Save_Data"},  //参数名
      {&CarInfo.base_speed ,&CarInfo.circur_speed ,&CarInfo.break_speed , &CarInfo.turn_speed ,NULL,NULL, NULL},                                                //第1列参数地址
      {NULL,NULL, NULL , NULL, NULL, NULL, NULL}},                                               //第2列参数地址
            //定距页
     {{"circur_dis", "break_dis", "cross_dis", "", "", "", "Save_Data", ""},  //参数名
      {&CarInfo.circur_dis,&CarInfo.break_dis,&CarInfo.cross_dis , NULL,NULL ,NULL , NULL},                                                //第1列参数地址
      {NULL, NULL, NULL,NULL , NULL, NULL, NULL}},                                               //第2列参数地址
            //调参页
     {{"", "", "", "", "", "", "", ""},  //参数名
      {NULL, NULL, NULL, NULL, NULL, NULL, NULL},                                                //第1列参数地址
      {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                               //第2列参数地址
       //角度环调参
     {{"a_kp", "a_kd", "target_Angle", "", "", "", "Save_Data", "Save_Data"},  //参数名
      {&CarInfo.a_kp, &CarInfo.a_kd ,&CarInfo.target_angle ,NULL,NULL,NULL, NULL},                                                //第1列参数地址
      {NULL, NULL ,NULL , NULL,  NULL, NULL, NULL}},  
      //第2列参数地址
     {{"forward0", "forward1", "forward2", "forward3", "forward4", "break_dir", "Save_Data", "Save_Data"},  //参数名
      {&DataInfo.forwardLook0, &DataInfo.forwardLook1, &DataInfo.forwardLook2, &DataInfo.forwardLook3, &DataInfo.forwardLook4, &CarInfo.break_dir, NULL},                                                //第1列参数地址
      {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                               //第2列参数地址
      //圆环断路直角页
      {{"break/circur_Angle", "left/right_Angle", "", "", "", "", "", ""},  //参数名
      {&CarInfo.break_Angle, &CarInfo.left_Angle,NULL,NULL , NULL, NULL, NULL},                                                //第1列参数地址
      {&CarInfo.circur_Angle, &CarInfo.right_Angle, NULL , NULL, NULL, NULL, NULL}},                                               //第2列参数地址
 };



_Bool Para_Mode = ZF_FALSE;                                    //调参模式
int8 PrecisionSelect = 2;                                   //调参精度等级
float Precision[5] = {100.0f, 10.0f, 1.0f, 0.1f, 0.01f};    //各个调参精度等级加减一次的数值

int8 Page = 0;                                              //当前页
int8 Choose_List = 0, Channel = 0;                          //参数选择列、行
int8 Choose_Mode = 0;                                       //图像显示选择模式

_Bool Is_Increase = ZF_FALSE, Is_Decrease = ZF_FALSE;             //参数是否加减标志位

_Bool Start;                                                //发车按键是否按下
uint16 CountStartTime;                                       //发车延时时间
uint8 StopTime;                                             //反转停车时间

//-------------------------------------------------------------------------------------------------------------------
//  @brief      参数加减函数
//-------------------------------------------------------------------------------------------------------------------
static void Change_Para(uint8 type)
{
    //参数加标志位
    if(Is_Increase == ZF_TRUE)
    {
        //第0列参数
        if(Choose_List == 0)
        {
            switch(type)
            {
                case 1:     ParameterIncrease((*(int8*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);         break;
                case 2:     ParameterIncrease((*(uint8*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);        break;
                case 3:     ParameterIncrease((*(int16*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);        break;
                case 4:     ParameterIncrease((*(uint16*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);       break;
                case 5:     ParameterIncrease((*(int32*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);        break;
                case 6:     ParameterIncrease((*(float*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], float);      break;
                default:    break;
            }
        }
        //第1列参数
        else if(Choose_List == 1)
        {
            switch(type)
            {
                case 1:     ParameterIncrease((*(int8*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);         break;
                case 2:     ParameterIncrease((*(uint8*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);        break;
                case 3:     ParameterIncrease((*(int16*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);        break;
                case 4:     ParameterIncrease((*(uint16*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);       break;
                case 5:     ParameterIncrease((*(int32*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);        break;
                case 6:     ParameterIncrease((*(float*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], float);      break;
                default:    break;
            }
        }

        Is_Increase = ZF_FALSE;
    }
    //参数减标志位
    else if(Is_Decrease == ZF_TRUE)
    {
        //第0列参数
        if(Choose_List == 0)
        {
            switch(type)
            {
                case 1:     ParameterDecrease((*(int8*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);         break;
                case 2:     ParameterDecrease((*(uint8*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);        break;
                case 3:     ParameterDecrease((*(int16*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);        break;
                case 4:     ParameterDecrease((*(uint16*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);       break;
                case 5:     ParameterDecrease((*(int32*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);        break;
                case 6:     ParameterDecrease((*(float*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], float);      break;
                default:    break;
            }
        }
        //第1列参数
        else if(Choose_List == 1)
        {
            switch(type)
            {
                case 1:     ParameterDecrease((*(int8*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);         break;
                case 2:     ParameterDecrease((*(uint8*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);        break;
                case 3:     ParameterDecrease((*(int16*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);        break;
                case 4:     ParameterDecrease((*(uint16*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);       break;
                case 5:     ParameterDecrease((*(int32*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);        break;
                case 6:     ParameterDecrease((*(float*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], float);      break;
                default:    break;
            }
        }

        Is_Decrease = ZF_FALSE;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示8位有符号数函数
//-------------------------------------------------------------------------------------------------------------------
//static void Show_int8(void)
//{
//    uint8 r = 0;
//
//    for(r = 0; r < MaxSelect[Page][0]; r++)
//    {
//        //参数地址为空指针则不显示，显示第一列参数
//        if(Parameter[Page].ParaOne[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 0)
//                ips114_show_intB(126, 16*r, (*(int8*)(Parameter[Page].ParaOne[r])),2);
//            else
//                ips114_show_int(126, 16*r, (*(int8*)(Parameter[Page].ParaOne[r])),2);
//        }
//
//        if(Parameter[Page].ParaTwo[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 1)
//                ips114_show_intB(184, 16*r, (*(int8*)(Parameter[Page].ParaTwo[r])),2);
//            else
//                ips114_show_int(184, 16*r, (*(int8*)(Parameter[Page].ParaTwo[r])),2);
//        }
//    }
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示8位无符号数函数
//-------------------------------------------------------------------------------------------------------------------
//static void Show_uint8(void)
//{
//    uint8 r = 0;
//
//    for(r = 0; r < MaxSelect[Page][0]; r++)
//    {
//        //参数地址为空指针则不显示，显示第一列参数
//        if(Parameter[Page].ParaOne[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 0)
//                ips114_show_uintB(126, 16*r, (*(uint8*)(Parameter[Page].ParaOne[r])),2);
//            else
//                ips114_show_uint(126, 16*r, (*(uint8*)(Parameter[Page].ParaOne[r])),2);
//        }
//
//        if(Parameter[Page].ParaTwo[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 1)
//                ips114_show_uintB(184, 16*r, (*(uint8*)(Parameter[Page].ParaTwo[r])),2);
//            else
//                ips114_show_uint(184, 16*r, (*(uint8*)(Parameter[Page].ParaTwo[r])),2);
//        }
//    }
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示16位有符号数函数
//-------------------------------------------------------------------------------------------------------------------
//static void Show_int16(void)
//{
//    uint8 r = 0;
//
//    for(r = 0; r < MaxSelect[Page][0]; r++)
//    {
//        //参数地址为空指针则不显示，显示第一列参数
//        if(Parameter[Page].ParaOne[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 0)
//                ips114_show_intB(126, 16*r, (*(int16*)(Parameter[Page].ParaOne[r])),4);
//            else
//                ips114_show_int(126, 16*r, (*(int16*)(Parameter[Page].ParaOne[r])),4);
//        }
//
//        if(Parameter[Page].ParaTwo[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 1)
//                ips114_show_intB(184, 16*r, (*(int16*)(Parameter[Page].ParaTwo[r])),4);
//            else
//                ips114_show_int(184, 16*r, (*(int16*)(Parameter[Page].ParaTwo[r])),4);
//        }
//    }
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示16位无符号数函数
//-------------------------------------------------------------------------------------------------------------------
//static void Show_uint16(void)
//{
//    uint8 r = 0;
//
//    for(r = 0; r < MaxSelect[Page][0]; r++)
//    {
//        //参数地址为空指针则不显示，显示第一列参数
//        if(Parameter[Page].ParaOne[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 0)
//                ips114_show_uintB(126, 16*r, (*(uint16*)(Parameter[Page].ParaOne[r])),4);
//            else
//                ips114_show_uint(126, 16*r, (*(uint16*)(Parameter[Page].ParaOne[r])),4);
//        }
//
//        if(Parameter[Page].ParaTwo[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 1)
//                ips114_show_uintB(184, 16*r, (*(uint16*)(Parameter[Page].ParaTwo[r])),4);
//            else
//                ips114_show_uint(184, 16*r, (*(uint16*)(Parameter[Page].ParaTwo[r])),4);
//        }
//    }
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示32位有符号数函数
//-------------------------------------------------------------------------------------------------------------------
//static void Show_int32(void)
//{
//    uint8 r = 0;
//
//    for(r = 0; r < MaxSelect[Page][0]; r++)
//    {
//        //参数地址为空指针则不显示，显示第一列参数
//        if(Parameter[Page].ParaOne[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 0)
//                ips114_show_intB(126, 16*r, (*(int32*)(Parameter[Page].ParaOne[r])), 4);
//            else
//                ips114_show_int(126, 16*r, (*(int32*)(Parameter[Page].ParaOne[r])), 4);
//        }
//
//        if(Parameter[Page].ParaTwo[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 1)
//                ips114_show_intB(184, 16*r, (*(int32*)(Parameter[Page].ParaTwo[r])), 4);
//            else
//                ips114_show_int(184, 16*r, (*(int32*)(Parameter[Page].ParaTwo[r])), 4);
//        }
//    }
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示浮点数函数
//-------------------------------------------------------------------------------------------------------------------
//static void Show_float(void)
//{
//    uint8 r = 0;
//
//    for(r = 0; r < MaxSelect[Page][0]; r++)
//    {
//        //参数地址为空指针则不显示，显示第一列参数
//        if(Parameter[Page].ParaOne[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 0)
//                ips114_show_floatB(126, 16*r, (*(float*)(Parameter[Page].ParaOne[r])), 3, 2);
//            else
//                ips114_show_float(126, 16*r, (*(float*)(Parameter[Page].ParaOne[r])), 3, 2);
//        }
//
//        if(Parameter[Page].ParaTwo[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 1)
//                ips114_show_floatB(184, 16*r, (*(float*)(Parameter[Page].ParaTwo[r])), 3, 2);
//            else
//                ips114_show_float(184, 16*r, (*(float*)(Parameter[Page].ParaTwo[r])), 3, 2);
//        }
//    }
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      调参页显示函数
//  @param      dat[]       该页面的显示格式 ，同一页面只能以相同类型显示
//                          （可选参数：uint8、char、int8、uint16、int16、int32、float）
//  Sample usage:           Para_Page("int");
//-------------------------------------------------------------------------------------------------------------------
//static void Para_Page(const char dat[])
//{
//    uint8 type = 0;         //参数类型
//    uint8 r = 0;
//
//    //判断参数类型
//    if(!strcmp(dat, "int8") || !strcmp(dat, "char"))
//        type = 1;
//    else if(!strcmp(dat, "uint8"))
//        type = 2;
//    else if(!strcmp(dat, "int16"))
//        type = 3;
//    else if(!strcmp(dat, "uint16"))
//        type = 4;
//    else if(!strcmp(dat, "int32"))
//        type = 5;
//    else if(!strcmp(dat, "float"))
//        type = 6;
//
//    //参数加减
//    Change_Para(type);
//
//    //进入调参模式，显示调参精度等级
//    if(Para_Mode == true)  ips114_show_float(90, 0, Precision[PrecisionSelect], 3, 2);
//
//    //显示参数名
//    for(r = 0; r < MaxSelect[Page][0] + 1; r++)
//    {
//        //变量名非0才显示
//        if(strcmp("0", Parameter[Page].ParaName[r]))
//        {
//            if(r == Channel)
//                ips114_show_stringB(0, 16*r, Parameter[Page].ParaName[r]);
//            else
//                ips114_show_string(0, 16*r, Parameter[Page].ParaName[r]);
//        }
//    }

    //显示参数
//    switch(type)
//    {
//        case 1:     Show_int8();        break;
//        case 2:     Show_uint8();       break;
//        case 3:     Show_int16();       break;
//        case 4:     Show_uint16();      break;
//        case 5:     Show_int32();       break;
//        case 6:     Show_float();       break;
//        default:    break;
//    }

    //显示页码
//    ips114_show_uint(84, 112, Page + 1,1);
//    ips114_show_string(116, 112, "/");
//    ips114_show_uint(128, 112, PAGE_MAX,1);
//}
//static uint8 r = 0;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示第0页（在显示函数中调用）
//-------------------------------------------------------------------------------------------------------------------
//static void Page0(void)
//{
//    uint8 c;
//    ips114_show_string(0, 0, "YAW:");
//    ips114_show_float(30, 0, CarInfo.Yaw,2,1);
//    ips114_show_int(0,1,CarInfo.L_encoder,3);
//    ips114_show_int(60,1,CarInfo.R_encoder,3);
//分割线
//    for(c = 0; c < 240; c++)
//    {
//       ips114_draw_point(c, 64, RGB565_BLUE);
//    }
        //横滚角偏航角
//    ips114_show_string(0,  72, "YawRate:");
//    ips114_show_float(60, 72, CarInfo.YawRate, 3, 2);
//    ips114_show_string(110,  72, "Yaw:");
//    ips114_show_float(150, 72, CarInfo.Yaw, 3, 2);
//    ips114_show_int(180,0,Col_WhiteSum,2);
//    ips114_show_int(180,16,Width[22],2);
//    
//            //左边界
//        ips114_show_string(0,  96, "Left:");
//        ips114_show_uint(40, 96, (uint8)Left,3);
//
//        //前瞻行白点数
//        ips114_show_string(80,  96, "White:");
//        ips114_show_uint(128, 96, WhiteSum,3);
//
//        //右边界
//        ips114_show_string(160,  96, "Right:");
//        ips114_show_uint(208, 96, (uint8)Right, 3);
//
//        //左边界
//        ips114_show_string(0,  112, "LongL:");
//        ips114_show_uint(40, 112, (uint8)LongLeft,3);
//
////        ips114_show_string(188, 1, "off");
//        ips114_show_int(123, 112, (int8)CarInfo.dirOffset,3);
//        ips114_show_int(75, 112, WhiteRow[22],3);
//        ips114_showuint8E(83, 7, RightBlack[23], RGB565_PURPLE);
//        ips114_showuint8E(123, 7, RightBlack[24], RGB565_BLUE);

//        ips114_show_string(170,  112, "LongR:");
//        ips114_show_uint(208, 112, (uint8)LongRight, 3);
//    Change_Para(2);
//    if(Para_Mode == true)  ips114_show_float(150, 3, Precision[PrecisionSelect], 3, 3);
        
    //显示参数名
//    for(r = 0; r < MaxSelect[Page][0] + 1; r++)
//    {
//        //变量名非0才显示
//        if(strcmp("0", Parameter[Page].ParaName[r]))
//        {
//            if(r == Channel)
//                ips114_show_stringB(48, 16*r, Parameter[Page].ParaName[r]);
//            else
//                ips114_show_string(48, 16*r, Parameter[Page].ParaName[r]);
//        }
//
//    }
//
//    for(r = 0; r < MaxSelect[Page][0]; r++)//参数
//    {
//        //参数地址为空指针则不显示，显示第一列参数
//        if(Parameter[Page].ParaTwo[r] != NULL)
//        {
//            if(r == Channel && Choose_List == 1)
//                ips114_show_uintB(180, 16*r, (*(uint8*)(Parameter[Page].ParaTwo[r])),5);
//            else
//                ips114_show_uint(180, 16*r, (*(uint8*)(Parameter[Page].ParaTwo[r])),5);
//          }
//       }
//    Choose_Mode = Channel;
//  }

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示第1页（在显示函数中调用）
//-------------------------------------------------------------------------------------------------------------------
//static void Page1(void)
//{
//    uint8 r, c, i;
//    uint8 left, middle, right;
//
//    if(Choose_Mode == 1)
//    {
//        ips114_displayimage032_zoomB_reverse((uint8 *)mt9v03x_image,120, 80,240, 120);//显示灰度图像像
//
//
//        //参数加减
//        Change_Para(2);
//
//        //进入调参模式，显示调参精度等级
//        if(Para_Mode == true)  ips114_show_float(100, 5, Precision[PrecisionSelect], 3, 2);
//        
//        for(r = 0; r < MaxSelect[Page][0] + 1; r++)
//        {
//            //变量名非0才显示
//            if(strcmp("0", Parameter[Page].ParaName[r]) && r <= 1)
//            {
//                if(r == Channel)
//                    ips114_show_stringB(0, 16*(5 + r), Parameter[Page].ParaName[r]);
//                else
//                    ips114_show_string(0, 16*(5 + r), Parameter[Page].ParaName[r]);
//            }
//            else if(strcmp("0", Parameter[Page].ParaName[r]) && r == MaxSelect[Page][0])
//            {
//                if(r == Channel)
//                    ips114_show_stringB(0, 112, Parameter[Page].ParaName[r]);
//                else
//                    ips114_show_string(0, 112, Parameter[Page].ParaName[r]);
//            }
//        }
//        //显示参数
//        for(r = 0; r < MaxSelect[Page][0]; r++)
//        {
//            //参数地址为空指针则不显示，显示第一列参数
//            if(Parameter[Page].ParaOne[r] != NULL && r <= 1)
//            {
//                if(r == Channel && Choose_List == 0)
//                    ips114_show_uintB(208, 16*(5 + r), (*(uint8*)(Parameter[Page].ParaOne[r])),3);
//                else
//                    ips114_show_uint(208, 16*(5 + r), (*(uint8*)(Parameter[Page].ParaOne[r])),3);
//            }
//            if(Parameter[Page].ParaOne[r] != NULL && r == MaxSelect[Page][0])
//            {
//               if(r == Channel && Choose_List == 0)
//                   ips114_show_uintB(208, 112, (*(uint8*)(Parameter[Page].ParaOne[r])),3);
//               else
//                   ips114_show_uint(208, 112, (*(uint8*)(Parameter[Page].ParaOne[r])),3);
//            }
//        }
//    }
//    else
//    {
//        ips114_displayimageB((uint8 *)Bmp, 120, 80, 240, 120);//显示灰度图像
//        //显示前瞻行
//        for(c = 0; c < 240; c++)
//        {
//            ips114_draw_point(c, ((ROW - (DataInfo.forwardLook0)) << 1), RGB565_RED);
//            ips114_draw_point(c, ((ROW - (DataInfo.forwardLook1)) << 1), RGB565_RED);
//            ips114_draw_point(c, ((ROW - (DataInfo.forwardLook2)) << 1), RGB565_RED);
//            ips114_draw_point(c, ((ROW - (DataInfo.forwardLook3)) << 1), RGB565_RED);
//        }
//
//        //显示左边界、中线、右边界
//        for(r = 0, i = 0; r < ROW; r++)
//        {
//            c = ROW - 1 - r;
//
//            left = (COL - LeftBlack[c])  << 1;
//            middle = (COL - MiddleLine[c]) << 1;
//            right = (COL - RightBlack[c]) << 1;
//
//            //第一行边界显示
//            ips114_draw_point(left, (r + i), RGB565_YELLOW);
//            ips114_draw_point(left - 1, (r + i), RGB565_YELLOW);
//
//            ips114_draw_point(middle, (r + i), RGB565_RED);
//
//            ips114_draw_point(right, (r + i), RGB565_GREEN);
//            ips114_draw_point(right + 1, (r + i), RGB565_GREEN);
//
//            i++;
//
//            //第二行边界显示
//            ips114_draw_point(left, (r + i), RGB565_YELLOW);
//            ips114_draw_point(left - 1, (r + i), RGB565_YELLOW);
//
//            ips114_draw_point(middle, (r + i), RGB565_RED);
//
//            ips114_draw_point(right, (r + i), RGB565_GREEN);
//            ips114_draw_point(right + 1, (r + i), RGB565_GREEN);
//        }
//        for(i = 0; i < 80; i ++)
//            ips114_draw_point(119,i,RGB565_BLUE);
//    }
//}
//    
//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示第2页（在显示函数中调用）
//-------------------------------------------------------------------------------------------------------------------
//static void Page2()
//{
//    //停车标志位
//    ips114_show_string(0,  0, "stopcar:");
//    ips114_show_uint(80, 0, RoadFlag.stopcar,1);
//
//    //直角弯标志位
//    ips114_show_string(120,  0, "TURN_L/R:");
//    ips114_show_uint(182, 0, RoadFlag.turn_left,1);
//    ips114_show_uint(194, 0, RoadFlag.turn_right,1);
//
//    //圆环
//    ips114_show_string(0,  16, "left_circle:");
//    ips114_show_uint(95, 16, RoadFlag.left_circle, 1);
//    
//    ips114_show_string(120,  16, "right_circle:");
//    ips114_show_uint(224, 16, RoadFlag.right_circle, 1);
//    
//    //十字
//    ips114_show_string(0,  32, "CROSS:");
//    ips114_show_uint(68, 32, RoadFlag.cross,1);
//    ips114_show_string(120,  32, "crossDis:");
//    ips114_show_float(195, 32, RoadFlag.crossDis, 2, 2);
//    //断路
//    ips114_show_string(0,  48, "break_Road:");
//    ips114_show_uint(90, 48, RoadFlag.break_road,1);
//    ips114_show_string(0,  64, "b_Dis:");
//    ips114_show_float(54, 64, RoadFlag.breakDis, 2, 2);
//    ips114_show_string(120,  64, "b_Angle:");
//    ips114_show_float(184, 64, RoadFlag.breakAngle, 2, 2);
//    
//    //圆环打角和距离
//    ips114_show_string(0,  80, "C_Dis:");
//    ips114_show_float(54, 80, RoadFlag.circleDis , 2, 2);
//    ips114_show_string(0,  96, "L/R_Angle:");
//    ips114_show_float(90, 96, RoadFlag.left_circle_Angle, 3, 2);
//    ips114_show_float(180, 96, RoadFlag.right_circle_Angle, 3, 2);
//    //元素个数
//    ips114_show_string(0,  112, "nums_cr/ci/br/tu");
//    ips114_show_uint(140,112,cross_nums,1);
//    ips114_show_uint(155,112,circle_nums,1);
//    ips114_show_uint(170,112,break_nums,1);
//    ips114_show_uint(185,112,turn_nums,1);
//
//}
//
//static void Page7()
//{
//    
//    ips114_show_string(0,80,"dirOffset:");
//    ips114_show_int(90, 80,CarInfo.dirOffset ,3);
//    ips114_show_string(0,96,"LeftBlack:");
//    ips114_show_uint(90, 96,LeftBlack[20] ,3);
//
//    ips114_show_string(0,112,"RightBlack:");
//    ips114_show_uint(90, 112,RightBlack[20] ,3);
//
//    
//}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示函数（在主函数中调用）
//-------------------------------------------------------------------------------------------------------------------
//  @explain    若显示页面仅用于调参，可直接调用Para_Page("XXX")函数
//              若显示其他页面，则需自己编写函数，并在相应的case条件中调用
//-------------------------------------------------------------------------------------------------------------------
//void IPS114_display(void)
//{
//        switch(Page)
//        {
//            case 0:     Page0();                break;
//            case 1:     Page1();                break;
//            case 2:     Page2();                break;
//            case 3:     Para_Page("float");     break;
//            case 4:     Para_Page("uint16");     break;
//            case 5:     Para_Page("float");     break;
//            case 6:     Page7();                break;
//            case 7:     Para_Page("float");     break;
//            case 8:     Para_Page("uint8");     break;
//            case 9:     Para_Page("float");     break;
//            default:    break;
//        }
//    }

//void Key_Scan(void)
//{
//    if (!SET)
//    {
//        // 发车按键按下
//        if (Page == 0 && Channel == 0 && !Start_Flag)
//        {
//            while (!SET);
//            Start = true;
//            ips114_clear();
//            BuzzerMs(100);
//        }
//        // 发车后重回调参页
//        else if (Page == 0 && Start_Flag && Channel == 0)
//        {
//            Start_Flag = ZF_FALSE;
//            Start =ZF_FALSE;
//            Para_Mode = ZF_FALSE;
//            Page = 0;
//            while (!SET);
//            ips114_clear();
//            BuzzerMs(10);
//        }
//        // 存储Flash参数
//        else if (Para_Mode == ZF_FALSE && Channel == MaxSelect[Page][0])
//        {
//            while (!SET);
//            Save_All_Data();
//            Channel = 0;
//            BuzzerMs(200);
//        }
//        // 进入调参模式
//        else if (Para_Mode == ZF_FALSE && MaxSelect[Page][1])
//        {
//            while (!SET);
//            Para_Mode = ZF_TRUE;
//            ips114_clear();
//        }
//        // 退出调参模式
//        else if (Para_Mode == ZF_TRUE)
//        {
//            while (!SET);
//            Para_Mode = ZF_FALSE;
//            ips114_clear();
//        }
//    }
//
//    // 非调参模式下的方向键处理
//    if (Para_Mode == ZF_FALSE)
//    {
//        // 向右翻页
//        if (!RIGHT)
//        {
//            while (!RIGHT);
//            Choose_List++;
//
//            if (Choose_List >= MaxSelect[Page][1])
//            {
//                Page++;
//                Choose_List = 0;
//                ips114_clear();
//            }
//
//            if (Page >= PAGE_MAX)
//            {
//                Page = 0;
//                ips114_clear();
//            }
//        }
//        // 向左翻页
//        else if (!LEFT)
//        {
//            while (!LEFT);
//            Choose_List--;
//
//            if (Choose_List < 0)
//            {
//                Page--;
//                if (Page < 0)
//                {
//                    Page = PAGE_MAX - 1;
//                }
//                if (MaxSelect[Page][1])
//                    Choose_List = MaxSelect[Page][1] - 1;
//                else
//                    Choose_List = 0;
//                ips114_clear();
//            }
//        }
//        // 向下换行
//        else if (!DOWN)
//        {
//            while (!DOWN);
//            Channel++;
//            if (Channel > MaxSelect[Page][0])
//                Channel = 0;
//        }
//        // 向上换行
//        else if (!UP)
//        {
//            while (!UP);
//            Channel--;
//            if (Channel < 0)
//                Channel = MaxSelect[Page][0];
//        }
//    }
//    // 调参模式下的按键处理
//    else if (Para_Mode == ZF_TRUE)
//    {
//        // 调参精度选择
//        if (!RIGHT)
//        {
//            while (!RIGHT);
//            PrecisionSelect++;
//            if (PrecisionSelect >= 5)
//                PrecisionSelect = 0;
//        }
//        else if (!LEFT)
//        {
//            while (!LEFT);
//            PrecisionSelect--;
//            if (PrecisionSelect < 0)
//                PrecisionSelect = 4;
//        }
//        // 参数加减
//        else if (!UP)
//        {
//            while (!UP);
//            Is_Increase = ZF_TRUE;
//        }
//        else if (!DOWN)
//        {
//            while (!DOWN);
//            Is_Decrease = ZF_TRUE; 
//        }
//    }
//}
//
//void Key_Text(void)
//{
//
//    if(!UP)
//    {
//        while(!UP);
//        ips114_clear();
//        ips114_show_string(120,0,"UP");
//        BuzzerMs(100);
//    }
//    else if(!DOWN)
//    {
//        while(!DOWN);
//        ips114_clear();
//        ips114_show_string(120,1,"DOWN");
//        BuzzerMs(100);
//    }
//    else if(!LEFT)
//    {
//        while(!LEFT);
//        ips114_clear();
//        ips114_show_string(120,2,"LEFT");
//        BuzzerMs(100);
//    }
//    else if(!RIGHT)
//    {
//        while(!RIGHT);
//        ips114_clear();
//        ips114_show_string(120,3,"RIGHT");
//        BuzzerMs(100);
//    }
//    else if(!SET)
//    {
//        while(!SET);
//        ips114_clear();
//        ips114_show_string(120,4,"SET");
//        BuzzerMs(100);
//    }
//}
//
