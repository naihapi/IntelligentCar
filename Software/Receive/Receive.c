#include "Receive.h"

uint8_t Receive_Buffer[1024];

/**
 * @brief 获取指定位置的字符
 *
 * @param str 需要检查的字符串
 * @param check 检查字符'?'
 * @param number 跳转次数
 *
 * @retval 返回指定字符的地址
 *
 * @note 例如获取“POST#LEDFront#1”的第二个#符号的位置
 */
uint8_t *Receive_FindChar(uint8_t *str, char check, uint8_t number)
{
    uint8_t *p = NULL;
    uint8_t *current = str;

    for (uint8_t i = 0; i < number; i++)
    {
        p = NULL;
        p = (uint8_t *)strchr((char *)current, check);
        if (p == NULL)
        {
            return NULL;
        }

        current = p + 1;
    }

    return p;
}

/**
 * @brief 获取指定位置的数字
 *
 * @param str 需要检查的字符串
 * @param start 数字起始位置
 * @param end 数字结束位置
 *
 * @retval 返回数字
 *
 * @note 例如获取“POST#LEDFront#123”的'1'
 * @note start='#' end='\0'
 */
int32_t Receive_FindNumber(uint8_t *start, uint8_t *end)
{
    uint8_t NumStr[16] = {0};
    int32_t Number = 0;
    start += 1;                     // 跳转到数字起始位置
    size_t NumLength = end - start; // 长度需+1，否则会缺少一位

    strncpy((char *)NumStr, (char *)start, NumLength); // 拷贝字符串
    // NumStr[NumLength] = '\0';                          // 索引从0开始

    // Number = atoi((char *)NumStr); // 硬件错误!
    Number = strtol((char *)NumStr, NULL, 10);

    return Number;
}

/**
 * @brief 检查组件类型
 *
 * @param check 需要检查的字符串
 *
 * @retval 0 解析失败
 * @retval 1 开关型组件数据
 * @retval 2 视图型组件数据
 * @retval 3 滑块型组件数据
 * @retval 4 自定义型组件数据
 *
 * @note 无
 */
uint8_t Receive_Parse_CheckModuleType(uint8_t *check)
{
    uint8_t *Start = NULL, *End = NULL;
    uint8_t Number = 0;

    if (check == NULL)
    {
        return 0; // 解析失败
    }

    Start = Receive_FindChar(check, '#', 2);
    End = Receive_FindChar(check, '#', 3);
    Number = Receive_FindNumber(Start, End);

    return Number;
}

/**
 * @brief 解析数据
 *
 * @param data 需要解析的字符串
 * @param datapack 解析得到的数据包
 *
 * @retval 0 解析失败
 * @retval 1 开关型组件数据
 * @retval 2 视图型组件数据
 * @retval 3 滑块型组件数据
 * @retval 4 自定义型组件数据
 *
 * @note 无
 */
uint8_t Receive_Parse(uint8_t *data, uint8_t **datapack)
{
    uint8_t type = 0;

    type = Receive_Parse_CheckModuleType(data);
    *datapack = Receive_FindChar(data, '#', 3);

    return type;
}

void Receive_Function(void)
{
    uint8_t ItemType = 0;
    uint8_t *Item_DataPack;

    if (USART2_RecFlag == 1)
    {
        strcpy((char *)Receive_Buffer, (char *)USART2_RecBuffer);

        ItemType = Receive_Parse(Receive_Buffer, &Item_DataPack);
        CC2530_Clear_RecState();

        switch (ItemType)
        {
        case ItemType_Switch:
        {
        }
        break;

        case ItemType_Slider:
        {
        }
        break;

        case ItemType_View:
        {
        }
        break;

        case ItemType_Custom:
        {
            // 读取名称
            // 通过名称找到相应的菜单
            // 执行菜单中的钩子函数
            //...
            // CC2530_SendString(Item_DataPack);
            Manual_CustomFunction(Item_DataPack);
        }
        break;
        }

        USART2_Clear_RecBuffer();
    }
}

/**
 * @brief 接收初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Receive_InitPro(void)
{
}
