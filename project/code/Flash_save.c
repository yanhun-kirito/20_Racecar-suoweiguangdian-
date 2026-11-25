#include "Flash_save.h"



//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//  @brief      需要调参部分
//  @explain    1、增加需要存进flash的数据只需在其对应类型的数组里添加其地址
//              (若为顺序添加，则不会影响在其之前的数据值，若为插入添加，会影响之前数据，需先抄录之前数据）
//              如果不在这里声明要修改的变量，则flash里的数据修改后无法保存，重启或者复位后还是原来的数据
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
void* Data_uint8[] ={
&DataInfo.forwardLook0,
&DataInfo.forwardLook1,
&DataInfo.forwardLook2,
&DataInfo.forwardLook3,
&DataInfo.forwardLook4,
&CarInfo.break_dir,
};
void* Data_int8[] = {NULL};
void* Data_uint16[] = {     
        &CarInfo.base_speed,
        &CarInfo.circur_speed,
        &CarInfo.break_speed,
        &CarInfo.turn_speed,
};
void* Data_int16[] = {NULL};
void* Data_uint32[] = {NULL};
void* Data_int32[] = {NULL};
void* Data_float[] ={
        //PID
&CarInfo.vl_kp, 
&CarInfo.vr_kp,

&CarInfo.vl_ki,
&CarInfo.vr_ki,

&CarInfo.w_kp1,
&CarInfo.w_kp2,

&CarInfo.w_ki,

&CarInfo.a_kp,
&CarInfo.a_kd,


&CarInfo.cross_dis,
&CarInfo.circur_dis,
&CarInfo.break_dis,
&CarInfo.circur_Angle,
&CarInfo.left_Angle,
&CarInfo.right_Angle,
&CarInfo.break_Angle,

&CarInfo.target_angle,
};



//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取Flash数据函数
//  @explain    1、此函数用于备份Flash数据
//              2、刷Flash需将Read_All_Data()前的‘1’改为‘0’，再手动确认保存数据
//-------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////2.0///////////////////////////////////////////////////////////////////
void FlashData_Init(void)
{
//    DataInfo.forwardLook0 = 15;
//    DataInfo.forwardLook1 = 20;
//    DataInfo.forwardLook2 = 30;
//    DataInfo.forwardLook3 = 35;
//    DataInfo.forwardLook4 = 10;
    DataInfo.forwardLook0 = 10;
    DataInfo.forwardLook1 = 15;
    DataInfo.forwardLook2 = 20;
    DataInfo.forwardLook3 = 40;
    DataInfo.forwardLook4 = 10;
//      DataInfo.forwardLook0 = 8;
//      DataInfo.forwardLook1 = 15;
//      DataInfo.forwardLook2 = 25;
//      DataInfo.forwardLook3 = 43;
//      DataInfo.forwardLook4 = 10;
      CarInfo.break_dir = 1;
    
    CarInfo.vl_kp = 14;//56;//25;
    CarInfo.vr_kp = 14;//56;//25;
    CarInfo.vl_ki = 1;//1.6;
    CarInfo.vr_ki = 1;//1.6;
    CarInfo.v_kd = 0;
    CarInfo.vl_kf = 0.25;
    CarInfo.vr_kf = 0.25;
    CarInfo.w_kp1 = 2;//0.85;//1.1;//3.0; 
    CarInfo.w_kp2 = 0.09;//0.024;//0.05;    0.02      0.01(20)
    CarInfo.w_ki =  0.7;//0.23;//0.49;//-0.40    0.2        0.2
    CarInfo.w_kd = 0.25;//0.19;
    CarInfo.a_kp = 1.98;//0.3;  //1.98
    CarInfo.a_kp2 = 0;//0.006;//0.011
    CarInfo.a_kd = 0.03;//0.024;
    CarInfo.a_kd2 =0.20;//0.23; 
    //速度页
    CarInfo.base_speed= 80;// - CarInfo.dirOffset * 0.5;// 80;
//    CarInfo.circur_speed = 30;//80;
//    CarInfo.break_speed = 35;
    CarInfo.turn_speed = CarInfo.base_speed * 0.5;//100;
    //定距和定角度页
    CarInfo.cross_dis = 3;
    CarInfo.circur_dis = 1.5;
    CarInfo.break_dis = 0.58;
    CarInfo.circur_Angle = 290;
    CarInfo.left_Angle = 30;
    CarInfo.right_Angle = -30;
    CarInfo.break_Angle = 60;
    CarInfo.target_angle = 0;
//刷Flash需先抄录flash，再将if判断前的‘1’改为‘0’，再手动确认保存数据，然后再改回1烧一次
//////////////////////////////////////////////////////2.0//////////////////////////////////////
#if 0
    Read_All_Data();
#endif
}


/*-------------------------------------------------------------------------------------------------------------------
  函数简介      写入单个扇区的数据到 Flash
  参数说明      page_num —— 页号（0～Max_Sector-1）
  说明          1. 首先在该页首写入校验标志（0xAABBCCC0 + 页号）
                2. 根据页号选择对应数据类型数组，将每项数据保存至缓冲区对应位置（索引从 1 开始）
                3. 最后将整个缓冲区写入 Flash
-------------------------------------------------------------------------------------------------------------------*/
static void Write_Data(uint8 page_num)
{
    uint32 dat = 0xAABBCCC0 + page_num;         // 校验标志
//    uint32 temp = 0;                            //临时变量，用于存储保存数据值
    uint8 j = 0;
    
    /* 清空缓冲区（设为 0xFF） */
    if(flash_check(FLASH_SECTION_INDEX, page_num))          //判断该页是否有数据
        flash_erase_page(FLASH_SECTION_INDEX, page_num);    //擦除这一页
      
    flash_buffer_clear();                                    //清空缓冲区
    flash_union_buffer[0].uint32_type = dat;
    
    /* 根据页号写入对应数据 */
    switch(page_num)
    {
        case 0:
            for(j = 0; j < sizeof(Data_uint8)/sizeof(void*); j++)
            {
                if(Data_uint8[j] != NULL)
                {
                    // 直接使用 uint8 成员写入数据
                    flash_union_buffer[j+1].uint8_type = *((uint8*)Data_uint8[j]);
                }
                else
                    break;
            }
            break;
        case 1:
            for(j = 0; j < sizeof(Data_int8)/sizeof(void*); j++)
            {
                if(Data_int8[j] != NULL)
                {
                    flash_union_buffer[j+1].uint32_type = *((int8*)Data_int8[j]);
                }
                else
                    break;
            }
            break;
        case 2:
            for(j = 0; j < sizeof(Data_uint16)/sizeof(void*); j++)
            {
                if(Data_uint16[j] != NULL)
                {
                    flash_union_buffer[j+1].uint32_type = *((uint16*)Data_uint16[j]);
                }
                else
                    break;
            }
            break;
        case 3:
            for(j = 0; j < sizeof(Data_int16)/sizeof(void*); j++)
            {
                if(Data_int16[j] != NULL)
                {
                    flash_union_buffer[j+1].uint32_type = *((int16*)Data_int16[j]);
                }
                else
                    break;
            }
            break;
        case 4:
            for(j = 0; j < sizeof(Data_uint32)/sizeof(void*); j++)
            {
                if(Data_uint32[j] != NULL)
                {
                    flash_union_buffer[j+1].uint32_type = *((uint32*)Data_uint32[j]);
                }
                else
                    break;
            }
            break;
        case 5:
            for(j = 0; j < sizeof(Data_int32)/sizeof(void*); j++)
            {
                if(Data_int32[j] != NULL)
                {
                    flash_union_buffer[j+1].uint32_type = *((int32*)Data_int32[j]);
                }
                else
                    break;
            }
            break;
        case 6:
            for(j = 0; j < sizeof(Data_float)/sizeof(void*); j++)
            {
                if(Data_float[j] != NULL)
                {
                    /* float 数据以 uint32 形式存储 */
                    flash_union_buffer[j+1].uint32_type = *((uint32*)Data_float[j]);
                }
                else
                    break;
            }
            break;
        default:
            break;
    }
    /* 将缓冲区中整个页写入 Flash */
    flash_write_page_from_buffer(0, page_num, FLASH_PAGE_LENGTH);
    flash_buffer_clear();                                    // 清空缓冲区
}





static void Read_Data(uint8 page_num)
{
    uint8 i;
    uint8 sum = 0;    // 计算数据项数量
    flash_buffer_clear();  // 清空缓冲区

    switch(page_num)
    {
        case 0:
            sum = sizeof(Data_uint8)/sizeof(void*);  
            // 读取时增加1，确保校验标志和数据都被读入
            flash_read_page_to_buffer(0, page_num, sum + 1);
            
            if(flash_union_buffer[0].uint32_type == (0xAABBCCC0 + page_num))
            {
                for(i = 0; i < sum; i++)
                {   
                    if(Data_uint8[i] != NULL)
                        *((uint8*)Data_uint8[i]) = flash_union_buffer[i + 1].uint8_type;
                    else
                        break;
                }
                flash_buffer_clear();
            }
            else
                ips114_show_string(0, 5, "flash readERR");
            break;
            
        case 1:
            sum = sizeof(Data_int8)/sizeof(void*);
            flash_read_page_to_buffer(0, page_num, sum + 1);
            
            if(flash_union_buffer[0].uint32_type == (0xAABBCCC0 + page_num))
            {
                for(i = 0; i < sum; i++)
                {   
                    if(Data_int8[i] != NULL)
                        *((int8*)Data_int8[i]) = flash_union_buffer[i + 1].int8_type;
                    else
                        break;
                }
                flash_buffer_clear();
            }
            else
                ips114_show_string(0, 5, "flash readERR");
            break;
            
        case 2:
            sum = sizeof(Data_uint16)/sizeof(void*);
            flash_read_page_to_buffer(0, page_num, sum + 1);
            
            if(flash_union_buffer[0].uint32_type == (0xAABBCCC0 + page_num))
            {
                for(i = 0; i < sum; i++)
                {   
                    if(Data_uint16[i] != NULL)
                        *((uint16*)Data_uint16[i]) = flash_union_buffer[i + 1].uint16_type;
                    else
                        break;
                }
                flash_buffer_clear();
            }
            else
                ips114_show_string(0, 5, "flash readERR");
            break;
            
        case 3:
            sum = sizeof(Data_int16)/sizeof(void*);
            flash_read_page_to_buffer(0, page_num, sum + 1);
            
            if(flash_union_buffer[0].uint32_type == (0xAABBCCC0 + page_num))
            {
                for(i = 0; i < sum; i++)
                {   
                    if(Data_int16[i] != NULL)
                        *((int16*)Data_int16[i]) = flash_union_buffer[i + 1].int16_type;
                    else
                        break;
                }
                flash_buffer_clear();
            }
            else
                ips114_show_string(0, 5, "flash readERR");
            break;
            
        case 4:
            sum = sizeof(Data_uint32)/sizeof(void*);
            flash_read_page_to_buffer(0, page_num, sum + 1);
            
            if(flash_union_buffer[0].uint32_type == (0xAABBCCC0 + page_num))
            {
                for(i = 0; i < sum; i++)
                {   
                    if(Data_uint32[i] != NULL)
                        *((uint32*)Data_uint32[i]) = flash_union_buffer[i + 1].uint32_type;
                    else
                        break;
                }
                flash_buffer_clear();
            }
            else
                ips114_show_string(0, 5, "flash readERR");
            break;
            
        case 5:
            sum = sizeof(Data_int32)/sizeof(void*);
            flash_read_page_to_buffer(0, page_num, sum + 1);
            
            if(flash_union_buffer[0].uint32_type == (0xAABBCCC0 + page_num))
            {
                for(i = 0; i < sum; i++)
                {   
                    if(Data_int32[i] != NULL)
                        *((int32*)Data_int32[i]) = flash_union_buffer[i + 1].int32_type;
                    else
                        break;
                }
                flash_buffer_clear();
            }
            else
                ips114_show_string(0, 5, "flash readERR");
            break;
            
        case 6:
            sum = sizeof(Data_float)/sizeof(void*);
            flash_read_page_to_buffer(0, page_num, sum + 1);
            
            if(flash_union_buffer[0].uint32_type == (0xAABBCCC0 + page_num))
            {
                for(i = 0; i < sum; i++)
                {   
                    if(Data_float[i] != NULL)
                        *((float*)Data_float[i]) = flash_union_buffer[i + 1].float_type;
                    else
                        break;
                }
                flash_buffer_clear();
            }
            else
                ips114_show_string(0, 5, "flash readERR");
            break;
            
        default:
            break;
    }
}

//------------------------------------------------------------------------------
// Save_All_Data: 保存所有数据（遍历每个数据页）
void Save_All_Data(void)
{
    uint8 i;
    for(i = 0; i < Max_Sector; i++)
    {
        Write_Data(i);
    }
}

//------------------------------------------------------------------------------
// Read_All_Data: 从flash中读取所有数据
void Read_All_Data(void)
{
    uint8 i;
    for(i = 0; i < Max_Sector; i++)
    {
        Read_Data(i);
    }
}