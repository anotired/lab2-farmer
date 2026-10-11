#include <stdio.h>
#define inventory_size 10
int main()
{
    int current_day = 1;
    int current_hour = 8;
    int inventory[inventory_size] = {1, 2, 3, 4, 0, 2, 5, 7, 0, 1};
    /* 0 - пусто, 1 - дерево, 2 - камень, 3 - семена, 4 - кирка, 5 - топор, 6 - мотыга,
    7 - броня, 8 - верстак, 9 - печь */
    
    int result;
    int choice;
    int ch;

    while (1) {
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Устранить дубликаты\n");
        
        printf("Введите действие: ");
        result = scanf("%d", &choice);
        ch = getchar();
        while (ch != '\n' && ch != EOF) {
            ch = getchar();
        }
        if (result == EOF) {
            return 0;
        }
        if (result != 1) {
            printf("Ошибка. Нужно ввести число\n");
            continue;
        }
        switch (choice) {
            case 0:
                printf("Вы вышли из игры\n");
                return 0;

            default:
                printf("Такого пункта нет в меню!\n");
                break;
        }
    }
}