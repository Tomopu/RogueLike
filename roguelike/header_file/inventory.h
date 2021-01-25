#ifndef STORAGE_H
#define STORAGE_H

// リスト
typedef struct LIST{
  int  type;	
  int  model_num;
  int  effect_value;
  char name[BUFLEN];
  struct LIST *next;
}List;

typedef struct{
	int type;
	int model_num;
} Property;

typedef struct{
  int type;	
  int model_num;
  int y;
  int x;
}Item;

extern List *new_node(void);										                    // 新しいノードを作成
extern void append_list(List *list, int effect_value, char name[]);	// リストを作成
extern void print_list(List *list);									                // リストを表示
extern int  list_len(List *list);									                  // リストの長さ
extern int  use_list(List **ndPtrPtr, Property *item, int index);	
extern int  remove_list(List **ndPtrPtr, int index);                // リストを削除
extern void get_item(Chara *player, List *storage, Item *item);
extern void add_item(int type, int model, List *storage);

#endif