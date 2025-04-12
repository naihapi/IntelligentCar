#include "Data.h"

DataBase_t *Head_DataBase;
SwitchModule_t Data_Buzzer;
SwitchModule_t Data_TipsLED;

/**
 * @brief 配置开关组件
 *
 * @param module 组件
 * @param state 状态
 *
 * @retval 无
 *
 * @note 无
 */
void Data_ConfigModule_Switch(SwitchModule_t *module, uint8_t state)
{
    module->state = state;
}

/**
 * @brief 配置视图组件
 *
 * @param module 组件
 * @param value 数值
 *
 * @retval 无
 *
 * @note 无
 */
void Data_ConfigModule_View(ViewModule_t *module, int value)
{
    module->value = value;
}

/**
 * @brief 配置滑块组件
 *
 * @param module 组件
 * @param now 当前值
 * @param max 最大值
 * @param mini 最小值
 *
 * @retval 无
 *
 * @note 无
 */
void Data_ConfigModule_Slider(SliderModule_t *module, int now, int max, int mini)
{
    module->nowvalue = now;
    module->maxvalue = max;
    module->minivalue = mini;
}

/**
 * @brief 获取数据库链表的末尾项
 *
 * @param head 数据库头
 *
 * @retval 无
 *
 * @note 无
 */
DataBase_t *Data_DataBase_GetEndItem(DataBase_t *head)
{
    if (head == NULL)
        return NULL;

    DataBase_t *current = head;

    while (current->next != NULL)
        current = current->next;

    return current;
}

/**
 * @brief 创建头节点
 *
 * @param 无
 *
 * @retval 返回头节点
 *
 * @note 无
 */
DataBase_t *Data_DataBase_CreateHead(void)
{
    // 为<head>分配内存
    DataBase_t *head = (DataBase_t *)malloc(sizeof(DataBase_t));
    if (head == NULL)
        return NULL;

    head->module = NULL;
    head->last = NULL;
    head->next = NULL;

    return head;
}

/**
 * @brief 创建一个节点
 *
 * @param name 名称(16字节以内)
 * @param module 指向具体的数据组件
 * @param type 类型(xxxType)
 *
 * @retval 返回打包好的节点
 *
 * @note 无
 */
DataBase_t *Data_DataBase_CreateItem(uint8_t *name, void *module, uint8_t type)
{
    // 为<data>分配内存
    DataBase_t *data = (DataBase_t *)malloc(sizeof(DataBase_t));
    if (data == NULL)
        return NULL;

    strcpy((char *)data->name, (char *)name);
    data->module = module;
    data->type = type;
    data->last = NULL;
    data->next = NULL;

    return data;
}

/**
 * @brief 获取指定的组件
 *
 * @param head 头节点
 * @param name 查找的组件名称
 *
 * @retval 返回组件(xxxModule_t)
 *
 * @note 无
 */
void *Data_DataBase_GetItem(DataBase_t *head, uint8_t *name)
{
    if (head == NULL || name == NULL)
        return NULL;

    DataBase_t *current = head;

    while (strstr((char *)current->name, (char *)name) == NULL)
    {
        current = current->next;
    }

    return current->module;
}

/**
 * @brief 包含一个节点
 *
 * @param head 头节点
 * @param item 需要包含的节点
 *
 * @retval 返回包含好的节点
 *
 * @note 无
 */
void Data_DataBase_AddItem(DataBase_t *head, DataBase_t *item)
{
    if (head == NULL || item == NULL)
        return;

    DataBase_t *current = Data_DataBase_GetEndItem(head);
    current->next = item;
    item->last = current;
}

/**
 * @brief 组件链表关联初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Data_DataBase_Init(void)
{
    DataBase_t *current;                        // 当前指向的项
    Head_DataBase = Data_DataBase_CreateHead(); // 创建头节点

    // 蜂鸣器
    current = Data_DataBase_CreateItem("Buzzer", &Data_Buzzer, SwitchType);
    Data_DataBase_AddItem(Head_DataBase, current);

    // 提示灯
    current = Data_DataBase_CreateItem("TipsLED", &Data_TipsLED, SwitchType);
    Data_DataBase_AddItem(Head_DataBase, current);
}

/**
 * @brief 组件数值初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Data_ModuleInit(void)
{
    Data_ConfigModule_Switch(&Data_Buzzer, 0);
    Data_ConfigModule_Switch(&Data_TipsLED, 0);
}

/**
 * @brief 数据初始化汇总
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Data_InitPro(void)
{
    Data_DataBase_Init();
    Data_ModuleInit();
}
