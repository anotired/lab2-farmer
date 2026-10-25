#include <stdio.h>
#define inventory_size 10
#define max_work_hours 24
#define hours_in_day 24
#define max_item_id 9

int main()
{
    int current_day = 1;
    int current_hour = 8;
    int inventory[inventory_size] = {1, 2, 3, 4, 8, 2, 5, 7, 8, 1};
    /* 0 - пусто, 1 - дерево, 2 - камень, 3 - семена, 4 - кирка, 5 - топор, 6 - мотыга,
    7 - броня, 8 - верстак, 9 - печь */
    
    int result;
    int choice;
    int ch;
    int hours;
    int i;
    int slot;
    int item_id;
    int j;

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
            
            case 1:
            printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
            break;

            case 2:
                printf("Сколько часов работать (0-%d)? ", max_work_hours);
                result = scanf("%d", &hours);
                while ((ch = getchar()) != '\n' && ch != EOF) {
                }
                if (result == EOF) {
                    return 0;
                }
                if (result != 1 || hours < 0 || hours > hours_in_day) {
                    printf("Ошибка: введите число от 0 до %d!\n", max_work_hours);
                    break;
                }
                current_hour = current_hour + hours;
                current_day = current_day + current_hour / hours_in_day;
                current_hour = current_hour % hours_in_day;
                printf("Вы поработали %d ч.\n", hours);
                printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
                break;
            
            case 3:
                for (i = 0; i < inventory_size; i++) {
                    printf("Слот %d: [%d]", i, inventory[i]);
                    switch (inventory[i]) {
                        case 1: printf(" (Дерево)"); break;
                        case 2: printf(" (Камень)"); break;
                        case 3: printf(" (Семена)"); break;
                        case 4: printf(" (Кирка)"); break;
                        case 5: printf(" (Топор)"); break;
                        case 6: printf(" (Мотыга)"); break;
                        case 7: printf(" (Броня)"); break;
                        case 8: printf(" (Верстак)"); break;
                        case 9: printf(" (Печь)"); break;
                        default: 
                            break;
                    }
                    printf("\n");
                }
                break;
            
            case 4:
                printf("Номер слота (0-%d): ", inventory_size - 1);
                result = scanf("%d", &slot);
                while ((ch = getchar()) != '\n' && ch != EOF) {
                }
                if (result == EOF) {
                    return 0;
                }
                if (result != 1 || slot < 0 || slot >= inventory_size) {
                    printf("Ошибка: слот должен быть числом от 0 до %d!\n", inventory_size - 1);
                    break;
                }
 
                printf("ID предмета (0-%d): ", max_item_id);
                result = scanf("%d", &item_id);
                while ((ch = getchar()) != '\n' && ch != EOF) {
                }
                if (result == EOF) {
                    return 0;
                }
                if (result != 1 || item_id < 0 || item_id > max_item_id) {
                    printf("Ошибка. ID должен быть числом от 0 до %d!\n", max_item_id);
                    break;
                }
 
                inventory[slot] = item_id;
                printf("Готово. В слоте %d теперь предмет %d.\n", slot, item_id);
                break;
 
            case 5:
                printf("Номер слота (0-%d): ", inventory_size - 1);
                result = scanf("%d", &slot);
                while ((ch = getchar()) != '\n' && ch != EOF) {
                }
                if (result == EOF) {
                    return 0;
                }
                if (result != 1 || slot < 0 || slot >= inventory_size) {
                    printf("Ошибка: слот должен быть числом от 0 до %d!\n", inventory_size - 1);
                    break;
                }
                inventory[slot] = 0;
                printf("Слот %d очищен.\n", slot);
                break;
            
            case 6:
                printf("До: ");
                for (i = 0; i < inventory_size; i++) {
                    printf("%d ", inventory[i]);
                }
                printf("\n");
 
                for (i = 0; i < inventory_size; i++) {
                    if (inventory[i] != 0) {
                        for (j = i + 1; j < inventory_size; j++) {
                            if (inventory[j] == inventory[i]) {
                                inventory[j] = 0;
                            }
                        }
                    }
                }
 
                printf("После: ");
                for (i = 0; i < inventory_size; i++) {
                    printf("%d ", inventory[i]);
                }
                printf("\n");
                break;
        }
    }
}