/*----------------------------------------------------------------------------
/ ‚ÄúTHE COFFEEWARE LICENSE‚Äù (Revision 1):
/ <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
/ can do whatever you want with this stuff. If we meet some day, and you think
/ this stuff is worth it, you can buy me a coffee in return.
/----------------------------------------------------------------------------*/
#include "menu.h"
#include "data_help.h"
#include "bsp_key.h"
#include "bsp_oled.h"
#include "drv_sh1106.h"
#include "string.h"

/*--------------------------------- Declare ---------------------------------*/
//ª˘¥°π‹¿Ì
struct menu menuBasicManager;
struct menu menuYcPort;
struct menu menuYcPortPara;
//IPπ‹¿Ì
struct menu subMenu2;
//÷’∂Àπ‹¿Ì
struct menu menuTermInfo;
struct menu menuSerialPara;
struct menu menuThisIPAddr;
//±æª˙–≈œ¢
struct menu menuDeviceInfo;
/*----------------------------- SubMenu Deepth0 -----------------------------*/
struct menuitem node0 = {"test",draw_Ip_Set_Page,NULL};
struct menuitem s0BasicManager = {"1.ª˘¥°π‹¿Ì    ",NULL,&menuBasicManager};
struct menuitem s0IPManager = {"2.IPπ‹¿Ì      ",NULL,&subMenu2};
struct menuitem s0TermInfo = {"3.÷’∂ÀΩ”ø⁄    ",NULL,&menuTermInfo};
struct menuitem s0DeviceInfo = {"4.±æª˙–≈œ¢    ",NULL,&menuDeviceInfo};
struct menuitem s0DhcpSwitch = {"5.DHCPø™πÿ…Ë÷√",menu1scallback,NULL};
struct menuitem* mainElements[] = {&s0BasicManager,&s0IPManager,&s0TermInfo,&s0DeviceInfo,&s0DhcpSwitch};
/*----------------------------- SubMenu Deepth1 -----------------------------*/
struct menuitem s1YcPort = {"1.‘∂¥´ø⁄",NULL,&menuYcPort};
struct menuitem* nodeBasicManager[] = {&s1YcPort};

struct menuitem s1SerialPara = {"1.¥Æø⁄≤Œ ˝",NULL,&menuSerialPara};
struct menuitem s1ThisGate = {"2.±æª˙Õ¯πÿ",menu1scallback,NULL};
struct menuitem* nodeTermInfo[] = {&s1SerialPara,&s1ThisGate};

struct menuitem s1Version = {"1.∞Ê±æ∫≈",menu1scallback,NULL};
struct menuitem* nodeDeviceInfo[] = {&s1Version};
/*----------------------------- SubMenu Deepth2 -----------------------------*/
struct menuitem s2YcPort1 = {"1.∂Àø⁄1",NULL,&menuYcPortPara};
struct menuitem s2YcPort2 = {"2.∂Àø⁄2",NULL,&menuYcPortPara};
struct menuitem s2YcPort3 = {"3.∂Àø⁄3",NULL,&menuYcPortPara};
struct menuitem* nodeBMYcPort[] = {&s2YcPort1,&s2YcPort2,&s2YcPort3};

struct menuitem s2SerialSeed = {"1.¥Æø⁄ÀŸ¬ ",menu1scallback,NULL};
struct menuitem s2ThisAddr = {"2.±æª˙µÿ÷∑",NULL,&menuThisIPAddr};
struct menuitem* nodeSerialPara[] = {&s2SerialSeed,&s2ThisAddr};

struct menuitem s2node1 = {"s2menu1",menu1scallback,NULL};
struct menuitem s2node2 = {"s2menu2",menu1scallback,NULL};
struct menuitem s2node3 = {"s2menu3",menu1scallback,NULL};
struct menuitem* subElements2[] = {&s2node1,&s2node2,&s2node3};
/*----------------------------- SubMenu Deepth3 -----------------------------*/
struct menuitem s3YcPort_Netmode = {"1.Õ¯¬Áƒ£ Ω",menu1scallback,NULL};
struct menuitem s3YcPort_Speed = {"2.∂Àø⁄ÀŸ¬ ",menu1scallback,NULL};
struct menuitem s3YcPort_Mode = {"3.‘∂¥´ø⁄ƒ£ Ω",menu1scallback,NULL};
struct menuitem* nodeBMYcPortPara[] = {&s3YcPort_Netmode,&s3YcPort_Speed,&s3YcPort_Mode};

struct menuitem s3ThisIPAddr = {"1.IPµÿ÷∑",menu1scallback,NULL};
struct menuitem* nodeThisIPAddr[] = {&s3ThisIPAddr};

/*----------------------------- Menu Define ---------------------------------*/
struct menu mainMenu = {5,NULL,mainElements};
//ª˘¥°π‹¿Ì
struct menu menuBasicManager = {1,&mainMenu,nodeBasicManager};
struct menu menuYcPort = {3,&menuBasicManager,nodeBMYcPort};
struct menu menuYcPortPara = {3,&menuYcPort,nodeBMYcPortPara};
//÷’∂Àπ‹¿Ì
struct menu menuTermInfo = {2,&mainMenu,nodeTermInfo};
struct menu menuSerialPara = {2,&menuTermInfo,nodeSerialPara};
struct menu menuThisIPAddr = {1,&menuSerialPara,nodeThisIPAddr};
//±æª˙–≈œ¢
struct menu menuDeviceInfo = {1,&mainMenu,nodeDeviceInfo};

struct menu subMenu2 = {3,&mainMenu,subElements2};
/*---------------------------------------------------------------------------*/
struct menustate myMenuState;


/*----------------------------- ªÊÕº∫Ø ˝BEGIN -------------------------------------*/
void draw_string_LineMiddle(uint8_t colIndex, char *str, uint8_t font, uint8_t mode)/*––æ”÷–*/
{
	uint8_t rowIndex,space;

	space = font*1;
	rowIndex = (OLED_ROW_PIXEL-space)/2;
	if(OLED_ROW_PIXEL>space)
		ssh1106_display_string(colIndex, rowIndex,str,font,mode);
	else
		ssh1106_display_string(colIndex,0,str,font,mode);
}

void draw_string_RankMiddle(uint8_t rowIndex, char *str, uint8_t font, uint8_t mode)/*¡–æ”÷–*/
{
	uint8_t colIndex,space;
	uint8_t len;

	len = strlen(str)-1;
	space = font/2*len;
	colIndex = (OLED_COL_PIXEL-space)/2;
	if(OLED_COL_PIXEL>space)
		ssh1106_display_string(colIndex,rowIndex,str,font,mode);
	else
		ssh1106_display_string(0,rowIndex,str,font,mode);
}

uint8_t key_num_convert(uint8_t key)
{
	switch(key)
	{
		case KEY_NUM0:
			return 0;
		case KEY_NUM1:
			return 1;
		case KEY_NUM2:
			return 2;
		case KEY_NUM3:
			return 3;
		case KEY_NUM4:
			return 4;
		case KEY_NUM5:
			return 5;
		case KEY_NUM6:
			return 6;
		case KEY_NUM7:
			return 7;
		case KEY_NUM8:
			return 8;
		case KEY_NUM9:
			return 9;
		default:
			return 0xFF;
	}
}
/*------------------------------ ªÊÕº∫Ø ˝END --------------------------------------*/

/*---------------------------- ≤Àµ•“≥ªÊÕºBEGIN ------------------------------------*/
#if 1
void handler_menu_cancel(struct menustate* ms)
{
	if(ms->currentMenu->parent != NULL)
	{
		ms->currentMenu = ms->currentMenu->parent;		
		ms->currentItem = ms->last_currentItem;		
	}
}

void handler_menu_select(struct menustate* ms)
{
	if(ms->currentMenu->menuArray[ms->currentItem]->child != NULL)
	{
		ms->currentMenu = ms->currentMenu->menuArray[ms->currentItem]->child;
		ms->last_currentItem = ms->currentItem;
		ms->currentItem = 0;
	}
	else if(ms->currentMenu->menuArray[ms->currentItem]->handlerFunc != NULL)
	{
		handler_menu_callFunction(ms->currentMenu->menuArray[ms->currentItem]->handlerFunc);
		//printf("handler func\n\r");
	}
}

void handler_menu_goDown(struct menustate* ms)
{
	handler_menu_goRight(ms);
	if(MENU_COL_NUM>1)
		handler_menu_goRight(ms);
}

void handler_menu_goUp(struct menustate* ms)
{
	handler_menu_goLeft(ms);
	if(MENU_COL_NUM>1)
		handler_menu_goLeft(ms);
}

void handler_menu_goLeft(struct menustate* ms)
{
	if(ms->currentItem!=0)
		ms->currentItem = ms->currentItem - 1;
}

void handler_menu_goRight(struct menustate* ms)
{
	if(ms->currentItem != (ms->currentMenu->length-1))
	{
		ms->currentItem += 1;
	}
}

bool display_menu_changed_by_key(uint8_t key,struct menustate* ms)
{
	switch(key)
	{
		case KEY_YES:
			handler_menu_select(ms);break;
		case KEY_NO:
			handler_menu_cancel(ms);break;
		case KEY_UP:
			handler_menu_goUp(ms);break;
		case KEY_DOWN:
			handler_menu_goDown(ms);break;
		case KEY_LEFT:
			handler_menu_goLeft(ms);break;
		case KEY_RIGHT:
			handler_menu_goRight(ms);break;
		/*
		case KEY_NUM0:
		case KEY_NUM1:
		case KEY_NUM2:
		case KEY_NUM3:
		case KEY_NUM4:
		case KEY_NUM5:
		case KEY_NUM6:
		case KEY_NUM7:
		case KEY_NUM8:
		case KEY_NUM9:
			lcdMenu_goBack(key,ms);break;
		*/
		default:
			return false;
	}
	return true;
}

/*---------------------------------------------------------------------------*/

 /**
  * @brief  µ±«∞≤Àµ•“≥—°œÓªÊ÷∆
	* @param1  rowIndex —°œÓµƒ–––Ú¡–£¨¥”1ø™ º
	* @param2  colIndex —°œÓµƒ¡––Ú¡–£¨¥”1ø™ º
	* @param3  str ¥Ú”°—°œÓ√˚◊÷
	* @param4  mode 0—°÷–ƒ£ Ω 1¥˝—°ƒ£ Ω
  * @retval	Œﬁ
*/
void draw_menu_item(uint8_t rowIndex, uint8_t colIndex, char *str, uint8_t mode)/**/
{
	uint8_t chXpos = MENU_COL_PIXEL_OFFSET + (colIndex-1)*MENU_COL_PIXEL_SPACE;
	uint8_t chYpos = MENU_ROW_PIXEL_OFFSET + (rowIndex-1)*MENU_ROW_PIXEL_SPACE;

	ssh1106_display_string(chXpos, chYpos, str, MENU_FONT_SIZE, mode);
}

 /**
  * @brief 	≤Àµ•“≥ªÊ÷∆
	* @param  ms ≤Àµ• 
  * @retval	Œﬁ
	*/
void draw_menu_Page(struct menustate* ms)
{
	uint8_t i,fstItem,lastItem;/*µ±«∞“≥√Êœ‘ æµƒµ⁄“ª∏ˆ—°œÓfstItem∫Õ◊Ó∫Û“ª∏ˆ—°œÓlastItem*/
	uint8_t rowIndex;
	uint8_t colIndex;
	
	ssh1106_clear_screen(0x00);
	fstItem = ms->currentItem - (ms->currentItem)%(ms->MAX_ROWS);
	lastItem = ms->MAX_ROWS + ms->currentItem - (ms->currentItem)%(ms->MAX_ROWS);
	for (i = fstItem; i < lastItem; ++i)
	{
		if(i<ms->currentMenu->length)
		{
			rowIndex = (i%ms->MAX_ROWS)/MENU_COL_NUM + 1;
			colIndex = (i%ms->MAX_ROWS)%MENU_COL_NUM + 1;
			if (ms->currentItem==i)
			{
				draw_menu_item(rowIndex,colIndex,(char *)(ms->currentMenu->menuArray[i]->name),0);
			}
			else
			{
				draw_menu_item(rowIndex,colIndex,(char *)(ms->currentMenu->menuArray[i]->name),1);
			}
		}
	}	
	ssh1106_refresh_gram();
}
#endif
/*----------------------------  ≤Àµ•“≥ªÊÕºEND  ------------------------------------*/


/*----------------------------  IP“≥ªÊÕºSTART  ------------------------------------*/
#if 1
bool display_page_ip_changed_by_key(uint8_t key, IPLine_pt ipLine)
{
	uint8_t len=15,num;
	uint8_t row_init = ipLine->font;
	uint8_t col_init = (OLED_COL_PIXEL-ipLine->font/2*len)/2;

	switch(key)
	{
		case KEY_YES:
			//handler_menu_select(ms);break;
			break;
		case KEY_NO:
			ipLine->flag_page=false;
			break;
		case KEY_UP:
			ipLine->row = row_init;
			ipLine->col = col_init;
			ipLine->index=0;
			ipLine->mode=0;
			break;
		case KEY_DOWN:
			ipLine->row = row_init + ipLine->row_step;
			ipLine->col = col_init;
			ipLine->index=0;
			ipLine->mode=1;
			break;
		case KEY_LEFT:
			if(ipLine->index>0){
				if((ipLine->index) %4==0){//"."Ã¯π˝
					ipLine->col -= ipLine->col_step;
					ipLine->index--;
				}
				ipLine->col -= ipLine->col_step;
				ipLine->index--;
			}
			break;	
		case KEY_RIGHT:
			if(ipLine->index<14){
				if((ipLine->index) %4==2){//"."Ã¯π˝
					ipLine->col += ipLine->col_step;
					ipLine->index++;	
				}
				ipLine->col += ipLine->col_step;
				ipLine->index++;
			}
			break;	

		case KEY_NUM0:
		case KEY_NUM1:
		case KEY_NUM2:
		case KEY_NUM3:
		case KEY_NUM4:
		case KEY_NUM5:
		case KEY_NUM6:
		case KEY_NUM7:
		case KEY_NUM8:
		case KEY_NUM9:
			num = key_num_convert(key);
			if(num==0xff) return false;
			if(ipLine->mode==0)
				ipLine->ip[ipLine->index] = num;
			else
				ipLine->mask[ipLine->index] = num;
			//sprintf(strIP,"%d",num);
			//ssh1106_display_string(ipLine->col,ipLine->row,strIP,ipLine->font,1);
			display_page_ip_changed_by_key(KEY_RIGHT,ipLine);
			break;	
		default:
			return false;
	}
	return true;
}

uint32_t draw_ip_line_item(uint8_t row, uint8_t *ipInput, char *str, uint8_t font)
{
	uint8_t i;
	//uint8_t tempIP[12];
	char temp[10];
	uint32_t ipData=0;

	memset(str,0,sizeof((unsigned char *)str));
	for(i=0;i<12;i++){
		if(ipInput[i]==0xFF){
			strcat(str,"_");
			//tempIP[i] = 0;
		}else{
			sprintf(temp,"%d",ipInput[i]&0x01);
			strcat(str,temp);
			//tempIP[i] = ipInput[i];
		}
		if(i%3==2){
			strcat(str,".");
			//ipData = (tempIP[i-2]*100+tempIP[i-1]*10+tempIP[i])<<((12-i-1)/3*8);
		}
	}
	draw_string_RankMiddle(row,str,font,1);
	return ipData;
}

void draw_Ip_Set_Page(void)
{
	uint8_t ch,len,i=0;
	char strIpLine[16]="___.___.___.___";
	char strMaskLine[16]="___.___.___.___";
	uint32_t cnt=1;
	IPLine_st ipLine;

#if 1	/*ªÊÕº≤Œ ˝*/
	ipLine.font = 12;
	ipLine.flag_page = true;
	len=sizeof(strIpLine)-1;
	ipLine.row = ipLine.font;
	ipLine.col = (OLED_COL_PIXEL-(ipLine.font)/2*len)/2;
	ipLine.row_step = 32;
	ipLine.col_step = ipLine.font/2;
	memset(ipLine.ip,0xFF,sizeof(ipLine.ip));
	ipLine.index=0;
#endif

	ssh1106_clear_screen(0x00);
	ssh1106_display_string(0,0,"IP Addr:",12,1);
	draw_string_RankMiddle(ipLine.font,strIpLine,ipLine.font,1);
	ssh1106_display_string(0,32,"Msak:",12,1);
	draw_string_RankMiddle(ipLine.font+ipLine.row_step,strMaskLine,ipLine.font,1);
	ssh1106_display_string(ipLine.col,ipLine.row," ",ipLine.font,0);
	ssh1106_refresh_gram();
	
	while(ipLine.flag_page){
		ch = GetKeyNum();
		if(ch!=0){/*∂¡»°∞¥º¸*/
			if(!display_page_ip_changed_by_key(ch,&ipLine)){
				TRACE(DEBUG_FAILURE_INFO,("display_page_ip_changed_by_key No such key num =%d\r",ch));
			}
			delay_soft();
		}
		cnt++;
		if(cnt%0xffff==0){
			cnt=0;
			draw_ip_line_item(ipLine.font,ipLine.ip,strIpLine,ipLine.font);
			draw_ip_line_item(ipLine.font+ipLine.row_step,ipLine.mask,strMaskLine,ipLine.font);
			if(i%2)
				ssh1106_display_string(ipLine.col,ipLine.row,"_",ipLine.font,1);
			else
				ssh1106_display_string(ipLine.col,ipLine.row," ",ipLine.font,0);
			ssh1106_refresh_gram();
			i++;
		}
	}
}
#endif
/*----------------------------  IP“≥ªÊÕºEND  ------------------------------------*/
void menu1scallback()
{
	//xprintf(PSTR("callback from submenu!\r\n"));
}
/*---------------------------------------------------------------------------*/
