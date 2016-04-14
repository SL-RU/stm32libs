#ifndef GUI_H
#define GUI_H 120

#include <stdio.h>
#include "fonts.h"
#include "ssd1306.h"

#define GUI_DefFont Font_7x10

/**
 * @brief  Initialize gui
 * @param  None
 * @retval None
 */
void gui_init(void);

void gui_set_cur_app(void (*appDraw)(), uint8_t (*appInput)(uint8_t));

/**
 * @brief  Simple draw text
 * @param  txt: Text
 * @param  x: X location of left corner
 * @param  y: Y location of up corner
 * @param  col: Color
 * @retval None
 */
void gui_text(char* txt, uint8_t x, uint8_t y, uint8_t col);
/**
 * @brief  Draw text with params
 * @param  txt: Text
 * @param  x: X location of left corner
 * @param  y: Y location of up corner
 * @param  w: Width
 * @param  h: Height
 * @param  bg: lable's background color. Text color is !bg
 * @param  border: width of border. border's color is !bg
 * @retval None
 */
void gui_lable(char* txt, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bg, uint8_t border);

/**
 * @brief  Draw multiline text with params
 * @param  txt: Text
 * @param  x: X location of left corner
 * @param  y: Y location of up corner
 * @param  w: Width
 * @param  h: Height
 * @param  bg: lable's background color. Text color is !bg
 * @param  border: width of border. border's color is !bg
 * @retval None
 */
void gui_lable_multiline(char* txt, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bg, uint8_t border);


//running line
#define GUI_TickerSpeed 500
#define GUI_TickerEndDelay 3
typedef struct 
{
	char *text;
	uint8_t x, y, w, h, bg, border;
	uint32_t startTick;
} GUI_TickerData;
/**
 * @brief  Draw running text line using GUI_TickerData structure
 * @param  txt: Text
 * @param  td: GUI_TickerData
 * @retval None
 */
void gui_ticker(GUI_TickerData *dt);
/**
 * @brief  Generate GUI_TickerData structure for running text line
 * @note   DO NOT FORGOT call @ref gui_removeTicker(dt);
 * @param  txt: Text
 * @param  x: X location of left corner
 * @param  y: Y location of up corner
 * @param  w: Width
 * @param  h: Height
 * @param  bg: lable's background color. Text color is !bg
 * @param  border: width of border. border's color is !bg
 * @retval None
 */
GUI_TickerData* gui_ticker_create(char *text, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bg, uint8_t border);
/**
 * @brief  Free memory from GUI_TickerData struct
 * @param  dt: GUI_TickerData
 * @retval None
 */
void gui_removeTicker(GUI_TickerData *dt);


//Event types:
#define GUI_event_click 1
#define GUI_event_focus 2
#define GUI_event_defocus 3

typedef struct
{
	uint16_t id; /*!< Item's id */
	char* text;  /*!< Item's text*/
	uint32_t arg; /*!< custom value */
	void (*ClickHandler)(uint16_t id, uint32_t arg, uint8_t eventType); /*!< Function, which will be called when user click OK button and this item is selected */
	void (*FocusHandler)(uint16_t id, uint32_t arg, uint8_t eventType); /*!< Function, which will be called when user focused on this item */
	void (*DeFocusHandler)(uint16_t id, uint32_t arg, uint8_t eventType); /*!< Function, which will be called when user defocused this item */
	
} GUI_ListItemData;

typedef struct
{
	char* header;  /*!< Header*/
	GUI_ListItemData **items;  /*!< Item's array*/
	uint16_t ItemsCount; /*!< Item's array*/
	uint8_t x, y, w, h;
	uint16_t selectedItem;
	
	void (*ClickHandler)(uint16_t id, uint32_t arg, uint8_t eventType); /*!< Function, which will be called when user click OK */
	void (*FocusHandler)(uint16_t id, uint32_t arg, uint8_t eventType); /*!< Function, which will be called when user focuses on item */
	void (*DeFocusHandler)(uint16_t id, uint32_t arg, uint8_t eventType); /*!< Function, which will be called when user defocused on item */

} GUI_ListData;
extern GUI_ListData *gui_CurList;
/**
 * @brief  Create list data structure for using listElement
 * @note   DO NOT FORGOT call @ref gui_remove_list(list);
 * @param  header: header 
 * @param  count: number of elements in items array
 * @param  items: array with elements
 * @param  x: X location of left corner
 * @param  y: Y location of up corner
 * @param  w: Width
 * @param  h: Height
 * @param  onClick: Function, which will be called when user click OK
 * @param  onFocus: Function, which will be called when user focuses on item
 * @param  onDeFocus: Function, which will be called when user defocused on item
 * @retval GUI_ListData
 */
GUI_ListData* gui_create_list(char* header, uint16_t count, GUI_ListItemData** items, uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
	void (*onClick)(uint16_t, uint32_t, uint8_t eventType), void (*onFocus)(uint16_t, uint32_t, uint8_t eventType), void (*onDeFocus)(uint16_t, uint32_t, uint8_t eventType));
/**
 * @brief  free memory from GUI_ListData
 * @param  list: GUI_ListData
 * @retval None
 */
void gui_remove_list(GUI_ListData* list);
/**
 * @brief  Create list's item data structure for using in listElement
 * @note   DO NOT FORGOT call @ref gui_remove_listItem(ld);
 * @param  text: text of the element 
 * @param  arg: custom variable  
 * @param  onClick: Function, which will be called when user click OK
 * @param  onFocus: Function, which will be called when user focuses on item
 * @param  onDeFocus: Function, which will be called when user defocused on item
 * @retval GUI_ListItemData
 */
GUI_ListItemData* gui_create_listItem(char* text, uint32_t arg,
	void (*onClick)(uint16_t, uint32_t, uint8_t eventType), void (*onFocus)(uint16_t, uint32_t, uint8_t eventType), void (*onDeFocus)(uint16_t, uint32_t, uint8_t eventType));
/**
 * @brief  free memory from GUI_ListItemData
 * @note   DO NOT FORGET gui_set_curList(0) to cancel drawing list
 * @param  ld: GUI_ListData
 * @retval None
 */
void gui_remove_listItem(GUI_ListItemData *ld);
/**
 * @brief  Ser current GUI_List element
 * @param  ld: GUI_ListItemData
 * @retval None
 */
void gui_set_curList(GUI_ListData* list);
/**
 * @brief  draw current GUI_List
 * @note   DO NOT USE IT BY YOURSELF! DO NOT FORGET gui_set_curList(data) first!
 * @param  ld: GUI_ListItemData
 * @retval None
 */
uint8_t gui_draw_list(void);
/**
 * @brief  draw current GUI_List
 * @note   DO NOT USE IT BY YOURSELF! DO NOT FORGET gui_set_curList(data) first!
 * @param  ld: GUI_ListItemData
 * @retval None
 */
uint8_t gui_input_list(int8_t key);


void gui_showMessage(char* text);
void gui_showCustomMessage(void (*drawmsg)(), uint8_t (*msginput)(uint8_t));
void gui_closeMessage(void);
uint8_t gui_draw_message(void);
uint8_t gui_input_message(uint8_t key);


void gui_input(int8_t key);

void gui_draw(void);
#endif
