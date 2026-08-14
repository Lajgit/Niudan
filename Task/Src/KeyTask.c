#include "KeyTask.h"
#include "CommTask.h"
#include "port_key.h"

/* 中文注释：新扭蛋机原理图不再有旧弹界19路球洞检测，仅保留PB6/PB7/PD0三路面板输入。 */
static GPIO_TypeDef *Keyboard_Port[] = {KeyBoard1_GPIO_Port, KeyBoard2_GPIO_Port, KeyBoard3_GPIO_Port};
static uint32_t Keyboard_Pin[] = {KeyBoard1_Pin, KeyBoard2_Pin, KeyBoard3_Pin};

Key_HandleTypeDef keyboard[3];
Key_HandleTypeDef *keyboard_list[3];

extern Tx_HandleTypeDef Tx1;

static void Keyboard_ShortCallback(uint16_t key_id)
{
    Comm_SendMesg_FillData(&Tx1, Board_to_Android, t_SettingButton, key_id + 1, 0x01);
}

static void Keyboard_LongCallback(uint16_t key_id)
{
    Comm_SendMesg_FillData(&Tx1, Board_to_Android, t_SettingButton, key_id + 1, 0x02);
}

static void Keyboard_ReleaseCallback(uint16_t key_id)
{
    Comm_SendMesg_FillData(&Tx1, Board_to_Android, t_SettingButton, key_id + 1, 0x03);
}

void Keyboard_Init(void)
{
    Key_InitTypeDef init;
    for (uint16_t i = 0; i < 3; i++)
    {
        init.short_callback = Keyboard_ShortCallback;
        init.long_callback = Keyboard_LongCallback;
        init.release_callback = Keyboard_ReleaseCallback;
        init.debounce_time = KEY_DEBOUNCE_TIME;
        init.longpress_time = KEY_LONG_PRESS_TIME;
        init.trigger_frequnecy = 1;
        init.trigger_level = GPIO_PIN_RESET;

        init.key_id = i;
        init.port = Keyboard_Port[i];
        init.pin = Keyboard_Pin[i];
        Key_Init(&keyboard[i], init);
        keyboard_list[i] = &keyboard[i];
    }
}

void KeyAll_Init(void)
{
    Keyboard_Init();
}

void Key_Task(void)
{
    Key_Scan(keyboard_list, 3);
}
