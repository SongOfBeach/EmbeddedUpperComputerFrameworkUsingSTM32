#ifndef _MENU_H_
#define _MENU_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/*---------------------------------------------------------------------------*/
#define handler_menu_callFunction(fp) ((*fp)())
/*-------------------------------- Struct -----------------------------------*/
typedef struct menustate
{	
	uint32_t top;		
	uint32_t MAX_ROWS;	/*ÿҳ��ʾ��ѡ�����*/
	uint32_t last_top;
	uint32_t currentItem;/*��ǰѡ�е�ѡ��*/
	uint32_t last_currentItem;
	struct menu* currentMenu;
}*menustate_pt,menustate_st;

typedef struct menuitem
{
	const char* name;	/*��ѡ����ʾ������*/
	void (*handlerFunc)();	/*��ѡ���Ӧ�Ĵ�����*/
	struct menu* child;		/*��ѡ���Ӧ�Ĳ˵�ҳ*/
}*menuitem_pt,menuitem_st;

typedef struct menu
{	
	uint8_t length;		/*��ǰ�˵�ҳ�µ�ѡ���Ա����*/
	struct menu* parent;
	struct menuitem** menuArray;	/*��ǰ�˵�ҳ�µ�ѡ���Ա*/
}*menu_pt,menu_st;
/*---------------------------------------------------------------------------*/
typedef struct IPLine
{	
	//��ͼ
	bool flag_page;	/*�Ƿ�����ڵ�ǰҳ������*/
	uint8_t font;
	uint8_t row;
	uint8_t col;
	uint8_t row_step;
	uint8_t col_step;

	//����ֵ
	uint8_t	index;	/*ָ��λ��*/
	uint8_t mode;	/* ip/mask ģʽ*/
	uint8_t ip[12];	/*��ʮ����������*/
	uint8_t mask[12];
}*IPLine_pt,IPLine_st;
/*---------------------------------------------------------------------------*/

/*---------------------------- Function -------------------------------------*/
void draw_string_LineMiddle(uint8_t colIndex, char *str, uint8_t font, uint8_t mode);
void draw_string_RankMiddle(uint8_t rowIndex, char *str, uint8_t font, uint8_t mode);
void handler_menu_goUp(struct menustate* ms);
void handler_menu_cancel(struct menustate* ms);
void handler_menu_select(struct menustate* ms);
void handler_menu_goDown(struct menustate* ms);
void handler_menu_goRight(struct menustate* ms);
void handler_menu_goLeft(struct menustate* ms);
bool display_menu_changed_by_key(uint8_t key,struct menustate* ms);
void draw_menu_item(uint8_t rowIndex, uint8_t colIndex, char *str, uint8_t mode);
void draw_menu_Page(struct menustate* ms);
/*---------------------------------------------------------------------------*/
bool display_page_ip_changed_by_key(uint8_t key, IPLine_pt ipLine);
void draw_Ip_Set_Page(void);
/*---------------------------------------------------------------------------*/
void menu1scallback(void);
#endif

